#pragma once

#include "AlignmentOffsets.hpp"

#include <algorithm>
#include <string.h>

struct Alignment {
    AlignmentOffsets offsets;
    uint32_t pos; //1-based start position of the alignment
    uint32_t rapos;
    int SQO;
    int EQO;
    char strand;
    char *chrom;

    Alignment()
        : offsets()
        , pos(0)
        , rapos(0)
        , SQO(0)
        , EQO(0)
        , strand(0)
        , chrom(NULL)
    {}

    Alignment(bam1_t *aln) {
        rapos = aln->core.pos + 1;
        offsets = AlignmentOffsets(bam_get_cigar(aln), aln->core.n_cigar);
        if (bam_is_rev(aln)) {
            strand = '-';
            pos = rapos + offsets.raLen + offsets.eclip - 1;
            SQO = offsets.eclip;
            EQO = offsets.eclip + offsets.qaLen - 1;
        }
        else {
            strand = '+';
            pos = rapos - offsets.sclip;
            SQO = offsets.sclip;
            EQO = offsets.sclip + offsets.qaLen - 1;
        }
    }

    //Alignment(char *sa_tag);
    
    int start_diagonal() const {
        return rapos - offsets.sclip;
    }

    int end_diagonal() const {
        return (rapos + offsets.raLen) - (offsets.sclip + offsets.qaLen);
    }
    
    friend int mno(Alignment const& left, Alignment const& right) {
        int overlap = std::max(1 + std::min(left.EQO, right.EQO) - std::max(left.SQO, right.SQO), 0);
        int alen1 = 1 + left.EQO - left.SQO;
        int alen2 = 1 + right.EQO - right.SQO;
        return std::min(alen1 - overlap, alen2 - overlap);
    }

    friend int desert(Alignment const& left, Alignment const& right) {
        return right.SQO - left.EQO - 1;
    }

    friend int insert_size(Alignment const& left, Alignment const& right) {
        if (left.strand == '-') {
            return right.end_diagonal() - left.start_diagonal();
        }
        else {
            return left.end_diagonal() - right.start_diagonal();
        }
    }

    friend bool should_check(Alignment const& left, Alignment const& right) {
        return (strcmp(left.chrom, right.chrom) == 0 && left.strand == right.strand);
    }
        
};
