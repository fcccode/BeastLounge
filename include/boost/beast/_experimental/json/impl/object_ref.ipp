//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_IMPL_OBJECT_REF_IPP
#define BOOST_BEAST_JSON_IMPL_OBJECT_REF_IPP

#include <boost/beast/_experimental/json/object_ref.hpp>
#include <boost/beast/_experimental/json/ref.hpp>
#include <boost/throw_exception.hpp>
#include <stdexcept>

namespace boost {
namespace beast {
namespace json {

object_ref::
value_type::
value_type(
    string_view key_,
    ref const& jr)
    : key(key_)
    , value(jr)
{
}

//------------------------------------------------------------------------------

ref
object_ref::
make_ref(
    detail::variant& v,
    storage store)
{
    return ref(v, store);
}

object_ref::
object_ref(ref const& other)
    : store_(other.store_)
    , v_(other.v_)
{
    if(! other.is_object())
        BOOST_THROW_EXCEPTION(
            std::logic_error("not an object"));
}

object_ref::
object_ref(value const& other)
    : store_(other.store_)
    , v_(other.v_)
{
    if(! other.is_object())
        BOOST_THROW_EXCEPTION(
            std::logic_error("not an object"));
}

// iterators

auto
object_ref::
begin() const noexcept ->
    iterator
{
    return iterator(
        v_.map_v.begin(),
        store_);
}

auto
object_ref::
end() const noexcept ->
    iterator
{
    return iterator(
        v_.map_v.end(),
        store_);
}

// capacity

bool
object_ref::
empty() const noexcept
{
    return v_.map_v.empty();
}

std::size_t
object_ref::
size() const noexcept
{
    return v_.map_v.size();
}

// modifiers

void
object_ref::
clear() const noexcept
{
    v_.set_kind(kind::object, *store_);
}

std::size_t
object_ref::
erase(key_param key) const noexcept
{
    // VFALCO This constructs a string needlessly...
    auto it = v_.map_v.find(key.str.to_string());
    if(it == v_.map_v.end())
        return 0;
    store_->destroy(*it->second);
    v_.map_v.erase(it);
    return 1;
}

auto
object_ref::
erase(iterator const& pos) const noexcept ->
    iterator
{
    store_->destroy(*pos.it_->second);
    return iterator(
        v_.map_v.erase(pos.it_), store_);
}

// lookup

auto
object_ref::
at(key_param key) const ->
    mapped_type
{
    // VFALCO This constructs a string needlessly...
    auto it = v_.map_v.find(key.str.to_string());
    if(it == v_.map_v.end())
        BOOST_THROW_EXCEPTION(
            std::out_of_range("key not found in object"));
    return ref(*it->second, store_);
}

auto
object_ref::
operator[](key_param key) const ->
    mapped_type
{
    // VFALCO This constructs a string needlessly...
    auto result = v_.map_v.emplace(
        std::make_pair(key.str.to_string(), nullptr));
    if(result.second)
    {
        try
        {
            result.first->second = &store_->construct();
        }
        catch(...)
        {
            v_.map_v.erase(result.first);
            throw;
        }
    }
    return make_ref(*result.first->second, store_);
}

std::size_t
object_ref::
count(key_param key) const noexcept
{
    // VFALCO This constructs a string needlessly...
    return v_.map_v.count(key.str.to_string());
}

auto
object_ref::
find(key_param key) const noexcept ->
    iterator
{
    // VFALCO This constructs a string needlessly...
    return iterator(
        v_.map_v.find(key.str.to_string()),
        store_);
}

} // json
} // beast
} // boost

#endif
