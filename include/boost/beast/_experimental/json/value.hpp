//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_VALUE_HPP
#define BOOST_BEAST_JSON_VALUE_HPP

#include <boost/beast/core/detail/config.hpp>
#include <boost/beast/_experimental/json/base.hpp>
#include <boost/beast/_experimental/json/exchange.hpp>
#include <boost/beast/_experimental/json/init_list.hpp>
#include <boost/beast/_experimental/json/key_param.hpp>
#include <boost/beast/_experimental/json/kind.hpp>
#include <boost/beast/_experimental/json/storage.hpp>
#include <boost/beast/_experimental/json/detail/variant.hpp>
#include <boost/beast/core/string.hpp>
#include <iosfwd>
#include <memory>

namespace boost {
namespace beast {
namespace json {

/** A JSON value.
*/
class value : public base
{ 
    friend class ptr;
    friend class ref;
    friend class object_ref;
    friend class array_ref;

public:
    value() = default;

    BOOST_BEAST_DECL
    ~value();

    BOOST_BEAST_DECL
    explicit
    value(storage const& store);

    BOOST_BEAST_DECL
    value(ref const& other);

    BOOST_BEAST_DECL
    value(value const& other);

    BOOST_BEAST_DECL
    value&
    operator=(ref const& other);

    BOOST_BEAST_DECL
    value&
    operator=(value const& other);

    // modifiers

    template<
        class T
#ifndef BOOST_BEAST_DOXYGEN
        ,class = typename std::enable_if<
            is_exchange_type<T>::value>::type
#endif
    >
    value(T&& t);

    BOOST_BEAST_DECL
    value(init_list list);

    template<
        class T
#ifndef BOOST_BEAST_DOXYGEN
        ,class = typename std::enable_if<
            is_exchange_type<T>::value>::type
#endif
    >
    value&
    operator=(T&& t);

    BOOST_BEAST_DECL
    value&
    operator=(init_list list);
};

} // json
} // beast
} // boost

#include <boost/beast/_experimental/json/impl/value.hpp>
#ifdef BOOST_BEAST_HEADER_ONLY
#include <boost/beast/_experimental/json/impl/value.ipp>
#endif

#endif
