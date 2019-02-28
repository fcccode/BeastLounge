//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_DETAIL_VARIANT_EXCHANGE_HPP
#define BOOST_BEAST_JSON_DETAIL_VARIANT_EXCHANGE_HPP

#include <boost/beast/_experimental/json/kind.hpp>
#include <boost/beast/_experimental/json/detail/unspecialized.hpp>
#include <type_traits>

namespace boost {
namespace beast {
namespace json {
namespace detail {

template<class T>
struct variant_exchange final
#ifndef BOOST_BEAST_DOXYGEN
    : detail::unspecialized
#endif
{
};

template<class T>
using is_variant_exchange =
    std::integral_constant<
        bool, ! std::is_base_of<unspecialized,
            variant_exchange<typename
                std::decay<T>::type>>::value>;

} // detail
} // json
} // beast
} // boost

#endif
