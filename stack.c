/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: stack.c
 * Author  : Kyle Loudon/Dan Levin
 * Date    : Wed Feb 20 10:55:46 GMT 2013
 * Version : 0.25
 * ---
 * Description: An implementation of a generic, stack ADT. 
 * 
 * Date        Revision message
 * 2012-12-20  Created this file
 * 2013-02-19  Made some revision to the Doxygen documentation. Enhanced the description of
 *             in/out parameters - i.e. double-pointers.             
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

