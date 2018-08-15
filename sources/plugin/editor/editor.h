//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <distrho/DistrhoUI.hpp>
#include <memory>

class Chorus_UI : public UI {
public:
    Chorus_UI();

protected:
    void parameterChanged(uint32_t index, float value) override;
    void onNanoDisplay() override;
    void uiIdle() override;

private:
    struct Impl;
    const std::unique_ptr<Impl> P;
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Chorus_UI)
};
