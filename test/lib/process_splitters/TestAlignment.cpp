#include "process_splitters/Alignment.hpp"
#include "TestData.hpp"
#include "testing/TestBamRecords.hpp"

#include <sam.h>

#include <stdint.h>
#include <gtest/gtest.h>
#include <string>
#include <stdexcept>

class TestAlignment : public ::testing::Test {
    public:
        bam1_t *record1;
        bam1_t *record2;
        TestBamRecords records;

        TestAlignment() {
            record1 = records.record;
            record2 = records.record2;
        }
};

TEST_F(TestAlignment, create_from_bam) {
    Alignment test_obj(record1, records.header);
    ASSERT_EQ(test_obj.chrom, "chr21");
    ASSERT_EQ(test_obj.strand, '+');
    ASSERT_EQ(test_obj.rapos, 44877125u);
    // TODO add additional tests once we're certain about the values
}

TEST_F(TestAlignment, create_from_sa) {
    uint8_t *sa_ptr = bam_aux_get(record2, "SA") + 1; //skipping the type field
    std::string data = (char const*) sa_ptr;
    char const* beg = data.data();
    char const* end = data.data() + data.size() - 1; //subtracting off the semicolon
    Alignment test_obj(beg, end);
    ASSERT_EQ(test_obj.chrom, "chr21");
    ASSERT_EQ(test_obj.strand, '+');
    ASSERT_EQ(test_obj.rapos, 44877125u);
}

TEST_F(TestAlignment, create_from_truncated_sa) {
    uint8_t *sa_ptr = bam_aux_get(record2, "SA") + 1; //skipping the type field
    std::string data((char const*) sa_ptr, 28); //truncate the string
    ASSERT_EQ(data, "chr21,44877125,+,101S41M9S,3");
    char const* beg = data.data();
    char const* end = data.data() + data.size();

    ASSERT_THROW(Alignment test_obj(beg, end), std::runtime_error);
}

TEST_F(TestAlignment, calculate_additional_offsets) {

}

TEST_F(TestAlignment, start_diagonal) {

}

TEST_F(TestAlignment, end_diagonal) {

}

TEST_F(TestAlignment, mno) {

}

TEST_F(TestAlignment, desert) {

}

TEST_F(TestAlignment, insert_size) {

}

TEST_F(TestAlignment, should_check) {

}

