#include "ToggleButtonGroup.hpp"
#include "ToggleButton.hpp"

ToggleButtonGroup::ToggleButtonGroup(LookAndFeel &lnf)
    : fLookAndFeel(lnf)
{
    fButtons.reserve(16);
}

ToggleButtonGroup::~ToggleButtonGroup()
{
}

ToggleButton *ToggleButtonGroup::createButton(Widget *group, double value)
{
    size_t index = fButtons.size();
    fButtons.emplace_back(new ValuedButton);
    ValuedButton &slot = *fButtons[index];

    ToggleButton *button = new ToggleButton(group, fLookAndFeel);
    slot.button.reset(button);
    slot.value = value;

    button->ValueChangedCallback = [this, index](bool value) {
                                       if (value)
                                           setValue(fButtons[index]->value);
                                   };

    updateButtonState(slot);
    return button;
}

double ToggleButtonGroup::value() const
{
    return fValue;
}

void ToggleButtonGroup::setValue(double value)
{
    if (value == fValue)
        return;

    fValue = value;
    updateButtonStates();

    if (ValueChangedCallback)
        ValueChangedCallback(value);
}

void ToggleButtonGroup::updateButtonStates()
{
    for (size_t i = 0, n = fButtons.size(); i < n; ++i)
        updateButtonState(*fButtons[i]);
}

void ToggleButtonGroup::updateButtonState(const ValuedButton &slot)
{
    ToggleButton &button = *slot.button;
    bool pressed = slot.value == fValue;
    button.setValue(pressed);
    button.setInteractible(!pressed);
}
