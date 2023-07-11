# CS50 Lab 6 TSE Querier Implementation Spec
## CS50 Spring 2023; Christian Nyamekye

### Implementation
This Implementation Spec is a further refinement of the Design Spec in DESIGN.md


#### Detailed pseudocode for components of querier

```c
int main(int argc, char *argv[]){
    Parses the arguments (pageDirectory and indexFilename) passed in as parameters.
    Checks and ensures their validity.
    Uses a while loop to continuosly allows queries  untill terminated by the user with ctrl c.
    Passes the 2 arguments into the query function inside the while loop.
}

void query(char * pageDirectory, char * indexFilename){
    Opens the index file.
    Gets the number of lines in the file.
    Creates an index struct with number of slots same as that of number of lines in file.
    Loads the index file into the index struct.
    Creats a char array to store the query.
    Uses a while loop to read input from stdin.
    Validates the syntax.
    Ranks the result.
    Prints the ranked results.
}

bool blankCheck(char *query){
    Iterates over the query array and returns false if non-white space character. Returns true if white space character.
}

int cleanQuery(char *query, char*words[]){
    Initializes query string and words array indices.
    Initializes a char pointer (char * word) to point to first non-space character.
    Initializes anothet char pointer (char * rest), which is the pointer from beginning of word to first non-letter, replaced with null.
    initializes a bool flag for a new word.
    Loops though each character in the word in the query string.
        Performs a check for bad characters in the query such as non-alphabets and blank spaces.
        Comverts all cases to lower case
        Reads each character and adds a null at the end of a word.
        Places the word into the words array.
    Increments the counter for the while loop.
    Adds the last word in the query to the words array.
}

bool validateSyntax(char *words[], int numofWords){
    Loops through the words in the array/
    If word is "and" or "or"
        If it is the first word in the array
            Print an error message to sderr
            Return false
        Else if it is the last word in the array
            Print an error message to sderr
            Return false;
    Return true
}

counters_t* satisfy(char **words, int wordCount, index_t *index){
    Creates a new and empty counter called allQuery
    Declares a counter to keep track of consecutive and sequence
    Declares a boolean flag to keep track of and sequences
    Loops through each word in the array
        If current word is and, continue
        Else if word is or
            Sets boolean flag to true
            Joins and sequence counter to allQuery counter
            Deletes and sequence counter if not null
        Else
            If boolean flag is still true
                Gets docID and count pairs associated with word
                Creates new and sequence counter
                Copies the current word into the and sequence counter
                Sets boolean flag to false
            Else
                continue with old and sequnce
                allocates memory for two counters which can be passed to 
                the iterator.
                find the intersection between two counters object
                which represents the set of documents in which a word appears
    Unionize andSeq and allQuery counters.
    Delete the counters
}

void intersection(void *arg, const int key, int count){
    Declare a new countergroup struct and assign it to arg
    Get the score associated with currentWord counter
    Compare the counts of both counters
    Updates andSequence with the minimum score
}

void unionFunc(counters_t *ctr1, counters_t *ctr2){
    Initialize and allocate memory for new countergroup struct
    Assigns andSeq counter to counter one of countergroup struct
    Assigns curWord counter to counter two of countergroup struct
    Iterate through counters with the help of unionFunc helper
    Free the countergroup struct
}

void unionFuncHelper(void *arg, const int key, const int count){
    Sums the scores of the two counters
}

void copyCtrs(counters_t *ctr1, counters_t *ctr2){
    Copies of counters into another counter
}

void copyCtrsHelper(void *arg, const int key, const int count){
    Sets the contents of one counter into the other
}

void docsCounter(void *arg, const int key, int count){
    Count the number of items in the counter
}


validDoc_t* rankResults(counters_t *results, int resultsNum, int filesNum){
    Creates a ranked array and allocates memory for it
    loops through the cocuments in the counter
    Gets the document IDs and the score
    Adds them to the ranked array
}

int crawlerFilesNum(char * pageDirectory){
    Repetitively open all files in pageDirectory untill exhaustes
    Keeps track of the number of files opened

}

int qsortHelper(const void * firstDoc, const void *secondDoc){
    Sorts in decreasing order of magnitude
}
```

#### Data Structures
*Counters*: This was used to keep track of the number of times a word appeared in a document.

*Index*: This was used to map the words to their document ID and count.


The output of the indexer is displayed as follows:

```c
Query? light or science
Query: light or science 
Matches 2 documents (ranked):
Score:  3 doc   3 http://cs50tse.cs.dartmouth.edu/tse/toscrape/
Score:  1 doc   2 http://cs50tse.cs.dartmouth.edu/tse/wikipedia/
----------------------------------
```
