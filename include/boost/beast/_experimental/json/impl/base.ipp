//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_IMPL_BASE_IPP
#define BOOST_BEAST_JSON_IMPL_BASE_IPP

#include <boost/beast/_experimental/json/base.hpp>
#include <boost/beast/_experimental/json/ref.hpp>
#include <boost/throw_exception.hpp>
#include <ostream>
#include <stdexcept>

namespace boost {
namespace beast {
namespace json {

base::
base()
    : v_(store_->construct())
{
}

base::
base(detail::variant& v,
    storage const& store) noexcept
    : store_(store)
    , v_(v)
{
}

base::
base(storage const& store)
    : store_(store)
    , v_(store_->construct())
{
}

storage const&
base::
get_storage() const noexcept
{
    return store_;
}

kind
base::
get_kind() const noexcept
{
    return v_.get_kind();
}

bool
base::
is_object() const noexcept
{
    return get_kind() == kind::object;
}

bool
base::
is_array() const noexcept
{
    return get_kind() == kind::array;
}

bool
base::
is_string() const noexcept
{
    return get_kind() == kind::string;
}

bool
base::
is_signed64() const noexcept
{
    return get_kind() == kind::signed64;
}

bool
base::
is_unsigned64() const noexcept
{
    return get_kind() == kind::unsigned64;
}

bool
base::
is_floating() const noexcept
{
    return get_kind() == kind::floating;
}

bool
base::
is_boolean() const noexcept
{
    return get_kind() == kind::boolean;
}

bool
base::
is_null() const noexcept
{
    return get_kind() == kind::null;
}

ref
base::
operator[](key_param key)
{
    return ref::array_operator(
        key.str, v_, store_);
}

ref
base::
operator[](std::size_t i)
{
    return ref::array_operator(
        i, v_, store_);
}

std::ostream&
operator<<(std::ostream& os, base const& jb)
{
    jb.v_.output(os);
    return os;
}

} // json
} // beast
} // boost

#endif
