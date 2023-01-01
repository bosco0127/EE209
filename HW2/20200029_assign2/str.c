#include <assert.h> /* to use assert() */
#include <stdio.h>
#include <stdlib.h> /* for strtol() */
#include <string.h>
#include <ctype.h> /* for isspace() and isdigit() */
#include "str.h"

/* Your task is: 
   1. Rewrite the body of "Part 1" functions - remove the current
      body that simply calls the corresponding C standard library
      function.
   2. Write appropriate comment per each function
*/

/* Part 1 */
/*------------------------------------------------------------------------*/
size_t StrGetLength(const char* pcSrc)
{
  const char *pcEnd; /* Detect end of string */
  
  if (NULL == pcSrc) assert(0); /* NULL address, 0, and FALSE are identical. */
  else pcEnd = pcSrc;
	
  while (*pcEnd != '\0') /* null character and FALSE are identical. */
    pcEnd++;

  return (size_t)(pcEnd - pcSrc); /* Return difference of its address */
}

/*------------------------------------------------------------------------*/
char *StrCopy(char *pcDest, const char* pcSrc)
{
  size_t len; /* Length of pcSrc */
  
  if (NULL==pcSrc) assert(0); /* Assert when NULL address  */

  len=StrGetLength(pcSrc); /* Get length of pcSrc */

  for(size_t i=0;i<=len;i++) /* Copy the element include terminating null byte */
    pcDest[i]=pcSrc[i];
  
  return pcDest;
}

/*------------------------------------------------------------------------*/
int StrCompare(const char* pcS1, const char* pcS2)
{
  while( (*pcS1 != '\0') || (*pcS2 != '\0') ) { /* Compare until one of them meet NULL */
    
    if ( *pcS1 != *pcS2 ) return (*pcS1-*pcS2); /* Return their difference */

    else {
      pcS1++; /* Go to the next element of pcS1 */
      pcS2++; /* Go to the next element of pcS2 */
    }
  }
 
  return (*pcS1-*pcS2); /* Return difference between elements */
}

/*------------------------------------------------------------------------*/
char *StrFindChr(const char* pcHaystack, int c)
{
  size_t len=StrGetLength(pcHaystack); /* Get length of haystack */
  
  for(size_t i=0; i<=len ; i++) { /* Find char until investigate hole string */

    if (*pcHaystack == c) return (char*)pcHaystack; /* Return current address if find char */
    else pcHaystack++; /* Else, increase the address */
  }
  
  return NULL; /* Return Null if char is not found */
}

/*------------------------------------------------------------------------*/
char *StrFindStr(const char* pcHaystack, const char *pcNeedle)
{
  size_t Haystack_len=StrGetLength(pcHaystack); /* Get length of Haystack */
  size_t Needle_len=StrGetLength(pcNeedle); /* Get length of needle */
  
  if(pcHaystack == NULL || pcNeedle == NULL) assert(0); /* Assert if one of them is NULL */

  if(*pcNeedle == '\0') /* If pcNeedle is empty, */
    return (char*)pcHaystack; /* treat it as it is always matched */

  for(size_t i=0; i<=Haystack_len ; i++) { /* Search until investigate hole string */

    if (*pcHaystack == *pcNeedle) { /* If the first letter are same */
      
      for(size_t j=0; j<Needle_len ; j++) { /* Investigate whether others are same too */
	
	if(pcHaystack[j]!=pcNeedle[j]) break; /* Break for loop if element is different */

	if(Needle_len-1 == j) /* If all elements are same */
	  return (char*)pcHaystack; /* return current address */
      }
      pcHaystack++; /* Go to next letter */
    }
    
    else pcHaystack++; /* Go to next letter if the first letter is different */
  }
 
  return NULL; /* Return NULL if there's no needle string */
}

/*------------------------------------------------------------------------*/
char *StrConcat(char *pcDest, const char* pcSrc)
{
  char *begin_pcDest = pcDest; /* Beginning point of pcDest */
   
  size_t len_pcDest = StrGetLength(pcDest); /* Get length of pcDest */

  for(size_t i=0; i<len_pcDest; i++) pcDest++; /* Increase address until meet terminating null */

  StrCopy(pcDest,pcSrc); /* Start copy string from end of pcDest including terminating null */

  pcDest=begin_pcDest; /* Change address of pcDest to origianl address */
  
  return pcDest; /* return address of pcDest*/
}

/*------------------------------------------------------------------------*/
long int StrToLong(const char *nptr, char **endptr, int base)
{
  /* handle only when base is 10 */
  if (base != 10) return 0;

  /* TODO: fill this function */
  return strtol(nptr, endptr, 10);/* handle only when base is 10 */
  if (base != 10) return 0;

  /* Coverting Part*/
  char *startptr = (char *)nptr; /* Store first address of nptr */
  long int num = 0; /* Store long integer */
  int temp = 0; /* Temporary store a digit */

  while(*nptr != '\0') {
    if (isspace(*nptr)) nptr++; /* If character is space, go to next character */

    else if (*nptr == '+') { /* If it is +, */

      nptr++; /* Go to next character */

      if((*nptr == '\0')||(isdigit(*nptr) == 0)) { /* If next character is not digit, */
	*endptr = startptr; /* Assign the first address of nptr */
	break; /* Break while loop */
      }
      
      /* Convert number */
      while(*nptr && isdigit(*nptr)) { /* Detect whether it is digit */
	temp = *nptr - '0'; /* Convert character into digit */
	num = 10*num + temp; /* Add temp to a number */
	nptr++; /* Go to next character */
	if(num < 0) /* If num is greater than LONG_MAX so that num changes into - value, */
	  num = LONG_MAX; /* num keeps LONG_MAX */
      }
      *endptr = (char *)nptr; /* Assign the current address of nptr */
      break; /* Break while loop */
    }

    else if (*nptr == '-') { /* If it is -, */

      nptr++; /* Go to next character */

      if((*nptr == '\0')||(isdigit(*nptr) == 0)) { /* If next character is not digit, */
	*endptr = startptr; /* Assign the first address of nptr */
	break; /* Break while loop */
      }
      
      /* Convert number */
      while(*nptr && isdigit(*nptr)) { /* Detect whether it is digit */
	temp = *nptr - '0'; /* Convert character into digit */
	num = 10*num + temp; /* Add temp to a number */
	nptr++; /* Go to next character */
      }
      num = -num;
      if(num > 0) /* If num is less than LONG_MIN so that num changes into + value, */
	num = LONG_MIN; /* num keeps LONG_MIN */
      *endptr = (char *)nptr; /* Assign the current address of nptr */
      break; /* Break while loop */
    }

    else if ((*nptr != '\0') && isdigit(*nptr)) { /* If it is digit, */

      if((*nptr == '\0')||(isdigit(*nptr) == 0)) { /* If character is not digit, */
	*endptr = startptr; /* Assign the first address of nptr */
	break; /* Break while loop */
      }
      
      /* Convert number */
      while(*nptr && isdigit(*nptr)) { /* Detect whether it is digit */
	temp = *nptr - '0'; /* Convert character into digit */
	num = 10*num + temp; /* Add temp to a number */
	nptr++; /* Go to next character */
	if(num < 0) /* If num is greater than LONG_MAX so that num changes into - value, */
	  num = LONG_MAX; /* num keeps LONG_MAX */
      }
      *endptr = (char *)nptr; /* Assign the current address of nptr */
      break; /* Break while loop */
    }

    else { /* In case of not meeting digit */
      *endptr = startptr; /* Assign the first address of nptr */
      break; /* Break while loop */
    }
  }

  return num; /* If num is inside of long integer, return num */
}
