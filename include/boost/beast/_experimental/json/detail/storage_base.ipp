//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_DETAIL_STORAGE_BASE_IPP
#define BOOST_BEAST_JSON_DETAIL_STORAGE_BASE_IPP

#include <boost/beast/_experimental/json/detail/storage_base.hpp>
#include <boost/beast/_experimental/json/detail/variant.hpp>

namespace boost {
namespace beast {
namespace json {
namespace detail {

storage_base::
storage_base()
    : count_(1)
{
}

variant&
storage_base::
find_or_create(
    variant& obj,
    string_view key)
{
    // VFALCO This constructs a string needlessly...
    auto s = key.to_string();
    auto it = obj.map_v.find(s);
    if(it != obj.map_v.end())
        return *it->second;
    variant* v = nullptr;
    try
    {
        v = &construct();
        obj.map_v.insert(std::make_pair(
            std::move(s), v));
    }
    catch(...)
    {
        if(v)
            destroy(*v);
        throw;
    }
    return *v;
}

} // detail
} // json
} // beast
} // boost


#endif
