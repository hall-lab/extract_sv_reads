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
    ASSERT_EQ("chr21", test_obj.chrom);
    ASSERT_EQ('+', test_obj.strand);
    ASSERT_EQ(44877125u, test_obj.rapos);
    // TODO add additional tests once we're certain about the values
}

TEST_F(TestAlignment, create_from_sa) {
    uint8_t *sa_ptr = bam_aux_get(record2, "SA") + 1; //skipping the type field
    std::string data = (char const*) sa_ptr;
    char const* beg = data.data();
    char const* end = data.data() + data.size() - 1; //subtracting off the semicolon
    Alignment test_obj(beg, end);
    ASSERT_EQ("chr21", test_obj.chrom);
    ASSERT_EQ('+', test_obj.strand);
    ASSERT_EQ(44877125u, test_obj.rapos);
    ASSERT_EQ(41u, test_obj.offsets.raLen);
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
    AlignmentOffsets data("2H5S10M2D5M3I5M4S");
    Alignment test_obj;
    test_obj.rapos = 50u;
    test_obj.strand = '+';
    test_obj.offsets = data;
    test_obj.calculate_additional_offsets();
    ASSERT_EQ(43u, test_obj.pos);
    ASSERT_EQ(test_obj.SQO, 7);
    ASSERT_EQ(test_obj.EQO, 29);

    test_obj.strand = '-';
    test_obj.calculate_additional_offsets();
    ASSERT_EQ(75u, test_obj.pos);
    ASSERT_EQ(test_obj.SQO, 4);
    ASSERT_EQ(test_obj.EQO, 26);
}

TEST_F(TestAlignment, start_diagonal) {
    Alignment a1(record1, records.header);
    ASSERT_EQ(44877125 - 101, a1.start_diagonal());
}

TEST_F(TestAlignment, end_diagonal) {
    Alignment a1(record1, records.header);
    ASSERT_EQ((44877125 + 41) - (101 + 41), a1.end_diagonal());
}

TEST_F(TestAlignment, mno) {

}

TEST_F(TestAlignment, desert) {

}

TEST_F(TestAlignment, insert_size) {

}

TEST_F(TestAlignment, should_check) {

}

