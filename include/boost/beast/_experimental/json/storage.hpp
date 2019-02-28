//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_STORAGE_HPP
#define BOOST_BEAST_JSON_STORAGE_HPP

#include <boost/beast/core/detail/config.hpp>
#include <boost/beast/_experimental/json/kind.hpp>
#include <boost/beast/_experimental/json/detail/storage_base.hpp>
#include <type_traits>

namespace boost {
namespace beast {
namespace json {

namespace detail {
class variant;
class access;
} // detail

/** Storage for JSON.

    A storage object is not thread-safe. JSON objects
    which use the same storage may not be accessed
    concurrently.
*/
class storage
{
    detail::storage_base* p_ = nullptr;

    template<class Allocator>
    class impl;

    friend class base;
    friend class ref;
    friend class value;
    friend class ptr;
    friend class object_ref;
    friend class array_ref;
    friend class detail::variant;
    friend class detail::access;

    detail::storage_base&
    operator*() const noexcept
    {
        return *p_;
    }

    detail::storage_base*
    operator->() const noexcept
    {
        return p_;
    }

    BOOST_BEAST_DECL
    storage(std::nullptr_t) noexcept;

    BOOST_BEAST_DECL
    void
    addref() const noexcept;

    BOOST_BEAST_DECL
    void
    release() const noexcept;

public:
    /** Destructor
    */
    BOOST_BEAST_DECL
    ~storage();

    /** Default Constructor

        A default constructed storage object will use an
        instance of `std::allocator<void>` as the allocator.
    */
    BOOST_BEAST_DECL
    storage();

    /** Copy Constructor
    */
    BOOST_BEAST_DECL
    storage(storage const& other) noexcept;

    /** Assignment
    */
    BOOST_BEAST_DECL
    storage& operator=(storage const&) noexcept;

    /** Constructor

        @param alloc The allocator to use for JSON objects that
        are associated with this storage.
    */
    template<
        class Allocator,
        class = typename std::enable_if<
            ! std::is_same<Allocator, storage>::value>::type>
    explicit
    storage(Allocator const& alloc);

    bool
    operator==(storage const& other) const noexcept
    {
        return p_ == other.p_;
    }

    bool
    operator!=(storage const& other) const noexcept
    {
        return p_ != other.p_;
    }
};

} // json
} // beast
} // boost

#include <boost/beast/_experimental/json/impl/storage.hpp>
#ifdef BOOST_BEAST_HEADER_ONLY
#include <boost/beast/_experimental/json/impl/storage.ipp>
#endif

#endif
