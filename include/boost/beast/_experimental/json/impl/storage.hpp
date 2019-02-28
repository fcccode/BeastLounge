//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_IMPL_STORAGE_HPP
#define BOOST_BEAST_JSON_IMPL_STORAGE_HPP

#include <boost/beast/_experimental/json/detail/storage_impl.hpp>

namespace boost {
namespace beast {
namespace json {

template<class Allocator, class>
storage::
storage(Allocator const& alloc)
    : p_(new detail::storage_impl<Allocator>(alloc))
{
}

} // json
} // beast
} // boost

#endif
