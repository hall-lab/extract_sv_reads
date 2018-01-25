[![Version](https://img.shields.io/github/release/hall-lab/extract_sv_reads.svg)](https://github.com/hall-lab/extract_sv_reads/releases)
[![install with bioconda](https://img.shields.io/badge/install%20with-bioconda-brightgreen.svg?style=flat-square)](http://bioconda.github.io/recipes/extract-sv-reads/README.html)
[![DOI](https://zenodo.org/badge/75641367.svg)](https://zenodo.org/badge/latestdoi/75641367)
[![License](https://img.shields.io/github/license/hall-lab/extract_sv_reads.svg)](LICENSE.txt)

[![Build Status](https://travis-ci.org/hall-lab/extract_sv_reads.svg?branch=master)](https://travis-ci.org/hall-lab/extract_sv_reads)
[![Coverage Status](https://coveralls.io/repos/github/hall-lab/extract_sv_reads/badge.svg?branch=master)](https://coveralls.io/github/hall-lab/extract_sv_reads?branch=master)

# Description
The purpose of this program is to extract splitter and discordant reads from a CRAM or BAM file using logic identical to [SAMBLASTER](https://github.com/GregoryFaust/samblaster). This allows the generation of splitter and discordant files without name-sorting the input file. Unlike SAMBLASTER which appends '_1' and '_2' to splitter read names, read names in the splitter output file are altered by changing the first character to an 'A' for read1 and a 'B' for read2.

# Usage Notes
Splitters and discordants are output in BAM files. Duplicates are included by default, but can be excluded using the `-e` option. As of version 1.2.0, threading affects the performance of both BAM and CRAM files and specifying more than one thread will speed up the program significantly. CRAM is supported as an input format, however, I highly recommend that when running on a CRAM file the `-T` option is utilized. The `-T` option prevents htslib from downloading the reference sequence used to encode the CRAM to the `REF_CACHE` location. By default, this is in the current user's home directory and may prove problematic for those with smallish home directories. See the [htslib documentation](http://www.htslib.org/workflow/) for more information.

As of version 1.3.0, a new option `--ignore-invalid-sa` is available. This option specifically ignores split read alignments where the SA tag contains a negative position. These tags have been observed in versions of 10x Genomics' Lariat aligner, but have been reported and will be fixed in subsequent releases. By default, `extract-sv-reads` will report an error on encountering SA tags with negative positions. However, if you are confident that this is because of the aforementioned bug in Lariat, then you should be able to generate valid output by using the `--ignore-invalid-sa` option and proceed.

# Credits
This program is heavily based on code from [SAMBLASTER](https://github.com/GregoryFaust/samblaster), unpublished code from [Ryan Layer](https://github.com/ryanlayer) and code written by [Travis Abbott](https://github.com/tabbott) in [diagnose_dups](https://github.com/genome/diagnose_dups).

# Compilation and Installation
Currently, `extract_sv_reads` must be compiled from source code. It is routinely tested using both the `gcc` and `clang` compilers on Ubuntu 12.04. It should work on other Unix-based operating systems, but they are not supported.

## External Dependencies

* git
* cmake 2.8+ ([cmake.org](http://cmake.org))
 
## Included Dependencies
Boost 1.59, htslib 1.6, and zlib 1.2.8 are included with the source code and will be utilized during compilation. Older versions of Boost will not work if specified directly.

## Basic Build Instructions

### Installing dependencies

* For APT-based systems (Debian, Ubuntu), install the following packages:

```
sudo apt-get install build-essential git-core cmake
```

### Download a stable version or clone the repository

Download and extract the code of the [latest release](https://github.com/hall-lab/extract_sv_reads/releases/latest)

or clone from the master branch using git

```
git clone git://github.com/hall-lab/extract_sv_reads.git
```

### Build the program

`extract-sv-reads` does not support in-source builds. So create a subdirectory, enter it, build, and run tests:

```
mkdir extract_sv_reads/build
cd extract_sv_reads/build
cmake ..
make -j
make test
```
Tests should pass. The binary `extract-sv-reads` can then be found under `extract_sv_reads/build/bin`. If you have administrative rights, then run `sudo make install` to install the tool for all users under `/usr/bin`.

## Building with additional libraries
htslib can be linked against curl for interaction with AWS and GCS. In addition, it can be linked with lzma and bz2 for full read support of all types of CRAM files. To enable these features install the following packages.

### Dependencies
* For APT-based systems (Debian, Ubuntu):

```
sudo apt-get install libbz2-dev liblzma-dev libssl-dev libcurl4-openssl-dev 
```

### Building
```
mkdir extract_sv_reads/build
cd extract_sv_reads/build
cmake -DHTSLIB_USE_LIBCURL=1 -DHTSLIB_USE_LZMA=1 -DHTSLIB_USE_BZ2=1 ..
make -j
make test
```

# Citing
Please cite `extract-sv-reads` using its [DOI](https://zenodo.org/badge/latestdoi/75641367). Note that this link corresponds to the latest version. If you used an earlier version then your DOI may be different and you can find it on [Zenodo](https://zenodo.org).

# Getting Help
Please open issues on the github [repository](https://github.com/hall-lab/extract_sv_reads/issues) to obtain help.
