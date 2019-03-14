//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_IMPL_OBJECT_HPP
#define BOOST_BEAST_JSON_IMPL_OBJECT_HPP

#include <boost/beast/_experimental/json/value.hpp>
#include <boost/beast/_experimental/json/detail/varint.hpp>
#include <boost/core/exchange.hpp>
#include <type_traits>

namespace boost {
namespace beast {
namespace json {

namespace detail {

template<class T>
class arrow_proxy
{
    T t_;

public:
    arrow_proxy(T const& t)
        : t_(t)
    {
    }

    T*
    operator->() noexcept
    {
        return &t_;
    }
};

template<class T>
arrow_proxy<T>
make_arrow_proxy(T const& t)
{
    return arrow_proxy<T>(t);
}

} // detail

//------------------------------------------------------------------------------

struct object_t::list_hook
{
public:
    element* prev_;
    element* next_;
};

struct object_t::element
    : public list_hook
{
    value v_;
    element* local_next_;

    string_view
    key() const noexcept
    {
        auto p =
            reinterpret_cast<
                char const*>(this + 1);
        auto const result =
            detail::varint_read(p);
        return {
            p + result.second,
            static_cast<std::size_t>(
                result.first) };
    }

    template<class... Args>
    static
    element*
    construct(
        storage_ptr const& sp,
        string_view key,
        Args&&... args)
    {
        auto const n =
            detail::varint_size(key.size());
        auto const size =
            sizeof(element) +
                n + key.size() + 1 +
            alignof(element);
        auto p = reinterpret_cast<char*>(
            sp->allocate(
                sizeof(element) +
                    n + key.size() + 1,
                alignof(element)));

        struct cleanup
        {
            std::size_t size;
            storage_ptr const& sp;

            void
            operator()(char* p)
            {
                sp->deallocate(p,
                    size, alignof(element));
            }
        };
        std::unique_ptr<
            char, cleanup> up(
                p, cleanup{size, sp});
        ::new(p) element(
            std::forward<Args>(args)...);
        up.release();

        detail::varint_write(
            p + sizeof(element), key.size());
        std::memcpy(
            p + sizeof(element) + n,
            key.data(),
            key.size());
        p[sizeof(element) +
            n + key.size()] = '\0';
        return reinterpret_cast<element*>(p);
    }

    static
    void
    destroy(
        element* e,
        storage_ptr const& sp)
    {
        auto const len =
            e->key().size();
        auto const n =
            detail::varint_size(len);
        e->~element();
        sp->deallocate(e,
            sizeof(element) +
                n + len + 1,
            alignof(element));
    }

    // varint key size
    // char[] key name follows

private:
    template<class... Args>
    explicit
    element(Args&&... args)
        : v_(std::forward<Args>(args)...)
    {
    }
};

//------------------------------------------------------------------------------

class object_t::hasher
{
    BOOST_BEAST_DECL
    static
    std::pair<
        std::uint64_t, std::uint64_t>
    init(std::true_type) noexcept;

    BOOST_BEAST_DECL
    static
    std::pair<
        std::uint32_t, std::uint32_t>
    init(std::false_type) noexcept;

public:
    BOOST_BEAST_DECL
    std::size_t
    operator()(key_type key) const noexcept;
};

class object_t::key_equal
{
public:
};

//------------------------------------------------------------------------------

class object_t::pointer
{
    reference t_;

public:
    pointer(reference const& t)
        : t_(t)
    {
    }

    reference*
    operator->() noexcept
    {
        return &t_;
    }
};

class object_t::const_pointer
{
    const_reference t_;

public:
    const_pointer(
        const_reference const& t)
        : t_(t)
    {
    }

    const_reference*
    operator->() noexcept
    {
        return &t_;
    }
};

//------------------------------------------------------------------------------

class object_t::iterator
{
    element* e_ = nullptr;

    friend class object_t;

    iterator(element* e)
        : e_(e)
    {
    }

public:
    using value_type = object_t::value_type;
    
    using reference = object_t::reference;

    iterator() = default;

    iterator(
        iterator const&) = default;

    iterator& operator=(
        iterator const&) = default;

    bool
    operator==(
        iterator const& other) const noexcept
    {
        return e_ == other.e_;
    }

    bool
    operator!=(
        iterator const& other) const noexcept
    {
        return e_ != other.e_;
    }

    bool
    operator==(
        const_iterator const& other) const noexcept;

    bool
    operator!=(
        const_iterator const& other) const noexcept;

    iterator&
    operator++() noexcept
    {
        e_ = e_->next_;
        return *this;
    }

    iterator
    operator++(int) noexcept
    {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    iterator&
    operator--() noexcept
    {
        e_ = e_->prev_;
        return *this;
    }

    iterator
    operator--(int) noexcept
    {
        auto tmp = *this;
        --*this;
        return tmp;
    }

    pointer
    operator->() const noexcept
    {
        return reference{
            e_->key(), e_->v_ };
    }

    reference
    operator*() const noexcept
    {
        return {
            e_->key(), e_->v_ };
    }
};

//------------------------------------------------------------------------------

class object_t::const_iterator
{
    element const* e_ = nullptr;

    friend class object_t;

