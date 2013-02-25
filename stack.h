/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: stack.h
 * Author  : Kyle Loudon/Dan Levin
 * Date    : Mon Feb 25 16:52:00 GMT 2013
 * Version : 0.30
 * ---
 * Description: A C interface for a generic stack ADT.
 * 
 * Revision history - coming up below:
 *
 * Date        Revision message
 * 2012-12-20  Created this file
 * 2013-02-19  Made some revision to the Doxygen documentation. Enhanced the description of
 *             in/out parameters - i.e. double-pointers.             
 *
 */

/**
 * @file stack.h
 **/

#ifndef _STACK_H_
#define _STACK_H_

#include <stdio.h>
#include <stdlib.h>

#include "slist.h"

#ifdef __cplusplus
extern "C" {
#endif

  typedef Slist Stack;

  /* INTERFACE FUNCTION DECLARATIONS */
  /**
   * 
   * Initiate the stack. 
   *
   * @param[in] destroy - A reference to a user-made function, reponsible
   * for freeing element data, when the stack is deleted. If @a destroy is 
   * NULL - then element data will be left untouched when the list is 
   * destroyed.
   * @return A reference - to a new, empty stack - if dynamic memory
   * allocation for the ADT was successful - or NULL otherwise. Take really
   * good care of this return value, since it will be needed as a parameter in 
   * subsequent calls - to the majority of other stack handling functions 
   * in this stack function interface - i.e. a sort of "handle" to the stack.
   * @see STACKdestroy()
   **/
  Stack STACKinit(void (*destroy)(void *data));

  /**
   * Destroy the stack. 
   * 
   * The stack is destroyed - that is, all memory occupied by the 
   * elements is deallocated. The user-defined callback function 
   * @a destroy, given as an argument to @b STACKinit(), is responsible
   * for freeing dynamically allocated element data, when this function
   * is called. When all elements and data have been deallocated - the
   * stack header is deallocated, too.
   * 
   * @param[in] stk - a reference to current stack.
   * @return Nothing.
   * @see STACKinit()
   **/
  void STACKdestroy(Stack stk);

  /**
   * Insert(=push) a new element - at the top of the stack.
   *
   * This function inserts an new element - with a reference to 
   * its corresponding data, given by parameter @a data - at the 
   * top of the stack.
   * 
   * @param[in] stk - reference to current stack
   * @param[in] data - reference to data to be stored in the new
   * element, which is to be inserted at the top of the stack.
   *
   * @return Value 0 - if everything went OK - or value -1 otherwise.
   **/
  int STACKpush(Stack stk, const void *data);

  /**
   * Remove(=pop) the top element.
   *
   * When called, the 2nd parameter of this function, @a data,
   * should reference an (external, user-defined) pointer.
   * After the call - this referenced, external pointer has been
   * redirected, to point to the data of the removed element - 
   * if the call was succesful. The caller is responsible for
   * the future  of this memory - deallocating it, if needed,
   * for example.
   *
   * @param[in] stk - reference to current stack.
   * @param[out] data - reference to a pointer. After the call, 
   * this referenced pointer has been redirected to point to
   * the data of the removed element - if the call was
   * successful. The caller is responsible for the future  of
   * this memory - deallocating it, for example.
   * 
   * @return Value 0 - if the call was OK - or 
   * value -1 otherwise.
   **/
  int STACKpop(Stack stk, void **data);

  /**
   * Peek at the top of the stack.
   *
   * @param[in] stk - reference to the current stack.
   *
   * @return NULL if the stack is empty - or a reference
   * to data of the top element, otherwise.
   *
   **/
  void *STACKpeek(Stack stk);

  /**
   * Determine if the stack is empty - or not. 
   * 
   * @param[in] stk - a reference to the current stack.
   * @return Value 1 - if the stack is indeed empty - 
   * or 0 otherwise.
   **/
  int STACKisempty(Stack stk);

  /**
   * Get the stack size.
   * 
   * @param[in] stk - a reference to the current stack.
   *
   * @return The size, that is, the number of elements in the stack.
   **/
  int STACKsize(Stack stk);

#ifdef __cplusplus
}
#endif 

#endif /* _STACK_H_ */

