#pragma once
#include "PluginEnsembleChorus.hpp"
#include "DistrhoUI.hpp"
#include "ui/Geometry.h"
#include <jsl/dynarray>
#include <vector>
#include <string>

class FontEngine;
class Slider;
class ToggleButton;
class ToggleButtonGroup;
class TextLabel;
class ModulatorView;
class LookAndFeel;

class UIEnsembleChorus : public UI {
public:
    UIEnsembleChorus();
    ~UIEnsembleChorus();

protected:
    void parameterChanged(uint32_t index, float value) override;
    void programLoaded(uint32_t index) override;

    void uiIdle() override;

    void onDisplay() override;

private:
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UIEnsembleChorus)

    static const std::string *initializeFontDataOnce();
    void initializeFonts(const std::string *fontData);

    void createWidgets();

    Slider *createSliderForParameter(uint32_t index, const Rect &rect);
    ToggleButton *createToggleButtonForParameter(uint32_t index, const Rect &rect);
    ToggleButtonGroup *createToggleButtonGroupForParameter(uint32_t index, const jsl::dynarray<Rect> &rects);

    template <class W, class... A> W *createSubwidget(A &&... a)
    {
        W *w = new W(std::forward<A>(a)...);
        return fSubwidgets.push_back(std::unique_ptr<W>{w}), w;
    }

    std::unique_ptr<Parameter[]> fParameters;
    std::unique_ptr<FontEngine> fFontEngine;
    std::unique_ptr<LookAndFeel> fLookAndFeel;
    std::unique_ptr<Slider> fParameterSlider[EC_PARAMETER_COUNT];
    std::unique_ptr<ToggleButton> fParameterToggleButton[EC_PARAMETER_COUNT];
    std::unique_ptr<ToggleButtonGroup> fParameterToggleButtonGroup[EC_PARAMETER_COUNT];
    std::unique_ptr<TextLabel> fPhaseLabel[6];
    std::unique_ptr<ModulatorView> fModulatorView[6];
    std::vector<std::unique_ptr<Widget>> fSubwidgets;
};
