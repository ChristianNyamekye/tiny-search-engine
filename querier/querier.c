/* 
 * querier.c - CS50 'querier' module
 *
 * see IMPLEMENTATION.md for more information.
 *
 * Christian Nyamekye, Spring 2023.
 */


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <dirent.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>
#include "../libcs50/mem.h"
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"
#include "../libcs50/bag.h"
#include "../libcs50/counters.h"
#include "../libcs50/hashtable.h"
#include "../common/pagedir.h"
#include "../common/words.h"
#include "../common/index.h"
// #include "../common/readlinep.h"


/**************** local types ****************/
typedef struct counterGroup {
    counters_t* counterOne;
    counters_t* counterTwo;
} counterGroup_t;

typedef struct validDoc {
    int docID;
    int score;
} validDoc_t;

/**************** functions ****************/
int fileno(FILE *stream);
static void prompt(void);
void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename);
void copyCtrs(counters_t *counter1, counters_t *counter2);
void copyCtrsHelper(void *arg, const int key, const int count);
void intersection(void *arg, const int key, int count);
void unionFunc(counters_t *ctr1, counters_t *ctr2);
void unionFuncHelper(void *arg, const int key, const int count);
void docsCounter(void *arg, const int key, int count);
int crawlerFilesLength(char * pageDirectory);
validDoc_t *rankResults(counters_t *results, int resultsNum, int filesNum);
counters_t *satisfy(char **words, int wordCount, index_t *index);
int qsortHelper(const void * firstDoc, const void *secondDoc);
bool blankCheck(char *query);
int cleanQuery(char *query, char*words[]);
bool validateSyntax(char *words[], int numofWords);
void query(char * pageDirectory, char * indexFilename);

/**************** main ****************/
/*the main funcition first checks the arguments passed as parameters to ensure
 *that they're of the correct number and type. It also initialized the index
 * structure and loads the indexfile into it. It then passes as parameter 
 *the two arguments from main into a function called query, which reads the 
 *index produced by the Indexer and the page files produced by the Crawler, 
 *to interactively answer written queries entered by the user.
 */
int main(int argc, char *argv[]){
    char * pageDirectory = NULL;
    char * indexFilename = NULL;
    parseArgs(argc, argv, &pageDirectory, &indexFilename);
  
    query(pageDirectory, indexFilename);
    return 0;
}

/**************** parseArgs() ****************/

void
parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename) {
   // parse arguments
   if (argc != 3) {
       fprintf(stderr, "usage: %s:  pageDirectory indexFilename\n", argv[0]); 
       exit(1);
   }


   *pageDirectory = argv[1];
   *indexFilename = argv[2];
  
   const char* crawlerFile = ".crawler";


   char crawler_path[500];
 
   sprintf(crawler_path, "%s/%s", *pageDirectory, crawlerFile);


   bool file_exists = false;
   FILE* file = fopen(crawler_path, "r");
   if (file != NULL) {
       fclose(file);
       file_exists = true;
   }


   // exit if .crawler doesn't exist
   if (!file_exists) {
       fprintf(stderr, "not a crawler file\n");
       exit(2);
   }
   FILE* fp;


   // Check if indexFilename exists and is readable   
   if ((fp = fopen(*indexFilename, "r")) == NULL) {
       fprintf(stderr, "usage: indexFilename must exist and be readable\n");
       exit(3);
   }
 
}



/**************** query ****************/
/*this function loads the index file into index structure and takes input from
 *users, checks to see if they are valid, finds their frequency in the documents
 *and displays them in a ranked order.
 */
