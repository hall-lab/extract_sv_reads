#include "process_splitters/Parse.hpp"
#include "process_splitters/Alignment.hpp"
#include "process_splitters/Options.hpp"

#include <hts.h>
#include <sam.h>

#include <boost/format.hpp>

#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdexcept>

#define MIN_NON_OVERLAP 20
#define MAX_UNMAPPED_BASES 50
#define MIN_INDEL_SIZE 50

using boost::format;
namespace {
    void run(Options const& opts) {

        htsFile *disc = hts_open(opts.discordant_output_file.c_str(), "wb");
        if(disc == NULL) {
            throw std::runtime_error(str(format(
                            "Unable to open %1% for writing"
                            ) % opts.discordant_output_file));
        }

        htsFile *split = hts_open(opts.splitter_output_file.c_str(), "wb");
        if(split == NULL) {
            throw std::runtime_error(str(format(
                            "Unable to open %1% for writing"
                            ) % opts.splitter_output_file));
        }

        htsFile *in = hts_open(opts.input_file.c_str(), "r");
        if(in == NULL) {
            throw std::runtime_error(str(format(
                            "Unable to open input file %1%"
                            ) % opts.input_file));
        }

        if (!opts.reference.empty()) {
            if (hts_set_opt(in, CRAM_OPT_REFERENCE, opts.reference.c_str()) != 0) {
                throw std::runtime_error(str(format(
                                "Unable to use reference %1%"
                                ) % opts.reference));
            }
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
                        right = &first;
                        left = &second;
                    }

                    if (mno(*left, *right) < MIN_NON_OVERLAP) 
                        continue;

                    if (should_check(*left, *right)) {
                        if ((abs(insert_size(*left, *right)) < MIN_INDEL_SIZE)
                                || ((desert(*left, *right) > 0)
                                    && (desert(*left, *right) - (int) std::max(0, insert_size(*left, *right))) > MAX_UNMAPPED_BASES)) {
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
        bam_hdr_destroy(hdr);
        sam_close(in);
        sam_close(disc);
        sam_close(split);
    }
}

int main(int argc, char** argv) {
    try {
        Options opts = Options(argc, argv);
        run(opts);
    }
    catch (std::exception const& ex) {
        std::cerr << "ERROR: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}
