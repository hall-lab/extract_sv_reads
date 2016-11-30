#pragma once

#include <boost/program_options.hpp>

#include <string>

namespace po = boost::program_options;

class Options {
    private:
        po::options_description _options_desciption();
        po::positional_options_description _positional_description();

    public:
        po::variables_map vm;

        Options(int argc, char** argv);

        std::string input_file;
        std::string splitter_output_file;
        std::string discordant_output_file;
        std::string reference;

        bool exclude_dups;
        int min_non_overlap;
        int max_unmapped_bases;
        int min_indel_size;
};
