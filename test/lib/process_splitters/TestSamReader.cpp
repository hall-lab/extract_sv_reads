#include "extract_sv_reads/SamReader.hpp"
#include "TestData.hpp"

#include <sam.h>

#include <gtest/gtest.h>
#include <stdexcept>

TEST(TestSamReader, create) {
    SamReader reader(TEST_BAMS[0].path.c_str());
    bam1_t* record = bam_init1();

    ASSERT_TRUE(reader.next(record));
    ASSERT_TRUE(reader.next(record));
    ASSERT_FALSE(reader.next(record));
}

TEST(TestSamReader, failed_create) {
    ASSERT_THROW(SamReader reader("nonexistent_file"), std::runtime_error);
}

TEST(TestSamReader, failed_reference) {
    ASSERT_THROW(SamReader reader(TEST_BAMS[1].path.c_str(), "nonexistent_reference"), std::runtime_error);
}
