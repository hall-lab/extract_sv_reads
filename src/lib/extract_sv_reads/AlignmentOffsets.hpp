#pragma once

#include "Utility.hpp"
#include "Parse.hpp"

#include <htslib/sam.h>

#include <boost/format.hpp>

#include <stdint.h>
#include <string.h>

#include <stdexcept>

using boost::format;

struct AlignmentOffsets {
    uint32_t raLen;
    uint32_t qaLen;
    uint32_t sclip;
    uint32_t eclip;
    bool first;

    AlignmentOffsets()
        : raLen(0)
        , qaLen(0)
        , sclip(0)
        , eclip(0)
        , first(true)
    {}

    AlignmentOffsets(char const* cigar)
        : raLen(0)
        , qaLen(0)
        , sclip(0)
        , eclip(0)
        , first(true)
    {
        char const* beg = cigar;
        std::size_t len = strlen(beg);
        char const* end = beg + len;

        for(; *beg != '\0'; ++beg) {
            uint32_t oplen = 0;
            if (!auto_parse(beg, end, oplen) || !cigar::valid_cigar_len(oplen)) {
                throw std::runtime_error(str(format(
                    "Error parsing cigar string %1%: expected number at position %2%"
                    ) % cigar % (beg - cigar)));
            }

            int op = cigar::opcode_for_char(*beg);
            if (op < 0) {
                throw std::runtime_error(str(format(
                    "Error parsing cigar string %1%: invalid cigar op char at position %2%"
                    ) % cigar % (beg - cigar)));
            }
            update_offset(op, oplen);
        }
    }

    AlignmentOffsets(uint32_t const* cigar, uint32_t const& n_cigar)
        : raLen(0)
        , qaLen(0)
        , sclip(0)
        , eclip(0)
        , first(true)
    {
        for (uint32_t i = 0; i < n_cigar; ++i) {
            update_offset(bam_cigar_op(cigar[i]), bam_cigar_oplen(cigar[i]));
        }
    }

    void update_offset(int32_t const& opcode, uint32_t const& oplen);
};
