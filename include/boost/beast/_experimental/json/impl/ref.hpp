//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_IMPL_REF_HPP
#define BOOST_BEAST_JSON_IMPL_REF_HPP

#include <boost/beast/_experimental/json/exchange.hpp>
#include <boost/beast/_experimental/json/detail/variant.hpp>
#include <boost/optional.hpp>

namespace boost {
namespace beast {
namespace json {

template<class T, class>
ref&
ref::
operator=(T&& t)
{
    v_.assign(std::forward<T>(t), *store_);
    return *this;
}

} // json
} // beast
} // boost

#endif
