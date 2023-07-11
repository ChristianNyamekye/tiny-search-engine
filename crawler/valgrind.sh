#!/bin/bash
# valgrind.sh
# Christian Nyamekye; Spring 223

# Passing valgrind on letters
valgrind --leak-check=full ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-2 3  >& valgrind.out



