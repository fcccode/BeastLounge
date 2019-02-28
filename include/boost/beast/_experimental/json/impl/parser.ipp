//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_IMPL_PARSER_IPP
#define BOOST_BEAST_JSON_IMPL_PARSER_IPP

#include <boost/beast/_experimental/json/parser.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/assert.hpp>

namespace boost {
namespace beast {
namespace json {

parser::
parser()
{
}

parser::
parser(storage const& store)
    : jv_(store)
{
}

void
parser::
on_document_begin(error_code&)
{
    stack_.clear();
    stack_.emplace_back(jv_);
    s_.clear();
}

void
parser::
on_object_begin(error_code&)
{
    if(stack_.back().is_object())
    {
        BOOST_ASSERT(! key().empty());
        object_ref ro(stack_.back());
        stack_.emplace_back(
            ro.insert_or_assign(
                key(), object)->value);
    }
    else if(stack_.back().is_array())
    {
        BOOST_ASSERT(key().empty());
        array_ref ra(stack_.back());
        ra.push_back(object);
        stack_.push_back(ra.back());
    }
    else
    {
        BOOST_ASSERT(stack_.back().is_null());
        stack_.back() = object;
    }
}

void
parser::
on_object_end(error_code&)
{
    BOOST_ASSERT(stack_.back().is_object());
    stack_.pop_back();
}

void
parser::
on_array_begin(error_code&)
{
    if(stack_.back().is_object())
    {
        BOOST_ASSERT(! key().empty());
        object_ref ro(stack_.back());
        stack_.emplace_back(
            ro.insert_or_assign(
                key(), array)->value);
    }
    else if(stack_.back().is_array())
    {
        BOOST_ASSERT(key().empty());
        array_ref ra(stack_.back());
        ra.push_back(array);
        stack_.push_back(ra.back());
    }
    else
    {
        BOOST_ASSERT(stack_.back().is_null());
        stack_.back() = array;
    }
}

void
parser::
on_array_end(error_code&)
{
    BOOST_ASSERT(stack_.back().is_array());
    stack_.pop_back();
}

void
parser::
on_string_begin(error_code&)
{
    s_.clear();
}

void
parser::
on_string_piece(
    string_view s, error_code&)
{
    s_.append(s.data(), s.size());
}

void
parser::
on_string_end(error_code&)
{
    assign(s_);
    s_.clear();
}

void
parser::
on_number(number n, error_code&)
{
    if(n.is_integral())
    {
        assign(n.mant);
    }
    else
    {
        long exp = n.pos ?
            n.exp : -static_cast<long>(n.exp);
        auto mul = std::pow(10., exp);
        auto d = (n.neg ? -1. : 1.) * n.mant * mul;
        assign(d);
    }
}

void
parser::
on_bool(bool b, error_code&)
{
    assign(b);
}

void
parser::
on_null(error_code&)
{
    assign(null);
}

} // json
} // beast
} // boost

#endif
