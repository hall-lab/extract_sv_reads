#include "process_splitters/Parse.hpp"
#include "process_splitters/Alignment.hpp"

#include <sam.h>

#include <string>
#include <iostream>
#include <stdlib.h>

#define MIN_NON_OVERLAP 20
#define MAX_UNMAPPED_BASES 50
#define MIN_INDEL_SIZE 50

int main(int argc, char **argv)
{
    if (argc != 4) {
        std::cerr << "usage:\tprocess_splitters <bam> <split out> <discord out>" << std::endl;
        exit(1);
    }

    char *bam_file_name = argv[1];
    char *split_file_name = argv[2];
    char *disc_file_name = argv[3];

    samFile *disc = sam_open(disc_file_name, "wb");

    samFile *split = sam_open(split_file_name, "wb");

    samFile *in = sam_open(bam_file_name, "rb");
    if(in == NULL) {
        std::cerr << "Unable to open BAM/SAM file." << std::endl;
        exit(1);
    }

    bam_hdr_t *hdr = sam_hdr_read(in);

    int r = sam_hdr_write(disc, hdr);
    r = sam_hdr_write(split, hdr);

    bam1_t *aln = bam_init1();


    while(sam_read1(in, hdr, aln) >= 0) {
        if (((aln->core.flag) & 1294) == 0)
            r = sam_write1(disc, hdr, aln);

        if (aln->core.flag & (BAM_FUNMAP | BAM_FQCFAIL | BAM_FDUP)) {
            continue;
        }
        uint8_t *sa = bam_aux_get(aln, "SA");

        if (sa != 0) {
            std::string sa_string = bam_aux2Z(sa);

            char const* beg = sa_string.data();
            char const* end = beg + sa_string.size();
            SimpleTokenizer tok(beg, end, ';');
            std::string first_sa;
            if (!tok.extract(first_sa)) {
                //TODO THROW
            }

            if (tok.next_delim() == end) {
                Alignment first(aln, hdr);
                Alignment second(first_sa.data(), first_sa.data() + first_sa.size());

                Alignment const* left = &first;
                Alignment const* right = &second;

                if (left->SQO > right->SQO) {
                    left = &first;
                    right = &second;
                }

                if (mno(*left, *right) < MIN_NON_OVERLAP) 
                    continue;

                if (should_check(*left, *right)) {
                    if ((abs(insert_size(*left, *right) < MIN_INDEL_SIZE)
                                || ((desert(*left, *right) > 0)
                                    && (desert(*left, *right) - (int) std::max(0, insert_size(*left, *right))) > MAX_UNMAPPED_BASES))) {
                        continue;
                    }
                }

                char *qname =  bam_get_qname(aln);
                if ((aln->core.flag & 64) == 64)
                    qname[0] = 'A'; 
                else
                    qname[0] = 'B'; 

                r = sam_write1(split, hdr, aln);
            }
        }
    }

    bam_destroy1(aln);
    hts_idx_destroy(idx);
    bam_hdr_destroy(hdr);
    sam_close(in);
    sam_close(disc);
    sam_close(split);
}
