/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: chashtbl.h
 * Author  : Kyle Loudon/Dan Levin
 * Date    : Mon Feb 25 16:52:00 GMT 2013
 * Version : 0.30
 * ---
 * Description: A chained hash table - implemented as a pure, generic ADT container. 
 * 
 * Revision history - coming up below:
 *
 * Date        Revision message
 * 2013-01-30  Created this file initially
 * 2013-02-19  Made some revision to the Doxygen documentation. Enhanced the description of
 *             in/out parameters - i.e. double-pointers.             
 * 2013-03-21  Minor documentation clarifications.
 *
 */

/**
 * @file chashtbl.h
 **/

#ifndef _CHASHTBL_H_
#define _CHASHTBL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "slist.h"

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * Use a @b typedef - to hide the interior of @b CHtbl_ - in the 
   * implementation file. This is how @a data @a hiding can be done in C.
   * 
   **/
  typedef struct CHtbl_ *CHtbl;

  /* FUNCTION DECLARATIONS */


  /**
   * Initiate the hash table
   * 
   * @param[in] buckets - The number of buckets you want the
   * table to contain when created.
   * @param[in] h - A reference to a user-defined hash function. 
   * This function returns the @a hash @a value of the @a key 
   * parameter - given to function @b h()  - when called.
   * @param[in] match - A reference to a user-defined @a match 
   * function. This function shall return 1 - if @a key1 is equal 
   * to @a key2 - or 0 otherwise.
   * @param[in] destroy - A reference to a user-defined function,
   * reponsible for freeing @a element @a data, when the table is 
   * destroyed. If @a destroy is set to NULL - then element data will 
   * be left untouched upon table destruction.
   * @return A reference - to a new, empty table - if dynamic memory
   * allocation for the ADT was successful - or NULL otherwise. Take 
   * really good care of this return value, since it will be needed
   * as a parameter in subsequent calls - to the majority of other
   * table functions in this function interface - i.e. a sort
   * of "handle" to the table.
   * @see CHTBLdestroy()
   **/
  CHtbl CHTBLinit(int buckets, 
		  int (*h)(const void *key),
		  int (*match)(const void *key1, const void *key2),
		  void (*destroy)(void *data));

  /**
   * Destroy the hash table
   * 
   * The table is destroyed - that is, all memory occupied by
   * the elements - will be deallocated. The user-defined callback 
   * function @a destroy, given as an argument to @b CHTBLinit(), is 
   * responsible for freeing dynamically allocated element data, when 
   * this function is called. If @a destroy is set to NULL when 
   * @b CHTBLinit() is called, all data will be left untouched after 
   * the table is dismounted and destroyed. When all elements and data
   * have been deallocated - the rest of the table is freed, too.
   * 
   * @param[in] htbl - a reference to current table.
   * @return Nothing.
   * @see CHTBLinit()
   **/
  void CHTBLdestroy(CHtbl htbl);

  /**
   * Insert data into the hash table
   * 
   * Inserts an element into the current hash table - referenced by
   * the parameter @a htbl. The data to be inserted, is referenced by 
   * parameter @a data. It is the responsability of the caller to 
   * ensure,  that this memory is valid as long as it is present in the 
   * table.
   *
   * @param[in] htbl - a reference to current table.
   * @param[in] data - a reference to data to be inserted into
   * the table.
   * @return Value 0 - if insertion was succesful\n
   *         Value 1 - if the element is already present in the table\n
   *         Value -1 - otherwise (implies fatal error).
   **/
  int CHTBLinsert(CHtbl htbl, const void *data);

  /**
   * Remove an element from the hash table
   * 
   * When called, the 2nd parameter of this function, @a data,
   * should reference an (external, user-defined) pointer, that 
   * points to the search key data.    
   * After the call - this referenced, external pointer has been
   * redirected by this function, to point to the data of the 
   * removed element - if the call was succesful. The caller is 
   * responsible for the future  of this memory - deallocating it,
   * if needed, for example.
   * Moreover, a user-defined @b callback @b function, responsible 
   * for doing the @b matching of element data - and data referenced
   * by parameter @a data - must exist for this function to work -
   * otherwise -2 will be returned - always. 
   * This user-supplied @b match-callback was set when the hash
   * table was initialized - see CHTBLinit().
   *
   * @param[in] htbl - reference to current table.
   * @param[in,out] data - reference to a pointer. After the call, 
   * this referenced pointer has been redirected to point to
   * the data of the removed element - if the call was
   * successful. The caller is responsible for the future of
   * this memory - deallocating it, for example.
   * 
   * @return Value 0 --  if the call was OK - that is, element
   * found and removed.\n
   *         Value 1 --  node not found.\n
   *         Value -2 -- if match-callback is not set.\n
   *         Value -1 -- otherwise (implies fatal error).
   **/
  int CHTBLremove(CHtbl htbl, void **data);

  /**
   * Lookup data in the table - without removing it
   * 
   * Determines whether an element, with key data matching 
   * the data referenced by the parameter @a data - is
   * present in the current table @a htbl. This 2nd parameter,
   * @a data, should reference an (external, user-defined) 
   * pointer, that points to the search key data.    
   * After the call - this referenced, external pointer has been
   * redirected by this function, to point to the data of the 
   * element hit - if the call was succesful. 
   * Moreover, a user-defined @b callback @b function, responsible 
   * for doing the @b matching of element data - and data referenced
   * by parameter @a data - must exist for this function to work.
   * This user-supplied @b match-callback was set when the hash
   * table was initialized - see CHTBLinit().
   *
   * @param[in] htbl - a reference to current table.
   * @param[in,out] data - a reference to a pointer, pointing at
   * the data to be searched for - at the call. Upon return - this
   * pointer has been redirected by this function - and points 
   * instead to data in the element hit - if any.
   * 
   * @return Value 0 - if element with matching data was found.\n
   *         Value -1 - otherwise.
   * 
   * @see CHTBLinit()
   **/
  int CHTBLlookup(const CHtbl htbl, void **data);

  /**
   * Get the number of elements in the table
   * 
   * @param[in] htbl - a reference to the current table.
   *
   * @return The size, that is, the number of elements 
   * in the table.
   **/
  int CHTBLsize(CHtbl htbl);

  /**
   * Print all data within the table - on screen
   * 
   * @param[in] htbl - reference to current table.
   * @param[in] callback - reference to user-defined callback function, 
   * that gets @b read @b access to element data via its parameter 
   * @a data - to do whatever is relevant. In this case it is a matter
   * of formatting data for printing on screen. The printed data 
   * should be kept to a minimum (the key value, for example) in order 
   * not to clutter the screen. This function is primarily for small
   * tables and debugging purposes.
   *
   * @return - Nothing.
   **/
  void CHTBLprint(CHtbl htbl, void (*callback)(const void *data));


#ifdef __cplusplus
}
#endif 

#endif /* _CHASHTBL_H_ */

