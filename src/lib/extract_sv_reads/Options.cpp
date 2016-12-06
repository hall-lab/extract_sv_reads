#include "Options.hpp"

#include "version.h"

#include <string>
#include <iostream>
#include <cstdlib>
#include <exception>

namespace po = boost::program_options;

std::string Options::usage() {
    return "Usage: extract-sv-reads [OPTIONS...] <input_file> <splitter_file> <discordant_file>\n";
}

Options::Options(int argc, char** argv) {
    po::options_description desc = _options_desciption();
    po::positional_options_description p = _positional_description();

    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);

    if (vm.count("help")) {
        std::cerr << usage() << desc << std::endl;
        exit(0);
    }

    if (vm.count("version")) {
        std::cerr << "version: " << __g_prog_version
            << " (commit " << __g_commit_hash << ")\n";
        exit(0);
    }
    try {
        po::notify(vm);
    }
    catch(po::required_option& ex) {
        std::cerr << "ERROR: " << ex.what() << "\n\n";
        std::cerr << usage() << desc << std::endl;
        exit(0);
    }
}

po::options_description Options::_options_desciption() {
    po::options_description desc("Available options");
    desc.add_options()
        ("help,h", "produce this message")
        ("version,v", "output the version")
        ("input,i", po::value<>(&input_file)->default_value("-"), "input BAM/CRAM/SAM. Use '-' for stdin if using positional arguments")
        ("splitter,s", po::value<>(&splitter_output_file)->required(), "output split reads to this file in BAM format (Required)")
        ("discordant,d", po::value<>(&discordant_output_file)->required(), "output discordant reads to this file in BAM format (Required)")
        ("reference,T", po::value<>(&reference), "reference sequence used to encode CRAM file, recommended if reading CRAM")
        ("exclude-dups,e", po::bool_switch(&exclude_dups)->default_value(false), "exclude duplicate reads from output")
        ("reduce-output-bams,r", po::bool_switch(&shrink_bam)->default_value(false), "remove sequences and qualities from output bams")
        ("max-unmapped-bases", po::value<>(&max_unmapped_bases)->default_value(50), "maximum number of unaligned bases between two alignments to be included in the splitter file")
        ("min-indel-size", po::value<>(&min_indel_size)->default_value(50), "minimum structural variant feature size for split alignments to be included in the splitter file")
        ("min-non-overlap", po::value<>(&min_non_overlap)->default_value(20), "minimum number of non-overlapping base pairs between two alignments for a read to be included in the splitter file")
        ("input-threads", po::value<>(&input_threads)->default_value(1), "number of threads to use for reading the input. Only useful for CRAM files")
        ;
    return desc;
}

po::positional_options_description Options::_positional_description() {
    po::positional_options_description p;
    p.add("input", 1);
    p.add("splitter", 1);
    p.add("discordant", 1);
    return p;
}
