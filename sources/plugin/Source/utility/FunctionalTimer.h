//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"
#include <utility>

class FunctionalTimer : public Timer {
public:
    virtual ~FunctionalTimer() {}
    template <class T> static Timer *create(T fn);
};

template <class T>
class FunctionalTimerT : public FunctionalTimer {
public:
    explicit FunctionalTimerT(T fn) : fn_(std::move(fn)) {}
    void timerCallback() override { fn_(); }
private:
    T fn_;
};

template <class T>
Timer *FunctionalTimer::create(T fn)
{
    return new FunctionalTimerT<T>(std::move(fn));
}
