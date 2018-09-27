#include "Options.hpp"

#include "version.h"

#include <string>
#include <iostream>
#include <cstdlib>
#include <exception>

namespace po = boost::program_options;

std::string Options::usage() {
    //TODO Using manual line breaks below. This seems suboptimal...
    return "Usage: extract-sv-reads [OPTIONS...] <input_file> <splitter_file> <discordant_file>\n"
        "\n"
        "Examples:\n"
        "\textract-sv-reads input.bam splitters.bam discordants.bam\n"
        "\textract-sv-reads -i input.bam -s splitters.bam -d discordants.bam\n"
        "\textract-sv-reads -e -r --threads 4 -T /path/to/reference.fa \\\n"
        "\t  -i input.cram -s splitters.bam -d discordants.bam\n"
        "\n"
        "Notes:\n"
        "\t-T is only useful when the input file is a CRAM.\n\n"
        "\tWhen parsing CRAM, extract-sv-reads will download the entire reference\n"
        "\tused to encode the CRAM from EBI unless the -T option is specified to the\n"
        "\tproper local reference. This is both slow and may fill up your home\n"
        "\tdirectory. See the REF_PATH and REF_CACHE documentation of htslib and\n"
        "\tsamtools for more information.\n"
        "\n"
        ;
}

Options::Options(int argc, char** argv) {
    po::options_description desc = _options_description();
    po::options_description hidden = _deprecated_options_description();
    po::positional_options_description p = _positional_description();

    po::options_description all;
    all.add(desc).add(hidden);

    po::store(po::command_line_parser(argc, argv).options(all).positional(p).run(), vm);

    if (vm.count("help")) {
        std::cerr << usage() << desc << std::endl;
        exit(0);
    }

    if (vm.count("version")) {
        std::cerr << "version: " << __g_prog_version
            << " (commit " << __g_commit_hash << ")\n";
        exit(0);
    }

    if (!vm["input-threads"].defaulted()) {
        std::cerr << "WARNING: The --input-threads option will be removed in a future release. Please modify your pipeline to use --threads instead\n";
        if (!vm["threads"].defaulted()) {
            std::cerr << "WARNING: Both --input-threads and --threads specified. The larger value will be used for threading\n";
        }
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

po::options_description Options::_options_description() {
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
        ("with-nm,n", po::bool_switch(&needs_nm)->default_value(false), "ensure NM tag is present in output if reading CRAM file")
        ("max-unmapped-bases", po::value<>(&max_unmapped_bases)->default_value(50), "maximum number of unaligned bases between two alignments to be included in the splitter file")
        ("min-indel-size", po::value<>(&min_indel_size)->default_value(50), "minimum structural variant feature size for split alignments to be included in the splitter file")
        ("min-non-overlap", po::value<>(&min_non_overlap)->default_value(20), "minimum number of non-overlapping base pairs between two alignments for a read to be included in the splitter file")
        ("threads", po::value<>(&threads)->default_value(1), "number of threads to use")
        ;
    return desc;
}

po::options_description Options::_deprecated_options_description() {
    po::options_description desc("Deprecated options");
    desc.add_options()
        ("input-threads", po::value<>(&input_threads)->default_value(1), "number of threads to use")
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
