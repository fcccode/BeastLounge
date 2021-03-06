#
# Copyright (c) 2018-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/boostorg/beast
#

set (TEST_MAIN ${Boost_INCLUDE_DIRS}/boost/beast/_experimental/unit_test/main.cpp)

source_group (TREE ${Boost_INCLUDE_DIRS}/boost/beast PREFIX beast FILES ${BEAST_FILES})
source_group (TREE ${PROJECT_SOURCE_DIR}/include/boost/beast PREFIX beast FILES ${BEAST_EXTRA_FILES})
source_group ("beast-test" FILES ${TEST_MAIN})

#source_group (TREE ${PROJECT_SOURCE_DIR}/test/beast/json PREFIX "" FILES *)
GroupSources(test/beast/json "/")

add_definitions (-DBOOST_BEAST_SPLIT_COMPILATION)

add_executable (beast-json-tests
    ${BEAST_FILES}
    ${BEAST_EXTRA_FILES}
    ${TEST_MAIN}
    _detail_stack.cpp
    allocator.cpp
    assign_string.cpp
    assign_vector.cpp
    basic_parser.cpp
    json.cpp
    error.cpp
    key_param.cpp
    kind.cpp
    parse_file.cpp
    parser.cpp
    src.cpp
    storage.cpp
    types.cpp
    value.cpp
)
