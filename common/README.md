# Christian Nyamekye
## CS50 Spring 2023, Lab 3

GitHub username: ChristianNyamekye

*
Common directory.

### pagedir:
A re-usable module `pagedir.c` that handles the pagesaver mentioned in the design (writing a page to the pageDirectory), and marking it as a Crawler-produced pageDirectory. This separate module, in ../common, encapsulates all the knowledge about how to initialize and validate a pageDirectory. and how to write and read page files, in one place... anticipating future use by the Indexer and Querier.
The pagedir module has  `pagedir.c` which implements a `page_init`, `page_load`, and `page_save`, and exports them through `pagedir.h` (see that file for more detailed documentation comments): 

Assumption: The pageDirectory does not contain any files whose name is an integer (i.e., 1, 2, ...).

### index:

A module to implement an abstract index_t type that represents an index in memory, and supports functions like index_new(), index_delete(), index_save(), and so forth. Much of it is a wrapper for a hashtable. The specs allow the code to be optimistic about index files 
being in the correct format. The functions in `file.h` are use.


### words:
A module word.c common between the Indexer and Querier that implements normalizeWord.

*
