#pragma once

struct AlignmentLengths {
    uint32_t raLen;
    uint32_t qaLen;
    uint32_t sclip;
    uint32_t eclip;

    AlignmentLengths(char *cigar)
        : raLen(0)
        , qaLen(0)
        , sclip(0)
        , eclip(0)
    {}

    AlignmentLengths(uint32_t *cigar, uint32_t n_cigar)
        : raLen(0)
        , qaLen(0)
        , sclip(0)
        , eclip(0)
    {}

};

struct Alignment {
    AlignmentLengths lens;
    uint32_t pos; //1-based start position of the alignment
    uint32_t rapos;
    uint32_t SQO;
    uint32_t EQO;
    char strand;
    char *chrom;

    Alignment()
        : pos(0)
        , raLen(0)
        , rapos(0)
        , qaLen(0)
        , sclip(0)
        , eclip(0)
        , SQO(0)
        , EQO(0)
        , strand(0)
        , chrom(NULL)
    {}

    Alignment(bam1_t *aln);
    Alignment(char *sa_tag);
};
