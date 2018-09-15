/* Digital model of a bucket brigade delay (BBD)
 *
 * Adaptation of software located at
 *     http://colinraffel.com/software/bbdmodeling/echo.cpp
 *
 * References
 *     Raffel, C., & Smith, J. (2010, September).
 *     Practical modeling of bucket-brigade device circuits.
 *
 * Copyright (C) 2017-2018 Jean-Pierre Cimalando.
 * Copyright (C) 2010 Colin Raffel.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// Implementation notes
//     filter computations for any sample rate
//     5th order Legendre as anti-aliasing filter
//     bilinear transform instead of MATLAB's invfreqz

#include "bbd.h"
#include "dsp.h"
#include <DspFilters/Butterworth.h>
#include <jsl/dynarray>
#include <string.h>
#include <stdint.h>

struct BBD_Line::Impl {
    static constexpr unsigned AA_order = 5;
    typedef Dsp::Butterworth::LowPass<AA_order> AA_Filter;
    typedef typename AA_Filter::State<Dsp::DirectFormII> AA_Filter_State;
    float aa_freq_ = 15e3;
    std::shared_ptr<AA_Filter> aa_filter_;
    AA_Filter_State aa_fstate_;
    dsp::iir_t<double> r1flt_;
    dsp::iir_t<double> r2flt_;
    dsp::iir_t<double> compflt_;
    dsp::iir_t<double> expdflt_;
    float regen_ = 0.02;
    float prevcompout_ = 1;
    float bbdout_ = 0;
    float prevbbdout_ = 0;
    float previnval_ = 0;
    uint32_t rndseed_ = 0;
    unsigned istage_ = 0;
    unsigned nstages_ = 0;
    float stages_[BBD_Line::nstages_max] = {};
    float samplerate_ = 0;
};

BBD_Line::BBD_Line()
    : P(new Impl) {
}

BBD_Line::~BBD_Line()
{
}

bool BBD_Line::setup(float samplerate, float maxclockrate, unsigned nstages, BBD_Line *other)
{
    Impl &P = *this->P;
    P.samplerate_ = samplerate;

    this->nstages(nstages);

    // anti aliasing filter
    if (other)
        P.aa_filter_ = other->P->aa_filter_;
    else {
        Impl::AA_Filter *aa_filter = new Impl::AA_Filter;
        P.aa_filter_.reset(aa_filter);
        aa_filter->setup(P.AA_order, samplerate, P.aa_freq_);
    }

    // reconstruction filter 1
    {
        double R = 10e3, C1 = .0022e-6, C2 = .033e-6, C3 = .001e-6;
        dsp::coef_t<double> analog;
        analog.b = {1};
        analog.a = {R*R*R*C1*C2*C3, R*R*2*C1*C3 + R*R*2*C2*C3, R*C1+R*C3, 1};
        dsp::coef_t<double> r1coef = dsp::bilinear<double>(analog, samplerate);
        P.r1flt_ = dsp::iir_t<double>(r1coef);
    }
    // reconstruction filter 2
    {
        double R = 10e3, C1 = .039e-6, C2 = .00033e-6;
        dsp::coef_t<double> analog;
        analog.b = {1};
        analog.a = {R*R*C1*C2, 2*R*C2, 1};
        dsp::coef_t<double> r2coef = dsp::bilinear<double>(analog, samplerate);
        P.r2flt_ = dsp::iir_t<double>(r2coef);
    }
    // averager
    {
        double C = .82e-6;
        double smoothing = (1 / samplerate) / (10000 * C + (1 / samplerate));
        dsp::coef_t<double> avgcoef;
        avgcoef.b = {smoothing, 0};
        avgcoef.a = {1, -1 + smoothing};
        P.compflt_ = dsp::iir_t<double>(avgcoef);
        P.expdflt_ = dsp::iir_t<double>(avgcoef);
    }

    return true;
}

void BBD_Line::process(unsigned n, float *inout, const float *clock)
{
    Impl &P = *this->P;

    auto &r1flt = P.r1flt_;
    auto &r2flt = P.r2flt_;
    auto &compflt = P.compflt_;
    auto &expdflt = P.expdflt_;

    const unsigned nstages = P.nstages_;
    float *stages = P.stages_;
    unsigned istage = P.istage_;

    const float regen = P.regen_;
    float prevcompout = P.prevcompout_;
    float prevbbdout = P.prevbbdout_;
    float bbdout = P.bbdout_;
    float previnval = P.previnval_;
    uint32_t rndseed = P.rndseed_;

    Impl::AA_Filter &aa_filter = *P.aa_filter_;
    Impl::AA_Filter_State &aa_fstate = P.aa_fstate_;

    for (unsigned i = 0; i < n; ++i) {
        float clkin = clock[i];
        // Compress
        float bbdin = (0.5f * inout[i] + prevbbdout) /
            ((float)compflt.tick(std::fabs(prevcompout)) + 1e-5f);
        // Remember compressor output
        prevcompout = bbdin;
        // Anti-aliasing filter
        aa_filter.process(1, &bbdin, aa_fstate);
        // Sampled input/output
        if (clkin > 0) {
            // Tick in linearly interpolated value, get out value
            float delayin = clkin * bbdin + (1 - clkin) * previnval;
            bbdout = stages[istage];
            stages[istage++] = delayin;
            istage = (istage != nstages) ? istage : 0;
        }

        // Waveshaping nonlinearity
        if (true) {
            float x = bbdout;
            constexpr float poly[] = {0.0/*1.0/16*/, 1.0, -1.0/166, -1.0/32};
            constexpr float deriv[3] = {poly[1], 2 * poly[2], 3 * poly[3]};
            if (x < -1) {
                const float a = jsl::polyval<float>(deriv, -1);
                bbdout = a * (x + 1) + jsl::polyval<float>(poly, -1);
            }
            else if (x > 1) {
                const float a = jsl::polyval<float>(deriv, 1);
                bbdout = a * (x - 1) + jsl::polyval<float>(poly, 1);
            }
            else
                bbdout = jsl::polyval<float>(poly, x);
        }

        // Add in -60 dB noise
        bbdout += 2e-3f * (dsp::white<float>(&rndseed) - 0.5f);

        // Reconstruction filters
        float recout = r1flt.tick(bbdout);
        recout = r2flt.tick(recout);
        // Expand
        recout *= (float)expdflt.tick(std::fabs(recout));

        inout[i] = recout;
        prevbbdout = regen * recout;
        previnval = bbdin;
    }

    P.istage_ = istage;
    P.prevcompout_ = prevcompout;
    P.bbdout_ = bbdout;
    P.prevbbdout_ = prevbbdout;
    P.previnval_ = previnval;
    P.rndseed_ = rndseed;
}

void BBD_Line::regen(float r)
{
    Impl &P = *this->P;
    P.regen_ = r;
}

void BBD_Line::nstages(unsigned n)
{
    Impl &P = *this->P;
    n = adjust_nstages(n);

    P.istage_ = 0;
    P.nstages_ = n;
    memset(P.stages_, 0, nstages_max * sizeof(float));
}

void BBD_Line::aa_cutoff(float cutoff)
{
    Impl &P = *this->P;
    Impl::AA_Filter &aa_filter = *P.aa_filter_;
    aa_filter.setup(P.AA_order, P.samplerate_, cutoff);
}

void BBD_Line::aa_reset()
{
    Impl &P = *this->P;
    P.aa_fstate_.reset();
}

unsigned BBD_Line::adjust_nstages(unsigned n)
{
    return 1u << jsl::ilog2(jsl::clamp(n, nstages_min, nstages_max));
}
