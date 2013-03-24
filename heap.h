/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: heap.h
 * Author  : Kyle Loudon/Dan Levin
 * Date    : Fri Mar 22 12:40:45 GMT 2013
 * Version : 0.40
 * ---
 * Description: A heap ADT - written in ANSI C.
 * 
 * Revision history - coming up below:
 *
 * Date        Revision message
 * 2013-02-17  Created this file
 *
 */
/**
 * @file heap.h
 **/

#ifndef _HEAP_H_
#define _HEAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * Use a @b typedef - to hide the interior of @b Heap_ - in the 
   * implementation file. This is how @a data @a hiding can be done in C.
   * 
   **/
  typedef struct Heap_ *Heap;

  /* FUNCTION DECLARATIONS */

  /**
   * Initiate the heap
   * 
   * @param[in] compare - a reference to a user-defined function,
   * used by various heap operations to compare elements when
   * operating on the heap. This function should return +1 - if
   * @a key1 @a > @a key2 - 0 if the @a keys are @a equal - and -1 
   * if @a key1 @a < @a key2. This goes for a @a top-heavy heap.
   * If you want a @a bottom-heavy heap, you should swap the
   * the conditions for returning 1 and -1 instead.
   * @param[in] destroy - a reference to a user-defined function,
   * reponsible for freeing @a element @a data, when the heap is 
   * destroyed. If @a destroy is set to NULL - then element data will 
   * be left untouched upon heap destruction.
   * @return A reference - to a new, empty heap - if dynamic memory
   * allocation for the ADT was successful - or NULL otherwise. Take 
   * really good care of this return value, since it will be needed
   * as a parameter in subsequent calls - to the majority of other
   * heap functions in this function interface - i.e. a sort
   * of "handle" to the heap.
   * 
   **/
  Heap HEAPinit(int (*compare)(const void *key1, const void* key2), void (*destroy)(void *data));

  /**
   * Destroy the heap
   * 
   * The heap is destroyed - that is, all dynamically allocated 
   * memory occupied by the elements - will be destroyed.
   * It is the user-defined callback function @a destroy, given 
   * as an argument to @b HEAPinit(), that is responsible for 
   * freeing dynamically allocated element data, when this 
   * function is called. If, on the other hand, @a destroy is 
   * set to NULL when @b HEAPinit() is called, all element data 
   * will be left untouched after the heap is dismounted and 
   * destroyed. When all elements and data have been 
   * deallocated - the rest of the heap is freed, too.
   * 
   * @param[in] hp - a reference to current heap.
   * @return Nothing.
   * @see HEAPinit()
   **/
  void HEAPdestroy(Heap hp);
  
  /**
   * Insert data into the heap
   * 
   * Inserts an element into the current heap - referenced by
   * the parameter @a hp. The data to be inserted, is referenced by 
   * parameter @a data. It is the responsability of the caller to 
   * ensure,  that this memory is valid as long as it is present in the 
   * heap.
   *
   * @param[in] hp - a reference to current heap.
   * @param[in] data - a reference to data to be inserted into
   * the heap.
   * @return Value 0 - if insertion was succesful\n
   *         Value -1 - otherwise.
   **/
  int HEAPinsert(Heap hp, const void *data);

  /**
   * Inspect the top-priority element of the heap
   * 
   * @param[in] hp - a reference to the current heap.
   * 
   * @return A reference to the top-priority element
   * of the current heap.
   *
   **/
  const void *HEAPpeek(Heap hp);
  
  /**
   * Remove data from the top of the heap
   * 
   * When called, the 2nd parameter of this function, @a data,
   * should reference an (external, user-defined) pointer.
   * After the call - this referenced, external pointer has been
   * redirected by this function, to point to the data of the 
   * removed element - if the call was succesful. The caller is 
   * responsible for the future  of this memory - deallocating it,
   * if needed, for example.
   *
   * @param[in] hp - reference to current heap.
   * @param[out] data - reference to a pointer. After the call, 
   * this referenced pointer has been redirected to point to
   * the data of the removed element - if the call was
   * successful. The caller is responsible for the future of
   * this memory - deallocating it, for example.
   * 
   * @return Value 0 --  if the call was OK - that is, element
   * removed.\n
   *         Value -1 -- otherwise.
   **/
  int HEAPextract(Heap hp, void **data);

  /**
   * Get the size of the heap
   * 
   * @param[in] hp - a reference to the current heap.
   *
   * @return The size, that is, the number of elements 
   * in the heap.
   **/
  int HEAPsize(Heap hp);
  
  /**
   * Print heap data on screen
   * 
   * @param[in] hp - reference to current heap.
   * @param[in] callback - reference to user-defined callback function, 
   * that gets @b read @b access to element data via its parameter 
   * @a data - to do whatever is relevant. In this case it is a matter
   * of formatting data for printing on screen. The printed data 
   * should be kept to a minimum (the key value, for example) in order 
   * not to clutter the screen. This function is primarily for small
   * heaps - and debugging purposes.
   *
   * @return Nothing.
   **/
  void HEAPprint(Heap hp, void (*callback)(const void *data));


#ifdef __cplusplus
}
#endif 

#endif /* _HEAP_H_ */
