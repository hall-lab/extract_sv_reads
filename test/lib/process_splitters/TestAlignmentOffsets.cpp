#include "extract_sv_reads/AlignmentOffsets.hpp"

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

TEST(TestAlignmentOffsets, create_from_text_string) {
    AlignmentOffsets data("2H5S10M2D5M3I5M4S");
    EXPECT_EQ(data.sclip, 7);
    EXPECT_EQ(data.eclip, 4);
    EXPECT_EQ(data.raLen, 22);
    EXPECT_EQ(data.qaLen, 23);
}

TEST(TestAlignmentOffsets, create_from_bam_cigar) {
    const uint32_t cigar[8] = { bam_cigar_gen(2, BAM_CHARD_CLIP)
        , bam_cigar_gen(5, BAM_CSOFT_CLIP)
        , bam_cigar_gen(10, BAM_CMATCH)
        , bam_cigar_gen(2, BAM_CDEL)
        , bam_cigar_gen(5, BAM_CMATCH)
        , bam_cigar_gen(3, BAM_CINS)
        , bam_cigar_gen(5, BAM_CMATCH)
        , bam_cigar_gen(4, BAM_CSOFT_CLIP)
    };
    AlignmentOffsets data(cigar, 8);
    EXPECT_EQ(data.sclip, 7);
    EXPECT_EQ(data.eclip, 4);
    EXPECT_EQ(data.raLen, 22);
    EXPECT_EQ(data.qaLen, 23);
}
