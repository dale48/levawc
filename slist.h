/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: slist.h
 * Author  : Kyle Loudon/Dan Levin
 * Date    : Mon Feb 25 16:52:00 GMT 2013
 * Version : 0.30
 * ---
 * Description: A singly-linked list - implemented as a pure, generic ADT.
 *
 * Revision history - coming up below:
 *
 * Date        Revision message
 * 2012-12-03  Created this file
 * 2013-02-05  Made following changes to function 'int SLISTremnode(Slist list, void **data)':
 *             - Changed function name to - 'int SLISTfind_remove(Slist list, void **data)'
 *             - Changed return value - for missing "match-callback"(=not set) - from -1 to -2
 *             - Changed return value - for node not found - from -1 to 1.
 * 2013-02-19  Made some revision to the Doxygen documentation. Enhanced the description of
 *             in/out parameters - i.e. double-pointers.             
 *
 */

/**
 * @file slist.h
 **/

#ifndef _SLIST_H_
#define _SLIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SLIST_FWD 1   /* Macro for forward traversal of the list */
#define SLIST_BWD -1  /* Macro for backward traversal of the list */

  /**
   * Use a @b typedef - to hide the interior of @b SList_ - in the 
   * implementation file. This is how @a data @a hiding can be done in C.
   * 
   **/
  typedef struct SList_ *Slist;

  /**
   * Another @b typedef - for hiding the interior of @b SListElmt_  - 
   * in the implementation file.
   * 
   **/
  typedef struct SListElmt_ *SlistNode;

  /* FUNCTION DECLARATIONS */

  /**
   * 
   * Initiate the list. 
   *
   * @param[in] destroy - A reference to a user-made function, reponsible
   * for freeing node data, when the list is deleted. If @a destroy is 
   * NULL - then node data will be left untouched when the list is 
   * destroyed.
   * @return A reference - to a new, empty list - if dynamic memory
   * allocation for the ADT was successful - or NULL otherwise. Take 
   * really good care  of this return value, since it will be needed as
   * a parameter in subsequent calls - to the majority of other list 
   * handling functions in the this function interface - i.e. a sort of
   * "handle" to the list.
   * @see SLISTdestroy()
   **/
  Slist SLISTinit(void (*destroy)(void *data));

  /**
   * Destroy the list. 
   * 
   * The list is destroyed - that is, all the memory occupied by the nodes 
   * is deallocated. The user-defined callback function @a destroy, given
   * as an argument to @b SLISTinit(), is responsible for freeing 
   * dynamically allocated node data, when this function is called. When
   * all nodes and data have been deallocated - the list header is 
   * deallocated, too.
   * 
   * @param[in] list - a reference to current list.
   * @return Nothing.
   * @see SLISTinit()
   **/
  void SLISTdestroy(Slist list);

  /**
   * Insert a new node - after parameter @a node - into the list.
   *
   * This function inserts an new node, with a reference to node data
   * given by parameter @a data - @b after the node referenced by
   * parameter @a node - into @a list. 
   * If you want to insert the new node at the beginning, the  
   * parameter @a node should be set to NULL.
   * 
   * @param[in] list - reference to current list
   * @param[in] node - the node after which the new node is to be 
   * inserted. If set to @b NULL - the new node is inserted at the 
   * beginning.
   * @param[in] data - reference to data to be stored in the new node, 
   * that is inserted into the list.
   *
   * @return Value 0 - if everything went OK - or value -1 otherwise.
   **/
  int SLISTinsnext(Slist list, SlistNode node, const void *data);

  /**
   * Remove the node - directly @b after - parameter @a node.  
   *
   * Data referenced by the removed node can be accessed by 
   * dereferencing parameter @a data - that is, @a *data - after 
   * the call - if the call was successful. If parameter @a node 
   * is set to NULL, then the first node in @a list will be removed.
   * The caller is responsible for the future of this memory - 
   * deallocating it, for example.
   *
   * @param[in] list - reference to current list.
   * @param[in] node - the node just @b before the node to be removed.
   * @param[out] data - reference to a pointer, that will point to data 
   * of the removed node - after the call, if successful.
   * 
   * @return Value 0 - if everything went OK - or value -1 
   * otherwise.
   **/
  int SLISTremnext(Slist list, SlistNode node, void **data);

  /**
   * Get the list size.
   * 
   * @param[in] list - a reference to the current list.
   *
   * @return The size, that is, the number of nodes in the list.
   **/
  int SLISTsize(Slist list);

  /**
   * Get a reference to the head node of the list.
   * 
   * @param[in] list - a reference to the current list.
   * @return A reference to the @b first node in the list.
   **/
  SlistNode SLISThead(Slist list);

  /**
   * Get a reference to the tail node of the list.
   * 
   * @param[in] list - a reference to the current list.
   * @return A reference to the @b last node in the list.
   **/
  SlistNode SLISTtail(Slist list);

  /**
   * Determine if a certain node is the head node of 
   * the list - or not.
   * 
   * @param[in] list - a reference to the current list.
   * @param[in] node - a reference to the node to be tested.
   * @return Value 1 - if @a node indeed is the first node 
   * of the list - or 0 otherwise.
   **/
  int SLISTishead(Slist list, SlistNode node);

  /**
   * Determine if a certain node is the tail node 
   * of the list - or not.
   * 
   * @param[in] list - a reference to the current list.
   * @param[in] node - a reference to the node to be tested.
   * @return Value 1 - if @a node indeed is the last node 
   * of the list - or 0 otherwise.
   **/
  int SLISTistail(Slist list, SlistNode node);

  /**
   * Get a reference to data stored in a node.
   * 
   * @param[in] node - a reference to the current node.
   * @return Generic reference to data - stored in parameter @a node.
   **/
  void *SLISTdata(SlistNode node);

  /**
   * Get a reference to the next node in the list.
   * 
   * @param[in] node - a reference to @b current node.
   * @return A reference to the @b next node - following 
   * parameter @a node - in the list.
   **/
  SlistNode SLISTnext(SlistNode node);

  /**
   * Find the first node - with node data matching data referenced by
   * parameter @a data.
   * 
   * Search the list sequentially for a node, whose data matches the data
   * referenced by parameter @a data. A reference to the node with the 
   * @b first match will be returned - NULL otherwise. A @b user-defined
   * @b callback @b function, responsible for doing the @b matching 
   * of node data - with data referenced by parameter @a data - @b must 
   * exist for this function to work - otherwise NULL will be returned - 
   * always. This user-supplied @b match-callback is set into the list 
   * with another function - SLISTsetmatch().
   *
   * @param[in] list - reference to the current list.
   * @param[in] data - reference to the search key data. 
   *
   * @return Reference to the first node with a match to data referenced 
   * by parameter @a data - if found - NULL otherwise
   * @see SLISTsetmatch()
   **/
  SlistNode SLISTfindnode(Slist list, const void *data);

  /**
   * Set a valid match callback function for sequentially searching 
   * the list
   * 
   * @param[in] list - reference to current list.
   * @param[in] match - a reference to a user-defined function that 
   * receives references to node data - and search key data - via its 
   * parameters @a key1 and @a key2 - and thereby can make 
   * the actual matching. This callback function shall return 1 - 
   * in case of a hit - or 0 otherwise.
   *
   * @return Nothing.
   **/
  void SLISTsetmatch(Slist list, int (*match)(const void *key1, 
					      const void *key2));

  /**
   * Search - and remove a node - by using an in/out parameter.
   *
   * When called, the 2nd parameter of this function, @a data, should 
   * reference a pointer, that points to the search key data. Moreover, 
   * a @b user-defined @b callback @b function responsible for doing the 
   * @b matching of node data - and data referenced by parameter 
   * @a data - must exist for this function to work - otherwise -2 will
   * be returned - always.
   * This user-supplied @b match-callback is set into the list with a
   * call to another function, SLISTsetmatch().
   * 
   * After the call - an (external) pointer referenced by parameter 
   * @a data, has been redirected to point to data of the removed 
   * node - if the call was succesful. The caller is responsible 
   * for the future of this memory - deallocating it, if needed, 
   * for example.
   *
   * @param[in] list - reference to current list.
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
   *         Value -2 -- if match-callback is not set.\n
   *         Value -1 -- otherwise (implies fatal error).
   * 
   * @see SLISTsetmatch()
   **/
  int SLISTfind_remove(Slist list, void **data);

  /**
   * Reverse the list - physically.
   * 
   * @param[in] list - reference to current list.
   *
   * @return None.
   **/  
  void SLISTreverse(Slist list);

  /**
   * Sort a list according to the Selection Sort Algorithm -
   * with complexity &Theta;(n<sup>2</sup>)).
   * 
   * @param[in] list - reference to current list.
   * @param[in] cmp - reference to a user-defined callback function 
   * responsible for comparing node data. 
   * This callback function should return a value of -1
   * if data referenced by @a key1 is less than data referenced by 
   * @a key2 - or 0 if they are equal - or 1 otherwise. This will
   * produce an ascending sort order. If a descending order is what
   * you want - just swap the return values -1 and 1 for the 
   * comparisons made above.
   *
   * @return Nothing.
   **/
  void SLISTsort(Slist list, int (*cmp)(const void *key1, const void *key2));

  /**
   * Traverse the list from the beginning or the end - and have 
   * a user-defined function called - for each node in the list.
   * 
   * @param[in] list - reference to current list.
   * @param[in] callback - reference to user-defined callback function, 
   * that gets @b read @b access to node data via its parameter 
   * @a data - to do whatever is relevant. Print data, for example.
   * @param[in] direction - @a direction of @a traversal. Set to SLIST_FWD
   * for forward traversal - and SLIST_BWD for traversing backwards.
   *
   * @return Nothing.
   **/
  void SLISTtraverse(Slist list, void (*callback)(const void *data), int direction);

#ifdef __cplusplus
}
#endif 

#endif /* _SLIST_H_ */
