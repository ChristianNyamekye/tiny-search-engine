#!/bin/bash
chmod +x testing.sh
#
#script name: testing.sh
#
#summary: This executes a sequence of commands that demonstrate that my solution works and have designed test cases to address both normal and erroneous cases.
#
#Christian Nyamekye, Spring 2023

PROG="./indexer"
FILE="testqueries"

#Check to see if program is compiled, if not compile the program.
if [ ! -e "$PROG" ]
then
    make all
fi

cd ../crawler; make;
cd ../indexer; make;
cd ../common; make;
cd ../libcs50; make;
cd ../querier


# Test with no arguments
./querier 

if [ $? -eq 1 ]
then
    echo passed test with no arguments
else
    echo failed test with no arguments
fi

####### Arguments Test ######

# Test with more than 3 arguments
./querier ~/cs50-dev/shared/tse/output/toscrape-2 ~/cs50-dev/shared/tse/output/toscrape-2.index ~/cs50-dev/shared/tse/output/toscrape-2.index1  

if [ $? -eq 1 ]
then
    echo passed test with more than 3 arguments
else
    echo failed test with more than 3 arguments
fi


#test querier with non crawler directory

./querier ~/cs50-dev/tse-ChristianNyamekye/data ~/cs50-dev/shared/tse/output/toscrape-2.index 
if [ $? -eq 2 ]
then
    echo Passed test with non-crawler directory
else
    echo did not pass test with non-crawler directory
fi

# test querier program
./querier ~/cs50-dev/shared/tse/output/toscrape-2 ~/cs50-dev/shared/tse/output/toscrape-2.index  < testqueries 
valgrind ./fuzzquery ~/cs50-dev/shared/tse/output/toscrape-2.index 10 11111 | ./querier ~/cs50-dev/shared/tse/output/toscrape-2 ~/cs50-dev/shared/tse/output/toscrape-2.index

