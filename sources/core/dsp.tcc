//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "dsp.h"
#include <jsl/math>
#include <gsl.hpp>

namespace dsp {

template <class R>
auto coef_t<R>::padded(unsigned minsize, unsigned multiple) const -> coef_t
{
    Ensures(multiple > 0);

    const jsl::dynarray<R> &ob = this->b, &oa = this->a;
    unsigned nb = ob.size(), na = oa.size();
    while (nb > 0 && ob[nb - 1] == 0) --nb;
    while (na > 0 && oa[na - 1] == 0) --na;

    unsigned nn = std::max(minsize, std::max(nb, na));
    nn += multiple - 1;
    nn -= nn % multiple;

    jsl::dynarray<R> pb(nn), pa(nn);
    std::copy(ob.begin(), ob.begin() + nb, pb.begin());
    std::copy(oa.begin(), oa.begin() + na, pa.begin());
    return coef_t{std::move(pb), std::move(pa)};
}

template <class R>
void coef_t<R>::normalize()
{
    jsl::dynarray<R> &b = this->b;
    jsl::dynarray<R> &a = this->a;
    unsigned nb = b.size();
    unsigned na = a.size();
    // Trim leading zeros in denominator, leave at least one.
    unsigned nlz = 0;
    for (unsigned i = 0; i < na && a[i] == 0; ++i)
        ++nlz;
    if (nlz == na)
        --nlz;
    if (nlz > 0) {
        na -= nlz;
        a.assign(a.begin() + nlz, a.end());
    }
    // Normalize transfer function
    R k = a[0];
    for (unsigned i = 0; i < nb; ++i)
        b[i] /= k;
    for (unsigned i = 0; i < na; ++i)
        a[i] /= k;
}

//------------------------------------------------------------------------------
template <class R>
iir_t<R>::iir_t(const coef_t<R> &c)
{
    coef_t<R> &pc = this->c = c.padded(1);
    unsigned n = pc.a.size();
    this->x.reset(n);
    this->y.reset(n);
}

template <class R>
void iir_t<R>::reset()
{
    this->x.fill(0);
    this->y.fill(0);
}

template <class R>
R iir_t<R>::tick(R in)
{
    const jsl::dynarray<R> &a = this->c.a, &b = this->c.b;
    jsl::dynarray<R> &x = this->x, &y = this->y;
    unsigned n = a.size();

    R r = in * b[0];
    unsigned i = (this->i + n - 1) % n;
    x[i] = in;

    for (unsigned j = 1; j < n; ++j) {
        unsigned jj = (i + j) % n;
        r += x[jj] * b[j] - y[jj] * a[j];
    }

    r /= a[0];
    y[i] = r;
    this->i = i;
    return r;
}

//------------------------------------------------------------------------------
template <class R>
coef_t<R> bilinear(const coef_t<R> &in, R fs)
{
    const jsl::dynarray<R> &a = in.a;
    const jsl::dynarray<R> &b = in.b;
    unsigned D = a.size() - 1;
    unsigned N = b.size() - 1;
    unsigned M = (N > D) ? N : D;
    unsigned Np = M + 1;
    unsigned Dp = M + 1;

    jsl::dynarray<R> bprime(Np);
    jsl::dynarray<R> aprime(Dp);

    for (unsigned j = 0; j < Np; ++j) {
        R val = 0;
        for (unsigned i = 0; i < N + 1; ++i)
            for (unsigned k = 0; k < i + 1; ++k)
                for (unsigned l = 0; l < M - i + 1; ++l)
                    if (k + l == j)
                        val += jsl::binom<R>(i, k) *
                            jsl::binom<R>(M - i, l) * b[N - i] *
                            std::pow(2 * fs, i) * ((k & 1) ? -1 : 1);
        bprime[j] = val;
    }
    for (unsigned j = 0; j < Dp; ++j) {
        R val = 0;
        for (unsigned i = 0; i < D + 1; ++i)
            for (unsigned k = 0; k < i + 1; ++k)
                for (unsigned l = 0; l < M - i + 1; ++l)
                    if (k + l == j)
                        val += jsl::binom<R>(i, k) *
                            jsl::binom<R>(M - i, l) * a[D - i] *
                            std::pow(2 * fs, i) * ((k & 1) ? -1 : 1);
        aprime[j] = val;
    }

    coef_t<R> out{std::move(bprime), std::move(aprime)};
    out.normalize();
    return out;
}

//------------------------------------------------------------------------------
inline uint32_t fastrandom(uint32_t *pseed)
{
    return *pseed = *pseed * 1664525u + 1013904223u;
}

template <class R>
inline R white(uint32_t *pseed)
{
    return (int32_t)fastrandom(pseed) * (1 / (R)INT32_MAX);
}

//------------------------------------------------------------------------------
template <class Filter, class R>
void process_mono_DspFilter(Filter &f, R *in, unsigned count)
{
    R *channels[1] = {in};
    f.process(count, channels);
}

}  // namespace dsp
