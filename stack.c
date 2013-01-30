/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: stack.c
 * Author  : Dan Levin
 * Date    : Thu Dec 20 09:11:31 2012
 * Version : 0.1 
 * ---
 * Description: An implementation of a generic, stack ADT. 
 * 
 */

/**
 * @file stack.c
 * 
 **/

#include <stdio.h>
#include <stdlib.h>

#include "stack.h"


/* FUNCTION DEFINITIONS --------------------------------------------------- */

Stack STACKinit(void (*destroy)(void *data))
{
  return SLISTinit(destroy);
}

void STACKdestroy(Stack stk)
{
  SLISTdestroy(stk);
}

int STACKpush(Stack stk, const void *data)
{
  return SLISTinsnext(stk, NULL, data);
}

int STACKpop(Stack stk, void **data)
{
  return SLISTremnext(stk, NULL, data);
}

void *STACKpeek(Stack stk)
{
  return SLISTsize(stk) == 0 ? NULL : SLISTdata(SLISThead(stk));
}

int STACKisempty(Stack stk)
{
  return SLISTsize(stk) == 0 ? 1 : 0;
}

int STACKsize(Stack stk)
{
  return SLISTsize(stk);
}

