//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_VISITATION_HPP
#define BOOST_BEAST_JSON_VISITATION_HPP

#if 0

#include <boost/beast/core/detail/config.hpp>
#include <boost/beast/_experimental/json/ref.hpp>
#include <boost/beast/_experimental/json/detail/variant.hpp>
#include <new>
#include <vector>

namespace boost {
namespace beast {
namespace json {

/** Visit a JSON tree using a depth-first traversal
*/
class visitation
{
public:
    struct element
    {
        ref jv;
        string_view key;
        bool last;
    };

private:
    using map_it_type =
        detail::variant::map_type::const_iterator;

    using vec_it_type =
        detail::variant::vec_type::const_iterator;

    struct pos
    {
        ref jv;
        union
        {
            map_it_type map_it;
            vec_it_type vec_it;
            int i;
        };

        pos(json::ref const& jv_)
            : jv(jv_)
        {
            switch(jv.get_kind())
            {
            case kind::object:
                ::new(&map_it) map_it_type();
                break;

            case kind::array:
                ::new(&vec_it) vec_it_type();
                break;
            }
        }

        pos(pos const& other)
            : jv(other.jv)
        {
        }

        ~pos()
        {
            switch(jv.get_kind())
            {
            case kind::object:
                map_it.~map_it_type();
                break;

            case kind::array:
                vec_it.~vec_it_type();
                break;
            }
        }
    };

    std::vector<pos> stack_;

public:
    explicit
    visitation(ref const& jv)
    {
        stack_.emplace_back(jv);
    }

    void
    reset()
    {
        stack_.clear();
    }

    void
    reset(ref const& jv)
    {
        reset();
        stack_.emplace_back(jv);
    }

    bool
    is_done() const noexcept
    {
        return stack_.empty();
    }

    /** Retrieve the next element, or return false
    */
    bool
    next(json::ref& jv)
    {
        if(stack_.empty())
            return false;

        switch(stack_.back().jv.get_kind())
        {
        case kind::object:
            if(stack_.back().map_it == map_it_type{})
            {
                // first time
            }
            else
            {
            }
            break;

        case kind::array:
            break;

        default:
            jv = stack_.back().jv;
            //e.key = {}; // ???
            //e.last = true;
        }

        return true;
    }
};

} // json
} // beast
} // boost

//#include <boost/beast/_experimental/json/impl/visit.hpp>
#ifdef BOOST_BEAST_HEADER_ONLY
//#include <boost/beast/_experimental/json/impl/visit.ipp>
#endif

#endif

#endif
