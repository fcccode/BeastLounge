//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_IMPL_ARRAY_REF_HPP
#define BOOST_BEAST_JSON_IMPL_ARRAY_REF_HPP

namespace boost {
namespace beast {
namespace json {

class array_ref::iterator
{
    using iter_type =
        detail::variant::vec_type::iterator;

    storage store_ = nullptr;
    iter_type it_{};

    friend class array_ref;

    iterator(
        iter_type it,
        storage const& store)
        : store_(store)
        , it_(std::move(it))
    {
    }

public:
    using value_type = ref;
    using difference_type = typename
        std::iterator_traits<iter_type>::difference_type;
    using reference = ref;

    iterator() = default;

    iterator&
    operator+=(difference_type n) noexcept
    {
        it_ += n;
        return *this;
    }

    iterator&
    operator-=(difference_type n) noexcept
    {
        it_ -= n;
        return *this;
    }

    friend
    iterator
    operator+(iterator it, difference_type n) noexcept
    {
        return it += n;
    }

    friend
    iterator
    operator+(difference_type n, iterator it) noexcept
    {
        return it += n;
    }

    iterator
    operator-(difference_type n) const noexcept
    {
        return iterator(it_ - n, store_);
    }

    difference_type
    operator-(iterator const& rhs) const noexcept
    {
        return rhs.it_ - it_;
    }

    reference
    operator[](difference_type n) const
    {
        return array_ref::make_ref(
            *it_[n], store_);
    }

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
        return array_ref::make_ref(
            **it_, store_);
    }
};

//------------------------------------------------------------------------------

template<class T>
auto
array_ref::
insert(iterator const& pos, T&& t) const ->
    iterator
{
    auto i = pos.it_ - v_.vec_v.begin();
    detail::variant* v = &store_->construct();
    try
    {
        v->assign(std::forward<T>(t), *store_);
        return iterator(
            v_.vec_v.insert(
                v_.vec_v.begin() + i, v),
            store_);
    }
    catch(...)
    {
        store_->destroy(*v);
        throw;
    }
}

template<class T>
auto
array_ref::
insert(
    iterator const& pos,
    std::size_t count,
    T const& t) const ->
        iterator
{
    return {};
}

template<class T>
void
array_ref::
push_back(T&& t) const
{
    insert(end(), std::forward<T>(t));
}

} // json
} // beast
} // boost

#endif
