//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <jsl/dynarray>

namespace dsp {

// Coefficients H=[B,A]
template <class R>
struct coef_t {
    jsl::dynarray<R> b, a;
    // produce padded coefs so that |a|=|b|=n, n>=minsize and n%multiple=0
    coef_t padded(unsigned minsize, unsigned multiple = 1) const;
    // normalize for unit gain
    void normalize();
};

// IIR Direct Form I processor
template <class R>
struct iir_t {
    iir_t() noexcept {}
    explicit iir_t(const coef_t<R> &c);
    R tick(R in);
    void reset();
private:
    coef_t<R> c;
    unsigned i = 0;
    jsl::dynarray<R> x, y;
};

// Discretize analog filter with bilinear method
template <class R>
coef_t<R> bilinear(const coef_t<R> &in, R fs);

// generate a random number
uint32_t fastrandom(uint32_t *pseed);

// generate white noise
template <class R>
R white(uint32_t *pseed);

// DspFilters library helper
template <class Filter, class R>
void process_mono_DspFilter(Filter &f, R *in, unsigned count);

}  // namespace dsp

#include "dsp.tcc"
