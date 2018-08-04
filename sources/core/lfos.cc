//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "lfos.h"
#include "dsp.h"
#include <memory>
#include <cmath>
#include <cstring>
#include <cstdint>

struct LFOs::Impl {
    float fs_ = 0;
    float f_ = 0;
    float phase_ = 0;
    LFOs::Shape shape_ = LFOs::Shape::Sine;
    unsigned nlfos_ = 0;
    float rndamt_ = 0;
    uint32_t rndseed_ = 0;
    unsigned max_lfos_ = 0;
    std::unique_ptr<float[]> phaseoff_;
    std::unique_ptr<float[]> tmpbuf_;
    std::unique_ptr<float[]> last_value_;
    static float wrap_phase(float p);
};

static constexpr unsigned lfos_sintablen = 512;
static float lfos_sintable[lfos_sintablen + 1];
static float *lfos_sintabptr = nullptr;

LFOs::LFOs()
    : P(new Impl)
{
    if (!lfos_sintabptr) {
        for (unsigned i = 0; i < lfos_sintablen + 1; ++i)
            lfos_sintable[i] = 0.5 * (1 + std::sin(2 * M_PI * i / lfos_sintablen));
        lfos_sintabptr = lfos_sintable;
    }
}

LFOs::~LFOs()
{
}

bool LFOs::setup(float samplerate, unsigned bufsize, unsigned max_lfos)
{
    Impl &P = *this->P;
    P.fs_ = samplerate;
    P.phaseoff_.reset(new float[max_lfos]());
    P.tmpbuf_.reset(new float[bufsize]);
    P.last_value_.reset(new float[max_lfos]());
    P.max_lfos_ = max_lfos;
    return true;
}

LFOs::Shape LFOs::shape() const noexcept
{
    const Impl &P = *this->P;
    return P.shape_;
}

void LFOs::shape(Shape s) noexcept
{
    Impl &P = *this->P;
    P.shape_ = s;
}

float LFOs::random() const noexcept
{
    const Impl &P = *this->P;
    return P.rndamt_;
}

void LFOs::random(float r) noexcept
{
    Impl &P = *this->P;
    P.rndamt_ = r;
}

const float *LFOs::last_value() const noexcept
{
    const Impl &P = *this->P;
    return P.last_value_.get();
}

unsigned LFOs::phases(float *p) const noexcept
{
    const Impl &P = *this->P;
    unsigned n = P.nlfos_;

    const float *phaseoffs = P.phaseoff_.get();
    for (unsigned i = 0; i < n; ++i)
        p[i] = phaseoffs[i];

    return n;
}

void LFOs::phases(const float *p, unsigned n) noexcept
{
    Impl &P = *this->P;
    unsigned max_lfos = P.max_lfos_;
    n = (n < max_lfos) ? n : max_lfos;
    P.nlfos_ = n;

    float *phaseoffs = P.phaseoff_.get();
    for (unsigned i = 0; i < n; ++i)
        phaseoffs[i] = p[i];
}

void LFOs::perform(float f_hz, float *outs[], unsigned n) noexcept
{
    Impl &P = *this->P;

    float *tmpbuf = P.tmpbuf_.get();
    for (unsigned i = 0; i < n; ++i)
        tmpbuf[i] = f_hz;

    perform(tmpbuf, outs, n);
}

void LFOs::perform(const float *f_hz, float *outs[], unsigned n) noexcept
{
    if (n <= 0)
        return;

    Impl &P = *this->P;

    const float fs = P.fs_;
    const float ts = 1 / fs;

    const unsigned nlfos = P.nlfos_;
    const float phase0 = P.phase_;
    const Shape shape = P.shape_;
    const float rndamt = P.rndamt_;
    uint32_t rndseed = P.rndseed_;
    const float *phaseoffs = P.phaseoff_.get();
    float *last_value = P.last_value_.get();

    if (rndamt != 0) {
        float *tmpbuf = P.tmpbuf_.get();
        for (unsigned i = 0; i < n; ++i) {
            float f = f_hz[i];
            f *= 1 + rndamt * ((int32_t)dsp::fastrandom(&rndseed) * (1.0f / INT32_MAX));
            tmpbuf[i] = f;
        }
        f_hz = tmpbuf;
    }

    float phase = 0;
    for (unsigned ilfo = 0; ilfo < nlfos; ++ilfo) {
        float *out = outs[ilfo];
        const float phaseoff = phaseoffs[ilfo];
        phase = phase0;

        switch (shape) {
        case Shape::Sine:
            for (unsigned i = 0; i < n; ++i) {
                float f = f_hz[i];
                float p = Impl::wrap_phase(phase + phaseoff);
                const float *tab = lfos_sintabptr;
                float index = p * lfos_sintablen;
                unsigned i0 = (unsigned)index;
                float delta = index - i0;
                out[i] = tab[i0] * (1 - delta) + tab[i0 + 1] * delta;
                phase += f * ts;
            }
            break;

        case Shape::Square:
            for (unsigned i = 0; i < n; ++i) {
                float f = f_hz[i];
                float p = Impl::wrap_phase(phase + phaseoff);
                out[i] = (p < 0.5f) ? 0 : 1;
                phase += f * ts;
            }
            break;

        case Shape::Saw:
            for (unsigned i = 0; i < n; ++i) {
                float f = f_hz[i];
                float p = Impl::wrap_phase(phase + phaseoff);
                out[i] = 1 - p;
                phase += f * ts;
            }
            break;

        case Shape::Ramp:
            for (unsigned i = 0; i < n; ++i) {
                float f = f_hz[i];
                float p = Impl::wrap_phase(phase + phaseoff);
                out[i] = p;
                phase += f * ts;
            }
            break;

        case Shape::Triangle:
            for (unsigned i = 0; i < n; ++i) {
                float f = f_hz[i];
                float p = Impl::wrap_phase(phase + phaseoff);
                float ramp = 2 * p;
                out[i] = (p < 0.5f) ? ramp : (2 - ramp);
                phase += f * ts;
            }
            break;
        }

        last_value[ilfo] = out[n - 1];
    }

    P.phase_ = Impl::wrap_phase(phase);
    P.rndseed_ = rndseed;
}

float LFOs::Impl::wrap_phase(float p)
{
    p -= (int)p;
    return (p < 0) ? (p + 1) : p;
}
