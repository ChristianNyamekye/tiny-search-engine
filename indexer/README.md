# CS50 Lab 5 (Tiny Search Engine Indexer)
## CS50 Spring 2023, Christian Nyamekye

Check `IMPLEMENTATION.md` for implementation details.

### Indexer

The TSE indexer is a standalone program that reads the document files produced by the TSE crawler, builds an index, and writes that index to a file. Its companion, the index tester, loads an index file produced by the indexer and saves it to another file.

### Usage

The *indexer* program can be run as: *./indexer pageDirectory indexFilename* where pageDirectory is the path to a crawler produced directory , and indexFilename is the path to a file in which the index would be written. The file created by the indexer can be overwritten if it already exists.

The *indexer* program is implemented in `indexer.c` as:

```c
/**************** main ****************/
/* First performs a check on the argumens to verify that they are correct
 * It then creates and initializes an inverted-index data structure
 * Initializes the file ID to 1, which is then incremented accordingly. 
 * The file ID is used to open each of the crawler produced files.
 * The file ID is then appended to each of the file in the crawler produced files directory.
 * Each of the files in the directory is read by extracting their url, depth and html to build a new webpage object
 * The words in each html of the file, is checked to ensure that the length is greater than two.
 * Afterwards, they are normalized by converting all the cases of the word to lower cases.
 * They are then added to the inverted index data structure, which maps words to their file ID.
 * Afterwards, a file passed as a parameter to indexFilename placeholder is created and the index is written into it.
 */
int main(int argc, char *argv[]);
```

The *indextest* program can be run as: *oldIndexFilename newIndexFilename* where oldIndexFilename is a path to an index produced file, and newIndexFilename is the path to a file into which the index would be written. This file is overwritten if it already exists.

The *indextest* program is implemented in `indexer.c` as:

```c
/**************** main ****************/
/* First performs a check on the argumens to verify that they are correct
 * It then passes the arguments as parameters to the test function
 * Initializes the file ID to 1, which is then incremented accordingly. 
 */
int main(int argc, char *argv[]);

/**************** test ****************/
/* Loads the index from the oldIndexFilename into an inverted-index data structure.
 * Create a file newIndexFilename and write the index to that file, in the format described below.
 */
void test(char *oldIndexFilename, char *newIndexFilename);
```

### Assumptions

#### indexer.c

1. `pageDirectory` has files named 1, 2, 3, ..., without gaps.
2. The content of files in `pageDirectory` follow the format as defined in the specs for crawler.

#### indextest.c

1. The content of the index file follows the format specified by the indexer requirement specs.

### Exit Codes

0 - Program executed successfully.\
1 - Incorrect number of arguments.\
2 - Not crawler produced directory.\
3 - File cannot be written to/directory cannot be opened.\
4 - Failed to create new webpage object.\
5 - Malloc failure.\

### Compilation

To compile, simply `make`. To clean the indexer directory, type `make clean`. To test, type `make test`.
