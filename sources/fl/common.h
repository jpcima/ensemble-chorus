//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <memory>
#include <cstdio>

struct FILE_Deleter {
    void operator()(std::FILE *x) const noexcept
        { std::fclose(x); }
};
typedef std::unique_ptr<std::FILE, FILE_Deleter> FILE_u;
