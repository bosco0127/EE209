#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"

enum {HASH_MULTIPLIER = 65599};

/* Define hash_function.
   Need pcKey(const char *), and iBucketCount(int).
   Return corressponding hash value. */
static int hash_function(const char *pcKey, int iBucketCount) {
  int i;
  unsigned int uiHash = 0U;
  for(i=0; pcKey[i] != '\0'; i++)
    uiHash = uiHash*(unsigned int)HASH_MULTIPLIER+(unsigned int)pcKey[i];
  return (int)(uiHash % (unsigned int)iBucketCount);
}

/*----------------------------------------------------------------------
Uncomment and use the following code if you want
----------------------------------------------------------------------*/
#define UNIT_ARRAY_SIZE 1024 /* Define unit array size */

struct UserInfo {
  char *name;                // customer name
  char *id;                  // customer id
  int purchase;              // purchase amount (> 0)
  struct UserInfo *IDnext;   // ID connection pointer
  struct UserInfo *namenext; // name connection pointer
};

struct DB {
  struct UserInfo **IDarray;   // ID pointer of pointer for dynamically resizeable array.
  struct UserInfo **namearray; // name pointer of pointer for dynamically resizeable array.
  int curArrSize;            // current hash table size
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
  d->curArrSize = UNIT_ARRAY_SIZE;

