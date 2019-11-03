#pragma once
#include "BasicWidget.hpp"
#include <functional>

class LookAndFeel;

class Slider : public BasicWidget {
public:
    Slider(Widget *group, LookAndFeel &lnf);

    enum Orientation {
        Horizontal,
        Vertical,
        Radial,
    };

    double value() const noexcept { return fValue; }
    void setValue(double value);

    bool valueNotified() const { return fValueNotify; }
    void setValueNotified(bool notified);

    void setValueBounds(double v1, double v2);
    void setDefaultValue(double vd);
    void unsetDefaultValue();
    void setNumSteps(unsigned numSteps);
    void setOrientation(Orientation ori);
    void setLogarithmic(bool loga);

    bool onMouse(const MouseEvent &event) override;
    bool onMotion(const MotionEvent &event) override;
    bool onScroll(const ScrollEvent &event) override;
    void onDisplay() override;

    std::function<void(double)> ValueChangedCallback;

private:
    double clampToBounds(double value);

    double valueToProportion(double value);
    double proportionToValue(double prop);

private:
    double fValue = 0;
    double fValueBound1 = 0, fValueBound2 = 1;
    double fDefaultValue = 0;
    bool fHasDefaultValue = false;
    unsigned fNumSteps = 100;
    Orientation fOrientation = Horizontal;
    bool fLogarithm = false;
    double fAngleMin = -2.3561945;
    double fAngleMax = +2.3561945;
    bool fValueNotify = true;
    bool fIsDragging = false;
    LookAndFeel &fLookAndFeel;
};
