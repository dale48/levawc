/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: pqueue.h
 * Author  : Kyle Loudon/Dan Levin
 * Date    : Mon Feb 25 16:52:00 GMT 2013
 * Version : 0.30
 * ---
 * Description: A priority queue ADT - in ANSI C. 
 * 
 * Revision history - coming up below:
 *
 * Date        Revision message
 * 2012-12-20  Created this file
 *
 */

/**
 * @file pqueue.h
 **/

#ifndef _PQUEUE_H_
#define _PQUEUE_H_

#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

#ifdef __cplusplus
extern "C" {
#endif

  typedef Heap PQueue;

  /* --- FUNCTION DECLARATIONS --- */
  /**
   * Initiate the priority queue
   * 
   * @param[in] compare - a reference to a user-defined function,
   * used by various priority queue operations to compare 
   * elements when operating on the priority queue. This 
   * function should return +1 - if @a key1 @a > @a key2 - 0 if
   * the @a keys are @a equal - and -1 if @a key1 @a < @a key2.
   * This goes for a @a top-heavy priority queue.
   * If you want a @a bottom-heavy priority queue, you should swap
   * the the conditions for returning 1 and -1 instead.
   * @param[in] destroy - a reference to a user-defined function,
   * reponsible for freeing @a element @a data, when the priority
   * queue is destroyed. If @a destroy is set to NULL - then
   * element data will be left untouched upon priority queue
   * destruction.
   * @return A reference - to a new, empty priority queue - if 
   * dynamic memory allocation for the ADT was successful - or
   * NULL otherwise. Take really good care of this return value,
   * since it will be needed as a parameter in subsequent calls -
   * to the majority of other priority queue functions in
   * this function interface - i.e. a sort of "handle" to the
   * priority queue.
   **/
  PQueue PQUEUEinit(int (*compare)(const void *key1, const void* key2), void (*destroy)(void *data));

  /**
   * Destroy the priority queue
   * 
   * The priority queue is destroyed - that is, all dynamically 
   * allocated memory occupied by the elements - will be destroyed.
   * It is the user-defined callback function @a destroy, given 
   * as an argument to @b PQUEUEinit(), that is responsible for 
   * freeing dynamically allocated element data, when this 
   * function is called. If, on the other hand, @a destroy is 
   * set to NULL when @b PQUEUEinit() is called, all element data 
   * will be left untouched after the priority queue is dismounted
   * and destroyed. When all elements and data have been 
   * deallocated - the rest of the priority queue is freed, too.
   * 
   * @param[in] pq - a reference to current priority queue.
   * @return Nothing.
   * @see PQUEUEinit()
   **/
  void PQUEUEdestroy(PQueue pq);

  /**
   * Insert data into the priority queue
   * 
   * Inserts an element into the current priority queue - 
   * referenced by the parameter @a pq. The data to be inserted, 
   * is referenced by parameter @a data. It is the responsability
   * of the caller to ensure,  that this memory is valid as
   * long as it is present in the priority queue.
   *
   * @param[in] pq - a reference to current priority queue.
   * @param[in] data - a reference to data to be inserted into
   * the priority queue.
   * @return Value 0 - if insertion was succesful\n
   *         Value -1 - otherwise.
   **/
  int PQUEUEinsert(PQueue pq, const void *data);
  
  /**
   * Peek at the top-priority element in the queue
   * 
   * @param[in] pq - a reference to the current priority 
   * queue.
   * 
   * @return A reference to the top-priority element
   * of the current priority queue.
   *
   **/
  const void *PQUEUEpeek(const PQueue pq);

  /**
   * Remove top-priority data from the priority queue
   * 
   * When called, the 2nd parameter of this function, @a data,
   * should reference an (external, user-defined) pointer.
   * After the call - this referenced, external pointer has been
   * redirected by this function, to point to the data of the 
   * removed element - if the call was succesful. The caller is 
   * responsible for the future  of this memory - deallocating it,
   * if needed, for example.
   *
   * @param[in] pq - reference to current priority queue.
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
  int PQUEUEextract(PQueue pq, void **data);
  
  /**
   * Get the number of elements in the priority queue
   * 
   * @param[in] pq - a reference to the current priority
   * queue.
   *
   * @return The size, that is, the number of elements 
   * in the priority queue.
   **/
  int PQUEUEsize(PQueue pq);

  /**
   * Print data contents of the priority queue on screen
   * 
   * @param[in] pq - reference to current priority queue.
   * @param[in] callback - reference to user-defined callback function, 
   * that gets @b read @b access to element data via its parameter 
   * @a data - to do whatever is relevant. In this case it is a matter
   * of formatting data for printing on screen. The printed data 
   * should be kept to a minimum (the key value, for example) in order 
   * not to clutter the screen. This function is primarily for small
   * priority queues - and debugging purposes.
   *
   * @return Nothing.
   **/  
  void PQUEUEprint(PQueue pq, void (*callback)(const void *data));

#ifdef __cplusplus
}
#endif 

#endif /* _PQUEUE_H_ */
