//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_IMPL_INIT_LIST_IPP
#define BOOST_BEAST_JSON_IMPL_INIT_LIST_IPP

#include <boost/beast/_experimental/json/init_list.hpp>
#include <functional>

namespace boost {
namespace beast {
namespace json {

init_ref::
base::
~base()
{
}

init_ref::
~init_ref()
{
    get().~base();
}

} // json
} // beast
} // boost

#endif
