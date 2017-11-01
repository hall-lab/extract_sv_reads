#pragma once

#include <sam.h>
#include <hts.h>
#include <thread_pool.h>

#include <boost/format.hpp>

#include <string>
#include <stdexcept>
#include <iostream>

class SamReader {
    public:
        SamReader(char const* path, char const* reference=NULL, int nthreads=1)
            : _in(hts_open(path, "r"))
            , _thread_pool()
            , _required_flags(0)
            , _skip_flags(0)
        {
            using boost::format;
            if (!_in) {
                throw std::runtime_error(str(format(
                                "Failed to open input file %1%"
                                ) % path));
            }
            if (reference && *reference != '\0') {
                if (hts_set_opt(_in, CRAM_OPT_REFERENCE, reference) != 0) {
                    throw std::runtime_error(str(format(
                                    "Unable to use reference %1%"
                                    ) % reference));
                }
            }

            if (nthreads > 1) {
                if (!(_thread_pool.pool = hts_tpool_init(nthreads))) {
                    throw std::runtime_error("Failed to initialize thread pool");
                }
                if (hts_set_opt(_in, HTS_OPT_THREAD_POOL, &_thread_pool) != 0) {
                    throw std::runtime_error(str(format(
                                    "Failed to use threads to read CRAM %1%"
                                    ) % path));
                }
            }

            if (!(_hdr = sam_hdr_read(_in))) {
                throw std::runtime_error(str(format(
                                "Failed to read header from file %1%"
                                ) % path));
            }
        }

        ~SamReader() {
            if (_hdr) {
                bam_hdr_destroy(_hdr);
            }

            if (_in) {
                hts_close(_in);
            }

            if (_thread_pool.pool) {
                hts_tpool_destroy(_thread_pool.pool);
            }
        }

        bam_hdr_t* header() const {
            return _hdr;
        }

        void required_flags(uint32_t flags) {
            _required_flags = flags;
        }

        void skip_flags(uint32_t flags) {
            _skip_flags = flags;
        }

        bool want(uint32_t flag) const {
            return (flag & _required_flags) == _required_flags
                && (flag & _skip_flags) == 0;
        }

        bool next(bam1_t* record) {
            while (sam_read1(_in, _hdr, record) > 0) {
                if (want(record->core.flag)) {
                    return true;
                }
            }
            return false;
        }

    private:
        htsFile* _in;
        htsThreadPool _thread_pool;
        bam_hdr_t* _hdr;
        uint32_t _required_flags;
        uint32_t _skip_flags;
};
