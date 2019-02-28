//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_DETAIL_STORAGE_IMPL_HPP
#define BOOST_BEAST_JSON_DETAIL_STORAGE_IMPL_HPP

#include <boost/beast/_experimental/json/detail/storage_base.hpp>
#include <boost/assert.hpp>
#include <boost/core/empty_value.hpp>

namespace boost {
namespace beast {
namespace json {

class storage;

namespace detail {

template<class Allocator>
class storage_impl
    : private boost::empty_value<Allocator>
    , public storage_base
{
    variant* free_ = nullptr;

public:
    ~storage_impl()
    {
        while(free_)
        {
            auto v = free_;
            free_ = free_->next;
            BOOST_ASSERT(
                v->get_kind() ==
                    kind::null);
            delete v;
        }
    }

    explicit
    storage_impl(Allocator const& alloc)
        : boost::empty_value<Allocator>(
            boost::empty_init_t{}, alloc)
    {
    }

    void
    destroy(variant& v) override
    {
        v.set_kind(kind::null, *this);
        v.next = free_;
        free_ = &v;
    }

    variant&
    construct() override
    {
        variant* v;
        if(free_)
        {
            v = free_;
            free_ = free_->next;
        }
        else
        {
            v = new variant;
        }
        BOOST_ASSERT(
            v->get_kind() == kind::null);
        return *v;
    }

    void
    copy(
        variant& lhs,
        variant const& rhs) override
    {
        lhs.set_kind(rhs.get_kind(), *this);
        switch(lhs.get_kind())
        {
        case kind::object:
//            for(auto e : rhs.map_v)
            {
                
            }
            break;

        case kind::array:
//            for(auto rv : rhs.vec_v)
            {
                
            }
            break;

        case kind::string:
            lhs.str_v = rhs.str_v;
            break;

        case kind::signed64:
            lhs.int64_v = rhs.int64_v;
            break;

        case kind::unsigned64:
            lhs.uint64_v = rhs.uint64_v;
            break;

        case kind::floating:
            lhs.double_v = rhs.double_v;
            break;

        case kind::boolean:
            lhs.bool_v = rhs.bool_v;
            break;

        case kind::null:
            break;
        }
    }
};

} // detail
} // json
} // beast
} // boost

#endif
