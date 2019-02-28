//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/vinniefalco/BeastLounge
//

// Test that header file is self-contained.
#include <boost/beast/_experimental/json/visitation.hpp>

#if 0

#include <boost/beast/_experimental/unit_test/suite.hpp>
#include <boost/beast/_experimental/json/parse_file.hpp>
#include <boost/beast/_experimental/json/parser.hpp>

namespace boost {
namespace beast {
namespace json {

class visitation_test : public unit_test::suite
{
public:
    void
    run()
    {
#if 0
        error_code ec;
        parser p;
        json::parse_file(
            "C:\\Users\\Vinnie\\src\\lounge\\client\\test.json",
            p, ec);
        //log << p.release() << "\n";
        visitation visit(p.get());
        storage st;
        ref jv(st, kind::null);
        std::string s;
        while(visit.next(jv))
        {
            switch(jv.get_kind())
            {
            case kind::object:
                s += '{';
                break;

            case kind::array:
                s += '[';
                break;

            default:
                s += '?';
                break;
            }
        }
        log << s << "\n";
#endif
        pass();
    }
};

BEAST_DEFINE_TESTSUITE(beast,json,visitation);

} // json
} // beast
} // boost

#endif
