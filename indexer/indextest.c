/* 
 * indextest.c - CS50 'indextest' module
 *
 *
 * Christian Nyamekye, Spring 2023.
 */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "../common/index.h"
#include "file.h"

static void test(char* oldIndexFilename, char* newIndexFilename);


int main(int argc, char* argv[]) {

    if (argc != 3) {
        fprintf(stderr, "usage: ./indextest oldIndexFilename newIndexFilename\n");
        exit(1);
    }

    char* oldIndexFilename = argv[1];
    char* newIndexFilename = argv[2];

    test(oldIndexFilename, newIndexFilename);
    
    exit(0);
}


static void
test(char* oldIndexFilename, char* newIndexFilename){
    FILE *fp = fopen(oldIndexFilename, "r");

    if (fp == NULL){
        fprintf(stderr, "%s cannot be read\n", oldIndexFilename);
        exit(4);
    }

    // load the index from the oldIndexFilename into an inverted-index data structure
    int fileSize = file_numLines(fp);
    index_t* index = index_new(fileSize);

    index_load(index, fp);
    
    fclose(fp);

    // create a file newIndexFilename and write the index to that file, in the format described below
    index_writeToFile(index, newIndexFilename);

    // delete the index data structure
    index_delete(index);

}
