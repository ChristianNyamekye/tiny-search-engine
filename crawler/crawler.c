/* 
 * crawler.c - CS50 'crawler' module
 *
 * see crawler.h for more information.
 *
 * Christian Nyamekye, Spring 2023.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "webpage.h"
#include "hashtable.h"
#include "bag.h"
#include "pagedir.h"
#include "crawler.h"
#include "mem.h"

int currentDepth = 0;
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

/**************** main() ****************/
/* The main function simply calls parseArgs and crawl, then exits zero. */
int 
main(const int argc, char* argv[]) {
  
  char* seedURL = NULL;
  char* pageDirectory = NULL;
  int maxDepth = 0;


  parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
  crawl(seedURL, pageDirectory, maxDepth);

  return 0;

}

/**************** parseArgs() ****************/
/* see crawler.h for description */

static void 
parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth) {
    // parse arguments
    if (argc != 4) {
        fprintf(stderr, "usage: %s: seedURL pageDirectory maxDepth\n", argv[0]);
        exit(1);
    }

    *seedURL = argv[1];
    *pageDirectory = argv[2];
    *maxDepth = atoi(argv[3]);

    if (*maxDepth <= 0 || *maxDepth > 10) {
        fprintf(stderr, "usage: maxDepth should be an integer from 1 to 10\n");
        exit(1);
    }

    // Check if pageDirectory exists and is writable
    if (!pagedir_init(*pageDirectory)) {
        fprintf(stderr, "usage: pageDirectory must exist and be writable\n");
        exit(1);
    }

    // Normalize seedURL and check if it is a valid internal URL
    *seedURL = normalizeURL(*seedURL);
    if (!isInternalURL(*seedURL)) {
        fprintf(stderr, "usage: seedURL must be a valid internal URL\n");
        exit(1);
    }
}


/**************** crawl() ****************/
/* see crawler.h for description */

static void 
crawl(char* seedURL, char* pageDirectory, const int maxDepth){
  
  int docID = 1;
  int depth = 0;
  hashtable_t* pagesSeen = hashtable_new(200); // initialize the hashtable and add the seedURL
  bag_t* pagesToCrawl = bag_new();
  webpage_t* webpage = webpage_new(seedURL, depth, NULL);

  hashtable_insert(pagesSeen, seedURL, "");
  bag_insert(pagesToCrawl, webpage);
  while (( webpage = bag_extract(pagesToCrawl)) != NULL){    // pull a webpage from the bag
    if (webpage_fetch(webpage)){ // fetch the HTML for that webpage
      fprintf(stdout, "%d\tFetched: %s\n", webpage_getDepth(webpage), webpage_getURL(webpage));
      pagedir_save(webpage, pageDirectory, docID); // save the webpage to pageDirectory
      docID += 1;
      if (webpage_getDepth(webpage) < maxDepth){ // if the webpage is not at maxDepth,
        pageScan(webpage, pagesToCrawl, pagesSeen); // pageScan that HTML
      }
    }
    depth++;
    webpage_delete(webpage); // delete that webpage
  }
  hashtable_delete(pagesSeen, NULL); // delete the hashtable
  bag_delete(pagesToCrawl, NULL); // delete the bag

}


/**************** pageScan() ****************/
/* 
* This function implements the pagescanner mentioned in the design. 
* Given a webpage, scan the given page to extract any links (URLs), ignoring non-internal URLs; 
* for any URL not already seen before (i.e., not in the hashtable), 
* add the URL to both the hashtable pages_seen 
* and to the bag pages_to_crawl
*/
static void 
pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen) {
    fprintf(stdout, "%d\tScanning: %s\n", webpage_getDepth(page), webpage_getURL(page));
    char* nextURL;
    int pos = 0;
    while ((nextURL = webpage_getNextURL(page, &pos)) != NULL) { // while there is another URL in the page
        
        fprintf(stdout, "%d\tFound: %s\n", webpage_getDepth(page), nextURL);
        if (isInternalURL(nextURL)) { // if that URL is Internal,
            
            if (hashtable_insert(pagesSeen, nextURL, "") == false) { // insert the webpage into the hashtable
                fprintf(stdout, "%d\tIgnDupl: %s\n", webpage_getDepth(page), nextURL);
                mem_free(nextURL);
                continue;
            }
            webpage_t* webpage = webpage_new(nextURL, webpage_getDepth(page) + 1, NULL); // if that succeeded, create a webpage_t for it
            bag_insert(pagesToCrawl, webpage); // insert the webpage into the bag
            fprintf(stdout, "%d\tAdded: %s\n", webpage_getDepth(page),  webpage_getURL(webpage));

            if (webpage == NULL) {
                
                mem_free(nextURL); // 	free the URL
                continue;
            }
            
        } else {
            fprintf(stdout, "%d\tIgnExtern: %s\n", webpage_getDepth(page), nextURL);
            mem_free(nextURL);
        }
    }
}
