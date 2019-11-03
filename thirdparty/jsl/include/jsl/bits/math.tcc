//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <jsl/math>
#include <cmath>

namespace jsl {

template <class R>
inline R clamp(R x, R min, R max)
{
  x = (x < min) ? min : x;
  x = (x > max) ? max : x;
  return x;
}

template <class R>
inline R square(R x)
{
    return x * x;
}

template <class R>
inline R cube(R x)
{
    return x * x * x;
}

template <class R>
R sinc(R x)
{
    return (x == 0) ? 1 : (std::sin(x) / x);
}

template <class R>
R binom(unsigned n, unsigned k)
{
  R r = 1;
  for (unsigned i = 1; i <= k; ++i)
    r *= (n + 1 - i) / (R)i;
  return r;
}

template <class R>
void poly(gsl::span<const R> z, gsl::span<R> p)
{
    Expects(p.size() == z.size() + 1);
    size_t n = z.size();
    p[0] = 1;
    for (size_t j = 0; j < n; ++j)
        p[j + 1] = 0;
    for (size_t j = 0; j < n; ++j)
        for (size_t k = j + 1; k-- > 0;)
            p[k + 1] -= z[j] * p[k];
}

template <class R>
R polyval(gsl::span<const R> p, R x)
{
    R y = 0;
    R xi = 1;
    for (size_t i = 0, n = p.size(); i < n; ++i) {
        y += xi * p[i];
        xi *= x;
    }
    return y;
}

namespace ilog2_detail {

template <class T>
struct ilog2 {
    T operator()(T value)
    {
        T l = 0;
        while((value >> l) > 1)
            ++l;
        return l;
    }
};

#if defined(__GNUC__)
template <>
struct ilog2<unsigned> {
    unsigned operator()(unsigned value)
    {
        return sizeof(unsigned) * 8 - __builtin_clz(value) - 1;
    }
};

template <>
struct ilog2<unsigned long> {
    unsigned long operator()(unsigned long value)
    {
        return sizeof(unsigned long) * 8 - __builtin_clzl(value) - 1;
    }
};

template <>
struct ilog2<unsigned long long> {
    unsigned long long operator()(unsigned long long value)
    {
        return sizeof(unsigned long long) * 8 - __builtin_clzll(value) - 1;
    }
};
#endif

}  // namespace ilog2_detail

template <class T>
T ilog2(T value)
{
    ilog2_detail::ilog2<T> fn;
    return fn(value);
}

}  // namespace jsl
