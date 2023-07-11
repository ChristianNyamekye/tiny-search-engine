# CS50 Lab 6 (Tiny Search Engine Querier)
## CS50 Spring 2023, Christian Nyamekye

Check `IMPLEMENTATION.md` for implementation details.

### Querier

The TSE Querier is a standalone program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Querier, and answers search queries submitted via stdin.

### Usage

The *querier* program can be run as: *./querier pageDirectory indexFilename* where pageDirectory is the path to a crawler produced directory , and indexFilename is the path to a file produced by the index.

The *querier* program is implemented in `querier.c` as:

```c
/**************** local types ****************/
typedef struct counterGroup {
    counters_t *counterOne;
    counters_t *counterTwo;
} counterGroup_t;

typedef struct validDoc {
    int docID;
    int score;
} validDoc_t;

/**************** functions ****************/
void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename);
void copyCtrs(counters_t *counter1, counters_t *counter2);
void copyCtrsHelper(void *arg, const int key, const int count);
void intersection(void *arg, const int key, int count);
void unionFunc(counters_t *ctr1, counters_t *ctr2);
void unionFuncHelper(void *arg, const int key, const int count);
void docsCounter(void *arg, const int key, int count);
int crawlerFilesNum(char * pageDirectory);
validDoc_t *rankResults(counters_t *results, int resultsNum, int filesNum);
counters_t *satisfy(char **words, int wordCount, index_t *index);
int qsortHelper(const void * firstDoc, const void *secondDoc);
bool blankCheck(char *query);
int cleanQuery(char *query, char*words[]);
bool validateSyntax(char *words[], int numofWords);
void query(char * pageDirectory, char * indexFilename);

/**************** main ****************/
/*the main funcition first checks the arguments passed as parameters with parseArgs to ensure
 *that they're of the correct number and type.  It then passes as parameter 
 *the two arguments from main into a function called query, which reads the 
 *index produced by the Indexer and the page files produced by the Crawler, 
 *to interactively answer written queries entered by the user.
 */
int main(int argc, char *argv[]);

/**************** parseArgs ****************/
/*this function constructs the path to the .crawler file in the pageDirectory
 *validates arguments passed into it, and initializes the two arguments in main
 */
void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename);

/**************** query ****************/
/*this function loads the index file into index structure and takes input from
 *users, checks to see if they are valid, finds their frequency in the documents
 *and displays them in a ranked order.
 */
void query(char * pageDirectory, char * indexFilename);

/**************** blankCheck ****************/
/*this function checks if the query is a blankspace and
 *returns a boolean.
 */
bool blankCheck(char *query);

/**************** cleanQuery ****************/
/*this function cleans and parses each query according to the
 *syntax described in the REQUIREMENTS.md
 */
int cleanQuery(char *query, char*words[]);

/**************** validateSyntax ****************/
/*this function checks to see if the query follows the syntax rules
 * for 'and' and 'or'
 */
bool validateSyntax(char *words[], int numofWords);

/**************** satisfy ****************/
/*this function returns a counter of documents that satisfies the query
 */
counters_t *satisfy(char **words, int wordCount, index_t *index);

/**************** intersection ****************/
/*this function finds the intersection between two counters
 */
void intersection(void *arg, const int key, int count);

/**************** unionFunc ****************/
/*this function finds the union between two counters
 */
void unionFunc(counters_t *ctr1, counters_t *ctr2);

/**************** unionFuncHelper ****************/
/*this function is a helper mehtod for unionFunc
 */
void unionFuncHelper(void *arg, const int key, const int count);

/**************** copyCtrs ****************/
/*this function copies curWord counter into andSeq to begin a new andSeq
 */
void copyCtrs(counters_t *ctr1, counters_t *ctr2);

/**************** copyCtrsHelper ****************/
/*serves as helper function for copyCtrs
 */
void copyCtrsHelper(void *arg, const int key, const int count);

/**************** docsCounter ****************/
/*this function counts the number of items in the counters to determine
 * size of validDoc_t array
 */   
void docsCounter(void *arg, const int key, int count);

/**************** rankResults ****************/
/*this function takes a counters struct of docID count(score) pairs
 * and creates a sorted array of validDoc_t strucs
 */   
validDoc_t *rankResults(counters_t *results, int resultsNum, int filesNum);

/**************** crawlerFilesNum ****************/
/*this function gets and returns the number of of crawler outputfiles
 * in the pageDirectory
 */ 
int crawlerFilesNum(char * pageDirectory);

/**************** qsortHelper ****************/
/*this is a helper function for quicksort
 */ 
int qsortHelper(const void * firstDoc, const void *secondDoc);
```


### Assumptions

#### indexer.c

1. `pageDirectory` has files named 1, 2, 3, ..., without gaps.
2. The content of files in `pageDirectory` follow the format as defined in the specs for crawler.
3. The content of the file named by `indexFilename` follows our Index file format

### Functionalities
1. Prints the set of documents that contain all the words in the query
2. Supports ‘and’ and ‘or’ operators, but without precedence
3. Prints the document set in decreasing order by score


### Exit Codes

0 - Program executed successfully.\
1 - Incorrect number of arguments.\
2 - Not crawler produced directory.\
3 - File cannot be read.\


### Compilation

To compile, simply `make`. To clean the querier directory, type `make clean`. To test, type `make test`.


```c
Query? 
Query: dartmouth 
Matches 1 documents (ranked):
Score:  1 doc   2 http://cs50tse.cs.dartmouth.edu/tse/wikipedia/
----------------------------------
Query? 
Query: college 
Matches 1 documents (ranked):
Score:  1 doc   2 http://cs50tse.cs.dartmouth.edu/tse/wikipedia/
----------------------------------
Query? 
Query: the or browser 
Matches 1 documents (ranked):
Score:  1 doc   2 http://cs50tse.cs.dartmouth.edu/tse/wikipedia/
----------------------------------
Query? 
Query: sapiens and or humans 
Error: 'and' and 'or' cannot follow each other directly.
----------------------------------
Query? 
Query: light or science 
Matches 2 documents (ranked):
Score:  3 doc   3 http://cs50tse.cs.dartmouth.edu/tse/toscrape/
Score:  1 doc   2 http://cs50tse.cs.dartmouth.edu/tse/wikipedia/
----------------------------------
Query? 
Query: and 
Error:'and' cannot be the first word.
----------------------------------
Query? 
Query: or 
Error:'or' cannot be the first word.
----------------------------------
Query? 
Query: and dartmouth 
Error:'and' cannot be the first word.
----------------------------------
Query? 
Query: or dartmouth 
Error:'or' cannot be the first word.
----------------------------------
Query? 
Query: dartmouth college or 
Error: 'or' cannot be the last word.
----------------------------------
Query? 
Query: dartmouth college and 
Error: 'and' cannot be the last word.
----------------------------------
Query? 
Query: dartmouth college and or computer 
Error: 'and' and 'or' cannot follow each other directly.
----------------------------------
Query? 
Query: dartmouth college and and computer 
Error: 'and' and 'and' cannot follow each other directly.
----------------------------------
Query? 
Query: dartmouth college or and computer 
Error: 'or' and 'and' cannot follow each other directly.
----------------------------------
Query? 
Error: bad character '5' in query.
Query? 
Error: bad character '!' in query.
Query? 
Error: bad character '-' in query.
```
