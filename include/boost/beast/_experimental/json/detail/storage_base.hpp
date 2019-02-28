//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_DETAIL_STORAGE_BASE_HPP
#define BOOST_BEAST_JSON_DETAIL_STORAGE_BASE_HPP

#include <boost/beast/core/string.hpp>
#include <boost/assert.hpp>
#include <atomic>

namespace boost {
namespace beast {
namespace json {

class storage;

namespace detail {

class variant;

class storage_base
{
    friend class json::storage;

    std::atomic<std::size_t> count_;

public:
    virtual ~storage_base() = default;

    BOOST_BEAST_DECL
    storage_base();

    virtual
    variant&
    construct() = 0;

    virtual
    void
    destroy(variant& v) = 0;

    virtual
    void
    copy(
        variant& lhs,
        variant const& rhs) = 0;

    BOOST_BEAST_DECL
    variant&
    find_or_create(
        variant& obj,
        string_view key);
};

} // detail
} // json
} // beast
} // boost

#ifdef BOOST_BEAST_HEADER_ONLY
#include <boost/beast/_experimental/json/detail/storage_base.ipp>
#endif

#endif
