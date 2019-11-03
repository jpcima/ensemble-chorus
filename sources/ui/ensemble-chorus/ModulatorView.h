#pragma once
#include "ui/components/BasicWidget.hpp"

class LookAndFeel;

class ModulatorView : public BasicWidget {
public:
    explicit ModulatorView(Widget *group, LookAndFeel &lnf);

    double value1() const { return fValue1; }
    void setValue1(double value);

    double value2() const { return fValue2; }
    void setValue2(double value);

    void onDisplay() override;

private:
    LookAndFeel &fLookAndFeel;
    double fValue1 = 0.0;
    double fValue2 = 0.0;
};
