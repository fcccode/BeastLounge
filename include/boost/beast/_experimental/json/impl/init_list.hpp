//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_IMPL_INIT_LIST_HPP
#define BOOST_BEAST_JSON_IMPL_INIT_LIST_HPP

#include <boost/beast/_experimental/json/ref.hpp>
#include <boost/beast/_experimental/json/storage.hpp>
#include <boost/assert.hpp>
#include <new>
#include <type_traits>

namespace boost {
namespace beast {
namespace json {

template<class T>
init_ref::
impl<T>::
impl(T const& t) noexcept
    : t_(t)
{
}

template<class T>
void
init_ref::
impl<T>::
move(void* to) noexcept
{
    ::new(to) impl(t_);
}

template<class T>
void
init_ref::
impl<T>::
assign(
    detail::variant& v,
    detail::storage_base& store) const
{
    v.assign(t_, store);
}

//------------------------------------------------------------------------------

template<class T>
void
init_ref::
construct(T const& t) noexcept
{
    BOOST_ASSERT(
        sizeof(impl<T>) <= sizeof(buf_));
    ::new(&buf_) impl<T>(t);
}

template<class T, class>
init_ref::
init_ref(T const& t) noexcept
{
    construct(t);
}

} // json
} // beast
} // boost

#endif
