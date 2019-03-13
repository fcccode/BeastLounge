//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

// Test that header file is self-contained.
#include <boost/beast/_experimental/json/assign_vector.hpp>

#include <boost/beast/_experimental/unit_test/suite.hpp>
#include <type_traits>

namespace boost {
namespace beast {
namespace json {

class assign_vector_test : public unit_test::suite
{
public:
    void
    testAssign()
    {
        value jv(array_type);
        {
            auto& a = jv.raw_array();
            a.push_back(1);
            a.push_back(2);
            a.push_back(3);
        }
        std::vector<int> v;
        error_code ec;
        jv.assign(v, ec);
        BEAST_EXPECTS(! ec, ec.message());
        BEAST_EXPECT(v.size() == 3);
        BEAST_EXPECT(v[0] == 1);
        BEAST_EXPECT(v[1] == 2);
        BEAST_EXPECT(v[2] == 3);
    }

    void run() override
    {
        testAssign();
    }
};

BEAST_DEFINE_TESTSUITE(beast,json,assign_vector);

} // json
} // beast
} // boost
