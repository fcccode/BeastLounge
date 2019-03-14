//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_DETAIL_VARINT_HPP
#define BOOST_BEAST_JSON_DETAIL_VARINT_HPP

#include <cstdlib>
#include <utility>

namespace boost {
namespace beast {
namespace json {
namespace detail {

BOOST_BEAST_DECL
int
varint_size(std::uint64_t value);

BOOST_BEAST_DECL
std::pair<std::uint64_t, int>
varint_read(void const* src);

BOOST_BEAST_DECL
int
varint_write(
    void* dest,
    std::uint64_t value);

} // detail
} // json
} // beast
} // boost

#ifdef BOOST_BEAST_HEADER_ONLY
#include <boost/beast/_experimental/json/detail/varint.ipp>
#endif

#endif
