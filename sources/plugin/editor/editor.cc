//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "editor.h"
#include "widgets/knob.h"
#include "graphics/box.h"
#include "graphics/font.h"
#include "ensemble_chorus.h"

struct Chorus_UI::Impl : public Knob::Callback {
    Chorus_UI *const Q = nullptr;

    Color bg_{0xc0, 0xc0, 0xc0};
    std::unique_ptr<FontCollection> fonts_;

    std::unique_ptr<Knob> kn_mod_range_;
    std::unique_ptr<Knob> kn_slow_rate_;
    std::unique_ptr<Knob> kn_fast_rate_;
    std::unique_ptr<Knob> kn_slow_rand_;
    std::unique_ptr<Knob> kn_fast_rand_;
    std::unique_ptr<Knob> kn_lpf_cutoff_;
    std::unique_ptr<Knob> kn_lpf_q_;

    explicit Impl(Chorus_UI *q)
        : Q(q) {}

    void knobDragStarted(Knob *knob) override {}
    void knobDragFinished(Knob *knob) override {}
    void knobValueChanged(Knob *knob, float value) override;
};

Chorus_UI::Chorus_UI()
    : UI(625, 280),
      P(new Impl(this))
{
    Window &win = getParentWindow();

    P->fonts_.reset(new FontCollection(*this));

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
    switch (index) {
    case ECP_BYPASS:
        // TODO
        break;
    case ECP_CHANNEL_LAYOUT:
        // TODO
        break;
    case ECP_DELAY:
        // TODO
        break;
    case ECP_NSTAGES:
        // TODO
        break;
    case ECP_MOD_RANGE:
        P->kn_mod_range_->setValue(value);
        break;
    case ECP_SLOW_RATE:
        P->kn_slow_rate_->setValue(value);
        break;
    case ECP_SLOW_WAVE:
        // TODO
        break;
    case ECP_SLOW_RAND:
        P->kn_slow_rand_->setValue(value);
        break;
    case ECP_FAST_RATE:
        P->kn_fast_rate_->setValue(value);
        break;
    case ECP_FAST_WAVE:
        // TODO
        break;
    case ECP_FAST_RAND:
        P->kn_fast_rand_->setValue(value);
        break;
    case ECP_LPF_CUTOFF:
        P->kn_lpf_cutoff_->setValue(value);
        break;
    case ECP_LPF_Q:
        P->kn_lpf_q_->setValue(value);
        break;
    case ECP_GAIN_IN:
        // TODO
        break;
    case ECP_GAIN_OUT:
        // TODO
        break;
    case ECP_MIX_DRY:
        // TODO
        break;
    case ECP_MIX_WET:
        // TODO
        break;

    case ECP_ENABLE1:
        // TODO
        break;
    case ECP_PHASE1:
        // TODO
        break;
    case ECP_DEPTH1:
        // TODO
        break;
    case ECP_ROUTE_L1:
        // TODO
        break;
    case ECP_ROUTE_R1:
        // TODO
        break;

    case ECP_ENABLE2:
        // TODO
        break;
    case ECP_PHASE2:
        // TODO
        break;
    case ECP_DEPTH2:
        // TODO
        break;
    case ECP_ROUTE_L2:
        // TODO
        break;
    case ECP_ROUTE_R2:
        // TODO
        break;

    case ECP_ENABLE3:
        // TODO
        break;
    case ECP_PHASE3:
        // TODO
        break;
    case ECP_DEPTH3:
        // TODO
        break;
    case ECP_ROUTE_L3:
        // TODO
        break;
    case ECP_ROUTE_R3:
        // TODO
        break;

    case ECP_ENABLE4:
        // TODO
        break;
    case ECP_PHASE4:
        // TODO
        break;
    case ECP_DEPTH4:
        // TODO
        break;
    case ECP_ROUTE_L4:
        // TODO
        break;
    case ECP_ROUTE_R4:
        // TODO
        break;

    case ECP_ENABLE5:
        // TODO
        break;
    case ECP_PHASE5:
        // TODO
        break;
    case ECP_DEPTH5:
        // TODO
        break;
    case ECP_ROUTE_L5:
        // TODO
        break;
    case ECP_ROUTE_R5:
        // TODO
        break;

    case ECP_ENABLE6:
        // TODO
        break;
    case ECP_PHASE6:
        // TODO
        break;
    case ECP_DEPTH6:
        // TODO
        break;
    case ECP_ROUTE_L6:
        // TODO
        break;
    case ECP_ROUTE_R6:
        // TODO
        break;
    }
}

void Chorus_UI::onNanoDisplay()
{
    int w = getWidth();
    int h = getHeight();
    FontCollection &fonts = *P->fonts_;
    Color bg = P->bg_;

    beginPath();
    rect(0, 0, w, h);
    fillColor(bg);
    fill();

    {
        int bx = 0;
        int by = 0;
        int bw = 230;
        int bh = 42;
        shadow_box(*this, bx, by, bw, bh, Color(0xc0, 0xc0, 0xc0));
        fontFaceId(fonts.getSerifBoldItalic());
        fontSize(22);
        beginPath();
        fillColor(Color(0.0f, 0.0f, 0.0f));
        textAlign(ALIGN_CENTER|ALIGN_MIDDLE);
        textBox(bx, by + 0.5f * bh, bw, "JPC Ensemble Chorus");
    }
}

void Chorus_UI::Impl::knobValueChanged(Knob *knob, float value)
{
    if (knob == kn_mod_range_.get()) {
        Q->setParameterValue(ECP_MOD_RANGE, value);
    }
    else if (knob == kn_slow_rate_.get()) {
        Q->setParameterValue(ECP_SLOW_RATE, value);
    }
    else if (knob == kn_fast_rate_.get()) {
        Q->setParameterValue(ECP_FAST_RATE, value);
    }
    else if (knob == kn_slow_rand_.get()) {
        Q->setParameterValue(ECP_SLOW_RAND, value);
    }
    else if (knob == kn_fast_rand_.get()) {
        Q->setParameterValue(ECP_FAST_RAND, value);
    }
    else if (knob == kn_lpf_cutoff_.get()) {
        Q->setParameterValue(ECP_LPF_CUTOFF, value);
    }
    else if (knob == kn_lpf_q_.get()) {
        Q->setParameterValue(ECP_LPF_Q, value);
    }
}

UI *DISTRHO::createUI()
{
    return new Chorus_UI;
}
