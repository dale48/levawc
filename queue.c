/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: queue.c
 * Author  : Dan Levin
 * Date    : Thu Dec 20 09:11:31 2012
 * Version : 0.1 
 * ---
 * Description: An implementation of a generic, queue ADT. 
 * 
 */

/**
 * @file queue.c
 * 
 **/

#include <stdio.h>
#include <stdlib.h>

#include "queue.h"


/* FUNCTION DEFINITIONS --------------------------------------------------- */

Queue QUEUEinit(void (*destroy)(void *data))
{
  return SLISTinit(destroy);
}

void QUEUEdestroy(Queue que)
{
  SLISTdestroy(que);
}

int QUEUEenqueue(Queue que, const void *data)
{
  return SLISTinsnext(que, SLISTtail(que), data);
}

int QUEUEdequeue(Queue que, void **data)
{
  return SLISTremnext(que, NULL, data);
}

void *QUEUEpeek(Queue que)
{
  return SLISTsize(que) == 0 ? NULL : SLISTdata(SLISThead(que));
}

int QUEUEisempty(Queue que)
{
  return SLISTsize(que) == 0 ? 1 : 0;
}

int QUEUEsize(Queue que)
{
  return SLISTsize(que);
}

