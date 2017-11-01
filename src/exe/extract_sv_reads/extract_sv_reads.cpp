#include "extract_sv_reads/Parse.hpp"
#include "extract_sv_reads/Alignment.hpp"
#include "extract_sv_reads/Options.hpp"
#include "extract_sv_reads/SamReader.hpp"
#include "extract_sv_reads/SamWriter.hpp"
#include "extract_sv_reads/ShrunkSamWriter.hpp"

#include <hts.h>
#include <sam.h>

#include <boost/format.hpp>

#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdexcept>

using boost::format;
namespace {
    template<typename R>
        void parse_file(Options const& opts, SamReader& reader, R const& disc, R const& split) {
            bam1_t *aln = bam_init1();
            int discordant_flag = BAM_FPROPER_PAIR | BAM_FMUNMAP | BAM_FSUPPLEMENTARY;

            while (reader.next(aln)) {
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
    void run(Options const& opts) {
        
        ThreadPool* pool = NULL;
        // NOTE Always creating the thread pool, even if it isn't needed
        // I don't think this has a high cost as there is only one.
        // An alternative would be to dynamically create it and then 
        // catch any exceptions during running and delete

        ThreadPool app_thread_pool(opts.input_threads);
        if (opts.input_threads > 1) {
            pool = &app_thread_pool;
        }

        SamReader reader(opts.input_file.c_str(), opts.reference.c_str(), pool);

        int skip_flag = BAM_FUNMAP | BAM_FQCFAIL;
        if (opts.exclude_dups) {
            skip_flag = skip_flag | BAM_FDUP;
        }
        reader.skip_flags(skip_flag);

        if (opts.shrink_bam) {
            ShrunkSamWriter disc(opts.discordant_output_file.c_str(), "wb", reader.header());
            ShrunkSamWriter split(opts.splitter_output_file.c_str(), "wb", reader.header());
            parse_file(opts, reader, disc, split);
        }
        else {
            SamWriter disc(opts.discordant_output_file.c_str(), "wb", reader.header());
            SamWriter split(opts.splitter_output_file.c_str(), "wb", reader.header());
            parse_file(opts, reader, disc, split);
        }
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
