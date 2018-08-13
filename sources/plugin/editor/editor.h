//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <distrho/DistrhoUI.hpp>

class Chorus_UI : public UI {
public:
    Chorus_UI();
    void parameterChanged(uint32_t index, float value) override;
    void onNanoDisplay() override;

private:
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Chorus_UI)
};
