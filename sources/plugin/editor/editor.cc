//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "plugin/editor/editor.h"
#include "plugin/editor/widgets/knob.h"

struct Chorus_UI::Impl : public Knob::Callback {
    Color bg_{0xc0, 0xc0, 0xc0};
    std::unique_ptr<Knob> kn_mod_range_;
    std::unique_ptr<Knob> kn_slow_rate_;
    std::unique_ptr<Knob> kn_fast_rate_;
    std::unique_ptr<Knob> kn_slow_rand_;
    std::unique_ptr<Knob> kn_fast_rand_;
    std::unique_ptr<Knob> kn_lpf_cutoff_;
    std::unique_ptr<Knob> kn_lpf_q_;

    void knobDragStarted(Knob *knob) override {}
    void knobDragFinished(Knob *knob) override {}
    void knobValueChanged(Knob *knob, float value) override;
};

Chorus_UI::Chorus_UI()
    : UI(625, 280),
      P(new Impl)
{
    Window &win = getParentWindow();

#define KNOB(id, x, y, w, h)                    \
    Knob *kn_##id = new Knob(win);              \
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

#undef KNOB
}

void Chorus_UI::parameterChanged(uint32_t index, float value)
{
    // TODO
    
    
}

void Chorus_UI::onNanoDisplay()
{
    int w = getWidth();
    int h = getHeight();
    Color bg = P->bg_;

    rect(0, 0, w, h);
    fillColor(bg);
    fill();
}

void Chorus_UI::Impl::knobValueChanged(Knob *knob, float value)
{
    // TODO

    if (knob == kn_mod_range_.get()) {
        
    }
    else if (knob == kn_slow_rate_.get()) {
        
    }
    else if (knob == kn_fast_rate_.get()) {
        
    }
    else if (knob == kn_slow_rand_.get()) {
        
    }
    else if (knob == kn_fast_rand_.get()) {
        
    }
    else if (knob == kn_lpf_cutoff_.get()) {
        
    }
    else if (knob == kn_lpf_q_.get()) {
        
    }
}

UI *DISTRHO::createUI()
{
    return new Chorus_UI;
}
