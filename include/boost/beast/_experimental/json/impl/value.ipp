//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_IMPL_VALUE_IPP
#define BOOST_BEAST_JSON_IMPL_VALUE_IPP

#include <boost/beast/_experimental/json/ref.hpp>
#include <boost/beast/_experimental/json/value.hpp>
#include <boost/throw_exception.hpp>
#include <ostream>
#include <stdexcept>
#include <utility>

namespace boost {
namespace beast {
namespace json {

value::
~value()
{
    store_->destroy(v_);
}

value::
value(storage const& store)
    : base(store)
{
}

value::
value(ref const& other)
    : base(other.store_)
{
    store_->copy(v_, other.v_);
}

value::
value(value const& other)
    : base(other.store_)
{
    store_->copy(v_, other.v_);
}

value&
value::
operator=(ref const& other)
{
    if(&v_ != &other.v_)
        store_->copy(v_, other.v_);
    return *this;
}

value&
value::
operator=(value const& other)
{
    if(&v_ != &other.v_)
        store_->copy(v_, other.v_);
    return *this;
}

value::
value(init_list list)
    : value()
{
    v_.assign(list, *store_);
}

value&
value::
operator=(init_list list)
{
    v_.assign(list, *store_);
    return *this;
}

} // json
} // beast
} // boost

#endif
