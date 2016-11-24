#include "AlignmentOffsets.hpp"

#include <iostream>

void AlignmentOffsets::update_offset(int32_t const& opcode, uint32_t const& oplen) {
    //This code from SAMBLASTER
    if (opcode == BAM_CMATCH || opcode == BAM_CEQUAL || opcode == BAM_CDIFF) {
        raLen += oplen;
        qaLen += oplen;
        first = false;
    }
    else if (opcode == BAM_CSOFT_CLIP || opcode == BAM_CHARD_CLIP) {
        if (first) {
            sclip += oplen;
        }
        else {
            eclip += oplen;
        }
    } 
    else if (opcode == BAM_CDEL || opcode == BAM_CREF_SKIP) {
        raLen += oplen;
    }
    else if (opcode == BAM_CINS)
    {
        qaLen += oplen;
    }
    else
    {
        // XXX This should be an exception
        std::cerr << "Unknown opcode '" << opcode << "%c'\n";
    }
}
