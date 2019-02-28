//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

// Test that header file is self-contained.
#include <boost/beast/_experimental/json/array_ref.hpp>

#include <boost/beast/_experimental/unit_test/suite.hpp>
#include <memory>
#include <sstream>

namespace boost {
namespace beast {
namespace json {

class array_ref_test : public unit_test::suite
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
            value jv = array;
            ref jr(jv);
            array_ref ro(jr);;
            BEAST_EXPECT(ro.empty());
            BEAST_EXPECT(ro.size() == 0);
        }

        // `value` ctor
        {
            value jv = array;
            array_ref ro(jv);
            BEAST_EXPECT(ro.empty());
            BEAST_EXPECT(ro.size() == 0);
        }

        // empty, size
        {
            value jv = array;
            array_ref ro(jv);
            BEAST_EXPECT(ro.empty());
            BEAST_EXPECT(ro.size() == 0);
        }
    }

    void
    testArray()
    {
        value jv = array;
#if 0
        jv["k1"] = "v1";
        jv["k2"] = "v2";
        jv["k3"] = "v3";
        array_ref ro(jv);
        BEAST_EXPECT(! ro.empty());
        BEAST_EXPECT(ro.size() == 3);
        BEAST_EXPECT(ro.exists("k1"));
        BEAST_EXPECT(ro.exists("k2"));
        BEAST_EXPECT(ro.exists("k3"));
        BEAST_EXPECT(ro.find("k1") != ro.end());
        BEAST_EXPECT(ro.erase("k2") == 1);
        BEAST_EXPECT(ro.size() == 2);
        ro.erase(ro.begin());
        BEAST_EXPECT(ro.size() == 1);
        ro.clear();
        BEAST_EXPECT(ro.empty());
        BEAST_EXPECT(ro.size() == 0);
#endif
    }

    void run() override
    {
        testMembers();
        testArray();
    }
};

BEAST_DEFINE_TESTSUITE(beast,json,array_ref);

} // json
} // beast
} // boost
