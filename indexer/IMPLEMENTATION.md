# CS50 Lab 5 TSE Indexer Implementation Spec
## CS50 Spring 2023, Christian Nyamekye

### Implementation

We implement this crawler with a `main` function that takes and reads input from the user. These inputs are the page directory, and the index filename. When these commands are passed as arguments, we check to see if the correct number of arguments have been passed. We then assign these arguments to variables. The next check that is performed is to ensure that the directory is a crawler produced one. We also check to see if the file is writable. 

We then process each file, to normalize the words, and add them to the inverted index data structure, which maps words to their file ID. After a file containing the contents of the index generated data is written, the content of the index data structure is deleted.

Modules included are `index.c`, through the header file `index.h`. This contains the data structure to allow the indexer to work.

In `index.c` file, i have methods such as `index_new`, `index_save`, `countersDelete`, `index_delete`, `index_WriteToFile`, `writeKeys`, `index_find`, and `index_load`. Refer to index.h for more information about these functions.

In `indextest.c`, the arguments are validated, and they are passed as parameters to a function called `test`. It then loads the index from the oldIndexFilename into an inverted-index data structure. Afterwards, a file called newIndexFilename is created and index is written into it, in the format outlined below.

The output of the indexer is displayed as follows:

```c
playground 1 1
page 1 1
this 1 1
home 1 2
the 1 1
for 1 1
tse 1 1
```
