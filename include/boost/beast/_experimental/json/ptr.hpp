//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_PTR_HPP
#define BOOST_BEAST_JSON_PTR_HPP

#include <boost/beast/core/detail/config.hpp>
#include <boost/beast/_experimental/json/storage.hpp>
#include <boost/beast/_experimental/json/ref.hpp>
#include <boost/beast/_experimental/json/value.hpp>
#include <boost/beast/_experimental/json/detail/variant.hpp>
#include <cstddef>
#include <memory>

namespace boost {
namespace beast {
namespace json {

class ptr
{ 
    storage store_;
    detail::variant* v_ = nullptr;

    friend class value;
    friend class detail::access;

    BOOST_BEAST_DECL
    ptr(storage const& store,
        detail::variant& v);

public:
#ifndef BOOST_BEAST_DOXYGEN
    // these are deleted to give better diagnostics
    ptr(ref const&) = delete;
    ptr(value const&) = delete;
    ptr& operator=(ptr const*) = delete;
    ptr& operator=(ref const&) = delete;
    ptr& operator=(value const&) = delete;
#endif

    ~ptr() = default;

    /// Default Constructor
    BOOST_BEAST_DECL
    ptr() noexcept;

    /** Construct a null pointer
    */
    BOOST_BEAST_DECL
    ptr(std::nullptr_t) noexcept;

    /** Construct a pointer to a JSON value.
    */
    BOOST_BEAST_DECL
    ptr(ptr const& other) noexcept;

    /** Construct a pointer to a JSON value.
    */
    BOOST_BEAST_DECL
    ptr(ref const* other) noexcept;

    /** Construct a pointer to a JSON value.
    */
    BOOST_BEAST_DECL
    ptr(value const* other) noexcept;

    /** Assign null to the pointer
    */
    ptr&
    operator=(std::nullptr_t) noexcept;

    /** Assign a JSON value to the pointer
    */
    BOOST_BEAST_DECL
    ptr&
    operator=(ptr const& other) noexcept;

    /** Assign a JSON value to the pointer
    */
    BOOST_BEAST_DECL
    ptr&
    operator=(ref const* other) noexcept;

    /** Assign a JSON value to the pointer
    */
    BOOST_BEAST_DECL
    ptr&
    operator=(value const* other) noexcept;

    BOOST_BEAST_DECL
    ref
    operator->() const noexcept;

    BOOST_BEAST_DECL
    ref
    operator*() const noexcept;

private:
    BOOST_BEAST_DECL
    friend
    bool
    operator==(ptr const& p, std::nullptr_t) noexcept;

    BOOST_BEAST_DECL
    friend
    bool
    operator==(std::nullptr_t, ptr const& p) noexcept;

    BOOST_BEAST_DECL
    friend
    bool
    operator!=(ptr const& p, std::nullptr_t) noexcept;

    BOOST_BEAST_DECL
    friend
    bool
    operator!=(std::nullptr_t, ptr const& p) noexcept;

    BOOST_BEAST_DECL
    friend
    bool
    operator==(ptr const& lhs, ptr const& rhs) noexcept;

    BOOST_BEAST_DECL
    friend
    bool
    operator!=(ptr const& lhs, ptr const& rhs) noexcept;
};

} // json
} // beast
} // boost

#ifdef BOOST_BEAST_HEADER_ONLY
#include <boost/beast/_experimental/json/impl/ptr.ipp>
#endif

#endif
