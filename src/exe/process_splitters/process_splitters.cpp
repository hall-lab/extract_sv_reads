#include "process_splitters/Parse.hpp"
#include "process_splitters/Alignment.hpp"
#include "process_splitters/Options.hpp"
#include "process_splitters/SamReader.hpp"
#include "process_splitters/SamWriter.hpp"

#include <hts.h>
#include <sam.h>

#include <boost/format.hpp>

#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdexcept>

using boost::format;
namespace {
    void run(Options const& opts) {

        SamReader reader(opts.input_file.c_str(), opts.reference.c_str());

        int skip_flag = BAM_FUNMAP | BAM_FQCFAIL | BAM_FSECONDARY;
        if (opts.exclude_dups) {
            skip_flag = skip_flag | BAM_FDUP;
        }
        reader.skip_flags(skip_flag);

        SamWriter disc(opts.discordant_output_file.c_str(), "wb", reader.header());
        SamWriter split(opts.splitter_output_file.c_str(), "wb", reader.header());

        bam1_t *aln = bam_init1();
        int discordant_flag = BAM_FPROPER_PAIR | BAM_FMUNMAP | BAM_FSUPPLEMENTARY;

        while(reader.next(aln)) {
            if (((aln->core.flag) & discordant_flag) == 0)
                disc.write(aln);

            uint8_t *sa = bam_aux_get(aln, "SA");

            if (sa != 0) {
                std::string sa_string = bam_aux2Z(sa);

                char const* beg = sa_string.data();
                char const* end = beg + sa_string.size();
                SimpleTokenizer tok(beg, end, ';');
                std::string first_sa;
                if (!tok.extract(first_sa)) {
                    throw std::runtime_error(str(format(
                                    "Error parsing SA tag: %1%"
                                    ) % sa_string));
                }

                if (tok.next_delim() == end) {
                    Alignment first(aln, reader.header());
                    Alignment second(first_sa.data(), first_sa.data() + first_sa.size());

                    Alignment const* left = &first;
                    Alignment const* right = &second;

                    if (left->SQO > right->SQO) {
                        right = &first;
                        left = &second;
                    }

                    if (mno(*left, *right) < opts.min_non_overlap) 
                        continue;

                    if (should_check(*left, *right)) {
                        if ((abs(insert_size(*left, *right)) < opts.min_indel_size)
                                || ((desert(*left, *right) > 0)
                                    && (desert(*left, *right) - (int) std::max(0, insert_size(*left, *right))) > opts.max_unmapped_bases)) {
                            continue;
                        }
                    }

                    char *qname =  bam_get_qname(aln);
                    if ((aln->core.flag & 64) == 64)
                        qname[0] = 'A'; 
                    else
                        qname[0] = 'B'; 

                    split.write(aln);
                }
            }
        }

        bam_destroy1(aln);
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
