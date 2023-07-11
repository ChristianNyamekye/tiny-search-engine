# CS50 TSE Querier
## Design Spec

The TSE *querier* is a standalone program that reads the index file produced by the indexer, and page files produced by the crawler, and answers search queries submitted via stdin.

### User interface

The querier's only interface with the user is on the command-line; it must always have two arguments.

```
./querier pageDirectory indexFilename
```

### Inputs and outputs

**Input**: the querier reads an index from `indexFilename`. It also reads files from a directory by constructing file pathnames from the `pageDirectory` parameter followed by a numeric document ID (as described in the Requirements). The querier reads the first line from certain document files (see below) to obtain the URL source for the document.

**Output**: We print to `stdout` the scores, docIDs and URLs of documents that satisfy each query.

### Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which parses arguments and initializes other modules;
 2. *query*, which initiates all the functionality of the system;
 2. *rankResults*, which ranks the matches to the query, sorts them into an array.

We do not anticipate writing any new modules for the querier.


### Pseudo code for logic/algorithmic flow

The querier will run as follows:

    parse the command line, validate parameters, initialize other modules
    call query, with pageDirectory and indexFilename which in turn
    calls validateSyntax, with wordsArray and noOfWords (from cleanQuery) 

where *validateSyntax:*

      validates query and gives way for satisfy to run and return queryScore counter
            queryScore, numDocs and fileNum are the passed into rankedResults to
            create a sorted array of validDoc_t strucs

where *query:*

     completes the program by reading queries from stdin until EOF
           cleans the queries and splits them into individual words
           fetches the set of documents that satisfy the query (matches)
           prints out all the matches

### Major data structures

We will make use of two key data structures that we've already encountered, namely `index`, and `counters`. The `index` will hold our representation of word-frequencies in each document in `pageDirectory`, i.e. `counters` of documents and their word scores. 

### Testing plan

Given the similarity between querier's `parseArgs` function with the functions we have used to parse command-line arguments for `crawler` and `indexer`, we will focus our testing on a variety of queries, in place of testing with invalid arguments.

* queries beginning with operators ("and", "or")
* queries ending with operators
* queries containing two operators in sequence.
* queries with invalid characters (non-alphabetical)
* empty queries
* one-word queries
* queries with multiple words
