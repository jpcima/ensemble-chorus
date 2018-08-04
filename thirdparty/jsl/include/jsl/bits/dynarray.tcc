//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "../dynarray"
#include <algorithm>
#include <iterator>

namespace jsl {

template <class T, class A>
dynarray<T, A>::dynarray() noexcept
{
}

template <class T, class A>
dynarray<T, A>::~dynarray()
{
    A &alloc = _allocator();
    _delete(alloc, data(), size());
}

template <class T, class A>
void dynarray<T, A>::clear()
{
    A &alloc = _allocator();
    _delete(alloc, data(), size());
    _data() = nullptr;
    _size() = 0;
}

template <class T, class A>
dynarray<T, A>::dynarray(size_t count)
{
    reset(count);
}

template <class T, class A>
void dynarray<T, A>::reset(size_t count)
{
    if (count == size()) {
        T *dat = data();
        for (size_t i = 0; i < count; ++i)
            dat[i] = T();
    }
    else {
        A &alloc = _allocator();
        T *dat = _new(alloc, count);
        _delete(alloc, data(), size());
        _data() = dat;
        _size() = count;
    }
}

template <class T, class A> template <class I>
dynarray<T, A>::dynarray(I first, I last)
{
    assign(first, last);
}

template <class T, class A> template <class I>
void dynarray<T, A>::assign(I first, I last)
{
    size_t count = std::distance(first, last);
    if (count == size()) {
        std::copy(first, last, data());
    }
    else {
        A &alloc = _allocator();
        T *dat = _duplicate(alloc, count, first);
        _delete(alloc, data(), size());
        _data() = dat;
        _size() = count;
    }
}

template <class T, class A>
dynarray<T, A>::dynarray(size_t count, const T &value)
{
    assign(count, value);
}

template <class T, class A>
void dynarray<T, A>::assign(size_t count, const T &value)
{
    if (count == size()) {
        std::fill_n(data(), count, value);
    }
    else {
        A &alloc = _allocator();
        T *dat = _new_init(alloc, count, value);
        _delete(alloc, data(), size());
        _data() = dat;
        _size() = count;
    }
}

template <class T, class A>
dynarray<T, A>::dynarray(dynarray &&other) noexcept
{
    _data() = other.data();
    _size() = other.size();
    other._data() = nullptr;
    other._size() = 0;
}

template <class T, class A>
auto dynarray<T, A>::operator=(dynarray &&other) noexcept -> dynarray &
{
    if (&other != this) {
        A &alloc = _allocator();
        _delete(alloc, data(), size());
        _data() = other.data();
        _size() = other.size();
        other._data() = nullptr;
        other._size() = 0;
    }
    return *this;
}

template <class T, class A>
dynarray<T, A>::dynarray(const dynarray &other)
{
    assign(other.begin(), other.end());
}

template <class T, class A>
auto dynarray<T, A>::operator=(const dynarray &other) -> dynarray &
{
    assign(other.begin(), other.end());
    return *this;
}

template <class T, class A>
dynarray<T, A>::dynarray(std::initializer_list<T> ilist)
{
    assign(ilist);
}

template <class T, class A>
auto dynarray<T, A>::operator=(std::initializer_list<T> ilist) -> dynarray &
{
    assign(ilist);
    return *this;
}

template <class T, class A>
void dynarray<T, A>::assign(std::initializer_list<T> ilist)
{
    assign(ilist.begin(), ilist.end());
}

template <class T, class A>
void dynarray<T, A>::swap(dynarray &other) noexcept
{
    std::swap(_data(), other._data());
    std::swap(_size(), other._size());
}

template <class T, class A>
const T *dynarray<T, A>::data() const noexcept
{
    return std::get<0>(_dsa);
}

template <class T, class A>
T *dynarray<T, A>::data() noexcept
{
    return const_cast<T *>(
        const_cast<const dynarray &>(*this).data());
}

template <class T, class A>
size_t dynarray<T, A>::size() const noexcept
{
    return std::get<1>(_dsa);
}

template <class T, class A>
bool dynarray<T, A>::empty() const noexcept
{
    return size() == 0;
}

template <class T, class A>
size_t dynarray<T, A>::max_size() const noexcept
{
    return std::allocator_traits<A>::max_size(_allocator());
}

template <class T, class A>
void dynarray<T, A>::fill(const T &value)
{
    std::fill_n(data(), size(), value);
}

template <class T, class A>
T &dynarray<T, A>::operator[](size_t i) noexcept
{
    return const_cast<T &>(
        const_cast<const dynarray &>(*this)[i]);
}

template <class T, class A>
const T &dynarray<T, A>::operator[](size_t i) const noexcept
{
    return data()[i];
}

template <class T, class A>
T &dynarray<T, A>::at(size_t pos)
{
    return const_cast<T &>(
        const_cast<const dynarray &>(*this).at(pos));
}

template <class T, class A>
const T &dynarray<T, A>::at(size_t pos) const
{
    if (pos >= size())
        throw std::out_of_range("dynarray::at");
    return data()[pos];
}

template <class T, class A>
T &dynarray<T, A>::front() noexcept
{
    return const_cast<T &>(
        const_cast<const dynarray &>(*this).front());
}

template <class T, class A>
const T &dynarray<T, A>::front() const noexcept
{
    return data()[0];
}

template <class T, class A>
T &dynarray<T, A>::back() noexcept
{
    return const_cast<T &>(
        const_cast<const dynarray &>(*this).back());
}

template <class T, class A>
const T &dynarray<T, A>::back() const noexcept
{
    return data()[size() - 1];
}

template <class T, class A>
T *dynarray<T, A>::begin() noexcept
{
    return const_cast<T *>(cbegin());
}

template <class T, class A>
const T *dynarray<T, A>::begin() const noexcept
{
    return cbegin();
}

template <class T, class A>
const T *dynarray<T, A>::cbegin() const noexcept
{
    return &data()[0];
}

template <class T, class A>
T *dynarray<T, A>::end() noexcept
{
    return const_cast<T *>(cend());
}

template <class T, class A>
const T *dynarray<T, A>::end() const noexcept
{
    return cend();
}

template <class T, class A>
const T *dynarray<T, A>::cend() const noexcept
{
    return &data()[size()];
}

//------------------------------------------------------------------------------
template <class T, class A>
T *&dynarray<T, A>::_data() noexcept
{
    return std::get<0>(_dsa);
}

template <class T, class A>
size_t &dynarray<T, A>::_size() noexcept
{
    return std::get<1>(_dsa);
}

template <class T, class A>
A &dynarray<T, A>::_allocator() noexcept
{
    return std::get<2>(_dsa);
}

template <class T, class A>
const A &dynarray<T, A>::_allocator() const noexcept
{
    return std::get<2>(_dsa);
}

//------------------------------------------------------------------------------
template <class T, class A>
T *dynarray<T, A>::_new(A &a, size_t n)
{
    size_t i = 0;
    T *p = a.allocate(n);
    try {
        for (; i < n; ++i)
            a.construct(&p[i]);
    }
    catch (...) {
        _delete_i(a, p, i, n);
        throw;
    }
    return p;
}

template <class T, class A>
T *dynarray<T, A>::_new_init(A &a, size_t n, const T &v)
{
    size_t i = 0;
    T *p = a.allocate(n);
    try {
        for (; i < n; ++i)
            a.construct(&p[i], v);
    }
    catch (...) {
        _delete_i(a, p, i, n);
        throw;
    }
    return p;
}

template <class T, class A>
template <class I>
T *dynarray<T, A>::_duplicate(A &a, size_t n, I it)
{
    size_t i = 0;
    T *p = a.allocate(n);
    try {
        for (; i < n; ++i)
            a.construct(&p[i], *it++);
    }
    catch (...) {
        _delete_i(a, p, i, n);
        throw;
    }
    return p;
}

template <class T, class A>
inline void dynarray<T, A>::_delete(A &a, T *p, size_t n) noexcept
{
    _delete_i(a, p, n, n);
}

template <class T, class A>
void dynarray<T, A>::_delete_i(A &a, T *p, size_t i, size_t n) noexcept
{
    for (size_t j = i; j-- > 0;)
        a.destroy(&p[j]);
    a.deallocate(p, n);
}

//------------------------------------------------------------------------------
template <class T, class A>
bool operator==(const dynarray<T, A> &a, const dynarray<T, A> &b) {
    return std::equal(a.begin(), a.end(), b.begin(), b.end());
}

template <class T, class A>
bool operator!=(const dynarray<T, A> &a, const dynarray<T, A> &b) {
    return !operator==(a, b);
}

}  // namespace jsl

//------------------------------------------------------------------------------
namespace std {
    template <class T, class A>
    void swap(jsl::dynarray<T, A> &lhs, jsl::dynarray<T, A> &rhs) noexcept
    {
        lhs.swap(rhs);
    }
}
