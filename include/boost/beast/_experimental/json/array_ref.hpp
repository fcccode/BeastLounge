//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_ARRAY_REF_HPP
#define BOOST_BEAST_JSON_ARRAY_REF_HPP

#include <boost/beast/core/detail/config.hpp>
#include <boost/beast/_experimental/json/key_param.hpp>
#include <boost/beast/_experimental/json/ref.hpp>
#include <boost/beast/_experimental/json/value.hpp>
#include <boost/beast/_experimental/json/storage.hpp>
#include <boost/beast/_experimental/json/detail/variant.hpp>

namespace boost {
namespace beast {
namespace json {

class array_ref
{
    storage store_;
    detail::variant& v_;

    friend detail::variant;

    static
    ref
    make_ref(
        detail::variant& v,
        storage store);

    array_ref(
        detail::variant& v,
        storage const& store);

public:
    class iterator;

    ~array_ref() = default;

    BOOST_BEAST_DECL
    explicit
    array_ref(ref const& jr);

    BOOST_BEAST_DECL
    explicit
    array_ref(value const& jv);

    BOOST_BEAST_DECL
    ref
    at(std::size_t i) const;

    BOOST_BEAST_DECL
    ref
    operator[](std::size_t i) const noexcept;

    BOOST_BEAST_DECL
    ref
    front() const noexcept;

    BOOST_BEAST_DECL
    ref
    back() const noexcept;

    BOOST_BEAST_DECL
    iterator
    begin() const noexcept;

    BOOST_BEAST_DECL
    iterator
    end() const noexcept;

    BOOST_BEAST_DECL
    bool
    empty() const noexcept;

    BOOST_BEAST_DECL
    std::size_t
    size() const noexcept;

    BOOST_BEAST_DECL
    void
    clear() const noexcept;

    template<class T>
    iterator
    insert(iterator const& pos, T&& t) const;

    template<class T>
    iterator
    insert(
        iterator const& pos,
        std::size_t count,
        T const& t) const;

    BOOST_BEAST_DECL
    iterator
    erase(iterator const& pos) const noexcept;

    BOOST_BEAST_DECL
    iterator
    erase(
        iterator const& first,
        iterator const& last) const noexcept;

    template<class T>
    void
    push_back(T&& t) const;

    BOOST_BEAST_DECL
    void
    pop_back() const noexcept;

    BOOST_BEAST_DECL
    void
    resize(std::size_t n) const;
};

} // json
} // beast
} // boost

#include <boost/beast/_experimental/json/impl/array_ref.hpp>
#ifdef BOOST_BEAST_HEADER_ONLY
#include <boost/beast/_experimental/json/impl/array_ref.ipp>
#endif

#endif
