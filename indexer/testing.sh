#!/bin/bash
chmod +x testing.sh
#
#script name: testing.sh
#
#summary: This executes a sequence of commands that demonstrate that my solution works and have designed 
#test cases to address both normal and erroneous cases.
#
#Christian Nyamekye; Spring 2023


PROG="./indexer"


# Check to see if program is compiled, if not compile the program.
if [ ! -e "$PROG" ]
then
    make all
fi


####### Arguments, Directory and Bounds Test ######

# Test with no arguments
./indexer 

if [ $? -eq 1 ]
then
    echo passed test with no arguments
else
    echo failed test with no arguments
fi


# Test with more than 3 arguments
./indexer ~/cs50-dev/shared/tse/output/letters-1 ../data/letters.index ../data/letters.index1

if [ $? -eq 1 ]
then
    echo passed test with more than 3 arguments
else
    echo failed test with more than 3 arguments
fi


# Run indexer
./indexer ~/cs50-dev/shared/tse/output/letters-1 ../data/letters.index

# Run indextest
./indextest ../data/letters.index ../data/letters.index_copy 

# Sort the PATH of both files
sort ../data/letters.index > ../data/letters.index.sorted
sort ../data/letters.index_copy > ../data/letters.index_copy.sorted

# Check for differences
diff ../data/letters.index.sorted ../data/letters.index_copy.sorted

if [ $? -ne 0 ]
then
	echo sorting and difference test failed!
else
    echo sorting and difference test passed!
fi

echo testing completed!

