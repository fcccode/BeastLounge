//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

// Test that header file is self-contained.
#include <boost/beast/_experimental/json/object_ref.hpp>

#include <boost/beast/_experimental/unit_test/suite.hpp>
#include <memory>
#include <sstream>

namespace boost {
namespace beast {
namespace json {

class object_ref_test : public unit_test::suite
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
            value jv = object;
            ref jr(jv);
            object_ref ro(jr);;
            BEAST_EXPECT(ro.empty());
            BEAST_EXPECT(ro.size() == 0);
        }
        {
            value jv = 1;
            ref jr(jv);
            try
            {
                object_ref ro(jr);
                BEAST_FAIL();
            }
            catch(std::exception const&)
            {
                BEAST_PASS();
            }
        }

        // `value` ctor
        {
            value jv = object;
            object_ref ro(jv);
            BEAST_EXPECT(ro.empty());
            BEAST_EXPECT(ro.size() == 0);
        }
        {
            value jv = 1;
            try
            {
                object_ref ro(jv);
                BEAST_FAIL();
            }
            catch(std::exception const&)
            {
                BEAST_PASS();
            }
        }

        // empty, size
        {
            value jv = object;
            object_ref ro(jv);
            BEAST_EXPECT(ro.empty());
            BEAST_EXPECT(ro.size() == 0);
        }
    }

    void
    testObject()
    {
        value jv = object;
        jv["k1"] = "v1";
        jv["k2"] = "v2";
        jv["k3"] = "v3";
        object_ref ro(jv);
        BEAST_EXPECT(! ro.empty());
        BEAST_EXPECT(ro.size() == 3);
        BEAST_EXPECT(ro.count("k1") == 1);
        BEAST_EXPECT(ro.count("k2") == 1);
        BEAST_EXPECT(ro.count("k3") == 1);
        BEAST_EXPECT(ro.find("k1") != ro.end());
        BEAST_EXPECT(ro.erase("k2") == 1);
        BEAST_EXPECT(ro.size() == 2);
        BEAST_EXPECT(
            ro.erase(ro.begin())->value.is_string());
        BEAST_EXPECT(ro.size() == 1);
        ro.clear();
        BEAST_EXPECT(ro.empty());
        BEAST_EXPECT(ro.size() == 0);
    }

    void run() override
    {
        testMembers();
        testObject();
    }
};

BEAST_DEFINE_TESTSUITE(beast,json,object_ref);

} // json
} // beast
} // boost
