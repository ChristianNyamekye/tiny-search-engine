# CS50 TSE
## Christian Nyamekye (ChristianNyamekye)

The Tiny Search Engine (TSE) design consists of three subsystems:

### Crawler:
The crawler crawls a website and retrieves webpages starting with a specified URL. It parses the initial webpage, extracts any embedded URLs and retrieves those pages, and crawls the pages found at those URLs, but limiting itself to some threshold number of hops from the seed URL, and avoiding visiting any given URL more than once. It saves the pages, and the URL and depth for each, in files. When the crawler process is complete, the indexing of the collected documents can begin.

### Indexer:
The indexer extracts all the keywords for each stored webpage and creates a lookup table that maps each word found to all the documents (webpages) where the word was found. It saves this table in a file.

### Querier:
The query engine responds to requests (queries) from users. The query processor module loads the index file and searches for pages that include the search keywords. Because there may be many hits, we need a ranking module to rank the results (e.g., high to low number of instances of a keyword on a page).

Each subsystem is a standalone program executed from the command line, but they inter-connect through files in the file system.


## Organization of the TSE code

Let's take a look at the structure of our TSE solution - so you can see what you're aiming for.
Our TSE comprises six subdirectories:

* **libcs50** - a library of code we provide
* **common** - a library of code you write
* **crawler** - the crawler
* **indexer** - the indexer
* **querier** - the querier
* **data** - with subdirectories where the crawler and indexer can write files, and the querier can read files.

My top-level `.gitignore` file excludes `data` from the repository (in addition to the standard CS50 exclusion patterns), because the data files are big, changing often, and don't deserve to be saved.
*Please don't commit data files to your repo, either.*

My full tree looks like this; your implementation may have slightly different files related to testing.

```
.
|-- .gitignore
|-- Makefile
|-- README.md
|-- common
|   |-- Makefile
|   |-- index.c
|   |-- index.h
|   |-- pagedir.c
|   |-- pagedir.h
|   |-- word.c
|   `-- word.h
|-- crawler
|   |-- .gitignore
|   |-- DESIGN.md
|   |-- IMPLEMENTATION.md
|   |-- Makefile
|   |-- README.md
|   |-- REQUIREMENTS.md
|   |-- crawler.c
|   |-- testing.out
|   |-- testing.sh
|   `-- valgrind.sh
|-- data
|-- indexer
|   |-- .gitignore
|   |-- DESIGN.md
|   |-- IMPLEMENTATION.md
|   |-- Makefile
|   |-- README.md
|   |-- REQUIREMENTS.md
|   |-- indexer.c
|   |-- indextest.c
|   |-- testing.out
|   `-- testing.sh
|-- libcs50
|   |-- Makefile
|   |-- README.md
|   |-- bag.c
|   |-- bag.h
|   |-- counters.c
|   |-- counters.h
|   |-- file.c
|   |-- file.h
|   |-- hash.c
|   |-- hash.h
|   |-- hashtable.c
|   |-- hashtable.h
|   |-- libcs50-given.a
|   |-- mem.c
|   |-- mem.h
|   |-- set.c
|   |-- set.h
|   |-- webpage.c
|   `-- webpage.h
`-- querier
    |-- .gitignore
    |-- DESIGN.md
    |-- IMPLEMENTATION.md
    |-- Makefile
    |-- README.md
    |-- REQUIREMENTS.md
    |-- fuzzquery.c
    |-- querier.c
|   |-- testing.out
    |-- testing.sh
    `-- testing.txt
```

Our crawler, indexer, and querier each consist of just one `.c` file.
They share some common code, which we keep in the `common` directory:

* **pagedir** - a suite of functions to help the crawler write pages to the pageDirectory and help the indexer read them back in
* **index** - a suite of functions that implement the "index" data structure; this module includes functions to write an index to a file (used by indexer) and read an index from a file (used by querier).
* **word** - a function `normalizeWord` used by both the indexer and the querier.

Each of the program directories (crawler, indexer, querier) include a few files related to testing, as well.



The assignment and Specs are in a [public repo](https://github.com/CS50Spring2023/labs/tse).
Do not clone that repo; view it on GitHub.
Watch there for any commits that may represent updates to the assignment or specs.

Add here any assumptions you made while writing the crawler, any ways in which your implementation differs from the three Specs, or any ways in which you know your implementation fails to work.

### Assumptions:
The pageDirectory does not contain any files whose name is an integer (i.e., 1, 2, ...).

