/* 
 * index.c - CS50 'index' module
 *
 * see index.h for more information.
 *
 * Christian Nyamekye, Spring 2023.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/mem.h"
#include "index.h"
#include "../libcs50/set.h"
#include "../libcs50/file.h"

/**************** file-local global variables ****************/
/* none */
typedef struct index {
  hashtable_t* hashtable;
} index_t;

index_t* index_new(const size_t num_slots)
{ 
  if (num_slots == 0) {
    return NULL;
  }

  index_t* index = mem_malloc(sizeof(index_t));
  if (index == NULL) {
    return NULL;
  }

  index->hashtable = hashtable_new(num_slots);

  if (index->hashtable == NULL) {
    mem_free(index);
    return NULL;
  }

  return index;
}

/**************** index_save() ****************/
/* see index.h for description */
void
index_save(index_t* index, const char* key, int docID)
{ 
  
  if (index != NULL && key != NULL && docID >= 1){
      counters_t* counter = index_find(index, key);
      
      if (counter == NULL){
          
          counter = counters_new();
          if (counter != NULL){ // if counter is not NULL
          
          counters_add(counter, docID);
          
          hashtable_insert(index->hashtable, key, counter); // insert into the table
          }     
      }
      else{
          counters_add(counter, docID); // add new ID to counter and insert into table
          hashtable_insert(index->hashtable, key, counter);
      }
  }

#ifdef MEMTEST
  mem_report(stdout, "After index_save");
#endif
}



/**************** index_find() ****************/
/* see index.h for description */
counters_t*
index_find(index_t* index, const char* key)
{
  if (index == NULL || key == NULL) {
    return NULL;              // bad hashtable
  } else if (index->hashtable == NULL) {
    return NULL;              // hashtable is empty
  } else {
    counters_t* result = hashtable_find(index->hashtable, key); // return counter value
    if (result != NULL){
      return result;
    }
  }
  return NULL;
}

/**************** index_writeToFile() ****************/
/* see index.h for description */
void index_writeToFile(index_t* index, char* indexFilename) {
    if (index == NULL || indexFilename == NULL) {
        return;
    }
    FILE* fp = fopen(indexFilename, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error: could not open file '%s' for writing\n", indexFilename);
        index_delete(index);
        return;
    }
    hashtable_iterate(index->hashtable, fp, writeKeys); // writes keys into file fp

    fclose(fp);
}

/**************** writeKeys() ****************/
/* see index.h for description */
void writeKeys(void* filename, const char* key, void* item)
{ 
    FILE* fp = (FILE*) filename;
    if (fp == NULL) {
        return;
    }
    else{
    struct counters* counter = item;
    fprintf(fp, "%s ", key); // write key word
    counters_iterate(counter, fp, writeCounters);
    fprintf(fp, "\n");
    }
}

/**************** writeCounters() ****************/
/* see index.h for description */
void writeCounters(void* filename, int key, int count)
{
    FILE* fp = (FILE*) filename; 
    if (fp == NULL) {
        return;
    }
    else{
      fprintf(fp, "%d %d", key, count); // write counters
      } 
}

/**************** countersDelete() ****************/
/* see index.h for description */
void countersDelete(void *item)
{
    if (item != NULL){
    counters_delete(item);}
}

/**************** index_load() ****************/
/* see index.h for description */
void
index_load(index_t *index, FILE *fp)
{ 
  if (fp != NULL){
    char *word = NULL;
    int docID = 0;
    int count = 0;

    while ((word = file_readWord(fp)) != NULL){
      while ((fscanf(fp, "%d %d", &docID, &count)) > 0){
        if (index != NULL && word != NULL && docID >= 1){
          counters_t* counter = hashtable_find(index->hashtable, word);

          if (counter == NULL){
              counters_t *latestcounter = counters_new();
              counters_set(latestcounter, docID, count);
              hashtable_insert(index->hashtable, word, latestcounter);
          }
          else{
              counters_set(counter, docID, count);
          }
        }
      }
      mem_free(word);
    }

  }

}

/**************** index_delete() ****************/
/* see index.h for description */
void 
index_delete(index_t* index)
{
  if (index != NULL) {
    hashtable_delete(index->hashtable, (*countersDelete));
    mem_free(index);
  }
  
#ifdef MEMTEST
  mem_report(stdout, "End of hashtable_delete");
#endif
}


