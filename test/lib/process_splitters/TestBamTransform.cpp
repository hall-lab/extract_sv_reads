#include "extract_sv_reads/BamTransform.hpp"
#include "TestData.hpp"
#include "testing/TestBamRecords.hpp"

#include <htslib/sam.h>

#include <gtest/gtest.h>

class TestBamTransform : public ::testing::Test {
    public:
        bam1_t *record1;
        bam1_t *record2;
        TestBamRecords records;

        TestBamTransform() {
            record1 = records.record;
            record2 = records.record2;
        }
};

TEST_F(TestBamTransform, transform) {
    BamTransformer transformer;
    bam1_t* transformed = transformer(record1);
    ASSERT_EQ(0, transformed->core.l_qseq);
    ASSERT_EQ(record1->l_data - 152, transformed->l_data);
    ASSERT_EQ(*bam_get_aux(record1), *bam_get_aux(transformed));
    ASSERT_EQ(*(record1->data + record1->l_data - 1), *(transformed->data + transformed->l_data - 1));
}

