/* 
 * pagedir.c - CS50 'pagedir' module
 *
 * see pagedir.h for more information.
 *
 * Christian Nyamekye, Spring 2023: Tiny Search Engine
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include "../libcs50/webpage.h"
#include "pagedir.h"
#include "../libcs50/file.h"
#include "../libcs50/mem.h"

bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
bool pagedir_validate(const DIR* dir);
webpage_t* pagedir_load(const char* filename);

/**************** pagedir_init() ****************/
/* see pagedir.h for description */
  
bool pagedir_init(const char* pageDirectory){
  char filename[100];
  sprintf(filename, "%s/.crawler", pageDirectory); // construct the pathname for the .crawler file in that directory
  FILE* fp = fopen(filename, "w"); // open the file for writing; on error, return false.
  if (fp == NULL) { // validates the file
    fprintf(stderr, "Error: Could not open file %s\n", filename);
    return false;
  }
  // close the file and return true.
  fclose(fp);
  return true;
}


/**************** pagedir_save() ****************/
/* see pagedir.h for description */

void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID){
  char filename[100];
  sprintf(filename, "%s/%d", pageDirectory, docID); // construct the pathname for the page file in pageDirectory
  FILE* fp = fopen(filename, "w"); // open that file for writing
  if (fp == NULL) { // validates the file
    fprintf(stderr, "Error: Could not open file %s\n", filename); 
    return;
  }
  fprintf(fp,"%s\n", webpage_getURL(page)); // print the URL
  fprintf(fp,"%d\n", webpage_getDepth(page)); // print the depth
  fprintf(fp,"%s", webpage_getHTML(page)); // print the contents of the webpage
  fclose(fp); // close the file
}

/**************** pagedir_load() ****************/
/* see pagedir.h for description */
webpage_t* pagedir_load(const char* filename){
  FILE* fp = fopen(filename, "r");
 
  char* url = file_readLine(fp);
  char* depth = file_readLine(fp);
  char* html = file_readFile(fp);
  
  if (url == NULL || depth == NULL || html == NULL) {
    printf("Failed to read data from file %s\n", filename);
   
    fclose(fp);
    return NULL;
  }

  int intDepth = atoi(depth);

  mem_free(depth);
  webpage_t* webpage;
  webpage = webpage_new(url, intDepth, html);
 
  if (webpage == NULL){
      printf("webpage_new failed\n");
      
      return NULL;
  }
  fclose(fp);
  return webpage;
}


