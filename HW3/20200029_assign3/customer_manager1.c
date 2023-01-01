/* 20200029 Kong Hyun Deok
   Assignment 3
   customer_manager1.c */
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"

/*----------------------------------------------------------------------
Uncomment and use the following code if you want
----------------------------------------------------------------------*/
#define UNIT_ARRAY_SIZE 1024 /* set unit array size */

struct UserInfo {
  char *name;                // customer name
  char *id;                  // customer id
  int purchase;              // purchase amount (> 0)
};

struct DB {
  struct UserInfo *pArray;   // pointer to the array
  int curArrSize;            // current array size (max # of elements)
  int numItems;              // # of stored items, needed to determine
			     // # whether the array should be expanded
			     // # or not
};


/* Create customer data base.
   Needs no parameter to call. 
   Returns DB_T type pointer is success.
   If memory allocation is failed, it returns NULL. */
DB_T
CreateCustomerDB(void)
{
  /* Uncomment and use the following implementation if you want */
  DB_T d;
  
  d = (DB_T) calloc(1, sizeof(struct DB));
  if (d == NULL) {
    fprintf(stderr, "Can't allocate a memory for DB_T\n");
    return NULL;
  }
  d->curArrSize = UNIT_ARRAY_SIZE; // start with 1024 elements
  d->pArray = (struct UserInfo *)calloc(d->curArrSize, sizeof(struct UserInfo));
  if (d->pArray == NULL) {
    fprintf(stderr, "Can't allocate a memory for array of size %d\n", d->curArrSize);   
    free(d);
    return NULL;
  }
  return d;
}

/* Delete all the memories of customer.
   Needs DB_T type parameter, d.
   If d is NULL, function do nothing. */
void
DestroyCustomerDB(DB_T d)
{
  /* fill out this function */
  /* If given pointer is NULL, do nothing */
  if(d == NULL) return;

  /* Define UserInfo type pointers */
  //struct UserInfo *p; /* Variable for free the information */
  int i=0; /* index for finding address */

  /* Free from head to tail */
  for (i=0; i<d->curArrSize; i++) {
    if(d->pArray[i].id != NULL) /* If id is not NULL */
      free(d->pArray[i].id); /* free id */
    if(d->pArray[i].name != NULL) /* If name is not NULL */
      free(d->pArray[i].name); /* free name */
  }
  free(d->pArray); /* free Array */
  free(d); /* free DB */
  return;
}

/* Add new customer.
   Need d(DB_T), id(const char *), name(const char *), purchase(const int).
   If there's NULL, function is failed.
   If number of item is full, expand memory by amount of UNIT_ARRAY_SIZE.
   If an item with same id or name already exists, function does not add,
  and it is failure. 
   If register successfully, return 0 */
int
RegisterCustomer(DB_T d, const char *id,
		 const char *name, const int purchase)
{
  /* fill out this function */
  /* If any of d, id or name is NULL it is a failure
     If purchase is zero or a negative number, it is a failure */
  if((d==NULL)||(id==NULL)||(name==NULL)||(purchase<=0)) return -1;
  
  /* If number of item is equal to currArrsize, expand memory */
  if (d->numItems == d->curArrSize) {
    struct UserInfo *d_backup = d->pArray; /* Backup for when reallocation failed */
    int ArrSize_backup = d->curArrSize; /* Array size backup */
    d->curArrSize += UNIT_ARRAY_SIZE; /* expand current arraysize by 1024 */
    d->pArray = (struct UserInfo *)realloc(d->pArray,d->curArrSize*sizeof(struct UserInfo));
    /* reallocate memory */
    /* If fail to reallocate memory, free all the memory and return -1 */
    if (d->pArray == NULL) {
      fprintf(stderr, "Can't reallocate a memory for array of size %d\n",
	      d->curArrSize); /* print error message */
      d->pArray = d_backup; /* assign backup */
      d->curArrSize = ArrSize_backup; /* assign array size backup */
      return -1; /* Expanding Failure */
    }
  }

  /* Add customer */
  // struct UserInfo *p; /* pointer for check id, name, purchase */
  int i=0; /* index for finding address */

  /* Check if the same id or name already exists */
  for(int i=0; i<d->curArrSize; i++){
    // p=(struct UserInfo *)(d->pArray+i*sizeof(struct UserInfo));
    if(d->pArray[i].id==NULL) continue; /* pass if id is NULL */
    if(d->pArray[i].name==NULL) continue; /* pass if name is NULL */
    if(strcmp(d->pArray[i].id,id) == 0) return -1; /* If same id already exists, fail */
    if(strcmp(d->pArray[i].name,name) == 0) return -1; /* If same name already exists, fail */
  }

  /* Add new Customer to the empty place 
     Check it is empty whether purchase is zero */
  for(i=0;i<d->curArrSize; i++) {
    //p=(struct UserInfo *)(d->pArray+i*sizeof(struct UserInfo)); /* Assigen address to pointer */
    if(d->pArray[i].name==NULL) { /* If the element is empty, */
      d->pArray[i].id=strdup(id); /* Own id */
      d->pArray[i].name=strdup(name); /* Own name */
      if(d->pArray[i].id==NULL||d->pArray[i].name==NULL) return-1; /* For Owning failed */
      d->pArray[i].purchase=purchase; /* Copy purchase */
      d->numItems++; /* Increase number of items */
      return 0;
    }
  }
  return -1; /* Register failed */
}

