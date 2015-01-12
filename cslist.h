/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: cslist.h
 * Author  : Kyle Loudon/Dan Levin
 * Date    : Mon Apr 08 19:29:27 2013
 * Version : 0.1 
 * ---
 * Description: A circular, singly-linked list - implemented as a pure, generic ADT
 * 
 * Revision history - coming up below:
 *
 * Date        Revision message
 * 2013-04-09  Created this file
 *
 *
 */
/**
 * @file cslist.h
 **/

#ifndef _CSLIST_H_
#define _CSLIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * Use a @b typedef - to hide the interior of @b CSList_ - in the 
   * implementation file. This is how @a data @a hiding can be done in C.
   * 
   **/
  typedef struct CSList_ *CSlist;

  /**
   * Another @b typedef - for hiding the interior of @b CSListElmt_  - 
   * in the implementation file.
   * 
   **/
  typedef struct CSListElmt_ *CSlistNode;

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
   * handling functions in this function interface - i.e. a sort of
   * "handle" to the list.
   * @see CSLISTdestroy()
   **/
  CSlist CSLISTinit(void (*destroy)(void *data));

  /**
   * Destroy the list. 
   * 
   * The list is destroyed - that is, all the memory occupied by the nodes 
   * is deallocated. The user-defined callback function @a destroy, given
   * as an argument to @b CSLISTinit(), is responsible for freeing 
   * dynamically allocated node data, when this function is called. When
   * all nodes and data have been deallocated - the list header is 
   * deallocated, too.
   * 
   * @param[in] list - a reference to current list.
   * @return Nothing.
   * @see CSLISTinit()
   **/
  void CSLISTdestroy(CSlist clist);

  /**
   * Insert a new node - after parameter @a node - into the list.
   *
   * This function inserts an new node, with a reference to node data
   * given by parameter @a data - @b after the node referenced by
   * parameter @a node - into @a list. 
   * 
   * @param[in] clist - reference to current list
   * @param[in] node - the node after which the new node is to be 
   * inserted. When inserting a node into an empty list, @a node
   * may hold any value - but should be set to NULL - just to avoid
   * confusion.
   * @param[in] data - reference to data to be stored in the new node, 
   * that is inserted into the list.
   *
   * @return Value 0 - if everything went OK - or value -1 otherwise.
   **/
  int CSLISTinsnext(CSlist clist, CSlistNode node, const void *data);

  /**
   * Remove the node - directly @b after - parameter @a node.  
   *
   * After the call - an (external) pointer referenced by parameter 
   * @a data, has been redirected to point to data of the removed 
   * node - if the call was succesful. The caller is responsible 
   * for the future of this memory - deallocating it, if needed, 
   * for example.
   *
   * @param[in] clist - reference to current list.
   * @param[in] node - the node just @b before the node to be removed.
   * @param[out] data - reference to a pointer, that will point to data 
   * of the removed node - after the call, if successful.
   * 
   * @return Value 0 - if everything went OK - or value -1 
   * otherwise.
   **/
  int CSLISTremnext(CSlist clist, CSlistNode node, void **data);

  /**
   * Get the list size.
   * 
   * @param[in] clist - a reference to the current list.
   *
   * @return The size, that is, the number of nodes in the list.
   **/
  int CSLISTsize(CSlist clist);

  /**
   * Get a reference to the head node of the list.
   * 
   * @param[in] clist - a reference to the current list.
   * @return A reference to the @b first node in the list.
   **/
  CSlistNode CSLISThead(CSlist clist);

  /**
   * Determine if a certain node is the head node of 
   * the list - or not.
   * 
   * @param[in] clist - a reference to the current list.
   * @param[in] node - a reference to the node to be tested.
   * @return Value 1 - if @a node indeed is the first node 
   * of the list - or 0 otherwise.
   **/
  int CSLISTishead(CSlist clist, CSlistNode node);

  /**
   * Get a reference to data stored in a node.
   * 
   * @param[in] node - a reference to the current node.
   * @return Generic reference to data - stored in parameter @a node.
   **/
  void *CSLISTdata(CSlistNode node);

  /**
   * Get a reference to the next node in the list.
   * 
   * @param[in] node - a reference to @b current node.
   * @return A reference to the @b next node - following 
   * parameter @a node - in the list.
   **/
  CSlistNode CSLISTnext(CSlistNode node);

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
   * with another function - CSLISTsetmatch().
   *
   * @param[in] clist - reference to the current list.
   * @param[in] data - reference to the search key data. 
   *
   * @return Reference to the first node with a match to data referenced 
   * by parameter @a data - if found - NULL otherwise
   * @see CSLISTsetmatch()
   **/
  CSlistNode CSLISTfindnode(CSlist clist, const void *data);

  /**
   * Set a valid match callback function for sequentially searching 
   * the list
   * 
   * @param[in] clist - reference to current list.
   * @param[in] match - a reference to a user-defined function that 
   * receives references to node data - and search key data - via its 
   * parameters @a key1 and @a key2 - and thereby can make 
   * the actual matching. This callback function shall return 1 - 
   * in case of a hit - or 0 otherwise.
   *
   * @return Nothing.
   **/
  void CSLISTsetmatch(CSlist clist, int (*match)(const void *key1, 
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
   * call to another function, CSLISTsetmatch().
   * 
   * After the call - an (external) pointer referenced by parameter 
   * @a data, has been redirected to point to data of the removed 
   * node - if the call was succesful. The caller is responsible 
   * for the future of this memory - deallocating it, if needed, 
   * for example.
   *
   * @param[in] clist - reference to current list.
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
   * @see CSLISTsetmatch()
   **/
  int CSLISTfind_remove(CSlist clist, void **data);

  /**
   * Traverse the list from the beginning - and have 
   * a user-defined function called - for each node in the list.
   * 
   * @param[in] clist - reference to current list.
   * @param[in] callback - reference to user-defined callback function, 
   * that gets @b read @b access to node data via its parameter 
   * @a data - to do whatever is relevant. Print data, for example.
   *
   * @return Nothing.
   **/
  void CSLISTtraverse(CSlist clist, void (*callback)(const void *data));

#ifdef __cplusplus
}
#endif 

#endif /* _CSLIST_H_ */

