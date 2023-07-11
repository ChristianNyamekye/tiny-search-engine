#!/bin/bash
# testing.sh
# Christian Nyamekye; Spring 223

# Passing a non-integer maxDepth
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-3 a

# Passing multiple maxDepths
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-3 3 1

# Passing a wrong number of inputs.
./crawler

# Testing with letters
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-1 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-2 10

# Testing with wikipedia
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/wikipedia-1 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/wikipedia-2 1

# Testing with toscrape
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ../data/toscrape-1 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ../data/toscrape-2 1