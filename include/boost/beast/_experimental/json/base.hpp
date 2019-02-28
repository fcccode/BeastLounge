//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_BASE_HPP
#define BOOST_BEAST_JSON_BASE_HPP

#include <boost/beast/core/detail/config.hpp>
#include <boost/beast/_experimental/json/key_param.hpp>
#include <boost/beast/_experimental/json/kind.hpp>
#include <boost/beast/_experimental/json/storage.hpp>

namespace boost {
namespace beast {
namespace json {

/** Common functionality for JSON values and references
*/
class base
{ 
protected:
    storage store_;
    detail::variant& v_;

    friend class ptr;
    friend class ref;
    friend class object_ref;
    friend class array_ref;

    BOOST_BEAST_DECL
    base();

    BOOST_BEAST_DECL
    base(storage const& store);

    BOOST_BEAST_DECL
    base(
        detail::variant& v,
        storage const& store) noexcept;

public:
    BOOST_BEAST_DECL
    storage const&
    get_storage() const noexcept;

    BOOST_BEAST_DECL
    kind
    get_kind() const noexcept;

    BOOST_BEAST_DECL
    bool
    is_object() const noexcept;

    BOOST_BEAST_DECL
    bool
    is_array() const noexcept;

    BOOST_BEAST_DECL
    bool
    is_string() const noexcept;

    BOOST_BEAST_DECL
    bool
    is_signed64() const noexcept;

    BOOST_BEAST_DECL
    bool
    is_unsigned64() const noexcept;

    BOOST_BEAST_DECL
    bool
    is_floating() const noexcept;

    BOOST_BEAST_DECL
    bool
    is_boolean() const noexcept;

    BOOST_BEAST_DECL
    bool
    is_null() const noexcept;
    
    BOOST_BEAST_DECL
    ref
    operator[](key_param key);

    BOOST_BEAST_DECL
    ref
    operator[](std::size_t i);

private:
    friend
    BOOST_BEAST_DECL
    std::ostream&
    operator<<(std::ostream& os, base const& jb);
};

} // json
} // beast
} // boost

#include <boost/beast/_experimental/json/impl/base.hpp>
#ifdef BOOST_BEAST_HEADER_ONLY
#include <boost/beast/_experimental/json/impl/base.ipp>
#endif

#endif
