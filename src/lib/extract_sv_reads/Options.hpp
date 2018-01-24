#pragma once

#include <boost/program_options.hpp>

#include <string>

namespace po = boost::program_options;

class Options {
    private:
        po::options_description _options_description();
        po::options_description _deprecated_options_description();
        po::positional_options_description _positional_description();
        std::string usage();

    public:
        po::variables_map vm;

        Options(int argc, char** argv);

        std::string input_file;
        std::string splitter_output_file;
        std::string discordant_output_file;
        std::string reference;

        bool exclude_dups;
        bool shrink_bam;
        int min_non_overlap;
        int max_unmapped_bases;
        int min_indel_size;

        bool ignore_invalid_sa;

        int threads;
        // NOTE Deprecated options
        int input_threads;
};
