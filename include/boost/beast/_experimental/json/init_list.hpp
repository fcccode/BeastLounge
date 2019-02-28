//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_INIT_LIST_HPP
#define BOOST_BEAST_JSON_INIT_LIST_HPP

#include <boost/beast/core/detail/config.hpp>
#include <boost/beast/_experimental/json/exchange.hpp>
#include <boost/beast/_experimental/json/key_param.hpp>
#include <boost/beast/_experimental/json/storage.hpp>
#include <initializer_list>
#include <type_traits>
#include <utility>

namespace boost {
namespace beast {
namespace json {

class ref;
class storage;

namespace detail {
class variant;
} // detail

/** An initializer wrapper for array elements
*/
class init_ref
{
    friend class detail::variant;

    struct base
    {
        BOOST_BEAST_DECL
        virtual ~base();

        virtual void move(void* to) noexcept = 0;

        virtual void assign(
            detail::variant& v,
            detail::storage_base& store) const = 0;
    };

    template<class T>
    class impl : base
    {
        T const& t_;

    public:
        impl(impl&&) = default;

        explicit
        impl(T const& t) noexcept;

        void
        move(void* to) noexcept override;

        void
        assign(
            detail::variant& v,
            detail::storage_base& store) const override;
    };

    using exemplar = int;

    using buf_type = typename
        std::aligned_storage<
            sizeof(impl<exemplar>)>::type;

    buf_type buf_;

    template<class T>
    void
    construct(T const& t) noexcept;

    base const&
    get() const noexcept
    {
        return *reinterpret_cast<
            base const*>(&buf_);
    }

public:
    init_ref() = delete;
    init_ref(init_ref const&) = delete;

    BOOST_BEAST_DECL
    ~init_ref();

    BOOST_BEAST_DECL
    init_ref(init_ref&& other);

    template<class T
#ifndef BOOST_BEAST_DOXYGEN
        ,class = typename std::enable_if<
            is_exchange_type<typename
                std::decay<T>::type>::value>::type
#endif
    >
    init_ref(T const& t) noexcept;
};

/// An initializer list for arrays
using init_list =
    std::initializer_list<init_ref>;

} // json
} // beast
} // boost

#include <boost/beast/_experimental/json/impl/init_list.hpp>
#ifdef BOOST_BEAST_HEADER_ONLY
#include <boost/beast/_experimental/json/impl/init_list.ipp>
#endif

#endif
