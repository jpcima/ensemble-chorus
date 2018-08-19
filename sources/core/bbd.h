/* Digital model of a bucket brigade delay (BBD)
 *
 * Puredata adaptation of software located at
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

#pragma once
#include "ensemble_chorus.h"
#include <memory>

class BBD_Line {
public:
    BBD_Line();
    ~BBD_Line();
    bool setup(float samplerate, float maxclockrate, unsigned nstages);
    void process(unsigned n, float *inout, const float *clock);
    void regen(float r);
    void nstages(unsigned n);

    static constexpr float min_delay = 1e-5f;

    static constexpr unsigned supported_nstages_count = EC_SUPPORTED_NSTAGES_COUNT;
    static constexpr unsigned nstages_min = EC_NSTAGES_MIN;
    static constexpr unsigned nstages_max = EC_NSTAGES_MAX;

    static unsigned adjust_nstages(unsigned n);

private:
    struct Impl;
    std::unique_ptr<Impl> P;
};

class BBD_Clock {
public:
    void reset();
    float tick(float rate);
    static float hz_rate_for_delay(float delay, unsigned stages);
    static float delay_for_hz_rate(float rate, unsigned stages);

private:
    float p_ = 0;
};

#include "bbd.tcc"
