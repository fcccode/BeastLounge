//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

// Test that header file is self-contained.
#include <boost/beast/_experimental/json/ptr.hpp>

#include <boost/beast/_experimental/json/ref.hpp>
#include <boost/beast/_experimental/json/value.hpp>
#include <boost/beast/_experimental/unit_test/suite.hpp>
#include <memory>

namespace boost {
namespace beast {
namespace json {

class ptr_test : public unit_test::suite
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
            ptr jp;
            BEAST_EXPECT(jp == nullptr);
        }

        // nullptr ctor
        {
            ptr jp(nullptr);
            BEAST_EXPECT(jp == nullptr);
        }

        // copy ctor
        {
            value jv = 1;
            ptr jp1(&jv);
            BEAST_EXPECT(str(*jp1) == "1");
            ptr jp2(jp1);
            BEAST_EXPECT(str(*jp2) == "1");
            BEAST_EXPECT(jp1 == jp2);
        }

        // ref ctor / assign
        {
            value jv1 = 1;
            value jv2 = 2;
            ref jr1(jv1);
            ref jr2(jv2);
            ptr jp(&jr1);
            BEAST_EXPECT(str(*jp) == "1");
            jp = &jr2;
            BEAST_EXPECT(str(*jp) == "2");
        }

        // value ctor / assign
        {
            value jv1 = 1;
            value jv2 = 2;
            ptr jp(&jv1);
            BEAST_EXPECT(str(*jp) == "1");
            jp = &jv2;
            BEAST_EXPECT(str(*jp) == "2");
        }

        // derefrencing
        {
            value jv = array;
            ptr jp = &jv;
            BEAST_EXPECT(jp->is_array());
            BEAST_EXPECT(str(*jp) == "[]");
        }

        // comparisons
        {
            value jv = 1;
            ptr jp1;
            ptr jp2;
            BEAST_EXPECT(jp1 == jp2);
            BEAST_EXPECT(jp1 == nullptr);
            BEAST_EXPECT(nullptr == jp2);
            jp2 = &jv;
            BEAST_EXPECT(jp1 != jp2);
        }
    }

    void run() override
    {
        testMembers();
    }
};

BEAST_DEFINE_TESTSUITE(beast,json,ptr);

} // json
} // beast
} // boost
