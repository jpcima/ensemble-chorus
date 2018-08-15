//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <dgl/NanoVG.hpp>

class Modulation_View : public NanoWidget {
public:
    explicit Modulation_View(NanoWidget *groupWidget);

    void setValues(float slow, float fast);

protected:
    void onNanoDisplay() override;

private:
    float slow_ = 0.5f;
    float fast_ = 0.5f;
};
