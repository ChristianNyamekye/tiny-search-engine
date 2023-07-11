
/* 
 * pagedir.h - header file for CS50 'pagedir' module
 * 
 * The reusable pagedir module to handles the pagesaver mentioned in 
 * the crawler design (writing a page to the pageDirectory), and marking 
 * it as a Crawler-produced pageDirectory (as required in the spec). 
 * pagedir is written as a separate module, in ../common, to encapsulate all the 
 * knowledge about how to initialize and validate a pageDirectory, and how to write 
 * and read page files, in one place... anticipating future use by the Indexer and Querier.
 *
 * Christian Nyamekye; Spring 2023.
 */



#include <stdio.h>
#include <stdlib.h>
#include "../libcs50/webpage.h"


/**************** functions ****************/

/**************** pagedir_init ****************/
/* Create a new (empty) pagedir.
 *
 * We return:
 *   true on succes, false on error.
 * We guarantee:
 *   The new pagedir is initialized empty.
 *   
*/
bool pagedir_init(const char* pageDirectory);

/**************** pagedir_save ****************/
/* Save file to the pagedir.
 * construct the pathname for the page file in pageDirectory
 * Caller provides:
 *   a valid pagedir pointer and a valid item pointer.
 * We guarantee:
 *   print the URL to a file
 *   print the depth to a file
 *   print the contents to a file
 *   close the file
 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/**************** pagedir_load ****************/
/* Save file to the pagedir.
 * construct a webpage from a crawler file
 * Caller provides:
 *   a valid filename pointer.
 * We guarantee:
 *   freeing the depth of the webpage
 *   close the file
 * We return:
 *    a valid webpage from the file
 */
webpage_t* pagedir_load(const char* filename);
