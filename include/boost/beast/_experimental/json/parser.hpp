//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_PARSER_HPP
#define BOOST_BEAST_JSON_PARSER_HPP

#include <boost/beast/core/detail/config.hpp>
#include <boost/beast/_experimental/json/array_ref.hpp>
#include <boost/beast/_experimental/json/basic_parser.hpp>
#include <boost/beast/_experimental/json/ptr.hpp>
#include <boost/beast/_experimental/json/storage.hpp>
#include <new>
#include <string>
#include <type_traits>

namespace boost {
namespace beast {
namespace json {

class parser : public basic_parser
{
#if 0
    // VFALCO This can't work as written
    struct node
    {
        bool is_object;

        union
        {
            object_ref ro;
            array_ref ra;
        };
        
        node(node const&) = delete;
        node& operator=(node const&) = delete;

        node(ref const& jr)
            : is_object(jr.is_object())
        {
            if(is_object)
                ::new(&ro) object_ref(jr);
            else
                ::new(&ra) array_ref(jr);
        }

        ~node()
        {
            if(is_object)
                ro.~object_ref();
            else
                ra.~array_ref();
        }
    };
#endif

    storage store_;
    json::value jv_;
    std::vector<ref> stack_;
    std::string s_;

public:
    BOOST_BEAST_DECL
    parser();

    BOOST_BEAST_DECL
    parser(storage const& store);

    json::ref
    get() const noexcept
    {
        return jv_;
    }

    json::value
    release() noexcept
    {
        stack_.clear();
        return jv_;
    }

private:
    template<class T>
    void
    assign(T&& v);

    BOOST_BEAST_DECL
    void
    reset();

    BOOST_BEAST_DECL
    void
    on_document_begin(error_code& ec) override;

    BOOST_BEAST_DECL
    void
    on_object_begin(error_code& ec) override;

    BOOST_BEAST_DECL
    void
    on_object_end(error_code& ec) override;

    BOOST_BEAST_DECL
    void
    on_array_begin(error_code& ec) override;

    BOOST_BEAST_DECL
    void
    on_array_end(error_code& ec) override;

    BOOST_BEAST_DECL
    void
    on_string_begin(error_code& ec) override;

    BOOST_BEAST_DECL
    void
    on_string_piece(
        string_view s, error_code& ec) override;

    BOOST_BEAST_DECL
    void
    on_string_end(error_code& ec) override;

    BOOST_BEAST_DECL
    void
    on_number(number n, error_code& ec) override;

    BOOST_BEAST_DECL
    void
    on_bool(bool b, error_code& ec) override;

    BOOST_BEAST_DECL
    void
    on_null(error_code&) override;
};

} // json
} // beast
} // boost

#include <boost/beast/_experimental/json/impl/parser.hpp>
#ifdef BOOST_BEAST_HEADER_ONLY
#include <boost/beast/_experimental/json/impl/parser.ipp>
#endif

#endif
