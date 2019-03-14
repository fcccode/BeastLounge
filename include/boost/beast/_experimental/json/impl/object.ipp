//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_IMPL_OBJECT_IPP
#define BOOST_BEAST_JSON_IMPL_OBJECT_IPP

#include <boost/beast/_experimental/json/object.hpp>
#include <boost/core/ignore_unused.hpp>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <memory>
#include <new>
#include <type_traits>

namespace boost {
namespace beast {
namespace json {

namespace detail {

struct primes
{
    using value_type = std::size_t;
    using iterator = std::size_t const*;

    std::size_t const* begin_;
    std::size_t const* end_;

    iterator
    begin() const noexcept
    {
        return begin_;
    }

    iterator
    end() const noexcept
    {
        return end_;
    }
};

// Taken from Boost.Intrusive and Boost.MultiIndex code,
// thanks to Ion Gaztanaga and Joaquin M Lopez Munoz.

template<class = void>
primes
get_primes(std::false_type) noexcept
{
    static std::size_t constexpr list[] = {
        3UL,                     7UL,
        11UL,                    17UL,
        29UL,                    53UL,
        97UL,                    193UL,
        389UL,                   769UL,
        1543UL,                  3079UL,
        6151UL,                  12289UL,
        24593UL,                 49157UL,
        98317UL,                 196613UL,
        393241UL,                786433UL,
        1572869UL,               3145739UL,
        6291469UL,               12582917UL,
        25165843UL,              50331653UL,
        100663319UL,             201326611UL,
        402653189UL,             805306457UL,
        1610612741UL,            3221225473UL,

        4294967291UL,            4294967295UL
    };
    return {
        &list[0],
        &list[std::extent<
            decltype(list)>::value] };
}

template<class = void>
primes
get_primes(std::true_type) noexcept
{
    static std::size_t constexpr list[] = {
        3ULL,                     7ULL,
        11ULL,                    17ULL,
        29ULL,                    53ULL,
        97ULL,                    193ULL,
        389ULL,                   769ULL,
        1543ULL,                  3079ULL,
        6151ULL,                  12289ULL,
        24593ULL,                 49157ULL,
        98317ULL,                 196613ULL,
        393241ULL,                786433ULL,
        1572869ULL,               3145739ULL,
        6291469ULL,               12582917ULL,
        25165843ULL,              50331653ULL,
        100663319ULL,             201326611ULL,
        402653189ULL,             805306457ULL,
        1610612741ULL,            3221225473ULL,

        6442450939ULL,            12884901893ULL,
        25769803751ULL,           51539607551ULL,
        103079215111ULL,          206158430209ULL,
        412316860441ULL,          824633720831ULL,
        1649267441651ULL,         3298534883309ULL,
        6597069766657ULL,         13194139533299ULL,
        26388279066623ULL,        52776558133303ULL,
        105553116266489ULL,       211106232532969ULL,
        422212465066001ULL,       844424930131963ULL,
        1688849860263953ULL,      3377699720527861ULL,
        6755399441055731ULL,      13510798882111483ULL,
        27021597764222939ULL,     54043195528445957ULL,
        108086391056891903ULL,    216172782113783843ULL,
        432345564227567621ULL,    864691128455135207ULL,
        1729382256910270481ULL,   3458764513820540933ULL,
        6917529027641081903ULL,   13835058055282163729ULL,
        18446744073709551557ULL,  18446744073709551615ULL
    };
    return {
        &list[0],
        &list[std::extent<
            decltype(list)>::value] };
}

BOOST_BEAST_DECL
primes
get_primes() noexcept
{
    return get_primes(
        std::integral_constant<bool,
            sizeof(std::size_t) >=
                sizeof(unsigned long long)>{});
}

} // detail

//------------------------------------------------------------------------------

class object_t::table
{
public:
    // number of values in the object
    std::size_t count = 0;

    // number of buckets in table
    std::size_t bucket_count = 0;

    // insertion-order list of all objects
    element* head = nullptr;

    // placeholder for end iterators to
    // point to, this is dynamically allocated
    element* end = nullptr;

    element*&
    bucket(std::size_t n) noexcept
    {
        return reinterpret_cast<
            element**>(this + 1)[n];
    }

    static
    void
    destroy(table* p, storage_ptr const& sp)
    {
        auto const count =
            p->bucket_count;
        sp->deallocate(
            p->end,
            sizeof(element),
            alignof(element));
        p->~table();
        sp->deallocate(
            p,
            sizeof(table) +
                count * sizeof(element*),
            alignof(table));
    }

