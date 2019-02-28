//
// Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_JSON_DETAIL_SMALL_BUFFER_HPP
#define BOOST_BEAST_JSON_DETAIL_SMALL_BUFFER_HPP

#include <boost/core/empty_value.hpp>
#include <cstddef>
#include <type_traits>

namespace boost {
namespace beast {
namespace json {
namespace detail {

template<class T, std::size_t N>
class small_buffer
{
    using ptr_type = std::shared_ptr<void>;

    BOOST_STATIC_ASSERT(N >= sizeof(ptr_type));

    typename std::aligned_storage<N>::type buf_;
    bool inline_ = true;

public:
    template<class U>
    small_buffer(U&& u)
    {
        if(sizeof(T) <= N)
        {
            inline_ = false;
            return *new(&buf_) T(std::forward<T>(t));
        }
    }

    void
    destroy(void* p)
    {
        if(! inline_)

    }
};

} // detail
} // json
} // beast
} // boost

#endif
