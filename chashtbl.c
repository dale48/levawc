/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: chashtbl.c
 * Author  : Kyle Loudon/Dan Levin
 * Date    : Fri Mar 22 12:40:45 GMT 2013
 * Version : 0.51
 * ---
 * Description: A chained hash table - implemented as a pure, generic ADT container. 
 * 
 * Revision history - coming up below:
 *
 * Date        Revision message
 * 2013-01-30  Created this file initially
 * 2013-02-19  Made some revision to the Doxygen documentation. Enhanced the description of
 *             in/out parameters - i.e. double-pointers.             
 * 2015-03-31  This code ready for version 0.51
 *
 */

/**
 * @file chashtbl.c
 **/

#include <stdio.h>
#include <stdlib.h>

#include "chashtbl.h"


struct CHtbl_
{

  int   buckets;
  int   (*h)(const void *key);
  int   (*match)(const void *key1, const void *key2);
  void  (*destroy)(void *data);
  int   size;
  Slist *table;
};



/* FUNCTION DEFINITIONS --------------------------------------------------- */

CHtbl CHTBLinit(int buckets, int (*h)(const void *key), int (*match)(const void *key1, const void *key2), void (*destroy)(void *data))
{
  CHtbl htbl;
  int i;

  if ((htbl = (CHtbl)malloc(sizeof(struct CHtbl_)))==NULL)
    return NULL;

  if ((htbl->table = (Slist *)malloc(buckets * sizeof(Slist))) == NULL)
    return NULL;

  htbl->buckets = buckets;

  for (i = 0; i < htbl->buckets; i++)
    {
      if ((htbl->table[i] = SLISTinit(destroy)) == NULL)
        return NULL;
      SLISTsetmatch(htbl->table[i], match);
    }

  htbl->h = h;
  htbl->match = match;
  htbl->destroy = destroy;
  htbl->size = 0;

  return htbl;
}

void CHTBLdestroy(CHtbl htbl)
{
  int i;

  for (i = 0; i < htbl->buckets; ++i)
    {
      SLISTdestroy(htbl->table[i]);
    }

  free(htbl->table);
  free(htbl);
}

int CHTBLinsert(CHtbl htbl, const void *data)
{
  int bucket, retval;
  void *tmp;

  tmp = (void *)data;

  if (CHTBLlookup(htbl, &tmp) == 0)
    return 1;

  /* Hash the key */
  bucket = htbl->h(data) % htbl->buckets;

  if ((retval = SLISTinsnext(htbl->table[bucket], NULL, data)) == 0)
    htbl->size++;

  return retval;
}

int CHTBLremove(CHtbl htbl, void **data)
{
  int bucket, retval;

  /* Hash the key */
  bucket = htbl->h(*data) % htbl->buckets;

  /* Remove the node */
  if ((retval = SLISTfind_remove(htbl->table[bucket], data)) == 0) /* Node removal successful.. */
    htbl->size--;

  return retval;
}

int CHTBLlookup(const CHtbl htbl, void **data)
{
  int bucket;
  SlistNode tmpnode;

  /* Hash the key */
  bucket = htbl->h(*data) % htbl->buckets;

  if ((tmpnode = SLISTfindnode(htbl->table[bucket], *data))) /* Data found */
    {
      *data = SLISTdata(tmpnode); /* Pass data back to caller */
      return 0;
    }

  return -1;
}

void CHTBLprint(CHtbl htbl, void (*callback)(const void *data))
{
  int i;

  for (i = 0; i < htbl->buckets; ++i)
    {
      printf("\nBucket #%03d: ", i);
      SLISTtraverse(htbl->table[i], callback, SLIST_FWD);
    }
}

int CHTBLsize(CHtbl htbl)
{
  return htbl->size;
}
