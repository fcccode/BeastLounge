//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_REF_HPP
#define BOOST_BEAST_JSON_REF_HPP

#include <boost/beast/core/detail/config.hpp>
#include <boost/beast/_experimental/json/base.hpp>
#include <boost/beast/_experimental/json/exchange.hpp>
#include <boost/beast/_experimental/json/init_list.hpp>
#include <boost/beast/_experimental/json/key_param.hpp>
#include <boost/beast/_experimental/json/kind.hpp>
#include <boost/beast/_experimental/json/storage.hpp>
#include <boost/beast/_experimental/json/value.hpp>
#include <boost/beast/_experimental/json/detail/variant.hpp>
#include <boost/beast/core/string.hpp>
#include <iosfwd>
#include <memory>
#include <type_traits>

namespace boost {
namespace beast {
namespace json {

class ref : public base
{
    friend class base;
    friend class ptr;
    friend class value;
    friend class object_ref;
    friend class array_ref;

    BOOST_BEAST_DECL
    ref(detail::variant& v,
        storage const& store);

    BOOST_BEAST_DECL
    static
    ref
    array_operator(
        string_view key,
        detail::variant& v,
        storage const& store);

    BOOST_BEAST_DECL
    static
    ref
    array_operator(
        std::size_t i,
        detail::variant& v,
        storage const& store);

public:
    ~ref() = default;

    /** Construct a reference to a JSON value.
    */
    BOOST_BEAST_DECL
    ref(ref const& other) noexcept;

    /** Construct a reference to a JSON value.
    */
    BOOST_BEAST_DECL
    ref(value const& other) noexcept;

    /** Assign a copy of another JSON value.
    */
    BOOST_BEAST_DECL
    ref&
    operator=(ref const& other);

#ifndef BOOST_BEAST_DOXYGEN
    // required for ptr::operator->() to work
    ref*
    operator->() noexcept
    {
        return this;
    }
#endif

    template<
        class T
#ifndef BOOST_BEAST_DOXYGEN
        ,class = typename std::enable_if<
            is_exchange_type<T>::value>::type
#endif
    >
    ref&
    operator=(T&& t);

    BOOST_BEAST_DECL
    ref&
    operator=(init_list list);
};

} // json
} // beast
} // boost

#include <boost/beast/_experimental/json/impl/ref.hpp>
#ifdef BOOST_BEAST_HEADER_ONLY
#include <boost/beast/_experimental/json/impl/ref.ipp>
#endif

#endif
