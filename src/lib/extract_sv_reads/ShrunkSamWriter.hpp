#pragma once

#include "SamWriter.hpp"
#include "BamTransform.hpp"

#include <htslib/sam.h>

class ShrunkSamWriter : public SamWriter {
    private:
        BamTransformer _transform;
    public:
        ShrunkSamWriter(char const* path, char const* mode, bam_hdr_t const* hdr)
            : SamWriter(path, mode, hdr)
            , _transform(BamTransformer())
        {}
        void write(bam1_t const* record) const {
            SamWriter::write(_transform(record));
        }
};
