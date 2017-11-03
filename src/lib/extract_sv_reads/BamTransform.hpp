#pragma once

#include <htslib/sam.h>
#include <cstddef>
#include <string.h>
#include <htslib/kseq.h>
#include <stdint.h>

class BamTransformer {
    private:
        bam1_t* _aln;

        std::size_t _pre_seq_bytes(bam1_t const* b) const {
            return (b->core.n_cigar<<2) + b->core.l_qname;
        }

        std::size_t _seq_qual_bytes(bam1_t const* b) const {
            // sequence bytes + quality bytes
            return (((b->core.l_qseq + 1)>>1) + b->core.l_qseq);
        }

        std::size_t _post_qual_bytes(bam1_t const* b) const {
            // Total - pre-sequence bytes - sequence bytes - quality bytes
            return (b->l_data - _pre_seq_bytes(b) - _seq_qual_bytes(b));
        }

    public:
        BamTransformer()
            : _aln(bam_init1())
        {}

        ~BamTransformer() {
            bam_destroy1(_aln);
        }

        bam1_t* operator()(bam1_t const* b) const {
            // Largely based off of htslib's bam_copy1 function
            uint8_t* data = _aln->data;
            int m_data = _aln->m_data;
            if (m_data < b->l_data) {
                m_data = b->l_data; kroundup32(m_data);
                data = (uint8_t*) realloc(data, m_data);
            }
            // NOTE Copying everything BUT sequence and quality
            memcpy(data, b->data, _pre_seq_bytes(b));
            memcpy(data + _pre_seq_bytes(b), bam_get_aux(b), _post_qual_bytes(b));
            *_aln = *b;
            _aln->l_data = b->l_data - _seq_qual_bytes(b);
            _aln->core.l_qseq = 0;
            _aln->m_data = m_data;
            _aln->data = data;

            return _aln;
        }
};
