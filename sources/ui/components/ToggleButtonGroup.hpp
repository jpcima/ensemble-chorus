#pragma once
#include "Widget.hpp"
#include <functional>
#include <vector>
#include <memory>

class ToggleButton;
class LookAndFeel;

class ToggleButtonGroup {
public:
    explicit ToggleButtonGroup(LookAndFeel &lnf);
    ~ToggleButtonGroup();

    ToggleButton *createButton(Widget *group, double value);

    size_t size() const { return fButtons.size(); }
    ToggleButton *button(size_t index) const { return fButtons[index]->button.get(); }

    double value() const;
    void setValue(double value);

    std::function<void(double)> ValueChangedCallback;

private:
    struct ValuedButton {
        std::unique_ptr<ToggleButton> button;
        double value = 0.0;
    };

    std::vector<std::unique_ptr<ValuedButton>> fButtons;
    double fValue = 0.0;

    LookAndFeel &fLookAndFeel;

    void updateButtonStates();
    void updateButtonState(const ValuedButton &slot);
};
