//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <memory>

class LFOs {
public:
    enum class Shape {
        Sine, Square, Saw, Ramp, Triangle
    };

    LFOs();
    ~LFOs();

    bool setup(float samplerate, unsigned bufsize, unsigned max_lfos);

    Shape shape() const noexcept;
    void shape(Shape s) noexcept;

    float random() const noexcept;
    void random(float r) noexcept;

    const float *last_value() const noexcept;

    unsigned phases(float *p) const noexcept;
    void phases(const float *p, unsigned n) noexcept;

    void perform(float f_hz, float *outs[], unsigned n) noexcept;
    void perform(const float *f_hz, float *outs[], unsigned n) noexcept;

private:
    struct Impl;
    std::unique_ptr<Impl> P;
};
