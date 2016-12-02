cmake_minimum_required(VERSION 2.8)

set(TEST_LIBS
    process_splitters
    ${HTSlib_LIBRARIES}
    ${Boost_LIBRARIES})
include_directories(${GTEST_INCLUDE_DIRS})

add_unit_tests(TestProcessSplittersLib
    TestParse.cpp
    TestAlignmentOffsets.cpp
    TestAlignment.cpp
    TestSamReader.cpp
    )
