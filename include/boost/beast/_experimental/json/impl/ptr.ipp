//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_IMPL_PTR_IPP
#define BOOST_BEAST_JSON_IMPL_PTR_IPP

#include <boost/beast/_experimental/json/ptr.hpp>
#include <boost/beast/_experimental/json/ref.hpp>
#include <boost/beast/_experimental/json/value.hpp>

namespace boost {
namespace beast {
namespace json {

ptr::
ptr() noexcept
    : ptr(nullptr)
{
}

ptr::
ptr(std::nullptr_t) noexcept
    : store_(nullptr)
{
}

ptr::
ptr(ptr const& other) noexcept
    : store_(other.store_)
    , v_(other.v_)
{
}

ptr::
ptr(ref const* other) noexcept
    : store_(other ?
        other->store_ : storage(nullptr))
    , v_(other ?
        &other->v_ : nullptr)
{
}

ptr::
ptr(value const* other) noexcept
    : store_(other ?
        other->store_ : storage(nullptr))
    , v_(other ?
        &other->v_ : nullptr)
{
}

ptr&
ptr::
operator=(std::nullptr_t) noexcept
{
    store_ = {};
    v_ = nullptr;
    return *this;
}

ptr&
ptr::
operator=(ptr const& other) noexcept
{
    store_ = other.store_;
    v_ = other.v_;
    return *this;
}

ptr&
ptr::
operator=(ref const* other) noexcept
{
    if(other)
    {
        store_ = other->store_;
        v_ = &other->v_;
    }
    else
    {
        store_ = nullptr;
        v_ = nullptr;
    }
    return *this;
}

ptr&
ptr::
operator=(value const* other) noexcept
{
    if(other)
    {
        store_ = other->store_;
        v_ = &other->v_;
    }
    else
    {
        store_ = nullptr;
        v_ = nullptr;
    }
    return *this;
}

ref
ptr::
operator->() const noexcept
{
    BOOST_ASSERT(v_);
    return ref(*v_, store_);
}

ref
ptr::
operator*() const noexcept
{
    BOOST_ASSERT(v_);
    return ref(*v_, store_);
}

bool
operator==(ptr const& p, std::nullptr_t) noexcept
{
    return p.v_ == nullptr;
}

bool
operator==(std::nullptr_t, ptr const& p) noexcept
{
    return p.v_ == nullptr;
}

bool
operator!=(ptr const& p, std::nullptr_t) noexcept
{
    return p.v_ != nullptr;
}

bool
operator!=(std::nullptr_t, ptr const& p) noexcept
{
    return p.v_ != nullptr;
}

bool
operator==(ptr const& lhs, ptr const& rhs) noexcept
{
    return lhs.v_ == rhs.v_;
}

bool
operator!=(ptr const& lhs, ptr const& rhs) noexcept
{
    return lhs.v_ != rhs.v_;
}

} // json
} // beast
} // boost

#endif
