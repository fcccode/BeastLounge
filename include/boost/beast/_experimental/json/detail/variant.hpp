//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_DETAIL_VARIANT_HPP
#define BOOST_BEAST_JSON_DETAIL_VARIANT_HPP

#include <boost/beast/_experimental/json/init_list.hpp>
#include <boost/beast/_experimental/json/kind.hpp>
#include <boost/beast/_experimental/json/detail/storage_base.hpp>
#include <boost/beast/_experimental/json/detail/variant_exchange.hpp>
#include <cstddef>
#include <iosfwd>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace boost {
namespace beast {
namespace json {
namespace detail {

class variant final
{
public:
    using map_type =
        std::unordered_map<
            std::string, variant*>;

    using vec_type =
        std::vector<variant*>;

    using str_type =
        std::string;

    union
    {
        variant*        next;       // with kind::null
        map_type        map_v;
        vec_type        vec_v;
        str_type        str_v;
        std::int64_t    int64_v;
        std::uint64_t   uint64_v;
        double          double_v;
        bool            bool_v;
    };

private:
    kind kind_ = kind::null;
public:

    variant(variant const&) = delete;
    variant& operator=(variant const&) = delete;

    BOOST_BEAST_DECL
    variant();

    BOOST_BEAST_DECL
    ~variant();

    kind
    get_kind() const noexcept
    {
        return kind_;
    }

    BOOST_BEAST_DECL
    void
    set_kind(kind k, storage_base& store);

    BOOST_BEAST_DECL
    void
    output(std::ostream& os);

    BOOST_BEAST_DECL
    void
    assign(
        init_list list,
        storage_base& store);

    template<class T>
    void
    assign(
        T&& t,
        storage_base& store)
    {
        assign(t, store,
            is_variant_exchange<T>{});
    }

    template<class T>
    void
    assign(
        T&& t,
        storage_base& store,
        std::true_type)
    {
        using type = typename
            std::decay<T>::type;
        variant_exchange<type>::assign(
            *this,
            store,
            std::forward<T>(t));
    }

    template<class T>
    void
    assign(T&& t,
        storage_base& store,
        std::false_type)
    {
        // If this assert goes off, it means that the type you
        // are trying to assign is not valid. Either assign a known
        // type, or provide a specialization of `json::exchange<>`
        // for your user defined type.

        static_assert(
            is_variant_exchange<T>::value,
            "Exchange requirements not met");
    }
};

//------------------------------------------------------------------------------

template<>
struct variant_exchange<object_t>
{
    static void assign(
        variant& lhs,
        storage_base& store,
        object_t) noexcept
    {
        lhs.set_kind(kind::null, store);
        lhs.set_kind(kind::object, store);
    }
};

template<>
struct variant_exchange<array_t>
{
    static void assign(
        variant& lhs,
        storage_base& store,
        array_t) noexcept
    {
        lhs.set_kind(kind::null, store);
        lhs.set_kind(kind::array, store);
    }
};

template<class CharT, class Allocator>
struct variant_exchange<
    std::basic_string<char, CharT, Allocator>>
{
    static void assign(
        variant& lhs,
        storage_base& store,
        std::basic_string<char, CharT, Allocator> const& v) noexcept
    {
        lhs.set_kind(kind::string, store);
        lhs.str_v = v;
    }
};

template<>
struct variant_exchange<char const*>
{
    static void assign(
        variant& lhs,
        storage_base& store,
        string_view v) noexcept
    {
        lhs.set_kind(kind::string, store);
        lhs.str_v = v.to_string();
    }
};

template<>
struct variant_exchange<short>
{
    static void assign(
        variant& lhs,
        storage_base& store,
        short v) noexcept
    {
        lhs.set_kind(kind::signed64, store);
        lhs.int64_v = v;
    }
};

template<>
struct variant_exchange<int>
{
    static void assign(
        variant& lhs,
        storage_base& store,
        int v) noexcept
    {
        lhs.set_kind(kind::signed64, store);
        lhs.int64_v = v;
    }
};

template<>
struct variant_exchange<long>
{
    static void assign(
        variant& lhs,
        storage_base& store,
        long v) noexcept
    {
        lhs.set_kind(kind::signed64, store);
        lhs.int64_v = v;
    }
};

template<>
struct variant_exchange<long long>
{
    static void assign(
        variant& lhs,
        storage_base& store,
        long long v) noexcept
    {
        lhs.set_kind(kind::signed64, store);
        lhs.int64_v = v;
    }
};

template<>
struct variant_exchange<unsigned short>
{
    static void assign(
        variant& lhs,
        storage_base& store,
        unsigned short v) noexcept
    {
        lhs.set_kind(kind::unsigned64, store);
        lhs.uint64_v = v;
    }
};

template<>
struct variant_exchange<unsigned int>
{
    static void assign(
        variant& lhs,
        storage_base& store,
        unsigned int v) noexcept
    {
        lhs.set_kind(kind::unsigned64, store);
        lhs.uint64_v = v;
    }
};

template<>
struct variant_exchange<unsigned long>
{
    static void assign(
        variant& lhs,
        storage_base& store,
        unsigned long v) noexcept
    {
        lhs.set_kind(kind::unsigned64, store);
        lhs.uint64_v = v;
    }
};

template<>
struct variant_exchange<unsigned long long>
{
    static void assign(
        variant& lhs,
        storage_base& store,
        unsigned long long v) noexcept
    {
        lhs.set_kind(kind::unsigned64, store);
        lhs.uint64_v = v;
    }
};

template<>
struct variant_exchange<float>
{
    static void assign(
        variant& lhs,
        storage_base& store,
        float v) noexcept
    {
        lhs.set_kind(kind::floating, store);
        lhs.double_v = v;
    }
};

template<>
struct variant_exchange<double>
{
    static void assign(
        variant& lhs,
        storage_base& store,
        double v) noexcept
    {
        lhs.set_kind(kind::floating, store);
        lhs.double_v = v;
    }
};

template<>
struct variant_exchange<bool>
{
    static void assign(
        variant& lhs,
        storage_base& store,
        bool v) noexcept
    {
        lhs.set_kind(kind::boolean, store);
        lhs.bool_v = v;
    }
};

template<>
struct variant_exchange<null_t>
{
    static void assign(
        variant& lhs,
        storage_base& store,
        null_t) noexcept
    {
        lhs.set_kind(kind::null, store);
    }
};

template<>
struct variant_exchange<std::nullptr_t>
{
    static void assign(
        variant& lhs,
        storage_base& store,
        std::nullptr_t) noexcept
    {
        lhs.set_kind(kind::null, store);
    }
};

} // detail
} // json
} // beast
} // boost

#ifdef BOOST_BEAST_HEADER_ONLY
#include <boost/beast/_experimental/json/detail/variant.ipp>
#endif

#endif
