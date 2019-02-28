//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_IMPL_PARSER_HPP
#define BOOST_BEAST_JSON_IMPL_PARSER_HPP

#include <boost/beast/_experimental/json/array_ref.hpp>
#include <boost/beast/_experimental/json/object_ref.hpp>

namespace boost {
namespace beast {
namespace json {

template<class T>
void
parser::
assign(T&& t)
{
    if(stack_.back().is_object())
    {
        BOOST_ASSERT(! key().empty());
        object_ref ro(stack_.back());
        ro[key()] = std::forward<T>(t);
    }
    else if(stack_.back().is_array())
    {
        BOOST_ASSERT(key().empty());
        array_ref ra(stack_.back());
        ra.push_back(std::forward<T>(t));
    }
    else
    {
        BOOST_ASSERT(stack_.back().is_null());
        stack_.back() = std::forward<T>(t);
    }
}

} // json
} // beast
} // boost

#endif
