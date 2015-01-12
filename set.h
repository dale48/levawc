/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: set.h
 * Author  : Kyle Loudon/Dan Levin
 * Date    : Mon Apr 08 12:29:13 2013
 * Version : 0.1 
 * ---
 * Description: A pure, generic set ADT - written in ANSI C
 * 
 * Revision history - coming up below:
 *
 * Date        Revision message
 * 2013-04-13  Created this file
 */
/**
 * @file set.h
 **/

#ifndef _SET_H_
#define _SET_H_

#include <stdio.h>
#include <stdlib.h>

#include "slist.h"

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * Macro for forward traversal of the set
   * 
   **/
#define SET_FWD 1
  /**
   * Macro for backward traversal of the set
   * 
   **/
#define SET_BWD -1

  typedef Slist Set;

  /* FUNCTION DECLARATIONS */

  /**
   * Initialize the set
   * 
   * @param[in] match - a reference to a user-defined function that 
   * receives references to node data - and search key data - via its 
   * parameters @a key1 and @a key2. Hence this callback function
   * can make the actual matching of search key and node data and 
   * determine if search data is present in the set. This callback 
   * function shall return 1 - in case of a hit - or 0 otherwise.
   * @param[in] destroy - A reference to a user-made function, reponsible
   * for freeing node data, when the set is deleted. If @a destroy is 
   * NULL - then node data will be left untouched when the set is 
   * destroyed.
   * @return A reference - to a new, empty set - if dynamic memory
   * allocation for the ADT was successful - or NULL otherwise. Take 
   * really good care  of this return value, since it will be needed as
   * a parameter in subsequent calls - to the majority of other set 
   * handling functions in this function interface - i.e. a sort of
   * "handle" to the set.
   * @see SETdestroy()
   **/
  Set SETinit(int (*match)(const void *key1, const void *key2), void (*destroy)(void *data));

  /**
   * Destroy the set. 
   * 
   * The set is destroyed - that is, all the memory occupied by the nodes 
   * is deallocated. The user-defined callback function @a destroy, given
   * as an argument to @b SETinit(), is responsible for freeing 
   * dynamically allocated node data, when this function is called. When
   * all nodes and data have been deallocated - the set header is 
   * deallocated, too.
   * 
   * @param[in] set - a reference to current set.
   * @return Nothing.
   * @see SETinit()
   **/
  void SETdestroy(Set set);

  /**
   * Insert data into the set.
   * 
   * @param[in] set - a reference to current set.
   * @param[in] data - reference to data to be
   * inserted into the set.
   * @return Value 0 - if insertion was successful.\n
   * Value 1 - if insertion was rejected - i.e. if data is
   * already present in the set.\n
   * Value -1 - otherwise, if another sort of error occurred.
   **/
  int SETinsert(Set set, const void *data);
  
  /**
   * Search and remove data - by an in/out parameter
   * 
   * When called, the 2nd parameter of this function, @a data, should 
   * reference a pointer, that points to the search key data. 
   * After the call - this (external) pointer referenced by parameter 
   * @a data, has been redirected to point to data of the removed 
   * node - if the call was succesful. The caller is responsible 
   * for the future of this memory - deallocating it, if needed, 
   * for example.
   *
   * @param[in] set - reference to current set.
   * @param[in,out] data - reference to an external pointer, that 
   * initially shall point to the search key data - when this 
   * function is called. After the call, this referenced (external) 
   * pointer, has been redirected - to point to node data of the 
   * removed node - if the call was successful. The caller is 
   * responsible for the future of this memory - deallocating it,
   * for example.
   * 
   * @return Value 0 --  if the call was OK - that is, node found and removed.\n
   *         Value 1 --  node not found.\n
   *         Value -1 -- otherwise (implies fatal error).
   **/
  int SETremove(Set set, void **data);

  /**
   * Build the union of two given sets.
   * 
   * @param set1 - one of the given sets, taking
   * part of building the union.
   * @param set2 - the other partial set that
   * comprises the union together with @a set1.
   * @return A new set - equal to the union of
   * @a set1 and @a set2 - i.e. mathematically
   * speaking, containing all the elements 
   * @a either in set1 and @a set2 - or in @a both.

   **/
  Set SETunion(Set set1, Set set2);

  /**
   * Build the intersection of two given sets.
   * 
   * @param set1 - one of the given sets, taking
   * part of building the intersection.
   * @param set2 - the other partial set that
   * comprises the intersection together with 
   * @a set1.
   * @return A new set - equal to the intersection of
   * @a set1 and @a set2 - i.e. mathematically
   * speaking, containing all the elements @a common to 
   * @a set1 and @a set2.
   **/
  Set SETintersection(Set set1, Set set2);

  /**
   * Build the difference of two given sets
   * 
   * @param set1 - one of the given sets, taking
   * part, together with @a set2, in building the 
   * difference.
   * @param set2 - the other set, that
   * makes up the difference - together with 
   * @a set1.
   * @return A new set - equal to the difference of
   * @a set1 and @a set2 - i.e. mathematically
   * speaking, containing all the elements @a only
   * present in @a set1.
   **/
  Set SETdifference(Set set1, Set set2);

  /**
   * Determine whether a data member exists in a given set
   * 
   * @param set - a reference to current set.
   * @param data - a reference to element key data.
   * @return Value 1 - if @a data is member of @a set -
   * or 0 ohterwise.
   **/
  int SETis_member(Set set, const void *data);

  /**
   * Determine whether one set is a subset of another
   * 
   * @param set1 - a reference to the set, tested for
   * being a subset of @a set2.
   * @param set2 - a reference to the set that might
   * contain @a set1 as a subset.
   * @return Value 1 - if @a set1 is a subset of @a set2 -
   * or 0 otherwise.
   **/
  int SETis_subset(const Set set1, const Set set2);

  /**
   * Determine whether one set is equal to another
   * 
   * @param set1 - a reference to one of the sets - 
   * tested for equality.
   * @param set2 - a reference to the other.
   * @return Value 1 - if @a set1 is equal to @a set2 -
   * or 0 otherwise.
   **/
  int SETis_equal(Set set1, Set set2);
  
  /**
   * Get the size of a set
   * 
   * @param set - a reference to current set.
   * @return The size - i.e.  number of elements 
   * in the set.
   **/  
  int SETsize(Set set);
  
  /**
   * Sort a set
   * 
   * @param[in] set - reference to current set.
   * @param[in] cmp - reference to a user-defined callback function 
   * responsible for comparing element data. 
   * This callback function should return a value of -1
   * if data referenced by @a key1 is less than data referenced by 
   * @a key2 - or 0 if they are equal - or 1 otherwise. This will
   * produce an ascending sort order. If a descending order is what
   * you want - just swap the return values -1 and 1 for the 
   * comparisons made above.
   *
   * @return Nothing.
   **/
  void SETsort(Set set, int (*cmp)(const void *key1, const void *key2));

  /**
   * Traverse all elements of the set
   * 
   * @param[in] set - reference to current set.
   * @param[in] callback - reference to user-defined callback function, 
   * that gets @b read @b access to node data via its parameter 
   * @a data - to do whatever is relevant. Print data, for example.
   * @param[in] direction - @a direction of @a traversal. Set to SET_FWD
   * for forward traversal - and SET_BWD for traversing backwards.
   *
   * @return Nothing.
   **/
  void SETtraverse(Set set, void (*callback)(const void *data), int direction);
  
#ifdef __cplusplus
}
#endif 

#endif /* _SET_H_ */