/* Delete memory of customer that has same id.
   Need d(DB_T), and id(const char*).
   If d or name is NULL, it is failure.
   If there's no same id, it is failure. */
int
UnregisterCustomerByID(DB_T d, const char *id)
{
  /* fill out this function */
  /* If d or id is NULL, it is failure */
  if( d == NULL || id == NULL ) return -1;

  /* make id, name and purchase become NULL or zero if the id is same */
  for(int i=0; i<d->curArrSize; i++) {
    if (d->pArray[i].id == NULL) continue; /* if current id is equal to NULL, pass this case */
    if (strcmp(d->pArray[i].id,id)==0) { /* if it has same id, */
      d->pArray[i].id = NULL; /* make id NULL */
      d->pArray[i].name = NULL; /* make name NULL */
      d->pArray[i].purchase = 0; /* make purchase 0 */
      d->numItems--; /* decrease number of items(customers) */
      return 0; /* Unregister success */
    }
  }
  
  return (-1); /* Unregister failed */
}

/* Delete memory of customer that has same name.
   Need d(DB_T), and name(const char*).
   If d or name is NULL, it is failure.
   If there's no same name, it is failure. */
int
UnregisterCustomerByName(DB_T d, const char *name)
{
  /* fill out this function */
  /* If d or name is NULL, it is failure */
  if( d == NULL || name == NULL ) return -1;

  /* make id, name and purchase become NULL or zero if the id is same */
  for(int i=0; i<d->curArrSize; i++) {
    if (d->pArray[i].name == NULL) continue; /* if current id is equal to NULL, pass this case */
    if (strcmp(d->pArray[i].name,name)==0) { /* if it has same name, */
      d->pArray[i].id = NULL; /* make id NULL */
      d->pArray[i].name = NULL; /* make name NULL */
      d->pArray[i].purchase = 0; /* make purchase 0 */
      d->numItems--; /* decrease number of items(customers) */
      return 0; /* Unregister success */
    }
  }
  
  return (-1); /* Unregister failed */
}

/* Get amount of purchase which customer's id is same.
   Need d(DB_T), and id(const char*).
   If d or id is NULL, it is failure.
   If there's no matching id, function failed.
   Return amount of purchase if there's matching id. */
int
GetPurchaseByID(DB_T d, const char* id)
{
  /* fill out this function */
  /* If d or id is NULL, it is failure */
  if( d == NULL || id == NULL ) return -1;

  /* find same id and return amount of purchase */
  for(int i=0; i < d->curArrSize; i++) {
    if (d->pArray[i].id == NULL) continue; /* if current id is equal to NULL, pass this case */
    if (strcmp(d->pArray[i].id,id)==0) /* if it has same name, */
      return d->pArray[i].purchase; /* return amount of purchase */
  }
  
  return (-1); /* If there's no matching id, function failed */
}

/* Get amount of purchase which customer's name is same.
   Need d(DB_T), and name(const char*).
   If d or id is name, it is failure.
   If there's no matching name, function failed.
   Return amount of purchase if there's matching name. */
int
GetPurchaseByName(DB_T d, const char* name)
{
  /* fill out this function */
  /* If d or name is NULL, it is failure */
  if( d == NULL || name == NULL ) return -1;

  /* find same id and return amount of purchase */
  for(int i=0; i < d->curArrSize; i++) {
    if (d->pArray[i].name == NULL) continue; /* if current name is equal to NULL, pass this case */
    if (strcmp(d->pArray[i].name,name)==0) /* if it has same name, */
      return d->pArray[i].purchase; /* return amount of purchase */
  }
  
  return (-1); /* If there's no matching id, function failed */
}

/* Sum all the value that return by fp.
   Need d(DB_T), and fp(FUNCPTR_T).
   If d or fp is NULL, it is failure.
   Return sum of all return value by fp. */
int
GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp)
{
  /* fill out this function */
  /* if d or fp is null, it is failure */
  if( d == NULL || fp == NULL ) return -1;

  /* give customer's id, name, and purchase parameters to fp,
     and get a non-negative integer value and add it to sum variable */
  int sum=0; /* store sum of all the return values of fp */

  for(int i=0; i < d->curArrSize; i++) {
    /* pass if id or name is NULL */
    if(d->pArray[i].id == NULL || d->pArray[i].name == NULL) continue;

    /* sum all the return value of fp for each case */
    sum += fp(d->pArray[i].id,d->pArray[i].name,d->pArray[i].purchase);
  }
  
  return sum; /* return result of sum of all return value of fp */
}
