//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_DETAIL_VARIANT_IPP
#define BOOST_BEAST_JSON_DETAIL_VARIANT_IPP

#include <boost/beast/_experimental/json/detail/variant.hpp>

namespace boost {
namespace beast {
namespace json {
namespace detail {

variant::
variant()
{
}

variant::
~variant()
{
}

void
variant::
set_kind(
    kind k, storage_base& store)
{
    switch(kind_)
    {
    case kind::object:
        for(auto e : map_v)
            store.destroy(*e.second);
        map_v.~map_type();
        break;

    case kind::array:
        for(auto v : vec_v)
            store.destroy(*v);
        vec_v.~vec_type();
        break;

    case kind::string:
        str_v.~str_type();
        break;
    }

    kind_ = k;

    switch(kind_)
    {
    case kind::object:
        ::new(&map_v) map_type();
        break;

    case kind::array:
        ::new(&vec_v) vec_type();
        break;

    case kind::string:
        ::new(&str_v) str_type();
        break;
    }
}

void
variant::
output(std::ostream& os)
{
    switch(kind_)
    {
    case kind::object:
        os << '{';
        for(auto it = map_v.begin(),
            end = map_v.end();
            it != end;)
        {
            os << '\"' << it->first << "\":";
            it->second->output(os);
            if(++it != end)
                os << ',';
        }
        os << '}';
        break;
        
    case kind::array:
        os << '[';
        for(auto it = vec_v.begin(),
            end = vec_v.end();
            it != end;)
        {
            (*it)->output(os);
            if(++it != end)
                os << ',';
        }
        os << ']';
        break;
        
    case kind::string:
        os << '\"' << str_v << '\"';
        break;
        
    case kind::signed64:
        os << int64_v;
        break;
        
    case kind::unsigned64:
        os << uint64_v;
        break;

    case kind::floating:
        os << double_v;
        break;
        
    case kind::boolean:
        if(bool_v)
            os << "true";
        else
            os << "false";
        break;

    case kind::null:
        os << "null";
        break;
    }
}

void
variant::
assign(
    init_list list,
    storage_base& store)
{
    set_kind(kind::array, store);
    auto const n0 = vec_v.size();
    try
    {
        vec_v.reserve(n0 + list.size());
        for(auto const& e : list)
        {
            vec_v.push_back(
                &store.construct());
            e.get().assign(
                *vec_v.back(), store);
        }
    }
    catch(...)
    {
        for(auto i = n0;
            i < vec_v.size();
            ++i)
            if(vec_v[i])
                store.destroy(*vec_v[i]);
        vec_v.resize(n0);
        throw;
    }
}

} // detail
} // json
} // beast
} // boost

#endif