void query(char * pageDirectory, char * indexFilename){
    FILE* indexFile;

    indexFile = fopen(indexFilename, "r");      // opens index file

    int numLines = file_numLines(indexFile);    // gets the number of lines in index file
    index_t* indexStruct = index_new(numLines);     // creates an index struct with number of slots same as that of number of lines in file

    index_load(indexStruct, indexFile);     // loads the index file into the index struct

    char *query;    // stores the queries entered by user

    while (!feof(stdin)){   // reads from standard inoput untill EOF
        prompt();
        if ( (query = file_readLine(stdin)) != NULL){ // stores user's inout into query
            
            if (blankCheck(query) == false){
                int lenWords = (strlen(query)/2) + 1;   // calculates the length of words in query and accounts for int division truncation

                char** wordsArray = malloc(lenWords * sizeof(char *) + 1); //allocates memory for array of words and newline character at the end
                int noOfWords = cleanQuery(query, wordsArray);  // cleans querry and gets number of words in it

                if (noOfWords > 0){     // proceeds only when number of words is greater than 0
                    printf("Query: ");
                    for (int a = 0; a < noOfWords; a++){
                        printf("%s ", wordsArray[a]);
                    }
                    printf("\n");
                }
                else{   // else prompts the user for query again
                    continue;
                }


                bool validSyntax = validateSyntax(wordsArray, noOfWords);  // validates the syntax of the query

                if (validSyntax){
                    // create counters strucuture for the documents
                    counters_t* queryScore = satisfy(wordsArray, noOfWords, indexStruct);

                    int* numDocs = malloc(sizeof(int));    // counts number of matching documents
                    if (numDocs == NULL){
                        fprintf(stderr, "unable to allocate memory for numDocs\n");
                        exit(5);
                    }

                    *numDocs = 0;
                    // counts number of documents in the counter
                    counters_iterate(queryScore, numDocs, (*docsCounter));

                    // Begins to rank documents
                    int fileNum = crawlerFilesLength(pageDirectory);
                    validDoc_t* rankedRes = rankResults(queryScore, *numDocs, fileNum);

                    if (*numDocs == 0){
                        printf("No documents match...\n");

                    }
                    else{
                        printf("Matches %d documents (ranked):\n", *numDocs);
                    

                        // Displays ranked results
                        for (int n = 0; n < *numDocs; n++){
                            // Process filename
                            int docID;
                            docID = rankedRes[n].docID;


                            char *stringfileId = malloc(sizeof(char *));    // allocates memory
                            assert(stringfileId != NULL);   // checks for malloc failure

                            sprintf(stringfileId, "/%d", docID);    // coverts the intger docID to a string and add '\'
                            
                            size_t length = strlen(pageDirectory) + strlen(stringfileId) + 1;   // gets length of new filename
                            char *filename = malloc(sizeof(char *) * length + 1);   // allocates memory for the filename

                            assert(filename != NULL);   // checks for malloc failure

                            strcpy(filename, pageDirectory);
                            strcat(filename, stringfileId); // creates filename
                            // Process URL
                            FILE* fp = fopen(filename, "r");
                            char* url = file_readLine(fp);

                            printf("Score: %2d doc %3d %s\n", rankedRes[n].score, rankedRes[n].docID, url);
                        

                            mem_free(stringfileId);
                            mem_free(filename);
                            mem_free(url);
                            
                        }
                    }
                    counters_delete(queryScore);
                    mem_free(rankedRes);
                    mem_free(numDocs);
                    mem_free(query);
                }
                printf("----------------------------------\n");
            }
        }
        else{
            mem_free(query);
        }
    }
    
    index_delete(indexStruct);
    fclose(indexFile);
}

/**************** blankCheck ****************/
/*this function checks if the query is a blankspace and
 *returns a boolean.
 */
bool blankCheck(char *query){
    int m = 0;
    while (query[m] != '\0'){
        if (isspace(query[m]) == 0){    
            return false;   // returns false if non-white space character
        }
        m++;
    }
    return true;    // returns trus if white space character
}


/**************** cleanQuery ****************/
/*this function cleans and parses each query according to the
 *syntax described in the REQUIREMENTS.md
 */
int cleanQuery(char *query, char*words[]){
    int i=0;    //index for query string
    int j=0;    //index for words array
    int wordNumber=0;      //keeps track of number of words in array
    bool newWord = true;
    char* word;     // pointer to first non-space character
    char* rest;     // pointer from beginning of word to first non-letter, replaced with null

    while (query[i] != '\0'){
        if ((isspace(query[i]) == 0) && (isalpha(query[i]) == 0)){
            fprintf(stderr, "Error: bad character '%c' in query.\n", query[i]);
            return 0;
        }

        query[i] = tolower(query[i]);   // converts characters to lowercase

        if (newWord){
            if (isalpha(query[i]) != 0){
                word = &(query[i]);     // points to the word
                rest = &(query[i]);     // points to the last read character
                newWord = false;        // first character in word search no longer needed
            } 
        }
        else{   // continues to read rest of characters in word
            rest = &(query[i]);

            if (isspace(*rest) != 0){   // if space character
                *rest = '\0';   // adds a null character at the end
                words[j] = word;    // places the word in an array
                j++;
                wordNumber++;
                newWord = true; // sets new word boolean flag to true after a null character is added at the end of a word
            }
        }
        i++;
    }
    if (!newWord){      //places last word in array if there was no space at the end to signal end of word
        words[j] = word;    // place the word in array
        wordNumber++;   // keeps track of number of words in array
    }

    return wordNumber;
}

