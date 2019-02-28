//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_IMPL_ARRAY_REF_IPP
#define BOOST_BEAST_JSON_IMPL_ARRAY_REF_IPP

#include <boost/beast/_experimental/json/array_ref.hpp>
#include <boost/beast/_experimental/json/ref.hpp>
#include <boost/throw_exception.hpp>
#include <stdexcept>

namespace boost {
namespace beast {
namespace json {

ref
array_ref::
make_ref(
    detail::variant& v,
    storage store)
{
    return ref(v, store);
}

array_ref::
array_ref(
    detail::variant& v,
    storage const& store)
    : store_(store)
    , v_(v)
{
}

array_ref::
array_ref(ref const& other)
    : store_(other.store_)
    , v_(other.v_)
{
    if(! other.is_array())
        BOOST_THROW_EXCEPTION(
            std::logic_error("not an array"));
}

array_ref::
array_ref(value const& other)
    : store_(other.store_)
    , v_(other.v_)
{
    if(! other.is_array())
        BOOST_THROW_EXCEPTION(
            std::logic_error("not an array"));
}

ref
array_ref::
at(std::size_t i) const
{
    if(i >= size())
        BOOST_THROW_EXCEPTION(
            std::out_of_range("array index out of range"));
    return array_ref::make_ref(
        *v_.vec_v[i], store_);
}

ref
array_ref::
operator[](std::size_t i) const noexcept
{
    return array_ref::make_ref(
        *v_.vec_v[i], store_);
}

ref
array_ref::
front() const noexcept
{
    return (*this)[0];
}

ref
array_ref::
back() const noexcept
{
    return (*this)[size() - 1];
}

auto
array_ref::
begin() const noexcept ->
    iterator
{
    return iterator(
        v_.vec_v.begin(), store_);
}

auto
array_ref::
end() const noexcept ->
    iterator
{
    return iterator(
        v_.vec_v.end(), store_);
}

bool
array_ref::
empty() const noexcept
{
    return v_.vec_v.empty();
}

std::size_t
array_ref::
size() const noexcept
{
    return v_.vec_v.size();
}

void
array_ref::
clear() const noexcept
{
    v_.set_kind(kind::array, *store_);
}

auto
array_ref::
erase(iterator const& pos) const noexcept ->
    iterator
{
    store_->destroy(**pos.it_);
    return iterator(
        v_.vec_v.erase(pos.it_),
        store_);
}

auto
array_ref::
erase(
    iterator const& first,
    iterator const& last) const noexcept ->
    iterator
{
    for(auto it = first; it != last; ++ it)
        store_->destroy(**it.it_);
    return iterator(
        v_.vec_v.erase(first.it_, last.it_),
        store_);
}

void
array_ref::
pop_back() const noexcept
{
    store_->destroy(*v_.vec_v.back());
    v_.vec_v.pop_back();
}

void
array_ref::
resize(std::size_t n) const
{
    if(n == v_.vec_v.size())
        return;
    if(n > v_.vec_v.size())
    {

    }
    else
    {

    }
    BOOST_THROW_EXCEPTION(
        std::logic_error("unimplemented"));
}

} // json
} // beast
} // boost

#endif
