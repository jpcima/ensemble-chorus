#pragma once
#include "BasicWidget.hpp"
#include <functional>

class LookAndFeel;

class ToggleButton : public BasicWidget {
public:
    ToggleButton(Widget *group, LookAndFeel &lnf);

    bool value() const noexcept { return fValue; }
    void setValue(bool value);

    bool interactible() const { return fInteractible; }
    void setInteractible(bool interactible) { fInteractible = interactible; }

    bool valueNotified() const { return fValueNotify; }
    void setValueNotified(bool notified);

    bool hasInvertedAppearance() const { return fHasInvertedAppearance; }
    void setHasInvertedAppearance(bool inv);

    bool onMouse(const MouseEvent &event) override;
    void onDisplay() override;

    std::function<void(bool)> ValueChangedCallback;

private:
    bool fValue = false;
    bool fIsPressed = false;
    bool fHasInvertedAppearance = false;
    bool fInteractible = true;
    bool fValueNotify = true;
    LookAndFeel &fLookAndFeel;
};
