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

#include "bbd.h"
#include <algorithm>
#include <limits>

inline void BBD_Clock::reset()
{
}

inline float BBD_Clock::tick(float rate)
{
    float p = p_ + rate;
    int i = (int)p;
    p_ = p - i;
    p = std::max(p - 1, std::numeric_limits<float>::epsilon());
    return (i > 0) ? p : 0;
}

inline float BBD_Clock::hz_rate_for_delay(float delay, unsigned stages)
{
    return stages / (2 * delay);
}

inline float BBD_Clock::delay_for_hz_rate(float rate, unsigned stages)
{
    return stages / (2 * rate);
}
