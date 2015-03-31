/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: queue.c
 * Author  : Kyle Loudon/Dan Levin
 * Date    : Fri Mar 22 12:40:45 GMT 2013
 * Version : 0.51
 * ---
 * Description: An implementation of a generic, queue ADT. 
 * 
 * 2013-02-19  Made some revision to the Doxygen documentation. Enhanced the description of
 *             in/out parameters - i.e. double-pointers.             
 * 2015-03-31  This code ready for ver. 0.51
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

