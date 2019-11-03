#include "UIEnsembleChorus.hpp"
#include "EnsembleChorusShared.hpp"
#include "EnsembleChorusUILayouts.hpp"
#include "Artwork.hpp"
#include "Window.hpp"
#include "ui/FontEngine.h"
#include "ui/components/LookAndFeel.hpp"
#include "ui/components/Slider.hpp"
#include "ui/components/TextLabel.hpp"
#include "ui/components/ToggleButton.hpp"
#include "ui/components/ToggleButtonGroup.hpp"
#include "ui/ensemble-chorus/ModulatorView.h"
#include <gzip/decompress.hpp>

#define Rsrc(t, x) \
    ((const t *)Artwork::x##Data), (Artwork::x##DataSize / sizeof(t))

struct FontDefinition {
    const char *name;
    const uint8_t *gzdata;
    size_t gzsize;
};

static const FontDefinition FontDefinitions[] = {
    {"Sans Regular", Rsrc(uint8_t, Sans_Regular)},
    {"Sans Bold", Rsrc(uint8_t, Sans_Bold)},
    {"Serif Bold Italic", Rsrc(uint8_t, Serif_BoldItalic)},
};

// -----------------------------------------------------------------------
// Init / Deinit

UIEnsembleChorus::UIEnsembleChorus()
    : UI(Main::Widgets::root.w, Main::Widgets::root.h),
      fParameters(new Parameter[EC_PARAMETER_COUNT]),
      fFontEngine(new FontEngine),
      fLookAndFeel(new LookAndFeel)
{
    for (unsigned i = 0; i < EC_PARAMETER_COUNT; ++i)
        InitParameter(i, fParameters[i]);

    static const std::string *fontData = initializeFontDataOnce();
    initializeFonts(fontData);

    fSubwidgets.reserve(32);

    createWidgets();
}

UIEnsembleChorus::~UIEnsembleChorus()
{
}

// -----------------------------------------------------------------------
// DSP/Plugin callbacks

/**
  A parameter has changed on the plugin side.
  This is called by the host to inform the UI about parameter changes.
*/
void UIEnsembleChorus::parameterChanged(uint32_t index, float value)
{
    if (index < EC_PARAMETER_COUNT) {
        if (Slider *sl = fParameterSlider[index].get())
            sl->setValue(value);

        if (ToggleButton *tb = fParameterToggleButton[index].get())
            tb->setValue(value != 0);

        if (ToggleButtonGroup *tg = fParameterToggleButtonGroup[index].get())
            tg->setValue(value);
    }

    switch (index) {
    case ECP_PHASE1: case ECP_PHASE2:
    case ECP_PHASE3: case ECP_PHASE4:
    case ECP_PHASE5: case ECP_PHASE6:
        fPhaseLabel[(index - ECP_PHASE1) / (ECP_PHASE2 - ECP_PHASE1)]->setText(std::to_string((int)value));
        break;

    case kOutSlowModulator1: case kOutSlowModulator2:
    case kOutSlowModulator3: case kOutSlowModulator4:
    case kOutSlowModulator5: case kOutSlowModulator6:
        fModulatorView[index - kOutSlowModulator1]->setValue1(value);
        break;

    case kOutFastModulator1: case kOutFastModulator2:
    case kOutFastModulator3: case kOutFastModulator4:
    case kOutFastModulator5: case kOutFastModulator6:
        fModulatorView[index - kOutFastModulator1]->setValue2(value);
        break;

    default:
        DISTRHO_SAFE_ASSERT_RETURN(index < EC_PARAMETER_COUNT, );
    }
}

/**
  A program has been loaded on the plugin side.
  This is called by the host to inform the UI about program changes.
*/
void UIEnsembleChorus::programLoaded(uint32_t index)
{
    DISTRHO_SAFE_ASSERT_RETURN(index < FactoryPresetCount, );

    for (int i = 0; i < EC_PARAMETER_COUNT; i++) {
        // set values for each parameter and update their widgets
        parameterChanged(i, FactoryPresets[index].params[i]);
    }
}

// -----------------------------------------------------------------------
// Optional UI callbacks

/**
  Idle callback.
  This function is called at regular intervals.
*/
void UIEnsembleChorus::uiIdle()
{
}

// -----------------------------------------------------------------------
// Widget callbacks


/**
  A function called to draw the view contents.
*/
void UIEnsembleChorus::onDisplay()
{
    cairo_t *cr = getParentWindow().getGraphicsContext().cairo;
    LookAndFeel &lnf = *fLookAndFeel;

    cairo_set_line_width(cr, 1.0);

    ColorRGBA8 bgColor = lnf.getBackgroundColor();
    ColorRGBA8 passiveFillColor = lnf.getPassiveFillColor();

    cairo_set_source_rgba8(cr, bgColor);
    cairo_paint(cr);

    FontEngine &fe = *fFontEngine;

    const Label *mainTitle = &Main::Labels::lbl_title;

    static const Label *titles[] = {
        &Main::Labels::lbl_stages,
        &Main::Labels::lbl_delay,
        &Main::Labels::lbl_mix,
        &Main::Labels::lbl_gain,
        &Main::Labels::lbl_phase,
        &Main::Labels::lbl_routing,
        &Main::Labels::lbl_modulator,
        &Main::Labels::lbl_aa,
        &Main::Labels::lbl_chorus,
        &Main::Labels::lbl_vibrato,
        &Main::Labels::lbl_lpf,
    };

    static const Label *normalTexts[] = {
        &Main::Labels::sl_wet,
        &Main::Labels::sl_dry,
        &Main::Labels::sl_gain_in,
        &Main::Labels::sl_gain_out,
        &Main::Labels::dl_aa_cutoff,
        &Main::Labels::dl_mod_range,
        &Main::Labels::dl_slow_rand,
        &Main::Labels::dl_fast_rand,
        &Main::Labels::lbl_lpf_cutoff,
        &Main::Labels::lbl_lpf_q,
    };

    cairo_set_source_rgba8(cr, passiveFillColor);
    cairo_rectangle(cr, Main::Labels::lbl_title.bounds.to<double>().off_by({4.5, 4.5}).from_bottom(4.0));
    cairo_fill(cr);
    cairo_rectangle(cr, Main::Labels::lbl_title.bounds.to<double>().off_by({4.5, 4.5}).from_right(4.0));
    cairo_fill(cr);

    cairo_set_source_rgba8(cr, passiveFillColor);
    cairo_rectangle(cr, Main::Labels::lbl_title.bounds.to<double>().off_by({0.5, 0.5}));
    cairo_stroke(cr);

    ColorRGBA8 textColor = lnf.getTextColor();

    {
        const Label *label = mainTitle;
        Font font;
        font.name = "Serif Bold Italic";
        font.size = 22;
        font.color = textColor;
        fe.drawInBox(cr, label->text, font, label->bounds, label->align);
    }

    for (const Label *label : titles) {
        Font font;
        font.name = "Serif Bold Italic";
        font.size = 20;
        font.color = textColor;
        fe.drawInBox(cr, label->text, font, label->bounds, label->align);
    }

    for (const Label *label : normalTexts) {
        Font font;
        font.name = "Sans Regular";
        font.size = 14;
        font.color = textColor;
        fe.drawInBox(cr, label->text, font, label->bounds, label->align);
    }
}

// -----------------------------------------------------------------------

const std::string *UIEnsembleChorus::initializeFontDataOnce()
{
    constexpr unsigned count = sizeof(FontDefinitions) / sizeof(FontDefinitions[0]);
    static std::string data[count];

    for (unsigned i = 0; i < count; ++i) {
        const FontDefinition &fd = FontDefinitions[i];
        data[i] = gzip::decompress((const char *)fd.gzdata, fd.gzsize);
    }

    return data;
}

void UIEnsembleChorus::initializeFonts(const std::string *fontData)
{
    FontEngine &fe = *fFontEngine;
    constexpr unsigned count = sizeof(FontDefinitions) / sizeof(FontDefinitions[0]);

    for (unsigned i = 0; i < count; ++i) {
        const FontDefinition &fd = FontDefinitions[i];
        fe.addFont(fd.name, (const uint8_t *)fontData[i].data(), fontData[i].size());
    }
}

// -----------------------------------------------------------------------

void UIEnsembleChorus::createWidgets()
{
    FontEngine &fe = *fFontEngine;
    LookAndFeel &lnf = *fLookAndFeel;

    ColorRGBA8 textColor = lnf.getTextColor();

    Slider *slider;
    ToggleButton *toggle;
    ToggleButtonGroup *toggleGroup;
    TextLabel *text;

    Font font;
    font.name = "Sans Bold";
    font.size = 16;
    font.color = textColor;

    toggle = createToggleButtonForParameter(ECP_BYPASS, Main::Widgets::tick_bypass);

    text = createSubwidget<TextLabel>(fe, this);
    text->setBounds(Main::Widgets::tick_bypass);
    text->setText("Bypass");
    text->setTextFont(font);

    toggleGroup = createToggleButtonGroupForParameter(ECP_CHANNEL_LAYOUT, jsl::dynarray<Rect>{Main::Widgets::btn_stereo, Main::Widgets::btn_mono});

    text = createSubwidget<TextLabel>(fe, this);
    text->setBounds(Main::Widgets::btn_stereo);
    text->setText("Stereo");
    text->setTextFont(font);

    text = createSubwidget<TextLabel>(fe, this);
    text->setBounds(Main::Widgets::btn_mono);
    text->setText("Mono");
    text->setTextFont(font);

    slider = createSliderForParameter(ECP_DELAY, Main::Widgets::sl_delay);
    slider->setOrientation(Slider::Vertical);

    slider = createSliderForParameter(ECP_AA_CUTOFF, Main::Widgets::dl_aa_cutoff);
    slider->setOrientation(Slider::Radial);

    slider = createSliderForParameter(ECP_MOD_RANGE, Main::Widgets::dl_mod_range);
    slider->setOrientation(Slider::Radial);

    slider = createSliderForParameter(ECP_SLOW_RATE, Main::Widgets::dl_slow_rate);
    slider->setOrientation(Slider::Radial);

    slider = createSliderForParameter(ECP_SLOW_RAND, Main::Widgets::dl_slow_rand);
    slider->setOrientation(Slider::Radial);

    slider = createSliderForParameter(ECP_FAST_RATE, Main::Widgets::dl_fast_rate);
    slider->setOrientation(Slider::Radial);

    slider = createSliderForParameter(ECP_FAST_RAND, Main::Widgets::dl_fast_rand);
    slider->setOrientation(Slider::Radial);

    slider = createSliderForParameter(ECP_LPF_CUTOFF, Main::Widgets::dl_lpf_cutoff);
    slider->setOrientation(Slider::Radial);

    slider = createSliderForParameter(ECP_LPF_Q, Main::Widgets::dl_lpf_q);
    slider->setOrientation(Slider::Radial);

    slider = createSliderForParameter(ECP_GAIN_IN, Main::Widgets::sl_gain_in);
    slider->setOrientation(Slider::Vertical);

    slider = createSliderForParameter(ECP_GAIN_OUT, Main::Widgets::sl_gain_out);
    slider->setOrientation(Slider::Vertical);

    slider = createSliderForParameter(ECP_MIX_DRY, Main::Widgets::sl_dry);
    slider->setOrientation(Slider::Vertical);

    slider = createSliderForParameter(ECP_MIX_WET, Main::Widgets::sl_wet);
    slider->setOrientation(Slider::Vertical);

#define LINE(x)                                                         \
    toggle = createToggleButtonForParameter(ECP_ENABLE##x, Main::Widgets::btn_enable##x); \
                                                                        \
    text = createSubwidget<TextLabel>(fe, this);                        \
    text->setBounds(Main::Widgets::btn_enable##x);                      \
    text->setText(#x);                                                  \
    text->setTextFont(font);                                            \
                                                                        \
    slider = createSliderForParameter(ECP_PHASE##x, Main::Widgets::sl_phase##x); \
    slider->setOrientation(Slider::Horizontal);                         \
                                                                        \
    slider = createSliderForParameter(ECP_DEPTH##x, Main::Widgets::sl_depth##x); \
    slider->setOrientation(Slider::Horizontal);                         \
                                                                        \
    toggle = createToggleButtonForParameter(ECP_ROUTE_L##x, Main::Widgets::btn_routeL##x); \
                                                                        \
    text = createSubwidget<TextLabel>(fe, this);                        \
    text->setBounds(Main::Widgets::btn_routeL##x);                      \
    text->setText("L");                                                 \
    text->setTextFont(font);                                            \
                                                                        \
    toggle = createToggleButtonForParameter(ECP_ROUTE_R##x, Main::Widgets::btn_routeR##x); \
                                                                        \
    text = createSubwidget<TextLabel>(fe, this);                        \
    text->setBounds(Main::Widgets::btn_routeR##x);                      \
    text->setText("R");                                                 \
    text->setTextFont(font);

    LINE(1) LINE(2) LINE(3) LINE(4) LINE(5) LINE(6);
#undef LINE

    for (unsigned i = 0; i < 6; ++i) {
        TextLabel *label = new TextLabel(fe, this);
        fPhaseLabel[i].reset(label);
        const Rect *bounds[] = {
            &Main::Widgets::sl_phase1, &Main::Widgets::sl_phase2,
            &Main::Widgets::sl_phase3, &Main::Widgets::sl_phase4,
            &Main::Widgets::sl_phase5, &Main::Widgets::sl_phase6,
        };
        label->setBounds(*bounds[i]);

        Font font;
        font.name = "Sans Regular";
        font.size = 14;
        font.color = textColor;
        label->setTextFont(font);
    }

    for (unsigned i = 0; i < 6; ++i) {
        ModulatorView *view = new ModulatorView(this, lnf);
        fModulatorView[i].reset(view);
        const Rect *bounds[] = {
            &Main::Widgets::visu_mod1, &Main::Widgets::visu_mod2,
            &Main::Widgets::visu_mod3, &Main::Widgets::visu_mod4,
            &Main::Widgets::visu_mod5, &Main::Widgets::visu_mod6,
        };
        view->setBounds(*bounds[i]);
    }
}

Slider *UIEnsembleChorus::createSliderForParameter(uint32_t index, const Rect &rect)
{
    DISTRHO_SAFE_ASSERT_RETURN(index < EC_PARAMETER_COUNT, nullptr);

    Slider *slider = new Slider(this, *fLookAndFeel);
    fParameterSlider[index].reset(slider);

    slider->setBounds(rect);

    const Parameter &param = fParameters[index];
    slider->setValueBounds(param.ranges.min, param.ranges.max);
    slider->setDefaultValue(param.ranges.def);
    if (param.hints & kParameterIsLogarithmic)
        slider->setLogarithmic(true);

    slider->ValueChangedCallback = [this, index](double value)
    {
        setParameterValue(index, value);
        parameterChanged(index, value);
    };

    return slider;
}

ToggleButton *UIEnsembleChorus::createToggleButtonForParameter(uint32_t index, const Rect &rect)
{
    DISTRHO_SAFE_ASSERT_RETURN(index < EC_PARAMETER_COUNT, nullptr);

    ToggleButton *toggle = new ToggleButton(this, *fLookAndFeel);
    fParameterToggleButton[index].reset(toggle);

    toggle->setBounds(rect);

    // const Parameter &param = fParameters[index];
    // toggle->setDefaultValue(param.ranges.def);

    toggle->ValueChangedCallback = [this, index](bool value)
    {
        setParameterValue(index, value);
        parameterChanged(index, value);
    };

    return toggle;
}

ToggleButtonGroup *UIEnsembleChorus::createToggleButtonGroupForParameter(uint32_t index, const jsl::dynarray<Rect> &rects)
{
    ToggleButtonGroup *toggleGroup = new ToggleButtonGroup(*fLookAndFeel);
    fParameterToggleButtonGroup[index].reset(toggleGroup);

    const Parameter &param = fParameters[index];
    for (unsigned i = 0, n = param.enumValues.count; i < n; ++i) {
        ToggleButton *toggle = toggleGroup->createButton(this, param.enumValues.values[i].value);
        toggle->setBounds(rects.at(i));
    }

    toggleGroup->ValueChangedCallback = [this, index](double value)
    {
        setParameterValue(index, value);
        parameterChanged(index, value);
    };

    return toggleGroup;
}

// -----------------------------------------------------------------------

UI *DISTRHO::createUI()
{
    return new UIEnsembleChorus;
}

// -----------------------------------------------------------------------
