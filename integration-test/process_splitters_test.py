#!/usr/bin/env python
"""This integration test uses Illumina Platinum Genomes data and compares to expected results"""

import os
from integrationtest import IntegrationTest, main
from testdata import TEST_DATA_DIRECTORY
import unittest
import subprocess

class TestProcessSplitters(IntegrationTest, unittest.TestCase):
    def setUp(self):
        IntegrationTest.setUp(self)
        self.data_dir = TEST_DATA_DIRECTORY
        self.orig_path = os.path.realpath(os.getcwd())
        self.exe_path = os.path.realpath(self.exe_path)
        os.chdir(self.data_dir)

    def tearDown(self):
        IntegrationTest.tearDown(self)
        os.chdir(self.orig_path)

    def test_program(self):
        expected_splitter_file = "expected_wdups.splitters.sorted.bam"
        expected_discordant_file = "expected_wdups.discordants.sorted.bam"
        output_splitter_file = self.tempFile("splitters.bam")
        output_discordant_file = self.tempFile("discordants.bam")
        input_bam = "integration_test.bam"
        cmdline = " ".join([self.exe_path, '-i', input_bam, '-s', output_splitter_file, '-d', output_discordant_file])
        print "Executing: ", cmdline
        print "CWD: ", os.getcwd()

        rv = subprocess.call(cmdline, shell=True)
        print "Return value: ", rv
        self.assertEqual(0, rv)
        self.assertFilesEqual(expected_splitter_file, output_splitter_file)
        self.assertFilesEqual(expected_discordant_file, output_discordant_file)

    def test_program_nodups(self):
        expected_splitter_file = "expected_nodups.splitters.sorted.bam"
        expected_discordant_file = "expected_nodups.discordants.sorted.bam"
        output_splitter_file = self.tempFile("splitters.bam")
        output_discordant_file = self.tempFile("discordants.bam")
        input_bam = "integration_test.bam"
        cmdline = " ".join([self.exe_path, '-e', '-i', input_bam, '-s', output_splitter_file, '-d', output_discordant_file])
        print "Executing: ", cmdline
        print "CWD: ", os.getcwd()

        rv = subprocess.call(cmdline, shell=True)
        print "Return value: ", rv
        self.assertEqual(0, rv)
        self.assertFilesEqual(expected_splitter_file, output_splitter_file)
        self.assertFilesEqual(expected_discordant_file, output_discordant_file)


if __name__ == "__main__":
    main()
