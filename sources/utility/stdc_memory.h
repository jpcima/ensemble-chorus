//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <memory>
#include <cstdlib>

struct STDC_Deleter {
    void operator()(void *p)
        { std::free(p); }
};

template <class T>
using stdc_unique_ptr = std::unique_ptr<T, STDC_Deleter>;
