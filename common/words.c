/* 
 * words.c - CS50 'words' module
 *
 * see words.h for more information.
 *
 * Christian Nyamekye, Spring 2023.
 */


#include "words.h"
#include <stdlib.h>
#include <ctype.h>

void
normalizeWord(char* word){
  for (int i = 0; word[i]; i++){
              word[i] = tolower(word[i]);
          }
    
}