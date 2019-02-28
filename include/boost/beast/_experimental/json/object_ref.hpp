//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_OBJECT_REF_HPP
#define BOOST_BEAST_JSON_OBJECT_REF_HPP

#include <boost/beast/core/detail/config.hpp>
#include <boost/beast/_experimental/json/key_param.hpp>
#include <boost/beast/_experimental/json/ref.hpp>
#include <boost/beast/_experimental/json/value.hpp>
#include <boost/beast/_experimental/json/storage.hpp>
#include <boost/beast/_experimental/json/detail/variant.hpp>
#include <utility>

namespace boost {
namespace beast {
namespace json {

class object_ref
{
    storage store_;
    detail::variant& v_;

    static
    ref
    make_ref(
        detail::variant& v,
        storage store);

public:
    using key_type = string_view;

    using mapped_type = ref;

    class value_type;

    using size_type =
        detail::variant::map_type::size_type;

    using difference_type =
        detail::variant::map_type::difference_type;

    using hasher =
        detail::variant::map_type::hasher;

    using key_equal =
        detail::variant::map_type::key_equal;

    class iterator;

    class value_type
    {
        friend class iterator;

        BOOST_BEAST_DECL
        value_type(
            string_view key_, ref const& jr);

    public:
        string_view key;
        ref value;

        value_type const*
        operator->() const noexcept
        {
            return this;
        }
    };

    ~object_ref() = default;

    BOOST_BEAST_DECL
    explicit
    object_ref(ref const& jr);

    BOOST_BEAST_DECL
    explicit
    object_ref(value const& jv);

    // iterators

    BOOST_BEAST_DECL
    iterator
    begin() const noexcept;

    BOOST_BEAST_DECL
    iterator
    end() const noexcept;

    // capacity

    BOOST_BEAST_DECL
    bool
    empty() const noexcept;

    BOOST_BEAST_DECL
    std::size_t
    size() const noexcept;

    // modifiers

    BOOST_BEAST_DECL
    void
    clear() const noexcept;

    template<class T>
    std::pair<iterator, bool>
    insert(key_param key, T&& t) const;

    template<class T>
    iterator
    insert_or_assign(key_param key, T&& t) const;

    BOOST_BEAST_DECL
    std::size_t
    erase(key_param key) const noexcept;

    BOOST_BEAST_DECL
    iterator
    erase(iterator const& pos) const noexcept;

    // lookup

    BOOST_BEAST_DECL
    mapped_type
    at(key_param key) const;

    BOOST_BEAST_DECL
    mapped_type
    operator[](key_param key) const;

    BOOST_BEAST_DECL
    std::size_t
    count(key_param key) const noexcept;

    BOOST_BEAST_DECL
    iterator
    find(key_param key) const noexcept;
};

} // json
} // beast
} // boost

#include <boost/beast/_experimental/json/impl/object_ref.hpp>
#ifdef BOOST_BEAST_HEADER_ONLY
#include <boost/beast/_experimental/json/impl/object_ref.ipp>
#endif

#endif