    const_iterator(element const* e)
        : e_(e)
    {
    }

public:
    using value_type = object_t::value_type;
    
    using reference = object_t::const_reference;

    const_iterator() = default;

    const_iterator(
        const_iterator const&) = default;

    const_iterator& operator=(
        const_iterator const&) = default;

    bool
    operator==(
        const_iterator const& other) const noexcept
    {
        return e_ == other.e_;
    }

    bool
    operator!=(
        const_iterator const& other) const noexcept
    {
        return e_ != other.e_;
    }

    bool
    operator==(
        iterator const& other) const noexcept
    {
        return e_ == other.e_;
    }

    bool
    operator!=(
        iterator const& other) const noexcept
    {
        return e_ != other.e_;
    }

    const_iterator&
    operator++() noexcept
    {
        e_ = e_->next_;
        return *this;
    }

    const_iterator
    operator++(int) noexcept
    {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    const_iterator&
    operator--() noexcept
    {
        e_ = e_->prev_;
        return *this;
    }

    const_iterator
    operator--(int) noexcept
    {
        auto tmp = *this;
        --*this;
        return tmp;
    }

    const_pointer
    operator->() const noexcept
    {
        return const_reference{
            e_->key(), e_->v_ };
    }

    const_reference
    operator*() const noexcept
    {
        return {
            e_->key(), e_->v_ };
    }
};

inline
bool
object_t::
iterator::
operator==(
    const_iterator const& other) const noexcept
{
    return e_ == other.e_;
}

inline
bool
object_t::
iterator::
operator!=(
    const_iterator const& other) const noexcept
{
    return e_ != other.e_;
}

//------------------------------------------------------------------------------

class object_t::local_iterator
{
    element* e_ = nullptr;

    friend class object_t;

    local_iterator(element* e)
        : e_(e)
    {
    }

public:
    using value_type = object_t::value_type;

    using reference = object_t::reference;

    local_iterator() = default;

    local_iterator(
        local_iterator const&) = default;

    local_iterator& operator=(
        local_iterator const&) = default;

    bool
    operator==(
        local_iterator const& other) const noexcept
    {
        return e_ == other.e_;
    }

    bool
    operator!=(
        local_iterator const& other) const noexcept
    {
        return e_ != other.e_;
    }

    bool
    operator==(
        iterator const& other) const noexcept
    {
        return e_ == other.e_;
    }

    bool
    operator!=(
        iterator const& other) const noexcept
    {
        return e_ != other.e_;
    }

    local_iterator&
    operator++() noexcept
    {
        e_ = e_->local_next_;
        return *this;
    }

    local_iterator
    operator++(int) noexcept
    {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    const_pointer
    operator->() const noexcept
    {
        return reference{
            e_->key(), e_->v_ };
    }

    const_reference
    operator*() const noexcept
    {
        return {
            e_->key(), e_->v_ };
    }
};

//------------------------------------------------------------------------------

class object_t::const_local_iterator
{
    element const* e_ = nullptr;

    friend class object_t;

    const_local_iterator(element const* e)
        : e_(e)
    {
    }

public:
    using value_type = object_t::value_type;
    
    using reference = object_t::const_reference;

    const_local_iterator() = default;

    const_local_iterator(
        const_local_iterator const&) = default;

    const_local_iterator& operator=(
        const_local_iterator const&) = default;

    bool
    operator==(
        const_local_iterator const& other) const noexcept
    {
        return e_ == other.e_;
    }

    bool
    operator!=(
        const_local_iterator const& other) const noexcept
    {
        return e_ != other.e_;
    }

    bool
    operator==(
        iterator const& other) const noexcept
    {
        return e_ == other.e_;
    }

    bool
    operator!=(
        iterator const& other) const noexcept
    {
        return e_ != other.e_;
    }

    const_local_iterator&
    operator++() noexcept
    {
        e_ = e_->local_next_;
        return *this;
    }

    const_local_iterator
    operator++(int) noexcept
    {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    const_pointer
    operator->() const noexcept
    {
        return const_reference{
            e_->key(), e_->v_ };
    }

    const_reference
    operator*() const noexcept
    {
        return {
            e_->key(), e_->v_ };
    }
};

//------------------------------------------------------------------------------

class object_t::node_type
{
    element* e_ = nullptr;
    storage_ptr sp_;

    friend class object_t;

    node_type(element* e)
        : e_(e)
    {
    }

public:
    node_type() = default;
    node_type(node_type const&) = delete;

    ~node_type()
    {
        element::destroy(e_, sp_);
    }

    node_type(node_type&& other)
        : e_(boost::exchange(other.e_, nullptr))
        , sp_(boost::exchange(other.sp_, nullptr))
    {
    }
};

//------------------------------------------------------------------------------

template<class... Args>
auto
object_t::
emplace(
    key_param key,
    Args&&... args) ->
        iterator
{
    maybe_rehash(size() + 1);
    auto* e = element::construct(
        sp_,
        key.str,
        std::forward<Args>(args)...);
    insert(e);
    return e;
}

inline
auto
object_t::
hash_function() const ->
    hasher
{
    return hasher{};
}

inline
auto
object_t::
key_eq() const ->
    key_equal
{
    return key_equal{};
}

} // json
} // beast
} // boost

#endif
