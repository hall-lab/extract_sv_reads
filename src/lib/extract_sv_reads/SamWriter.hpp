#pragma once

#include <htslib/sam.h>

#include <boost/format.hpp>

#include <string>
#include <stdexcept>

using boost::format;
class SamWriter {
    public:
        SamWriter(char const* path, char const* mode, bam_hdr_t const* hdr)
            : path(path)
            , _out(hts_open(path, mode))
            , _hdr(bam_hdr_dup(hdr))
        {
            if (!_out || !_hdr) {
                throw std::runtime_error(str(format(
                                "Failed to open output file %1%"
                                ) % path));
            }
            if (sam_hdr_write(_out, _hdr) != 0 ) {
                throw std::runtime_error(str(format(
                                "Failed to write header to %1%"
                                ) % path));
            }
        }

        ~SamWriter() {
            if (_hdr) {
                bam_hdr_destroy(_hdr);
            }

            if (_out) {
                hts_close(_out);
            }
        }

        void write(bam1_t const* record) const {
            if (sam_write1(_out, _hdr, record) < 0) {
                throw std::runtime_error(str(format(
                                "Failed while writing to file %1%"
                                ) % path));
            }

        }
    private:
        std::string path;
        htsFile* _out;
        bam_hdr_t* _hdr;
};
            
