/* 
 * indexer.c - CS50 'indexer' module
 *
 * see IMPLEMENTATION.md for more information.
 *
 * Christian Nyamekye, Spring 2023.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/bag.h"
#include "../common/pagedir.h"
#include "mem.h"
#include "index.h"
#include "words.h"

# define max_value 1000

static void parseArgs(const int argc, char* argv[], DIR** pageDirectory, char** dir_path, char** index1name);
index_t* indexBuild(DIR* pageDirectory, char* dir_path);
static void indexPage(index_t* index, webpage_t* webpage, int docID);
static void indexWrite(index_t* index, char* indexFilename);


/**************** main() ****************/
/* The main function simply calls parseArgs and crawl, then exits zero. */
int main(const int argc, char* argv[]) {
  
    DIR* pageDirectory = NULL;
    char* indexFilename = NULL;
    char* dir_path = NULL;

    // parse the command line, validate parameters, initialize other modules
    parseArgs(argc, argv, &pageDirectory, &dir_path, &indexFilename);
    

    // call indexBuild, with pageDirectory
    index_t* index = indexBuild(pageDirectory, dir_path);
    indexWrite(index, indexFilename);
    index_delete(index);
    exit(0);

}


static void parseArgs(const int argc, char* argv[], DIR** pageDirectory, char** dir_path, char** indexFilename) {
    // parse arguments
    if (argc != 3) {
        fprintf(stderr, "usage: %s: pageDirectory indexFilename\n", argv[0]);
        exit(1);
    }

    *pageDirectory = opendir(argv[1]);
    *dir_path = argv[1];
    *indexFilename = argv[2];
    const char* crawlerFile = ".crawler";

    char crawler_path[max_value];

    // construct the path to the .crawler file in the pageDirectory
    sprintf(crawler_path, "%s/%s", *dir_path, crawlerFile);

    bool file_exists = false;
    FILE* file = fopen(crawler_path, "r");
    if (file != NULL) {
        fclose(file);
        file_exists = true;
    }

    // exit if the file doesn't exist
    if (!file_exists) {
        exit(2);
    }

    FILE* fp; 

    // Check if pageDirectory exists and is writable
    if ((fp = fopen(*indexFilename, "w")) == NULL) {
        fprintf(stderr, "usage: indexFilename must exist and be writable\n");
        exit(3);
    }
    
    if (*pageDirectory == NULL) {
        perror("opendir failed!");
        exit(3);
    }
    fclose(fp);
}


index_t*
indexBuild(DIR* pageDirectory, char* dir_path){
    
    index_t* index = index_new(1000);
    struct dirent *entry;
    
    if (pageDirectory == NULL) {
    fprintf(stderr, "Error: pageDirectory is NULL\n");
    return NULL;
    }
    int fileID = 1;

    // loops over document ID numbers, counting from 1
    while ((entry = readdir(pageDirectory)) != NULL) {
        
        char filepath[max_value];
        snprintf(filepath, max_value, "%s/%s", dir_path, entry->d_name);
        
        char target_file_path[max_value];
        snprintf(target_file_path, max_value, "%s/%d", dir_path, fileID);
        
        if (strcmp(filepath, target_file_path) == 0) { // check if the entry is a regular file

            webpage_t* webpage = pagedir_load(filepath); // loads a webpage from the document file 'pageDirectory/id'
                        
            if (webpage != NULL){ // if successful, 
                indexPage(index, webpage, fileID); // passes the webpage and docID to indexPage
                fileID++;
            }
            webpage_delete(webpage);
        }
    }
    
    closedir(pageDirectory);
    return index;
}


static void
indexPage(index_t* index, webpage_t* webpage, int docID){
    int pos = 0;
    char* word = NULL;
    // steps through each word of the webpage,
    while ((word = webpage_getNextWord(webpage, &pos)) != NULL) {
        if (strlen(word) > 2){ // skips trivial words (less than length 3),
            normalizeWord(word); // normalizes the word (converts to lower case),
            index_save(index, word, docID); // looks up the word in the index, adding the word to the index if needed           
        }
        mem_free(word);
    }
}


static void 
indexWrite(index_t* index, char* indexFilename){
    index_writeToFile(index, indexFilename);
}

