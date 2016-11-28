#include "process_splitters/AlignmentOffsets.hpp"

#include <sam.h>

#include <gtest/gtest.h>
#include <stdexcept>

TEST(TestAlignmentOffsets, update_offset_match) {
    AlignmentOffsets data;
    data.update_offset(BAM_CMATCH, 5);
    EXPECT_EQ(data.raLen, 5);
    EXPECT_EQ(data.qaLen, 5);
    EXPECT_FALSE(data.first);
}

TEST(TestAlignmentOffsets, update_offset_clip_first) {
    AlignmentOffsets data;
    data.update_offset(BAM_CSOFT_CLIP, 5);
    EXPECT_EQ(data.sclip, 5);
    EXPECT_TRUE(data.first);
}

TEST(TestAlignmentOffsets, update_offset_clip_nonfirst) {
    AlignmentOffsets data;
    data.update_offset(BAM_CEQUAL, 2);
    data.update_offset(BAM_CSOFT_CLIP, 5);
    EXPECT_EQ(data.eclip, 5);
    EXPECT_FALSE(data.first);
}

TEST(TestAlignmentOffsets, update_offset_deletion) {
    AlignmentOffsets data;
    data.update_offset(BAM_CDEL, 2);
    EXPECT_EQ(data.raLen, 2);
    EXPECT_EQ(data.qaLen, 0);
}

TEST(TestAlignmentOffsets, update_offset_insertion) {
    AlignmentOffsets data;
    data.update_offset(BAM_CINS, 9);
    EXPECT_EQ(data.qaLen, 9);
    EXPECT_EQ(data.raLen, 0);
}

TEST(TestAlignmentOffsets, update_offset_invalid_cigar) {
    AlignmentOffsets data;
    EXPECT_THROW(data.update_offset(100, 3), std::runtime_error);
}
