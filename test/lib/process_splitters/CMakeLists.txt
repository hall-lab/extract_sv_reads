cmake_minimum_required(VERSION 2.8)

set(TEST_LIBS
    extract_sv_reads
    ${HTSlib_LIBRARIES}
    ${Boost_LIBRARIES})
include_directories(${GTEST_INCLUDE_DIRS})

add_unit_tests(TestExtractSvReadsLib
    TestParse.cpp
    TestAlignmentOffsets.cpp
    TestAlignment.cpp
    TestSamReader.cpp
    TestBamTransform.cpp
    )
