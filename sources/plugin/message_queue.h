//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <ring_buffer/ring_buffer.h>
#include <chrono>
struct Basic_Message;

class Message_Queue {
public:
    explicit Message_Queue(size_t capacity);

    bool write_message(const Basic_Message &msg);
    void write_message_retrying(const Basic_Message &msg, std::chrono::milliseconds delay);

    bool read_message(Basic_Message &msg);

    size_t size_used() const;
    size_t size_free() const;

private:
    Ring_Buffer rb_;
};

#include "message_queue.tcc"
