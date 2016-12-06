[![Build Status](https://travis-ci.org/hall-lab/extract_sv_reads.svg?branch=master)](https://travis-ci.org/hall-lab/extract_sv_reads)
[![Coverage Status](https://coveralls.io/repos/github/hall-lab/extract_sv_reads/badge.svg?branch=master)](https://coveralls.io/github/hall-lab/extract_sv_reads?branch=master)
[![Version](https://img.shields.io/github/release/hall-lab/extract_sv_reads.svg)](https://github.com/hall-lab/extract_sv_reads/releases)

# Description
The purpose of this program is to extract splitter and discordant reads from a CRAM or BAM file using logic identical to [SAMBLASTER](https://github.com/GregoryFaust/samblaster). This allows the generation of splitter and discordant files without name-sorting the input file. Unlike SAMBLASTER which appends '_1' and '_2' to splitter read names, read names in the splitter output file are altered by changing the first character to an 'A' for read1 and a 'B' for read2.

# Usage Notes
Splitters and discordants are output in BAM files. Duplicates are included by default, but can be excluded using the `-e` option. CRAM is supported as an input format, however, I highly recommend that when running on a CRAM file the `-T` option and `--input-threads` option are utilized. Neither of these are useful for BAM files, but allocating additional threads for CRAM reading will speed up the program significantly. In addition, `-T` prevents htslib from downloading the reference sequence used to encode the CRAM to the `REF_CACHE` location. By default, this is in the current user's home directory and may prove problematic for those with smallish home directories. See the [htslib documentation](http://www.htslib.org/workflow/) for more information.

# Compilation and Installation
Currently, `extract_sv_reads` must be compiled from source code. It is routinely tested using both the `gcc` and `clang` compilers on Ubuntu 12.04. It should work on other Unix-based operating systems, but they are not supported.

## External Dependencies

* git
* cmake 2.8+ ([cmake.org](http://cmake.org))
 
## Included Dependencies
Boost 1.59, htslib 1.3.2, and zlib 1.2.8 are included with the source code and will be utilized during compilation. Older versions of Boost will not work if specified directly.

## Basic Build Instructions

### Installing dependencies

* For APT-based systems (Debian, Ubuntu), install the following packages:

```
sudo apt-get install build-essential git-core cmake
```

### Clone the repository

```
git clone git://github.com/hall-lab/extract_sv_reads.git
```

### Build the program

`extract_sv_reads` does not support in-source builds. So create a subdirectory, enter it, build, and run tests:

```
mkdir extract_sv_reads/build
cd extract_sv_reads/build
cmake ..
make -j
make test
```
Tests should pass. The binary `extract-sv-reads` can then be found under `extract_sv_reads/build/bin`. If you have administrative rights, then run `sudo make install` to install the tool for all users under `/usr/bin`.

## Building with curl
htslib can be linked against curl if you have the curl-dev packages installed as follows:
```
mkdir extract_sv_reads/build
cd extract_sv_reads/build
cmake -DHTSLIB_USE_LIBCURL=1 ..
make -j
make test
```

# Getting Help
Please open issues on the github [repository](https://github.com/hall-lab/extract_sv_reads/issues) to obtain help.
