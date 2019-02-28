//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_IMPL_REF_IPP
#define BOOST_BEAST_JSON_IMPL_REF_IPP

#include <boost/beast/_experimental/json/ref.hpp>
#include <boost/beast/_experimental/json/value.hpp>
#include <boost/throw_exception.hpp>
#include <ostream>
#include <stdexcept>
#include <utility>

namespace boost {
namespace beast {
namespace json {

ref::
ref(detail::variant& v,
    storage const& store)
    : base(v, store)
{
}

ref
ref::
array_operator(
    string_view key,
    detail::variant& v,
    storage const& store)
{
    if(v.get_kind() != kind::null)
    {
        if(v.get_kind() != kind::object)
            BOOST_THROW_EXCEPTION(
                std::logic_error("object required"));
    }
    else
    {
        // implicit conversion to object
        v.set_kind(kind::object, *store);
    }

    return ref(
        store->find_or_create(v, key),
        store);
}

ref
ref::
array_operator(
    std::size_t i,
    detail::variant& v,
    storage const& store)
{
    if(v.get_kind() != kind::array)
        BOOST_THROW_EXCEPTION(
            std::logic_error("array required"));
    return ref(*v.vec_v[i], store);
}

ref::
ref(ref const& other) noexcept
    : base(other.v_, other.store_)
{
}

ref::
ref(value const& other) noexcept
    : base(other.v_, other.store_)
{
}

ref&
ref::
operator=(ref const& other)
{
    if(&other != this)
        store_->copy(v_, other.v_);
    return *this;
}

ref&
ref::
operator=(init_list list)
{
    v_.assign(list, *store_);
    return *this;
}

} // json
} // beast
} // boost

#endif
