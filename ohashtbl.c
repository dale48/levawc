/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: ohashtbl.c
 * Author  : Kyle Loudon/Dan Levin
 * Date    : Mon Apr 08 12:15:39 2013
 * Version : 0.1 
 * ---
 * Description: An open-addressed hashtable implemented as a pure, generic ADT - written in ANSI C
 * 
 */
/**
 * @file ohashtbl.c
 **/

#include <stdio.h>
#include <stdlib.h>

#include "ohashtbl.h"

struct OHtbl_ 
{
  int       positions;
  void      *vacated;

  int       (*h1)(const void *key);
  int       (*h2)(const void *key);
  int       (*match)(const void *key1, const void *key2);
  void      (*destroy)(void *data);

  int       size;
  void      **table;
};

/* Reserve a sentinel memory address for vacated elements */
static char vacated;

/* FUNCTION DEFINITIONS */

OHtbl OHTBLinit(int positions, int (*h1)(const void *key), int (*h2)(const void *key),
                int (*match)(const void *key1, const void *key2), void (*destroy)(void *data))
{
  OHtbl htbl;
  int i;

  /* Allocate space for the open-addressed hash table header */
  if ((htbl = malloc(sizeof(struct OHtbl_))) == NULL)
    return NULL;

  /* Allocate space for the hash table */
  if ((htbl->table = (void **)malloc(positions * sizeof(void *))) == NULL)
    return NULL;

  /* Initialize each position */
  htbl->positions = positions;

  for (i = 0; i < htbl->positions; i++)
    htbl->table[i] = NULL;

  /* Set the vacated member to the sentinel memory address reserved for this */
  htbl->vacated = &vacated;

  /* Encapsulate the functions */
  htbl->h1 = h1;
  htbl->h2 = h2;
  htbl->match = match;
  htbl->destroy = destroy;

  /* Initialize the number of elements in the table */
  htbl->size = 0;

  return htbl;
}

void OHTBLdestroy(OHtbl htbl)
{
  int i;
 
  if (htbl->destroy != NULL) 
    {
      /* Call a user-defined function to free dynamically allocated data */
      for (i = 0; i < htbl->positions; i++)
        {
          if (htbl->table[i] != NULL && htbl->table[i] != htbl->vacated)
            htbl->destroy(htbl->table[i]);
        }
    }

  /* Free the storage allocated for the hash table */
  free(htbl->table);

  /* Free the storage allocated for the table header */
  free(htbl);
}

int OHTBLinsert(OHtbl htbl, const void *data)
{
  void *temp;
  int position, i;
 
  /* Do not exceed the number of positions in the table */
  if (htbl->size == htbl->positions)
    return -1;

  /* Do nothing if the data is already in the table */
  temp = (void *)data;

  if (OHTBLlookup(htbl, &temp) == 0) /* Duplicate found! */
    return 1;

  /* Use double hashing to hash the key */
  for (i = 0; i < htbl->positions; i++)
    {
      position = (htbl->h1(data) + (i * htbl->h2(data))) % htbl->positions;

      if (htbl->table[position] == NULL || htbl->table[position] == htbl->vacated)
        {
          /* Insert the data into the table */
          htbl->table[position] = (void *)data;
          htbl->size++;
          return 0;
        }
    }

  /* Return that the hash functions were selected incorrectly */
  return -1;
}

int OHTBLremove(OHtbl htbl, void **data)
{
  int position, i;
 
  /* Use double hashing to hash the key */
  for (i = 0; i < htbl->positions; i++)
    {
      position = (htbl->h1(*data) + (i * htbl->h2(*data))) % htbl->positions;

      if (htbl->table[position] == NULL) 
        {
          /* Return that the data was not found */
          return -1;
        }
      else if (htbl->table[position] == htbl->vacated) 
        {
          /* Search beyond vacated positions */
          continue;
        }
      else if (htbl->match(htbl->table[position], *data))
        {
          /* Pass back the data from the table */
          *data = htbl->table[position];
          htbl->table[position] = htbl->vacated;
          htbl->size--;
          return 0;
        }
    }

  /* Return that the data was not found */
  return -1;
}

int OHTBLlookup(const OHtbl htbl, void **data)
{
  int position, i;
 
  /* Use double hashing to hash the key */
  for (i = 0; i < htbl->positions; i++) 
    {
      position = (htbl->h1(*data) + (i * htbl->h2(*data))) % htbl->positions;

      if (htbl->table[position] == NULL)
        {
          /* Return that the data was not found */
          return -1;
        }
      else if (htbl->match(htbl->table[position], *data)) 
        {
          /* Pass back the data from the table */
          *data = htbl->table[position];
          return 0;
        }
    }

  /* Return that the data was not found... */
  return -1;
}

int OHTBLsize(OHtbl htbl)
{
  return htbl->size;
}

void OHTBLprint(OHtbl htbl, void (*callback)(const void *data))
{
  int i;

  for (i=0; i<htbl->positions; i++)
    {
      if (htbl->table[i] == NULL)
	printf("\nNULL");
      else if (htbl->table[i] == htbl->vacated)
	printf("\nVACATED");
      else
	callback(htbl->table[i]);
    }
}
