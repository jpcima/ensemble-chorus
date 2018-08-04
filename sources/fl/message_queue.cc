//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "message_queue.h"
#include "messages.h"
#include <thread>

Message_Queue::Message_Queue(size_t capacity)
    : rb_(capacity)
{
}

bool Message_Queue::write_message(const Basic_Message &msg)
{
    Ring_Buffer &rb = rb_;
    size_t size = Messages::size_of(msg.tag);
    return rb.put(reinterpret_cast<const uint8_t *>(&msg), size);
}

void Message_Queue::write_message_retrying(const Basic_Message &msg, std::chrono::milliseconds delay)
{
    while (!write_message(msg))
        std::this_thread::sleep_for(delay);
}

bool Message_Queue::read_message(Basic_Message &msg)
{
    Ring_Buffer &rb = rb_;
    Basic_Message hdr((Message_Tag)0);
    if (!rb.peek(hdr))
        return false;

    size_t size = Messages::size_of(hdr.tag);
    return rb.get(reinterpret_cast<uint8_t *>(&msg), size);
}

size_t Message_Queue::size_used() const
{
    const Ring_Buffer &rb = rb_;
    return rb.size_used();
}

size_t Message_Queue::size_free() const
{
    const Ring_Buffer &rb = rb_;
    return rb.size_free();
}