/**************** validateSyntax ****************/
/*this function checks to see if the query follows the syntax rules
 * for 'and' and 'or'
 */
bool validateSyntax(char* words[], int numofWords){
    int i;
    for (i = 0; i < numofWords; i++){

        if ((strcmp(words[i], "and") == 0) ||  (strcmp(words[i], "or") == 0)){  // if current word is 'and' or 'or'
            if (i == 0){    // can't be first word in the query
                fprintf(stderr, "Error:'%s' cannot be the first word.\n", words[i]);
                return false;
            }
            else if (i == (numofWords - 1)){    // can't be last word in the query
                fprintf(stderr, "Error: '%s' cannot be the last word.\n", words[i]);
                return false;
            }
            else if ((strcmp(words[i + 1], "and") == 0) ||  (strcmp(words[i + 1], "or") == 0)){     // can't follow each othert directly
                fprintf(stderr, "Error: '%s' and '%s' cannot follow each other directly.\n", words[i], words[i + 1]);
                return false;
            }
        }
    }
    return true;
}


/**************** satisfy ****************/
/*this function returns a counter of documents that satisfies the query
 */
counters_t* satisfy(char **words, int wordCount, index_t *index){
    counters_t* allQuery = counters_new();  // creates a new and empty counter
    counters_t* andSeq;     // counter to keep track of consecutive andSequences
    bool andSeqFlag = true; // boolean flag to keep tack of and sequences

    for (int i = 0; i < wordCount; i++){        // loops through each word in array
        if (strcmp(words[i], "and") == 0){   // continue if current word is 'and'
            continue;
        }

        else if (strcmp(words[i], "or") == 0){  //if current word is or, it adds all previous andsequenced to counter
            andSeqFlag = true;  // then flips it back to true
            unionFunc( andSeq, allQuery);  // join andSeq with allQuery
            if (andSeq != NULL){    // if the andSeq is still full
                counters_delete(andSeq);    // it then deletes the current andSeq counter so a new one can be started
            }
        }

        else{   // else adds all words not joined explicitly by 'and'
            if (andSeqFlag){    // checks if boolean flag is still true
                counters_t* currWord = index_find(index, words[i]);  // gets docID and count pairs associated with word
                andSeq = counters_new();    // creates new andSequence counter

                if(currWord != NULL){   // it then copies the current word into the andSeq counter
                    copyCtrs(andSeq, currWord);
                }
                andSeqFlag = false; // sets andSeqFlag to false
            }
            else{   // else continue with old andSequence
                counters_t* currWord = index_find(index, words[i]);
                
                // allocates memory for two counters which can be passed to 
                // the iterator. This finds the intersection between two counters object
                // which represents the set of documents in which a word appears
                
                if(currWord != NULL){
                    counterGroup_t* twoCounters = malloc(sizeof(counterGroup_t));
                    twoCounters->counterOne = andSeq;
                    twoCounters->counterTwo = currWord;
                    counters_iterate(andSeq, twoCounters, intersection);
                    mem_free(twoCounters);
                }
            }
        }
    }
    unionFunc(andSeq, allQuery);
    counters_delete(andSeq);

    return allQuery;

}

static void
prompt(void)
{
  // print a prompt iff stdin is a tty (terminal)
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
}

/**************** intersection ****************/
/*this function finds the intersection between two counters
 */
void intersection(void* arg, const int key, int count){
    counterGroup_t* twoCounters = arg;
    int wordCountersCount = counters_get(twoCounters->counterTwo, key); // finds score associated with currentWord counter

    if (wordCountersCount < count){     
        counters_set(twoCounters->counterOne, key, wordCountersCount);  // updates andSequence with minimum score
    }

}

