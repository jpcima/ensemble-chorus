//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <ring_buffer/ring_buffer.h>
#include <chrono>
struct BasicMessage;

class MessageQueue {
public:
    explicit MessageQueue(size_t capacity);

    bool writeMessage(const BasicMessage &msg);
    bool readMessage(BasicMessage &msg);

    size_t sizeUsed() const;
    size_t sizeFree() const;

private:
    Ring_Buffer rb_;
};

#include "MessageQueue.tcc"
