/* 
 * index.h - header file for CS50 index module
 * 
 * A *index* ia module providing the data structure to represent the in-memory index
 * and functions to read and write index files;
 *
 * Christian Nyameekye; Spring 2023.
 */

#ifndef __index_H
#define __index_H

#include <stdio.h>
#include <stdbool.h>
#include "../libcs50/counters.h"

/**************** global types ****************/
typedef struct index index_t;  // opaque to users of the module

/**************** helper functions ****************/
/**************** writeKeys ****************/
/* Helper function for index_writeToFile that writes keys 
 */
void writeKeys(void* filename, const char* key, void* item);

/**************** writeCounters ****************/
/* Helper function for index_writeToFile that writes counters(values)
 */
void writeCounters(void* filename, int key, int count);

/**************** countersDelete ****************/
/* Helper function for index_writeToFile that deletes counters(values)
 */
void countersDelete(void *item);

/****************  functions ****************/

/**************** index_new ****************/
/* Create a new (empty) index.
 *
 * Caller provides:
 *   number of slots to be used for the index (must be > 0).
 * We return:
 *   pointer to the new index; return NULL if error.
 * We guarantee:
 *   index is initialized empty.
 * Caller is responsible for:
 *   later calling index_delete.
 */
index_t* index_new(const size_t num_slots);

/**************** index_save ****************/
/* Insert item, identified by key (string), into the given index.
 *
 * Caller provides:
 *   valid pointer to index, valid string for key, valid integer for item.
 * We do:
 *   insert the key and counter of docID into index->hashtable.
 */
void index_save(index_t* index, const char* key, int docID);


/**************** index_find ****************/
/* Return the item associated with the given key.
 *
 * Caller provides:
 *   valid pointer to index, valid string for key.
 * We return:
 *   pointer to counter corresponding to the given key, if found;
 *   NULL if index is NULL, key is NULL, or key is not found.
 * Notes:
 *   the index is unchanged by this operation.
 */
counters_t* index_find(index_t* index, const char* key);

/**************** index_writeToFile ****************/
/* Print the whole table; provide the output file and func to print each item.
 * 
 * Caller provides:
 *   valid pointer to index, 
 *   FILE open for writing,  
 * We write:
 *   nothing, if NULL indexFilename.
 *   "(null)" if NULL index.
 * Note:
 *   the index and its contents are not changed by this function,
 */
void index_writeToFile(index_t* index, char* indexFilename);

/**************** index_load ****************/
/* Loads index data from file passed as parameter into the index data structure
 * Does nothing if index, and word is null and docId is less than 1.
 */
void index_load(index_t *index, FILE *fp);

/**************** index_delete ****************/
/* Delete index, calling a delete function on each item.
 *
 * Caller provides:
 *   valid index pointer,
 *   valid pointer to function that handles one item (may be NULL).
 * We do:
 *   if index==NULL, do nothing.
 *   free all the key strings, and the index itself.
 */
void index_delete(index_t* index);

#endif // __index_H
