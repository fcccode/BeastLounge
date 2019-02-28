//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

// Test that header file is self-contained.
#include <boost/beast/_experimental/json/value.hpp>

#include <boost/beast/_experimental/json/ref.hpp>
#include <boost/beast/_experimental/unit_test/suite.hpp>
#include <memory>

namespace boost {
namespace beast {
namespace json {

class value_test : public unit_test::suite
{
public:
    static
    std::string
    str(json::ref const& jr)
    {
        std::stringstream ss;
        ss << jr;
        return ss.str();
    }

    void
    testMembers()
    {
        // default ctor
        {
            value jv;
            BEAST_EXPECT(jv.is_null());
            BEAST_EXPECT(str(jv) == "null");
        }
        // `storage` ctor
        {
            storage store;
            value jv(store);
            BEAST_EXPECT(jv.is_null());
            BEAST_EXPECT(
                jv.get_storage() == store);
        }

        // `ref` ctor
        {
            value jv1;
            ref jr1(jv1);
            jr1 = 1;
            BEAST_EXPECT(str(jv1) == "1");
            value jv2(jr1);
            BEAST_EXPECT(str(jv2) == "1");
            jv2 = 2;
            BEAST_EXPECT(str(jv2) == "2");
        }

        // `value` ctor
        {
            value jv1 = 1;
            value jv2(jv1);
            BEAST_EXPECT(str(jv1) == "1");
            BEAST_EXPECT(str(jv2) == "1");
            jv2 = 2;
            BEAST_EXPECT(str(jv1) == "1");
            BEAST_EXPECT(str(jv2) == "2");
        }

        // `ref` assignment
        {
            value jv1 = 1;
            ref jr(jv1);
            value jv2;
            jv2 = jr;
            BEAST_EXPECT(str(jv2) == "1");
            jr = 2;
            BEAST_EXPECT(str(jv1) == "2");
            BEAST_EXPECT(str(jv2) == "1");
        }

        // value assign, is_*
        {
            value jv;
            BEAST_EXPECT(jv.is_null());
            jv = object;
            BEAST_EXPECT(jv.is_object());
            BEAST_EXPECT(str(jv) == "{}");
            jv = array;
            BEAST_EXPECT(jv.is_array());
            BEAST_EXPECT(str(jv) == "[]");
            jv = "x";
            BEAST_EXPECT(jv.is_string());
            BEAST_EXPECT(str(jv) == "\"x\"");
            jv = 42;
            BEAST_EXPECT(str(jv) == "42");
            jv = -42;
            BEAST_EXPECT(str(jv) == "-42");
            jv = 1.5;
            BEAST_EXPECT(str(jv) == "1.5");
            jv = true;
            BEAST_EXPECT(str(jv) == "true");
            jv = null;
            BEAST_EXPECT(str(jv) == "null");
        }

        // operator[] for key
        {
            value jv;
            jv["key"];
            BEAST_EXPECT(jv.is_object());
            jv["key"] = "value";
            BEAST_EXPECT(str(jv) == "{\"key\":\"value\"}");
        }

        // operator[] for array index
    }

    void
    testValue()
    {
        value jv;
        jv = { 1, 2, 3 };
        log << jv << "\n";
        //jv = {"key" , "value"};
    }

    void run() override
    {
        log <<
            "sizeof(detail::variant) == " <<
            sizeof(detail::variant) << '\n';
        testMembers();
        testValue();
    }
};

BEAST_DEFINE_TESTSUITE(beast,json,value);

} // json
} // beast
} // boost
