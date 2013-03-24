/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: pqueue.c
 * Author  : Kyle Loudon/Dan Levin
 * Date    : Fri Mar 22 12:40:45 GMT 2013
 * Version : 0.40
 * ---
 * Description: A priority queue ADT - in ANSI C.  
 * 
 * Date        Revision message
 * 2012-12-20  Created this file
 *
 */

/**
 * @file pqueue.c
 **/

#include <stdio.h>
#include <stdlib.h>

#include "pqueue.h"

/* --- PUBLIC FUNCTION DEFINITIONS --- */
/* --- Function: PQueue PQUEUEinit(int (*compare)(const void *key1, const void* key2), void (*destroy)(void *data)) --- */
PQueue PQUEUEinit(int (*compare)(const void *key1, const void* key2), void (*destroy)(void *data))
{
  return HEAPinit(compare, destroy);  
}

/* --- Function: void PQUEUEdestroy(PQueue pq) --- */
void PQUEUEdestroy(PQueue pq)
{
  HEAPdestroy(pq);
}

/* --- Function:   int PQUEUEinsert(PQueue pq, const void *data) --- */
int PQUEUEinsert(PQueue pq, const void *data)
{
  return HEAPinsert(pq, data);
}

/* --- Function:   int PQUEUEextract(PQueue pq, void **data) --- */
int PQUEUEextract(PQueue pq, void **data)
{
  return HEAPextract(pq, data);
}

/* --- Function: void *PQUEUEpeek(const PQueue pq) --- */
const void *PQUEUEpeek(const PQueue pq)
{
  return HEAPpeek(pq);
}

/* --- Function: int PQUEUEsize(PQueue pq) --- */
int PQUEUEsize(PQueue pq)
{
  return HEAPsize(pq);
}

/* --- Function: void PQUEUEprint(PQueue pq, void (*callback)(const void *data)) --- */
void PQUEUEprint(PQueue pq, void (*callback)(const void *data))
{
  HEAPprint(pq, callback);
}
