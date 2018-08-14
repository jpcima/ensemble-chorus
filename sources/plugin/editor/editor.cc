//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "editor.h"
#include "widgets/knob.h"
#include "widgets/button.h"
#include "graphics/box.h"
#include "graphics/font.h"
#include "graphics/text.h"
#include "ensemble_chorus.h"

struct Chorus_UI::Impl :
    public Knob::Callback,
    public Button::Callback
{
    Chorus_UI *const Q = nullptr;

    Color bg_{0xc0, 0xc0, 0xc0};
    std::unique_ptr<FontCollection> fonts_;

    std::unique_ptr<Knob> kn_mod_range_;
    std::unique_ptr<Knob> kn_slow_rate_;
    std::unique_ptr<Knob> kn_fast_rate_;
    std::unique_ptr<Knob> kn_slow_rand_;
    std::unique_ptr<Knob> kn_fast_rand_;
    std::unique_ptr<Knob> kn_lpf_cutoff_;
    std::unique_ptr<Knob> kn_lpf_q_;
    std::unique_ptr<Button> btn_enable1_;
    std::unique_ptr<Button> btn_enable2_;
    std::unique_ptr<Button> btn_enable3_;
    std::unique_ptr<Button> btn_enable4_;
    std::unique_ptr<Button> btn_enable5_;
    std::unique_ptr<Button> btn_enable6_;

    explicit Impl(Chorus_UI *q)
        : Q(q) {}

    void knobDragStarted(Knob *knob) override {}
    void knobDragFinished(Knob *knob) override {}
    void knobValueChanged(Knob *knob, float value) override;

    void buttonValueChanged(Button *btn, bool value) override;
};

Chorus_UI::Chorus_UI()
    : UI(625, 280),
      P(new Impl(this))
{
    FontCollection *fonts = new FontCollection(*this);
    P->fonts_.reset(fonts);

#define KNOB(id, x, y, w, h)                    \
    Knob *kn_##id = new Knob(this);             \
    P->kn_##id##_.reset(kn_##id);               \
    kn_##id->setAbsolutePos(x, y);              \
    kn_##id->setSize(w, h);                     \
    kn_##id->setCallback(P.get());

    KNOB(mod_range, 370, 80, 45, 45);
    KNOB(slow_rate, 380, 210, 45, 45);
    KNOB(fast_rate, 500, 210, 45, 45);
    KNOB(slow_rand, 425, 240, 35, 35);
    KNOB(fast_rand, 545, 240, 35, 35);
    KNOB(lpf_cutoff, 160, 235, 40, 40);
    KNOB(lpf_q, 255, 235, 40, 40);

#define BUTTON(id, x, y, w, h, text)            \
    Button *btn_##id = new Button(this);        \
    P->btn_##id##_.reset(btn_##id);             \
    btn_##id->setAbsolutePos(x, y);             \
    btn_##id->setSize(w, h);                    \
    btn_##id->setLabel(text);                   \
    btn_##id->setCallback(P.get());

    BUTTON(enable1, 20, 80, 25, 25, "1");
    BUTTON(enable2, 20, 105, 25, 25, "2");
    BUTTON(enable3, 20, 130, 25, 25, "3");
    BUTTON(enable4, 20, 155, 25, 25, "4");
    BUTTON(enable5, 20, 180, 25, 25, "5");
    BUTTON(enable6, 20, 205, 25, 25, "6");

    for (Button *btn : {btn_enable1, btn_enable2, btn_enable3, btn_enable4, btn_enable5, btn_enable6}) {
        btn->setFont(fonts->getSansRegular());
        btn->setFontSize(16.0f);
        btn->setTextStyle(TS_ENGRAVED);
    }

#undef KNOB
#undef BUTTON
}

