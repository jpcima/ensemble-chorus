//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "ring_buffer/ring_buffer.h"
#include <algorithm>
#include <cassert>

#if defined(__cpp_if_constexpr)
#    define if_constexpr if constexpr
#else
#    define if_constexpr if
#endif

template <bool Atomic>
Ring_Buffer_Ex<Atomic>::Ring_Buffer_Ex(size_t capacity)
    : cap_(capacity + 1),
      rbdata_(new uint8_t[capacity + 1])
{
}

template <bool Atomic>
Ring_Buffer_Ex<Atomic>::~Ring_Buffer_Ex()
{
}

template <bool Atomic>
size_t Ring_Buffer_Ex<Atomic>::size_used() const
{
    const size_t rp = rp_, wp = wp_, cap = cap_;
    return wp + ((wp < rp) ? cap : 0) - rp;
}

template <bool Atomic>
bool Ring_Buffer_Ex<Atomic>::discard(size_t len)
{
    return getbytes_ex_(nullptr, len, true);
}

template <bool Atomic>
size_t Ring_Buffer_Ex<Atomic>::size_free() const
{
    const size_t rp = rp_, wp = wp_, cap = cap_;
    return rp + ((rp <= wp) ? cap : 0) - wp - 1;
}

template <bool Atomic>
bool Ring_Buffer_Ex<Atomic>::getbytes_(void *data, size_t len)
{
    return getbytes_ex_(data, len, true);
}

template <bool Atomic>
bool Ring_Buffer_Ex<Atomic>::peekbytes_(void *data, size_t len) const
{
    auto *ncthis = const_cast<Ring_Buffer_Ex<Atomic> *>(this);
    return ncthis->getbytes_ex_(data, len, false);
}

template <bool Atomic>
bool Ring_Buffer_Ex<Atomic>::getbytes_ex_(void *data, size_t len, bool advp)
{
    if (size_used() < len)
        return false;

    const size_t rp = rp_, cap = cap_;
    const uint8_t *src = rbdata_.get();
    uint8_t *dst = (uint8_t *)data;

    if (data) {
        const size_t taillen = std::min(len, cap - rp);
        if_constexpr (Atomic)
            std::atomic_thread_fence(std::memory_order_acquire);
        std::copy_n(&src[rp], taillen, dst);
        std::copy_n(src, len - taillen, dst + taillen);
    }

    if (advp)
        rp_ = (rp + len < cap) ? (rp + len) : (rp + len - cap);
    return true;
}

template <bool Atomic>
bool Ring_Buffer_Ex<Atomic>::putbytes_(const void *data, size_t len)
{
    if (size_free() < len)
        return false;

    const size_t wp = wp_, cap = cap_;
    const uint8_t *src = (const uint8_t *)data;
    uint8_t *dst = rbdata_.get();

    const size_t taillen = std::min(len, cap - wp);
    std::copy_n(src, taillen, &dst[wp]);
    std::copy_n(src + taillen, len - taillen, dst);
    if_constexpr (Atomic)
        std::atomic_thread_fence(std::memory_order_release);

    wp_ = (wp + len < cap) ? (wp + len) : (wp + len - cap);
    return true;
}

template class Ring_Buffer_Ex<true>;
template class Ring_Buffer_Ex<false>;

//------------------------------------------------------------------------------
#if defined(RING_BUFFER_ENABLE_SOFT)
template <class Mutex>
size_t Soft_Ring_Buffer_Ex<Mutex>::size_used() const
{
    std::shared_lock<Mutex> lock(shmutex_);
    return rb_.size_used();
}

template <class Mutex>
bool Soft_Ring_Buffer_Ex<Mutex>::discard(size_t len)
{
    std::shared_lock<Mutex> lock(shmutex_);
    return rb_.discard(len);
}

template <class Mutex>
size_t Soft_Ring_Buffer_Ex<Mutex>::size_free() const
{
    std::shared_lock<Mutex> lock(shmutex_);
    return rb_.size_free();
}

template <class Mutex>
bool Soft_Ring_Buffer_Ex<Mutex>::getbytes_(void *data, size_t len)
{
    std::shared_lock<Mutex> lock(shmutex_);
    return rb_.getbytes_(data, len);
}

template <class Mutex>
bool Soft_Ring_Buffer_Ex<Mutex>::peekbytes_(void *data, size_t len) const
{
    std::shared_lock<Mutex> lock(shmutex_);
    auto &ncrb = const_cast<Ring_Buffer_Ex<false> &>(rb_);
    return ncrb.peekbytes_(data, len);
}

template <class Mutex>
bool Soft_Ring_Buffer_Ex<Mutex>::putbytes_(const void *data, size_t len)
{
    bool good;
    size_t oldcap = rb_.capacity();
    size_t atleastcap = size_used() + len;
    if (atleastcap <= oldcap) {
        std::shared_lock<Mutex> lock(shmutex_);
        good = rb_.putbytes_(data, len);
    }
    else {
        std::unique_lock<Mutex> lock(shmutex_);
        grow_(atleastcap);
        good = rb_.putbytes_(data, len);
    }
    assert(good);
    return true;
}

template <class Mutex>
void Soft_Ring_Buffer_Ex<Mutex>::grow_(size_t atleastcap)
{
    size_t oldcap = rb_.capacity();
    size_t newcap = (oldcap < 16) ? 16 : oldcap;
    while (newcap < atleastcap) {
        if (newcap > std::numeric_limits<size_t>::max() / 3)
            throw std::bad_alloc();
        newcap = newcap * 3 / 2;
    }

    size_t len = rb_.size_used();
    std::unique_ptr<uint8_t[]> newdata(new uint8_t[newcap + 1]);

    {
        const size_t rp = rb_.rp_;
        const uint8_t *src = rb_.rbdata_.get();
        uint8_t *dst = newdata.get();

        const size_t taillen = std::min(len, oldcap + 1 - rp);
        std::copy_n(&src[rp], taillen, dst);
        std::copy_n(src, len - taillen, dst + taillen);
    }

    rb_.cap_ = newcap + 1;
    rb_.rp_ = 0;
    rb_.wp_ = len;
    rb_.rbdata_ = std::move(newdata);
}

#if defined(__cpp_lib_shared_mutex)
template class Soft_Ring_Buffer_Ex<std::shared_mutex>;
#endif
template class Soft_Ring_Buffer_Ex<std::shared_timed_mutex>;

#endif
