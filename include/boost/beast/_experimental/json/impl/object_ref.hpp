//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_IMPL_OBJECT_REF_HPP
#define BOOST_BEAST_JSON_IMPL_OBJECT_REF_HPP

#include <utility>

namespace boost {
namespace beast {
namespace json {

class object_ref::iterator
{
    using iter_type =
        detail::variant::map_type::iterator;

    storage store_ = nullptr;
    iter_type it_{};

    friend class object_ref;

    iterator(
        iter_type it,
        storage const& store)
        : store_(store)
        , it_(std::move(it))
    {
    }

public:
    using value_type = object_ref::value_type;
    using reference = ref;

    iterator() = default;

    iterator&
    operator++() noexcept
    {
        ++it_;
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
        --it_;
        return *this;
    }

    iterator
    operator--(int) noexcept
    {
        auto tmp = *this;
        --*this;
        return tmp;
    }

    bool
    operator==(iterator const& other) const noexcept
    {
        return it_ == other.it_;
    }

    bool
    operator!=(iterator const& other) const noexcept
    {
        return it_ != other.it_;
    }

    value_type
    operator->() const noexcept
    {
        return value_type(
            it_->first,
            object_ref::make_ref(
                *it_->second, store_));
    }

    value_type
    operator*() const noexcept
    {
        return value_type(
            it_->first,
            object_ref::make_ref(
                *it_->second, store_));
    }
};

//------------------------------------------------------------------------------

template<class T>
auto
object_ref::
insert(key_param key, T&& t) const ->
    std::pair<iterator, bool>
{
    // VFALCO This constructs a string needlessly...
    auto result = v_.map_v.emplace(
        std::make_pair(key.str.to_string(), nullptr));
    if(result.second)
    {
        try
        {
            result.first->second = &store_->construct();
            result.first->second->assign(
                std::forward<T>(t), store_);
            result.second = true;
        }
        catch(...)
        {
            if(result.first->second)
                store_.destroy(*result.first->second);
            v_.map_v.erase(result.first);
            throw;
        }

    }
    return std::make_pair(
        iterator(result.first, store_),
        result.second);
}

template<class T>
auto
object_ref::
insert_or_assign(key_param key, T&& t) const ->
    iterator
{
    // VFALCO This constructs a string needlessly...
    auto result = v_.map_v.emplace(
        std::make_pair(key.str.to_string(), nullptr));
    if(result.second)
    {
        try
        {
            result.first->second = &store_->construct();
            result.first->second->assign(
                std::forward<T>(t), *store_);
            result.second = true;
        }
        catch(...)
        {
            v_.map_v.erase(result.first);
            throw;
        }
    }
    else
    {
        result.first->second->assign(
            std::forward<T>(t), *store_);
    }
    return iterator(result.first, store_);
}

} // json
} // beast
} // boost

#endif
