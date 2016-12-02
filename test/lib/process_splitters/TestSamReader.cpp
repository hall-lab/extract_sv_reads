#include "process_splitters/SamReader.hpp"
#include "TestData.hpp"

#include <sam.h>

#include <gtest/gtest.h>

TEST(TestSamReader, create) {
    SamReader reader(TEST_BAMS[0].path.c_str());
    bam1_t* record = bam_init1();

    EXPECT_TRUE(reader.next(record));
    EXPECT_TRUE(reader.next(record));
    EXPECT_FALSE(reader.next(record));
}
