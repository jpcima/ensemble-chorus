//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "messages.h"

namespace Messages {

size_t size_of(Message_Tag tag)
{
    size_t size = 0;
    switch (tag) {
        #define HANDLE_CASE(x) case Message_Tag::x: size = sizeof(x); break;
        EACH_MESSAGE_TYPE(HANDLE_CASE)
        #undef HANDLE_CASE
    default:
            assert(false);
    }
    return size;
}

uint8_t *allocate_buffer()
{
    size_t size = 0;
    #define COMPUTE_MAX(x) size = (size < sizeof(Messages::x)) ? sizeof(Messages::x) : size;
    EACH_MESSAGE_TYPE(COMPUTE_MAX)
    #undef COMPUTE_MAX
    return new uint8_t[size];
}

}  // namespace Messages
