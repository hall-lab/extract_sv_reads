#pragma once

#include "AlignmentOffsets.hpp"
#include "Parse.hpp"

#include <boost/format.hpp>

#include <algorithm>
#include <string.h>
#include <stdexcept>

using boost::format;

struct Alignment {
    AlignmentOffsets offsets;
    uint32_t pos; //1-based start position of the alignment
    uint32_t rapos;
    int SQO;
    int EQO;
    char strand;
    std::string chrom;

    Alignment()
        : offsets()
        , pos(0)
        , rapos(0)
        , SQO(0)
        , EQO(0)
        , strand(0)
        , chrom()
    {}

    Alignment(bam1_t const* aln, bam_hdr_t const* hdr) {
        rapos = aln->core.pos + 1;
        chrom = std::string(hdr->target_name[aln->core.tid]);
        offsets = AlignmentOffsets(bam_get_cigar(aln), aln->core.n_cigar);
        if (bam_is_rev(aln)) {
            strand = '-';
        }
        else {
            strand = '+';
        }
        calculate_additional_offsets();
    }

    Alignment(char const* sa_tag, char const* end) {
        SimpleTokenizer tok(sa_tag, end, ',');

        if (!tok.extract(chrom)) {
            throw std::runtime_error(str(format(
                            "Error parsing chromomsome name from SA tag %1%"
                            ) % sa_tag));
        }
        if (!tok.extract(rapos)) {
            throw std::runtime_error(str(format(
                            "Error parsing position from SA tag %1%"
                            ) % sa_tag));
        }
        if (!tok.extract(strand)) {
            throw std::runtime_error(str(format(
                            "Error parsing strand from SA tag %1%"
                            ) % sa_tag));
        }
        std::string cigar;
        if (!tok.extract(cigar)) {
            throw std::runtime_error(str(format(
                            "Error parsing cigar from SA tag %1%"
                            ) % sa_tag));
        }
            
        offsets = AlignmentOffsets(cigar.c_str());

        if (tok.skip(2) != 2) {
            throw std::runtime_error(str(format(
                            "Truncated SA tag %1%"
                            ) % sa_tag));
        }
        calculate_additional_offsets();
    }

    void calculate_additional_offsets() {
        if (strand == '-') {
            pos = rapos + offsets.raLen + offsets.eclip - 1;
            SQO = offsets.eclip;
            EQO = offsets.eclip + offsets.qaLen - 1;
        }
        else {
            pos = rapos - offsets.sclip;
            SQO = offsets.sclip;
            EQO = offsets.sclip + offsets.qaLen - 1;
        }
    }

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
        return (left.chrom == right.chrom && left.strand == right.strand);
    }
        
};
