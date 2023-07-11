/* 
 * crawler.h - header file for CS50 'crawler' module
 * 
 * The Tiny Search Engine crawler is a standalone program that crawls the web and retrieves webpages 
 * starting from a "seed" URL. It parses the seed webpage, extracts any embedded URLs, 
 * then retrieves each of those pages, recursively, but limiting its exploration to a 
 * given "depth".
 *
 * Christian Nyamekye; Spring 2023
 */



#include <stdio.h>



/********************************* main **************************************/
/* 
 * main: method to run a generic crawler.
 * executes from a command line with usage syntax ./crawler seedURL pageDirectory maxDepth
 * seedURL is an 'internal' directory, to be used as the initial URL,
 * pageDirectory is the (existing) directory in which to write downloaded webpages, and
 * maxDepth is an integer in range [0..10] indicating the maximum crawl depth.
*/
int main(const int argc, char* argv[]);


/**************** functions ****************/

/**************** parseArgs ****************/
/* Given arguments from the command line, extract them into the function parameters;
 * 
 *
 * We return:
 *   return only if successful.
 *   if any trouble is found, print an error to stderr and exit non-zero.
 * We guarantee:
 *   for pageDirectory, call pagedir_init()
 *   for maxDepth, ensure it is an integer in specified range
 *   for seedURL, normalize the URL and validate it is an internal URL
 * Caller is responsible for:
 *   later calling crawler_delete.
 */
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);



/**************** crawl ****************/
/* Does the real work of crawling from seedURL to maxDepth and saving pages in pageDirectory
 *
 * Caller provides:
 *   a valid seedURL pointer, pageDirectory pointer, and a valid maxDepth integer.
 * We guarantee:
 *    loops over pages to explore, until the list is exhausted
 * Caller is responsible for:
 *   not free-ing the item as long as it remains in the crawler.
 */


static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);



/**************** pageScan ****************/
/* Add new item to the crawler.
 *
 * Caller provides:
 *   a valid crawler pointer and a valid item pointer.
 * We guarantee:
 *   a NULL crawler is ignored; a NULL item is ignored.
 *   the item is added to the crawler.
 * Caller is responsible for:
 *   not free-ing the item as long as it remains in the crawler.
 */

static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);