    static
    table*
    construct(
        table* from,
        size_type bucket_count,
        storage_ptr const& sp)
    {
        struct cleanup
        {
            storage_ptr const& sp;

            void
            operator()(element* e)
            {
                sp->deallocate(
                    e,
                    sizeof(element),
                    alignof(element));
            }
        };
        std::unique_ptr<
            element, cleanup> end(
                nullptr, cleanup{sp});
        if(! from)
            end.reset(
                reinterpret_cast<element*>(
                    sp->allocate(
                        sizeof(element),
                        alignof(element))));
        auto p = reinterpret_cast<
            table*>(sp->allocate(
                sizeof(table) +
                    bucket_count *
                    sizeof(element*),
                alignof(table)));
        
        if(from)
        {
            p->count = from->count;
            p->bucket_count = bucket_count;
            p->head = from->head;
            p->end = from->end;
        }
        else
        {
            p->count = 0;
            p->bucket_count = bucket_count;
            p->head = end.release();
            p->end = p->head;
        }
        for(size_type i = 0;
            i < bucket_count; ++i)
            p->bucket(i) = p->end;
        if(from)
            destroy(from, sp);
        return p;
    }
};

//------------------------------------------------------------------------------

std::pair<
    std::uint64_t, std::uint64_t>
object_t::
hasher::
init(std::true_type) noexcept
{
    return {
        0x100000001B3ULL,
        0xcbf29ce484222325ULL
    };
}

std::pair<
    std::uint32_t, std::uint32_t>
object_t::
hasher::
init(std::false_type) noexcept
{
    return {
        0x01000193UL,
        0x811C9DC5UL
    };
}

std::size_t
object_t::
hasher::
operator()(key_type key) const noexcept
{
    std::size_t prime;
    std::size_t hash;
    std::tie(prime, hash) = init(
        std::integral_constant<bool,
            sizeof(std::size_t) >=
        sizeof(unsigned long long)>{});
    for(auto p = key.begin(),
        end = key.end(); p != end; ++p)
        hash = (*p ^ hash) * prime;
    return hash;
}

//------------------------------------------------------------------------------
//
// Special Members
//
//------------------------------------------------------------------------------

object_t::
~object_t()
{
    if(tab_)
    {
        for(auto e = tab_->head;
            e != tab_->end;)
        {
            auto p = e++;
            element::destroy(p, sp_);
        }
        table::destroy(tab_, sp_);
    }
}

object_t::
object_t()
    : object_t(get_default_storage_ptr())
{
}

object_t::
object_t(storage_ptr sp)
    : sp_(std::move(sp))
{
}

//------------------------------------------------------------------------------
//
// Iterators
//
//------------------------------------------------------------------------------

auto
object_t::
begin() noexcept ->
    iterator
{
    if(! tab_)
        return {};
    return tab_->head;
}

auto
object_t::
begin() const noexcept ->
    const_iterator
{
    if(! tab_)
        return {};
    return tab_->head;
}

auto
object_t::
cbegin() noexcept ->
    const_iterator
{
    if(! tab_)
        return {};
    return tab_->head;
}

auto
object_t::
end() noexcept ->
    iterator
{
    if(! tab_)
        return {};
    return reinterpret_cast<
        element*>(&tab_->end);
}

auto
object_t::
end() const noexcept ->
    const_iterator
{
    if(! tab_)
        return {};
    return reinterpret_cast<
        element const*>(&tab_->end);
}

auto
object_t::
cend() noexcept ->
    const_iterator
{
    if(! tab_)
        return {};
    return reinterpret_cast<
        element const*>(&tab_->end);
}

//------------------------------------------------------------------------------
//
// Capacity
//
//------------------------------------------------------------------------------

bool
object_t::
empty() const noexcept
{
    return ! tab_ || tab_->count == 0;
}

auto
object_t::
size() const noexcept ->
    size_type
{
    if(! tab_)
        return 0;
    return tab_->count;
}

auto
object_t::
max_size() const noexcept ->
    size_type
{
    return (std::numeric_limits<
        size_type>::max)();
}

//------------------------------------------------------------------------------
//
// Modifiers
//
//------------------------------------------------------------------------------

void
object_t::
clear() noexcept
{
    if(! tab_)
        return;
    for(auto e = tab_->head;
        e != tab_->end;)
    {
        auto p = e++;
        element::destroy(p, sp_);
    }
}

auto
object_t::
insert(value_type const& v) ->
    iterator
{
    return {};
}

auto
object_t::
insert(value_type&& v) ->
    iterator
{
    return {};
}

auto
object_t::
insert(
    const_iterator hint,
    value_type& v) ->
        iterator
{
    return {};
}

auto
object_t::
insert(
    const_iterator hint,
    value_type&& v) ->
        iterator
{
    return {};
}

void
object_t::
insert(std::initializer_list<
    value_type> ilist)
{
}

auto
object_t::
insert(node_type&& nh) ->
    iterator
{
    return {};
}

auto
object_t::
insert(
    const_iterator hint,
    node_type&& nh) ->
        iterator
{
    return {};
}

auto
object_t::
erase(const_iterator pos) ->
    iterator
{
    return {};
}
    
auto
object_t::
erase(
    const_iterator first,
    const_iterator last) ->
        iterator
{
    return {};
}

auto
object_t::
erase(key_param key) ->
    size_type
{
    return 0;
}

auto
object_t::
extract(
    const_iterator pos) ->
        node_type
{
    return {};
}

auto
object_t::
extract(key_param key) ->
    node_type
{
    return {};
}

void
object_t::
merge(object_t& source)
{
}

void
object_t::
merge(object_t&& source)
{
}

//------------------------------------------------------------------------------
//
// Lookup
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//
// Bucket Interface
//
//------------------------------------------------------------------------------

auto
object_t::
begin(size_type n) noexcept ->
    local_iterator
{
    if(! tab_)
        return {};
    return tab_->bucket(n);
}

auto
object_t::
begin(size_type n) const noexcept ->
    const_local_iterator
{
    if(! tab_)
        return {};
    return tab_->bucket(n);
}

auto
object_t::
cbegin(size_type n) noexcept ->
    const_local_iterator
{
    if(! tab_)
        return {};
    return tab_->bucket(n);
}

auto
object_t::
end(size_type n)  noexcept ->
    local_iterator
{
    boost::ignore_unused(n);
    if(! tab_)
        return {};
    return tab_->end;
}

auto
object_t::
end(size_type n) const noexcept ->
    const_local_iterator
{
    boost::ignore_unused(n);
    if(! tab_)
        return {};
    return tab_->end;
}

auto
object_t::
cend(size_type n) noexcept ->
    const_local_iterator
{
    boost::ignore_unused(n);
    if(! tab_)
        return {};
    return tab_->end;
}

auto
object_t::
bucket_count() const noexcept ->
    size_type
{
    if(! tab_)
        return 0;
    return tab_->bucket_count;
}

auto
object_t::
max_bucket_count() const noexcept ->
    size_type
{
    return (std::numeric_limits<
        size_type>::max)();
}

auto
object_t::
bucket_size(size_type n) const noexcept ->
    size_type
{
    if(! tab_)
        return 0;
    size_type size = 0;
    for(auto e = tab_->bucket(n);
        e != tab_->end; ++e)
        ++size;
    return size;
}

auto
object_t::
bucket(key_param key) const noexcept ->
    size_type
{
    if(! tab_)
        return 0; // undefined
    return hasher{}(key.str) %
        bucket_count();
}

//------------------------------------------------------------------------------
//
// Hash Policy
//
//------------------------------------------------------------------------------

float
object_t::
load_factor() const noexcept
{
    if(! tab_)
        return 0;
    return static_cast<float>(
        size()) / bucket_count();
}

float
object_t::
max_load_factor() const
{
    return mf_;
}

void
object_t::
max_load_factor(float ml)
{
    mf_ = ml;
}

void
object_t::
rehash(size_type count)
{
    auto const lim = static_cast<
        size_type>(std::ceil(
            size() / max_load_factor()));
    if(count < lim)
        count = lim;
    tab_ = table::construct(
        tab_,
        count,
        sp_);
    for(auto e = tab_->head;
        e != tab_->end; ++e)
    {
        auto const n =
            hasher{}(e->key()) %
                count;
        e->local_next_ =
            tab_->bucket(n);
        tab_->bucket(n) = e;
    }
}

void
object_t::
reserve(size_type count)
{
    rehash(static_cast<
        size_type>(std::ceil(
            count / max_load_factor())));
}

// count is the new number of items
void
object_t::
maybe_rehash(size_type count)
{
    if(! tab_)
    {
        auto const p =
            detail::get_primes();
        return reserve(*std::lower_bound(
            p.begin(), p.end(), count));
    }

    if(
        static_cast<float>(count) /
        bucket_count() > mf_)
    {
        auto const p =
            detail::get_primes();
        return reserve(*std::lower_bound(
            p.begin(), p.end(), count));

    }
}

void
object_t::
insert(element* e)
{
    insert(e, hasher{}(e->key()));
}

void
object_t::
insert(element* e, std::size_t hash)
{
    auto bucket = tab_->bucket(
        hash % tab_->bucket_count);
    e->local_next_ = bucket;
    bucket= e;
    e->prev_ = tab_->end->prev_;
    if(! e->prev_)
        tab_->head = e;
    e->next_ = nullptr;
    ++tab_->count;
}

} // json
} // beast
} // boost

#endif
