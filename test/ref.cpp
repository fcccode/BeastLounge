//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

// Test that header file is self-contained.
#include <boost/beast/_experimental/json/ref.hpp>

#include <boost/beast/_experimental/unit_test/suite.hpp>
#include <memory>
#include <sstream>

namespace boost {
namespace beast {
namespace json {

class ref_test : public unit_test::suite
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
        // `ref` ctor
        {
            value jv = 1;
            BEAST_EXPECT(str(jv) == "1");
            ref jr1(jv);
            BEAST_EXPECT(str(jr1) == "1");
            ref jr2(jr1);
            BEAST_EXPECT(str(jr2) == "1");
            jr1 = 2;
            BEAST_EXPECT(str(jr1) == "2");
            BEAST_EXPECT(str(jr2) == "2");
        }

        // `value` ctor
        {
            value jv = 1;
            ref jr(jv);
            BEAST_EXPECT(str(jr) == "1");
            jv = 2;
            BEAST_EXPECT(str(jv) == "2");
            BEAST_EXPECT(str(jr) == "2");
        }

        // `ref` assign
        {
            value jv1 = 1;
            ref jr1(jv1);
            value jv2 = 2;
            ref jr2(jv2);
            jr2 = jr1;
            BEAST_EXPECT(str(jr2) == "1");
            jr2 = 2;
            BEAST_EXPECT(str(jr2) == "2");
            BEAST_EXPECT(str(jr1) == "1");
        }

        // value assign, is_*
        {
            value jv;
            ref jr(jv);
            BEAST_EXPECT(jr.is_null());
            jr = object;
            BEAST_EXPECT(jr.is_object());
            BEAST_EXPECT(str(jr) == "{}");
            jr = array;
            BEAST_EXPECT(jr.is_array());
            BEAST_EXPECT(str(jr) == "[]");
            jr = "x";
            BEAST_EXPECT(jr.is_string());
            BEAST_EXPECT(str(jr) == "\"x\"");
            jr = 42;
            BEAST_EXPECT(str(jr) == "42");
            jr = -42;
            BEAST_EXPECT(str(jr) == "-42");
            jr = 1.5;
            BEAST_EXPECT(str(jr) == "1.5");
            jr = true;
            BEAST_EXPECT(str(jr) == "true");
            jr = null;
            BEAST_EXPECT(str(jr) == "null");
        }

        // operator[] for key
        {
            value jv;
            ref jr(jv);
            jr["key"];
            BEAST_EXPECT(jr.is_object());
            jr["key"] = "value";
            BEAST_EXPECT(str(jv) == "{\"key\":\"value\"}");
        }

        // TODO
        // operator[] for array index
    }

    void
    testNlohmann()
    {
        storage store;
        value j(store);
        j["pi"] = 3.141;
        j["happy"] = true;
        j["name"] = "Niels";
        j["nothing"] = nullptr;
        j["answer"]["everything"] = 42;
        j["list"] = { 1, 0, 2 };
#if 0
        j["object"] = { {"currency", "USD"}, {"value", 42.99} };
        ref j2 =
        {
            {"pi", 3.141},
            {"happy", true},
            {"name", "Niels"},
            {"nothing", nullptr},
            {
                "answer", {
                    {"everything", 42}
                }
            },
            {"list", {1, 0, 2}},
            {
                "object", {
                    {"currency", "USD"},
                    {"value", 42.99}
                }
            }
        };
#endif
        //log << j << "\n";
    }

    void run() override
    {
        testMembers();
        testNlohmann();
    }
};

BEAST_DEFINE_TESTSUITE(beast,json,ref);

} // json
} // beast
} // boost
