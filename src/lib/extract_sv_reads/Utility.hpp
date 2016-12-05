#pragma once

#include <stdint.h>

namespace cigar {
    int32_t opcode_for_char(char code);
    bool valid_cigar_len(uint32_t len);
}
