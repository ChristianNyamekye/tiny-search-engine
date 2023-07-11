#!/bin/bash

# build the crawler program
make

# create a directory to hold test files
mkdir -p testdir

# create some test files
touch testdir/testfile1.html
touch testdir/testfile2.html
touch testdir/testfile3.html

# run the crawler on the test directory
./crawler http://localhost/testdir/ testdir 1

# check that the crawler created the expected output files
if [ -f testdir/1 ]
then
  echo "PASSED: Found file 1"
else
  echo "FAILED: Did not find file 1"
fi

if [ -f testdir/2 ]
then
  echo "PASSED: Found file 2"
else
  echo "FAILED: Did not find file 2"
fi

if [ -f testdir/3 ]
then
  echo "PASSED: Found file 3"
else
  echo "FAILED: Did not find file 3"
fi

# clean up the test files and directories
rm -rf testdir
make clean
