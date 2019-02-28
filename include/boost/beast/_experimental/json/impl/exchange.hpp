//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_IMPL_EXCHANGE_HPP
#define BOOST_BEAST_JSON_IMPL_EXCHANGE_HPP

#include <boost/beast/_experimental/json/kind.hpp>
#include <boost/beast/_experimental/json/detail/variant.hpp>
#include <memory>

namespace boost {
namespace beast {
namespace json {

#if 0

template<>
struct exchange<object_t>
{
    static
    ref
    create(storage& store, object_t)
    {
        return ref(store, kind::object);
    }
};

template<>
struct exchange<array_t>
{
    static
    ref
    create(storage& store, array_t)
    {
        return ref(store, kind::array);
    }
};

//------------------------------------------------------------------------------
//
// string
//

template<>
struct exchange<string_view>
{
    static
    ref
    create(storage& store, string_view v)
    {
        ref jv(store, kind::string);
        detail::access(jv)->str_v = v.to_string();
        return jv;
    }
};

template<>
struct exchange<char const*>
{
    static
    ref
    create(storage& store, char const* v)
    {
        ref jv(store, kind::string);
        detail::access(jv)->str_v =
            std::string(v);
        return jv;
    }
};

template<
    class CharT, class Traits, class Allocator>
struct exchange<
    std::basic_string<CharT, Traits, Allocator>>
{
    static
    ref
    create(storage& store, std::basic_string<
        CharT, Traits, Allocator> const& v)
    {
        ref jv(store, kind::string);
        detail::access(jv)->str_v = v;
        return jv;
    }
};

//------------------------------------------------------------------------------
//
// number
//

template<>
struct exchange<int>
{
    static
    ref
    create(storage& store, int v)
    {
        ref jv(store, kind::signed64);
        detail::access(jv)->int64_v = v;
        return jv;
    }
};

template<>
struct exchange<long long>
{
    static
    ref
    create(storage& store, long long v)
    {
        ref jv(store, kind::signed64);
        detail::access(jv)->int64_v = v;
        return jv;
    }
};

template<>
struct exchange<unsigned long long>
{
    static
    ref
    create(storage& store, unsigned long long v)
    {
        ref jv(store, kind::unsigned64);
        detail::access(jv)->uint64_v = v;
        return jv;
    }
};

template<>
struct exchange<float>
{
    static
    ref
    create(storage& store, float v)
    {
        ref jv(store, kind::floating);
        detail::access(jv)->double_v = v;
        return jv;
    }
};

template<>
struct exchange<double>
{
    static
    ref
    create(storage& store, double v)
    {
        ref jv(store, kind::floating);
        detail::access(jv)->double_v = v;
        return jv;
    }
};

//------------------------------------------------------------------------------
//
// boolean
//

template<>
struct exchange<bool>
{
    static
    ref
    create(storage& store, bool v)
    {
        ref jv(store, kind::boolean);
        detail::access(jv)->bool_v = v;
        return jv;
    }
};

template<>
struct exchange<null_t>
{
    static
    ref
    create(storage& store, null_t)
    {
        return ref(store, kind::null);
    }
};

//------------------------------------------------------------------------------
//
// null
//

template<>
struct exchange<std::nullptr_t>
{
    static
    ref
    create(storage& store, std::nullptr_t)
    {
        return ref(store, kind::null);
    }
};

//------------------------------------------------------------------------------

#endif

} // json
} // beast
} // boost

#endif
