//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_IMPL_STORAGE_IPP
#define BOOST_BEAST_JSON_IMPL_STORAGE_IPP

#include <boost/beast/_experimental/json/storage.hpp>
#include <memory>

namespace boost {
namespace beast {
namespace json {

// ptr needs this
storage::
storage(std::nullptr_t) noexcept
{
}

void
storage::
addref() const noexcept
{
    BOOST_ASSERT(p_);
    ++p_->count_;
}

void
storage::
release() const noexcept
{
    BOOST_ASSERT(p_);
    if(--p_->count_ > 0)
        return;
    delete p_;
}

storage::
~storage()
{
    if(p_)
        release();
}

storage::
storage()
    : storage(std::allocator<char>{})
{
}

storage::
storage(storage const& other) noexcept
    : p_(other.p_)
{
    if(p_)
        ++p_->count_;
}

storage&
storage::
operator=(storage const& other) noexcept
{
    if(other.p_)
        other.addref();
    if(p_)
        release();
    p_ = other.p_;
    return *this;
}

} // json
} // beast
} // boost

#endif