/**************** unionFunc ****************/
/*this function finds the union between two counters
 */
void unionFunc(counters_t* ctr1, counters_t* ctr2){
    counterGroup_t* twoCounters = malloc(sizeof(counterGroup_t));   // allocates memory for twocounters

    if (twoCounters == NULL){   // check for malloc failure
        fprintf(stderr, "unable to allocate memory for twoCounters\n");
        exit(5);
    }

    twoCounters->counterOne = ctr1;     // counter for andSeq
    twoCounters->counterTwo = ctr2;     // counter for curWord

    counters_iterate(ctr1, twoCounters, unionFuncHelper);
    mem_free(twoCounters);

}

/**************** unionFuncHelper ****************/
/*this function is a helper mehtod for unionFunc
 */
void unionFuncHelper(void *arg, const int key, const int count){
    counterGroup_t *twoCounters = arg;
    int wordCountersCount = counters_get(twoCounters->counterTwo, key);
    counters_set(twoCounters->counterTwo, key, wordCountersCount + count);  // adds the scores

}

/**************** copyCtrs ****************/
/*this function copies currWord counter into andSeq to begin a new andSeq
 */
void copyCtrs(counters_t* ctr1, counters_t* ctr2){
    counterGroup_t* twoCounters = malloc(sizeof(counterGroup_t));
    twoCounters->counterOne = ctr1;     // counter for andSeq
    twoCounters->counterTwo = ctr2;     // counter for curWord

    counters_iterate(ctr2, twoCounters, copyCtrsHelper);    
}


/**************** copyCtrsHelper ****************/
/*serves as helper function for copyCtrs
 */
void copyCtrsHelper(void* arg, const int key, const int count) {
    counterGroup_t *twoCounters = arg;
    counters_set(twoCounters->counterOne, key, count);
}

/**************** docsCounter ****************/
/*this function counts the number of items in the counters to determine
 * size of validDoc_t array
 */   
void docsCounter(void* arg, const int key, int count){
    if (count > 0){
        int *totalItems = arg;
        (*totalItems)++;
    }
}


/**************** rankResults ****************/
/*this function takes a counters struct of docID count(score) pairs
 * and creates a sorted array of validDoc_t strucs
 */   
validDoc_t* rankResults(counters_t *results, int resultsNum, int filesNum){
    validDoc_t* ranked = calloc(resultsNum, sizeof(validDoc_t));

    if (ranked == NULL){   // check for malloc failure
        fprintf(stderr, "unable to allocate memory for ranked\n");
        exit(5);
    }
    
    int resultsAdded = 0;

    for (int n = 1; n <= filesNum; n++){
        int count = counters_get(results, n);   // check if file in results counter then add to ranked array
        if (count > 0){
            validDoc_t res;
            res.docID = n;
            res.score = count;

            ranked[resultsAdded] = res;
            resultsAdded++;
        }
    }

    qsort(ranked, resultsNum, sizeof(validDoc_t), qsortHelper);     //use quicksort algorithm in sorting in descending order
    return ranked;
}

/**************** crawlerFilesLength ****************/
/*this function gets and returns the number of of crawler outputfiles
 * in the pageDirectory
 */ 
int crawlerFilesLength(char * pageDirectory){
    int fileId = 1;

    while (1) {     // open all files until exhausted
        char *stringfileId = malloc(sizeof(char *));
        assert(stringfileId != NULL);

        sprintf(stringfileId, "/%d", fileId);
        
        size_t length = strlen(pageDirectory) + strlen(stringfileId) + 1;
        char *filename = malloc(sizeof(char *) * length + 1);

        assert(filename != NULL);

        strcpy(filename, pageDirectory);
        strcat(filename, stringfileId);

        if (fopen(filename, "r") == NULL){
            mem_free(filename);
            mem_free(stringfileId);
            break;
        }
        else{
            mem_free(filename);
            mem_free(stringfileId);
            fileId++;
        }
    }
    
    return fileId - 1;
}

/**************** qsortHelper ****************/
/*this is a helper function for quicksort
 */ 
int qsortHelper(const void * firstDoc, const void *secondDoc){
    validDoc_t *docOne = (validDoc_t *) firstDoc;
    validDoc_t *docTwo = (validDoc_t *) secondDoc;

    return (docTwo->score - docOne->score);

}
