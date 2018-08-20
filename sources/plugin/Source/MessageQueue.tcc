//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "MessageQueue.h"
#include "Messages.h"
#include <thread>

inline MessageQueue::MessageQueue(size_t capacity)
    : rb_(capacity)
{
}

inline bool MessageQueue::writeMessage(const BasicMessage &msg)
{
    Ring_Buffer &rb = rb_;
    size_t size = Messages::sizeOf(msg.tag);
    return rb.put(reinterpret_cast<const uint8_t *>(&msg), size);
}

inline bool MessageQueue::readMessage(BasicMessage &msg)
{
    Ring_Buffer &rb = rb_;
    BasicMessage hdr((MessageTag)0);
    if (!rb.peek(hdr))
        return false;

    size_t size = Messages::sizeOf(hdr.tag);
    return rb.get(reinterpret_cast<uint8_t *>(&msg), size);
}

inline size_t MessageQueue::sizeUsed() const
{
    const Ring_Buffer &rb = rb_;
    return rb.size_used();
}

inline size_t MessageQueue::sizeFree() const
{
    const Ring_Buffer &rb = rb_;
    return rb.size_free();
}
