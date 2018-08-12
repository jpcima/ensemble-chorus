//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <memory>
#include <atomic>
#include <type_traits>
#include <cstdint>
#include <cstddef>

#if defined(RING_BUFFER_ENABLE_SOFT)
#include <shared_mutex>
#endif

#if defined(__cpp_lib_atomic_is_always_lock_free)
static_assert(
    std::atomic<size_t>::is_always_lock_free, "atomic<size_t> must be lock free");
#endif

//------------------------------------------------------------------------------
template <bool> class Ring_Buffer_Ex;
typedef Ring_Buffer_Ex<true> Ring_Buffer;

#if defined(RING_BUFFER_ENABLE_SOFT)
template <class> class Soft_Ring_Buffer_Ex;
#if defined(__cpp_lib_shared_mutex)
typedef Soft_Ring_Buffer_Ex<std::shared_mutex> Soft_Ring_Buffer;
#else
typedef Soft_Ring_Buffer_Ex<std::shared_timed_mutex> Soft_Ring_Buffer;
#endif
#endif

//------------------------------------------------------------------------------
template <class RB>
class Basic_Ring_Buffer {
public:
    // read operations
    template <class T> bool get(T &x);
    template <class T> bool get(T *x, size_t n);
    template <class T> bool peek(T &x);
    template <class T> bool peek(T *x, size_t n);
    // write operations
    template <class T> bool put(const T &x);
    template <class T> bool put(const T *x, size_t n);
};

//------------------------------------------------------------------------------
template <bool Atomic>
class Ring_Buffer_Ex final :
    private Basic_Ring_Buffer<Ring_Buffer_Ex<Atomic>> {
private:
    typedef Basic_Ring_Buffer<Ring_Buffer_Ex<Atomic>> Base;
public:
    // initialization and cleanup
    explicit Ring_Buffer_Ex(size_t capacity);
    ~Ring_Buffer_Ex();
    // attributes
    size_t capacity() const;
    // read operations
    size_t size_used() const;
    bool discard(size_t len);
    using Base::get;
    using Base::peek;
    // write operations
    size_t size_free() const;
    using Base::put;

private:
    size_t cap_{0};
    typename std::conditional<Atomic, std::atomic<size_t>, size_t>::type rp_{0}, wp_{0};
    std::unique_ptr<uint8_t[]> rbdata_ {};
    friend Base;
#if defined(RING_BUFFER_ENABLE_SOFT)
    template <class> friend class Soft_Ring_Buffer_Ex;
#endif
    bool getbytes_(void *data, size_t len);
    bool peekbytes_(void *data, size_t len) const;
    bool getbytes_ex_(void *data, size_t len, bool advp);
    bool putbytes_(const void *data, size_t len);
};

//------------------------------------------------------------------------------
#if defined(RING_BUFFER_ENABLE_SOFT)
template <class Mutex>
class Soft_Ring_Buffer_Ex final :
    private Basic_Ring_Buffer<Soft_Ring_Buffer_Ex<Mutex>> {
private:
    typedef Basic_Ring_Buffer<Soft_Ring_Buffer_Ex<Mutex>> Base;
public:
    // initialization and cleanup
    explicit Soft_Ring_Buffer_Ex(size_t capacity);
    ~Soft_Ring_Buffer_Ex();
    // attributes
    size_t capacity() const;
    // read operations
    size_t size_used() const;
    bool discard(size_t len);
    using Base::get;
    using Base::peek;
    // write operations
    size_t size_free() const;
    using Base::put;

private:
    Ring_Buffer_Ex<false> rb_;
    mutable Mutex shmutex_;
    friend Base;
    bool getbytes_(void *data, size_t len);
    bool peekbytes_(void *data, size_t len) const;
    bool putbytes_(const void *data, size_t len);
    void grow_(size_t newcap);
};
#endif

//------------------------------------------------------------------------------
#include "ring_buffer/ring_buffer.tcc"
