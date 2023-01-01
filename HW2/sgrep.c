#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for skeleton code */
#include <unistd.h> /* for getopt */
#include "str.h"

#define MAX_STR_LEN 1023

#define FALSE 0
#define TRUE  1

/*
 * Fill out your own functions here (If you need) 
 */
/* Get subpattern: pattern between '*'s. */
char *GetSubPat(char *subpat, const char *pattern, char **pat_loc) {
  char *begin = subpat; /* Begin address */

  while(*pattern == '*') pattern++; /* Increase address of pattern if it is '*' */

  while((*pattern != '*') && (*pattern != '\0')) { /* Get subpattern until it meets '*' or Null */
    *subpat = *pattern; /* Assign subpattern element */
    pattern++; /* Get next element of pattern */
    subpat++; /* Get next element of subpattern */
    // i++; /* Go to next index */
  }
  *pat_loc =(char *) pattern; /* Save Current address */

  *subpat = '\0'; /* Insert null at the end */

  subpat = begin; /* Assign first address of subpat */

  return subpat; /* Return new subpattern */
}

/*--------------------------------------------------------------------*/
/* PrintUsage()
   print out the usage of the Simple Grep Program                     */
/*--------------------------------------------------------------------*/
void 
PrintUsage(const char* argv0) 
{
  const static char *fmt = 
	  "Simple Grep (sgrep) Usage:\n"
	  "%s pattern [stdin]\n";

  printf(fmt, argv0);
}
/*-------------------------------------------------------------------*/
/* SearchPattern()
   Your task:
   1. Do argument validation 
   - String or file argument length is no more than 1023
   - If you encounter a command-line argument that's too long, 
   print out "Error: argument is too long"
   
   2. Read the each line from standard input (stdin)
   - If you encounter a line larger than 1023 bytes, 
   print out "Error: input line is too long" 
   - Error message should be printed out to standard error (stderr)
   
   3. Check & print out the line contains a given string (search-string)
      
   Tips:
   - fgets() is an useful function to read characters from file. Note 
   that the fget() reads until newline or the end-of-file is reached. 
   - fprintf(stderr, ...) should be useful for printing out error
   message to standard error

   NOTE: If there is any problem, return FALSE; if not, return TRUE  */
/*-------------------------------------------------------------------*/
int
SearchPattern(const char *pattern)
{
  char buf[MAX_STR_LEN + 2]; 
  int len;
  const char *begin = pattern; /* Store first address of pattern */
  char **pat_loc=(char**)&pattern; /* Change pattern address */
  char *bufptr = buf; /* Store current address of string */
  char subpat[MAX_STR_LEN+1]=""; /* Store subpattern */
  int subpat_len=0; /* Store length of subpattern */
  
  /* 
   *  TODO: check if pattern is too long
   */
  if ((len = StrGetLength(pattern)) > MAX_STR_LEN) {
    fprintf(stderr, "Error: pattern is too long\n");
    return FALSE;
  }
  
  /* Read one line at a time from stdin, and process each line */
  while (fgets(buf, sizeof(buf), stdin)) {
	  
    /* check the length of an input line */
    if ((len = StrGetLength(buf)) > MAX_STR_LEN) {
      fprintf(stderr, "Error: input line is too long\n");
      return FALSE;
    }
	
    /* TODO: fill out this function */
    /* Search pattern until line ends */

    bufptr = buf; /* Initialize bufptr to new buf */
    subpat_len = 0; /* Initialize length of subpattern as 0 */
    
    while(bufptr != NULL){ /* Search until line ends */

      /* If all subpatterns completely search until line does not end, */
      if(*pattern == '\0') { 
	printf("%s",buf); /* Print current string */
	break; /* Search for next string */
      }

      bufptr+=subpat_len; /* Change location after find subpattern */

      GetSubPat(subpat,pattern,pat_loc); /* Get new subpattern */
      subpat_len = StrGetLength(subpat); /* Get length of new subpattern */

      bufptr=StrFindStr(bufptr, subpat); /* Find subpattern from current location */
      
    }

    pattern = begin; /* Get first address again */
    
  }
   
  return TRUE;
}
/*-------------------------------------------------------------------*/
int 
main(const int argc, const char *argv[]) 
{
  /* Do argument check and parsing */
  if (argc < 2) {
	  fprintf(stderr, "Error: argument parsing error\n");
	  PrintUsage(argv[0]);
	  return (EXIT_FAILURE);
  }

  return SearchPattern(argv[1]) ? EXIT_SUCCESS:EXIT_FAILURE;
}
