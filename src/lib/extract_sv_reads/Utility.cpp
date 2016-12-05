#include <sam.h>

#include <boost/format.hpp>

#include <stdexcept>
#include <string.h>

using boost::format;

// This code from diagnose_dups
namespace cigar {

    int32_t opcode_for_char(char code) {
        //stupid, but easy
        //"MIDNSHP=XB"
        switch (code) {
            case 'M':
                return 0;
            case 'I':
                return 1;
            case 'D':
                return 2;
            case 'N':
                return 3;
            case 'S':
                return 4;
            case 'H':
                return 5;
            case 'P':
                return 6;
            case '=':
                return 7;
            case 'X':
                return 8;
            case 'B':
                return 9;
            default:
                throw std::runtime_error(str(format(
                                "Unsupported CIGAR character: %1%") % code));
        }
    }

    bool valid_cigar_len(uint32_t len) {
        return len <= bam_cigar_oplen(~0u);
    }
}