  /* Allocate memory for both ID and name array */
  d->IDarray = (struct UserInfo **)calloc(d->curArrSize, sizeof(struct UserInfo *));
  if (d->IDarray == NULL) {
    fprintf(stderr, "Can't allocate a memory for array of size %d\n", d->curArrSize);   
    free(d);
    return NULL;
  }
  d->namearray = (struct UserInfo **)calloc(d->curArrSize, sizeof(struct UserInfo *));
  if (d->namearray == NULL) {
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

  /* This case, we will free by ID hash table */

  /* Define UserInfo type pointers */
  struct UserInfo *p; /* Variable for free the information */
  struct UserInfo *nextp; /* Variable for storing next address */
  int i=0; /* index for array */

  /* Free data at ID hash table */
  for (i=0; i < d->curArrSize; i++) {
    for (p = d->IDarray[i];p != NULL; p = nextp) {
      nextp = p->IDnext; /* store next address connected by ID */
      free(p->id); /* free id */
      free(p->name); /* free name */
      free(p); /* free UserInfo */
    }
  }
  free(d->IDarray); /* free ID hash table array */
  free(d->namearray); /* free name hash table array */
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

  /* Below part is for expanding array size of hash table.
     I implement expanding array size of both ID and name hash table,
    but failed to implement reassign a new appropriate hash value to each
    already registered item.
     Therefore, below part is commented. */
  /*-----------------------------------------------------------------------*/
  /* If number of item is 75% of current array size, expand memory
    until curArrSize <= 2^19(524288). Then, expansion would be stop at 2^20 */
  if ((d->numItems == 0.75*d->curArrSize) && (d->curArrSize <= 524288)) {
    
    /* Set new array for reallocation failed */
    struct UserInfo **d_id_new; /* new hash table array for id  */
    struct UserInfo **d_name_new; /* new hash table array for name */
    int ArrSize_backup = d->curArrSize; /* Array size backup */
    
    /* expand current array size multiply by 2 */
    d->curArrSize *= 2;
    
    /* callocate memory to new ID hash table array */
    d_id_new = (struct UserInfo **)calloc(d->curArrSize,sizeof(struct UserInfo *));
    /* If fail to reallocate memory, free all the memory and return -1 */
    if (d_id_new == NULL) {
      fprintf(stderr, "Can't reallocate a memory for array of size %d\n",
  	      d->curArrSize); /* print error message */
      d->curArrSize = ArrSize_backup; /* assign array size backup */
      return -1; /* Expanding Failure */
    }
    
    /* callocate memory to new name hash table array */
    d_name_new = (struct UserInfo **)calloc(d->curArrSize,sizeof(struct UserInfo *));
    /* If fail to reallocate memory, free all the memory and return -1 */
    if (d_name_new == NULL) {
      fprintf(stderr, "Can't reallocate a memory for array of size %d\n",
  	      d->curArrSize); /* print error message */
      d->curArrSize = ArrSize_backup; /* assign array size backup */
      return -1; /* Expanding Failure */
    }

    /* move already existing items to new array, and remove old array */
    /* Define UserInfo type pointers */
    struct UserInfo *p; /* Variable for free the information */
    struct UserInfo *nextp; /* Variable for storing next address */
    int IDhash=0; /* variable for id hash value*/
    int namehash=0; /* variable for name hash value */
    int i=0; /* index for array */
    
    /* move data from old hash table to new hash table */
    /* Also remove old memory as soon as transporting is completed */
    /* Search already existing items by ID hash table */
    for (i=0; i < ArrSize_backup; i++) {
      for (p = d->IDarray[i];p != NULL; p = nextp) {
	/* store next address connected by ID */
	nextp = p->IDnext;

	/* Find new hash value */
	IDhash = hash_function(p->id,d->curArrSize); /* new id hash value */
	namehash = hash_function(p->name,d->curArrSize); /* new name hash value */

	/* move old items to new hash table */
	/* allocate memory for copy customer */
	struct UserInfo *copy = (struct UserInfo*)malloc(sizeof(struct UserInfo)); /* pointer for new customer */      
	copy->id = strdup(p->id); /* Own ID */
	copy->name = strdup(p->name); /* Own name */
	if(copy->id==NULL||copy->name==NULL) return -1; /* In case of owning id or name is failed */
	copy->purchase = p->purchase; /* Store purchase value */
	
	/* Connecting old item to the both new ID and name hash table */
	copy->IDnext = d_id_new[IDhash]; /* connect new UserInfo to the ID hash table */
	d_id_new[IDhash] = copy; /* connect new UserInfo to the ID hash table */
	copy->namenext = d_name_new[namehash]; /* connect new UserInfo to the name hash table */
	d_name_new[namehash] = copy; /* connect new UserInfo to the name hash table */
	
	/* remove old memory */
	free(p->id); /* free id */
	free(p->name); /* free name */
	free(p); /* free UserInfo */
      }
    }

    /* free old hash table */
    free(d->IDarray); /* free ID hash table array */
    free(d->namearray); /* free name hash table array */

    /* Assign new hash table to d */
    d->IDarray = d_id_new; /* assign new id hash table to d */
    d->namearray = d_name_new; /* assign new name hash table to d */
  }
  /*-----------------------------------------------------------------------*/
  
  /* Search whether id or name is already exist */
  struct UserInfo *q; /* Pointer for searching */
  int IDhash = hash_function(id,d->curArrSize); /* hash value for ID */
  int namehash = hash_function(name,d->curArrSize); /* hash value for name */

  /* Search whether id is already exist */
  for(q = d->IDarray[IDhash]; q != NULL; q=q->IDnext)
    if ((strcmp(q->id,id)) == 0) return -1; /* If same id already exist, it is failed */

  /* Search whether name is already exist */
  for(q = d->namearray[namehash]; q != NULL; q=q->IDnext)
    if ((strcmp(q->name,name)) == 0) return -1; /* If same name already exist, it is failed */

  /* Add customer */
  struct UserInfo *p = (struct UserInfo*)malloc(sizeof(struct UserInfo)); /* pointer for new customer */      
  p->id = strdup(id); /* Own ID */
  p->name = strdup(name); /* Own name */
  if(p->id==NULL||p->name==NULL) return -1; /* In case of owning id or name is failed */
  p->purchase = purchase; /* Store purchase value */

  /* Connecting new item to the both ID and name hash table */
  p->IDnext = d->IDarray[IDhash]; /* connect new UserInfo to the ID hash table */
  d->IDarray[IDhash] = p; /* connect new UserInfo to the ID hash table */
  p->namenext = d->namearray[namehash]; /* connect new UserInfo to the name hash table */
  d->namearray[namehash] = p; /* connect new UserInfo to the name hash table */
  d->numItems++; /* increase number of items */
  
  return 0; /* Register success */
}

/* Delete memory of customer that has same id.
   Need d(DB_T), and id(const char*).
   If d or name is NULL, it is failure.
   If there's no same id, it is failure. */
int
UnregisterCustomerByID(DB_T d, const char *id)
{
  /* fill out this function */
  /* If any of d, id is NULL, it is a failure */
  if((d==NULL)||(id==NULL)) return -1;
  
  struct UserInfo *p_id, *q_id; /* Pointer for free unregister customer & connect id link */
  struct UserInfo *p_name, *q_name; /* Pointer for connect name link */
  int IDhash = hash_function(id,d->curArrSize); /* hash value for ID */

  /* Delete when it is at first node of ID array */
  if(d->IDarray[IDhash] && strcmp(d->IDarray[IDhash]->id,id)==0) { /* If it's at first, */
    q_id = d->IDarray[IDhash]; /* assign first node address to q */
    d->IDarray[IDhash] = d->IDarray[IDhash]->IDnext; /* connect next address to the first node */
    
    int namehash = hash_function(q_id->name,d->curArrSize); /* find hash value for name */
    /* Delete when it is at first node of name array */
    if(d->namearray[namehash] && strcmp(d->namearray[namehash]->name,q_id->name)==0) { /* If it's at first, */
      q_name = d->namearray[namehash]; /* assign first node address to q */
      d->namearray[namehash] = d->namearray[namehash]->namenext; /* connect next address to the first node */
      
      free(q_name->name); /* free owned name */
      free(q_id->id); /* free owned id */
      free(q_id); /* free Userinfo */
      d->numItems--; /* decrease number of items(customers) */
      return 0; /* Unregister successed */
    }

    /* Delete when it is not at first node of name array */
    for (p_name = d->namearray[namehash]; p_name != NULL; p_name = p_name->namenext) {
      if(p_name->namenext && strcmp(p_name->namenext->name, q_id->name) == 0) { /* if next name is matched, */
	q_name = p_name->namenext; /* Store next id address */
	p_name->namenext = q_name->namenext; /* connect next to the next next element */
	
	free(q_name->name); /* free owned id */
	free(q_id->id); /* free owned id */
	free(q_id); /* free Userinfo */
	d->numItems--; /* decrease number of items(customers) */
	return 0; /* Unregister successed */
      }
    }
  }

  /* Delete when it is not at first node of ID array */
  for (p_id = d->IDarray[IDhash]; p_id != NULL; p_id = p_id->IDnext) {
    if(p_id->IDnext && strcmp(p_id->IDnext->id, id) == 0) { /* if next id is matched, */
      q_id = p_id->IDnext; /* Store next id address */
      p_id->IDnext = q_id->IDnext; /* connect next to the next next element */

      int namehash = hash_function(q_id->name,d->curArrSize); /* find hash value for name */
      /* Delete when it is at first node of name array */
      if(d->namearray[namehash] && strcmp(d->namearray[namehash]->name,q_id->name)==0) { /* If it's at first, */
	q_name = d->namearray[namehash]; /* assign first node address to q */
	d->namearray[namehash] = d->namearray[namehash]->namenext; /* connect next address to the first node */
	
	free(q_name->name); /* free owned name */
	free(q_id->id); /* free owned id */
	free(q_id); /* free Userinfo */
	d->numItems--; /* decrease number of items(customers) */
	return 0;
      }

      /* Delete when it is not at first node of name array */
      for (p_name = d->namearray[namehash]; p_name != NULL; p_name = p_name->namenext) {
	if(p_name->namenext && strcmp(p_name->namenext->name, q_id->name) == 0) { /* if next name is matched, */
	  q_name = p_name->namenext; /* Store next id address */
	  p_name->namenext = q_name->namenext; /* connect next to the next next element */
	  
	  free(q_name->name); /* free owned id */
	  free(q_id->id); /* free owned id */
	  free(q_id); /* free Userinfo */
	  d->numItems--; /* decrease number of items(customers) */
	  return 0;
	}
      }
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
  /* If any of d, name is NULL, it is a failure */
  if((d==NULL)||(name==NULL)) return -1;
  
  struct UserInfo *p_name, *q_name; /* Pointer for free unregister customer & connect name link */
  struct UserInfo *p_id, *q_id; /* Pointer for connect id link */
  int namehash = hash_function(name,d->curArrSize); /* hash value for name */

  /* Delete when it is at first node of name array */
  if(d->namearray[namehash] && strcmp(d->namearray[namehash]->name,name)==0) { /* If it's at first, */
    q_name = d->namearray[namehash]; /* assign first node address to q */
    d->namearray[namehash] = d->namearray[namehash]->namenext; /* connect next address to the first node */
    
    int IDhash = hash_function(q_name->id,d->curArrSize); /* find hash value for id */
    /* Delete when it is at first node of name array */
    if(d->IDarray[IDhash] && strcmp(d->IDarray[IDhash]->id,q_name->id)==0) { /* If it's at first, */
      q_id = d->IDarray[IDhash]; /* assign first node address to q */
      d->IDarray[IDhash] = d->IDarray[IDhash]->IDnext; /* connect next address to the first node */
      
      free(q_id->id); /* free owned id */
      free(q_name->name); /* free owned name */
      free(q_name); /* free Userinfo */
      d->numItems--; /* decrease number of items(customers) */
      return 0; /* Unregister successed */
    }

    /* Delete when it is not at first node of name array */
    for (p_id = d->IDarray[IDhash]; p_id != NULL; p_id = p_id->IDnext) {
      if(p_id->IDnext && strcmp(p_id->IDnext->id, q_name->id) == 0) { /* if next id is matched, */
	q_id = p_id->IDnext; /* Store next id address */
	p_id->IDnext = q_id->IDnext; /* connect next to the next next element */
	
	free(q_id->id); /* free owned id */
	free(q_name->name); /* free owned name */
	free(q_name); /* free Userinfo */
	d->numItems--; /* decrease number of items(customers) */
	return 0; /* Unregister successed */
      }
    }
  }

  /* Delete when it is not at first node of name array */
  for (p_name = d->namearray[namehash]; p_name != NULL; p_name = p_name->namenext) {
    if(p_name->namenext && strcmp(p_name->namenext->name, name) == 0) { /* if next name is matched, */
      q_name = p_name->namenext; /* Store next id address */
      p_name->namenext = q_name->namenext; /* connect next to the next next element */

      int IDhash = hash_function(q_name->id,d->curArrSize); /* find hash value for id */
      /* Delete when it is at first node of name array */
      if(d->IDarray[IDhash] && strcmp(d->IDarray[IDhash]->id,q_name->id)==0) { /* If it's at first, */
	q_id = d->IDarray[IDhash]; /* assign first node address to q */
	d->IDarray[IDhash] = d->IDarray[IDhash]->IDnext; /* connect next address to the first node */
	
	free(q_id->id); /* free owned id */
	free(q_name->name); /* free owned name */
	free(q_name); /* free Userinfo */
	d->numItems--; /* decrease number of items(customers) */
	return 0; /* Unregister successed */
      }

      /* Delete when it is not at first node of name array */
      for (p_id = d->IDarray[IDhash]; p_id != NULL; p_id = p_id->IDnext) {
	if(p_id->IDnext && strcmp(p_id->IDnext->id, q_name->id) == 0) { /* if next id is matched, */
	  q_id = p_id->IDnext; /* Store next id address */
	  p_id->IDnext = q_id->IDnext; /* connect next to the next next element */
	  
	  free(q_id->id); /* free owned id */
	  free(q_name->name); /* free owned name */
	  free(q_name); /* free Userinfo */
	  d->numItems--; /* decrease number of items(customers) */
	  return 0; /* Unregister successed */
	}
      }
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

  /* Set variable to find customer */
  struct UserInfo *p; /* Pointer for searching */
  int IDhash = hash_function(id,d->curArrSize); /* hash value for ID */

  /* Search item that has id, and return its amount of purchase */
  for(p = d->IDarray[IDhash]; p != NULL; p=p->IDnext)
    if ((strcmp(p->id,id)) == 0) /* If find item that has same id, */
      return p->purchase; /* return amount of purchase */
  
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

  /* Set variable to find customer */
  struct UserInfo *p; /* Pointer for searching */
  int namehash = hash_function(name,d->curArrSize); /* hash value for name */

  /* Search item that has name, and return its amount of purchase */
  for(p = d->namearray[namehash]; p != NULL; p=p->namenext)
    if ((strcmp(p->name,name)) == 0) /* If find item that has same name, */
      return p->purchase; /* return amount of purchase */
  
  return (-1); /* If there's no matching name, function failed */
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
  struct UserInfo *p; /* Pointer for searching */
  int sum=0; /* store sum of all the return values of fp */

  /* Search data at ID hash table */
  for (int i=0; i < d->curArrSize; i++) {
    for (p = d->IDarray[i];p != NULL; p = p->IDnext) {
      sum += fp(p->id,p->name,p->purchase);
    }
  }
  
  return sum; /* return result of sum of all return value of fp */
}
