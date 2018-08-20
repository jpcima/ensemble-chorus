//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cassert>

#define EACH_MESSAGE_TYPE(F)                    \
    F(NotifyModulation)

enum class MessageTag {
    #define DECLARE_MEMBER(x) x,
    EACH_MESSAGE_TYPE(DECLARE_MEMBER)
    #undef DECLARE_MEMBER
};

struct BasicMessage {
    explicit BasicMessage(MessageTag tag)
        : tag(tag) {}
    const MessageTag tag;
};

template <MessageTag Tag>
struct BasicMessageT : public BasicMessage {
    BasicMessageT()
        : BasicMessage(Tag) {}
};

namespace Messages {
    #define DEFMESSAGE(t)                                   \
        struct t : public BasicMessageT<MessageTag::t>

    DEFMESSAGE(NotifyModulation) {
        float slow[6];
        float fast[6];
    };

    #undef DEFMESSAGE

    size_t sizeOf(MessageTag tag);
    uint8_t *allocateBuffer();
}

#include "Messages.tcc"