void Chorus_UI::parameterChanged(uint32_t index, float value)
{
    switch (index) {
    case ECP_BYPASS:
        // TODO
        break;
    case ECP_CHANNEL_LAYOUT:
        // TODO
        break;
    case ECP_DELAY:
        // TODO
        break;
    case ECP_NSTAGES:
        // TODO
        break;
    case ECP_MOD_RANGE:
        P->kn_mod_range_->setValue(value);
        break;
    case ECP_SLOW_RATE:
        P->kn_slow_rate_->setValue(value);
        break;
    case ECP_SLOW_WAVE:
        // TODO
        break;
    case ECP_SLOW_RAND:
        P->kn_slow_rand_->setValue(value);
        break;
    case ECP_FAST_RATE:
        P->kn_fast_rate_->setValue(value);
        break;
    case ECP_FAST_WAVE:
        // TODO
        break;
    case ECP_FAST_RAND:
        P->kn_fast_rand_->setValue(value);
        break;
    case ECP_LPF_CUTOFF:
        P->kn_lpf_cutoff_->setValue(value);
        break;
    case ECP_LPF_Q:
        P->kn_lpf_q_->setValue(value);
        break;
    case ECP_GAIN_IN:
        // TODO
        break;
    case ECP_GAIN_OUT:
        // TODO
        break;
    case ECP_MIX_DRY:
        // TODO
        break;
    case ECP_MIX_WET:
        // TODO
        break;

    case ECP_ENABLE1:
        P->btn_enable1_->setValue(value);
        break;
    case ECP_PHASE1:
        // TODO
        break;
    case ECP_DEPTH1:
        // TODO
        break;
    case ECP_ROUTE_L1:
        // TODO
        break;
    case ECP_ROUTE_R1:
        // TODO
        break;

    case ECP_ENABLE2:
        P->btn_enable2_->setValue(value);
        break;
    case ECP_PHASE2:
        // TODO
        break;
    case ECP_DEPTH2:
        // TODO
        break;
    case ECP_ROUTE_L2:
        // TODO
        break;
    case ECP_ROUTE_R2:
        // TODO
        break;

    case ECP_ENABLE3:
        P->btn_enable3_->setValue(value);
        break;
    case ECP_PHASE3:
        // TODO
        break;
    case ECP_DEPTH3:
        // TODO
        break;
    case ECP_ROUTE_L3:
        // TODO
        break;
    case ECP_ROUTE_R3:
        // TODO
        break;

    case ECP_ENABLE4:
        P->btn_enable4_->setValue(value);
        break;
    case ECP_PHASE4:
        // TODO
        break;
    case ECP_DEPTH4:
        // TODO
        break;
    case ECP_ROUTE_L4:
        // TODO
        break;
    case ECP_ROUTE_R4:
        // TODO
        break;

    case ECP_ENABLE5:
        P->btn_enable5_->setValue(value);
        break;
    case ECP_PHASE5:
        // TODO
        break;
    case ECP_DEPTH5:
        // TODO
        break;
    case ECP_ROUTE_L5:
        // TODO
        break;
    case ECP_ROUTE_R5:
        // TODO
        break;

    case ECP_ENABLE6:
        P->btn_enable6_->setValue(value);
        break;
    case ECP_PHASE6:
        // TODO
        break;
    case ECP_DEPTH6:
        // TODO
        break;
    case ECP_ROUTE_L6:
        // TODO
        break;
    case ECP_ROUTE_R6:
        // TODO
        break;
    }
}

void Chorus_UI::onNanoDisplay()
{
    int w = getWidth();
    int h = getHeight();
    FontCollection &fonts = *P->fonts_;
    Color bg = P->bg_;
    Color textcolor = Color(0.0f, 0.0f, 0.0f);

    beginPath();
    rect(0, 0, w, h);
    fillColor(bg);
    fill();

    fontFaceId(fonts.getSerifBoldItalic());
    fontSize(22);
    shadow_box(*this, 0, 0, 230, 42, Color(0xc0, 0xc0, 0xc0));
    bounded_text(*this, TS_ENGRAVED, 0, 0, 230, 42, ALIGN_CENTER|ALIGN_MIDDLE, "JPC Ensemble Chorus", textcolor);

    fontFaceId(fonts.getSerifBoldItalic());
    fontSize(20);
    bounded_text(*this, TS_ENGRAVED, 365, 5, 65, 25, ALIGN_CENTER|ALIGN_MIDDLE, "Stages", textcolor);
    bounded_text(*this, TS_ENGRAVED, 435, 5, 55, 25, ALIGN_CENTER|ALIGN_MIDDLE, "Delay", textcolor);
    bounded_text(*this, TS_ENGRAVED, 500, 5, 50, 25, ALIGN_CENTER|ALIGN_MIDDLE, "Mix", textcolor);
    bounded_text(*this, TS_ENGRAVED, 560, 5, 50, 25, ALIGN_CENTER|ALIGN_MIDDLE, "Gain", textcolor);
    bounded_text(*this, TS_ENGRAVED, 65, 55, 105, 25, ALIGN_LEFT|ALIGN_MIDDLE, "Phase", textcolor);
    bounded_text(*this, TS_ENGRAVED, 170, 55, 75, 25, ALIGN_LEFT|ALIGN_MIDDLE, "Routing", textcolor);
    bounded_text(*this, TS_ENGRAVED, 245, 55, 95, 25, ALIGN_LEFT|ALIGN_MIDDLE, "Modulator", textcolor);
    bounded_text(*this, TS_ENGRAVED, 370, 185, 65, 25, ALIGN_CENTER|ALIGN_MIDDLE, "Chorus", textcolor);
    bounded_text(*this, TS_ENGRAVED, 490, 185, 65, 25, ALIGN_CENTER|ALIGN_MIDDLE, "Vibrato", textcolor);
    bounded_text(*this, TS_ENGRAVED, 30, 245, 125, 25, ALIGN_LEFT|ALIGN_MIDDLE, "Low-pass filter", textcolor);
}

void Chorus_UI::Impl::knobValueChanged(Knob *knob, float value)
{
    if (knob == kn_mod_range_.get()) {
        Q->setParameterValue(ECP_MOD_RANGE, value);
    }
    else if (knob == kn_slow_rate_.get()) {
        Q->setParameterValue(ECP_SLOW_RATE, value);
    }
    else if (knob == kn_fast_rate_.get()) {
        Q->setParameterValue(ECP_FAST_RATE, value);
    }
    else if (knob == kn_slow_rand_.get()) {
        Q->setParameterValue(ECP_SLOW_RAND, value);
    }
    else if (knob == kn_fast_rand_.get()) {
        Q->setParameterValue(ECP_FAST_RAND, value);
    }
    else if (knob == kn_lpf_cutoff_.get()) {
        Q->setParameterValue(ECP_LPF_CUTOFF, value);
    }
    else if (knob == kn_lpf_q_.get()) {
        Q->setParameterValue(ECP_LPF_Q, value);
    }
}

void Chorus_UI::Impl::buttonValueChanged(Button *btn, bool value)
{
    if (btn == btn_enable1_.get()) {
        Q->setParameterValue(ECP_ENABLE1, value);
    }
    else if (btn == btn_enable2_.get()) {
        Q->setParameterValue(ECP_ENABLE2, value);
    }
    else if (btn == btn_enable3_.get()) {
        Q->setParameterValue(ECP_ENABLE3, value);
    }
    else if (btn == btn_enable4_.get()) {
        Q->setParameterValue(ECP_ENABLE4, value);
    }
    else if (btn == btn_enable5_.get()) {
        Q->setParameterValue(ECP_ENABLE5, value);
    }
    else if (btn == btn_enable6_.get()) {
        Q->setParameterValue(ECP_ENABLE6, value);
    }
}

UI *DISTRHO::createUI()
{
    return new Chorus_UI;
}
