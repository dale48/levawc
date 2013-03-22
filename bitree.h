/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: bitree.h
 * Author  : Kyle Loudon/Dan Levin
 * Date    : Tue Feb 19 19:59:44 2013
 * Version : 0.1 
 * ---
 * Description: A basic, binary search tree ADT - written in ANSI C.
 * 
 * Date        Revision message
 * 2013-02-17  Created this file
 *
 */
/**
 * @file bitree.h
 **/

#ifndef _BITREE_H_
#define _BITREE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>

#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * Use a @b typedef - to hide the interior of @b BiTreeNode_ - in the 
   * implementation file. This is how @a data @a hiding can be done in C.
   * 
   **/
  typedef struct BiTreeNode_ *BiTreeNode;

  /**
   * Use a @b typedef - to hide the interior of @b BiTree_ - in the 
   * implementation file. This is how @a data @a hiding can be done in C.
   * 
   **/
  typedef struct BiTree_ *BiTree;

  /* FUNCTION DECLARATIONS */

  /**
   * Initialize the tree
   * 
   * @param[in] destroy - a reference to a user-defined function,
   * reponsible for freeing @a node @a data, when the tree is 
   * destroyed. If @a destroy is set to NULL - then node data will 
   * be left untouched upon tree destruction.
   * @return A reference - to a new, empty tree - if dynamic memory
   * allocation for the ADT was successful - or NULL otherwise. Take 
   * really good care of this return value, since it will be needed
   * as a parameter in subsequent calls - to the majority of other
   * tree functions in this function interface - i.e. a sort
   * of "handle" to the tree.
   * 
   **/
  BiTree BITREEinit(void (*destroy)(void *data));

  /**
   * Destroy the tree
   * 
   * The tree is destroyed - that is, all dynamically allocated 
   * memory occupied by the nodes - will be destroyed.
   * It is the user-defined callback function @a destroy, given 
   * as an argument to @b BITREEinit(), that is responsible for 
   * freeing dynamically allocated node data, when this 
   * function is called. If, on the other hand, @a destroy is 
   * set to NULL when @b BITREEinit() is called, all node data 
   * will be left untouched after the tree is dismounted and 
   * destroyed. When all nodes and data have been 
   * deallocated - the rest of the tree is freed, too.
   * 
   * @param[in] tree - a reference to current tree.
   * @return Nothing.
   * @see BITREEinit()
   **/
  void BITREEdestroy(BiTree tree);

  /**
   * Set the compare callback function
   * 
   * @param[in] tree - a reference to current tree.
   * @param[in] compare - reference to a user-defined callback 
   * function responsible for comparing node data. This 
   * callback function should return a value of -1 if data 
   * referenced by key1 is less than data referenced by key2 - 
   * or 0 if they are equal - or 1 otherwise. The purpose of 
   * this function is to implement node data searching - and it
   * @b must be called (at least) - @b once - prior to calls 
   * of such searching functions - for example, BITREEinsert(), 
   * BITREElookup() and BITREEremove().
   *
   * @return Nothing.
   **/
  void BITREEsetcompare(BiTree tree, int (*compare)(const void *key1, const void *key2));

  /**
   * Insert a new node as a left child node of parameter
   * @a node.
   * 
   * The insertion of a new node will only be succesful,
   * if parameter @a node has no left child. If parameter
   * @a node is set to NULL, the new node will be 
   * inserted as the root node of the tree. This can only
   * take place - if the tree is empty prior to the call.
   * The data to be inserted, is referenced by parameter data. 
   * It is the responsability of the caller to ensure, that this
   *  memory is valid as long as it is present in the tree.
   *
   * @param[in] tree - a reference to current tree.
   * @param[in] node - a reference to the parent node of
   * the new, left child, which implies, that the parent
   * must not have any left child prior to the call.\n
   * If @a node is set to NULL, the
   * new node will be the root node of the tree. This 
   * will only happen if the tree is empty prior to call.
   * @param[in] data - a reference to data to be inserted 
   * into the tree.
   *
   * @return Value 0 if insertion was successful - or
   * -1 otherwise, i.e. if parameter @a node already
   * has a left child - or, if @a node is set to NULL and
   * the tree is not empty.
   **/
  int BITREEinsleft(BiTree tree, BiTreeNode node, const void *data);

  /**
   * Insert a new node as a right child node of parameter
   * @a node.
   * 
   * The insertion of a new node will only be succesful,
   * if parameter @a node has no right child. If parameter
   * @a node is set to NULL, the new node will be 
   * inserted as the root node of the tree. This can only
   * take place - if the tree is empty prior to the call.
   * The data to be inserted, is referenced by parameter data. 
   * It is the responsability of the caller to ensure, that 
   * this memory is valid as long as it is present in the tree.
   *
   * @param[in] tree - a reference to current tree.
   * @param[in] node - a reference to the parent node of
   * the new, right child, which implies, that the parent
   * must not have any right child prior to the call.\n
   * If @a node is set to NULL, the
   * new node will be the root node of the tree. This 
   * will only happen if the tree is empty prior to call.
   * @param[in] data - a reference to data to be inserted 
   * into the tree.
   *
   * @return Value 0 if insertion was successful - or
   * -1 otherwise, i.e. if parameter @a node already
   * has a right child - or, if @a node is set to NULL and 
   * the tree is not empty.
   **/
  int BITREEinsright(BiTree tree, BiTreeNode node, const void *data);

  /**
   * Insert a new node with data - into the tree
   *
   * First a search is made to determine if a node
   * with data equal to parameter @a data already
   * exists in the tree. If so, the function returns
   * immediately - with value 1. Otherwise a new 
   * node is created, to be inserted into
   * the tree according to the rules for a binary
   * search tree - that is, at any given node - all
   * nodes in the @a left subtree hold data @a less 
   * than data of the new node - and all nodes of
   * the @a right subtree has data @a larger than 
   * that of the new node. It is the responsability 
   * of the caller to ensure, that this memory is 
   * valid as long as it is present in the tree.
   * @param[in] tree - reference to current tree.
   * @param[in] data - reference to data to be 
   * inserted into the tree. It is the responsability
   * of the caller to ensure, that this memory is valid
   * as long as it is present in the tree.
   * @return Value 0, if insertion was succesful.\n
   * Value -2, if BITREEsetcompare() not has been called 
   * prior to this call.\n
   * Value 1, if data already exists in the tree.\n
   * Value -1, indicates fatal error.
   **/
  int BITREEinsert(BiTree tree, const void *data);

  /**
   * Lookup data in the tree - without removing it.
   * 
   * Determines whether a node, with key data matching
   * the data referenced by the parameter @a data - is 
   * present in the current tree. This 2nd parameter, 
   * @a data, should reference an (external, user-defined)
   * pointer, that points to the search key data. After 
   * the call - this referenced, external pointer has 
   * been redirected by this function, to point to the 
   * data of the node hit  - if the call was succesful.
   * Moreover, a user-defined callback function, 
   * responsible for doing the matching of node data -
   * and data referenced by parameter @a data - @a must 
   * exist for this function to work. This user-supplied
   * callback is set with a prior call to function 
   * BITREEsetcompare().
   *
   * @param[in] tree - reference to current tree.
   * @param[in,out] data - a reference to an external
   * pointer, pointing at the data to be searched for -
   * at the call. Upon return - this pointer has been
   * redirected by this function - and points instead 
   * to data contained in the node hit - if any.
   * @return Value 0 - if call was successful.\n
   * Value -2, if BITREEsetcompare() not has been called 
   * prior to this call.\n
   * Value -1, if searched node not found.
   **/
  int BITREElookup(BiTree tree, void **data);

  /**
   * Physically remove a node - with its data
   * from the tree.
   * 
   * This function, when successful, removes the
   * searched node from the tree - and hands node 
   * data back to the caller - while preserving
   * the validity of the tree as a binary search tree.
   * When called, the 2nd parameter of this function, 
   * @a data, should reference an (external, user-defined)
   * pointer, that points to the search key data.    
   * After the call - this referenced, external pointer
   * has been redirected by this function, to point to
   * the data of the removed element - if the call was
   * succesful. The caller is responsible for the future
   * of this memory - deallocating it,
   * if needed, for example.
   * Moreover, a user-defined callback function, 
   * responsible for doing the matching of node data -
   * and data referenced by parameter @a data - @a must 
   * exist for this function to work. This user-supplied
   * callback is set with a prior call to function 
   * BITREEsetcompare().

   * @param[in] tree - reference to current tree.
   * @param[in,out] data - a reference to an external
   * pointer, pointing at the data to be removed -
   * at the call. Upon return - this pointer has been
   * redirected by this function - and points instead 
   * to data contained in the node hit - if any.
   * @return Value 0 - if call was successful.\n
   * Value -2, if BITREEsetcompare() not has been called 
   * prior to this call.\n
   * Value -1, if searched node not found.
   * @see BITREEsetcompare()
   **/
  int BITREEremove(BiTree tree, void **data);

  /**
   * Remove the left subtree
   *
   * This function removes the subtree rooted as the
   * left child of parameter @a node. The removal is
   * accomplished by a postorder traversal beginning
   * at this left child of parameter @a node. However,
   * if @a node is set to NULL, the removal will instead 
   * start at the root of the tree - and thereby remove
   * the entire tree. The function referenced by 
   * @a destroy, initially passed as a parameter to 
   * BITREEinit(), will be called exactly once 
   * for each destroyed node - provided @a destroy
   * was not set to NULL - when BITREEinit() was 
   * called.
   * 
   * @param[in] tree - reference to current tree.
   * @param[in] node - reference to a node - or 
   * set to NULL.
   * @return Nothing.
   * @see BITREEinit()
   **/
  void BITREEremleft(BiTree tree, BiTreeNode node);

  /**
   * Remove the right subtree
   * 
   * This function removes the subtree rooted as the
   * right child of parameter @a node. The removal is
   * accomplished by a postorder traversal beginning
   * at this right child of parameter @a node. However,
   * if @a node is set to NULL, the removal will instead 
   * start at the root of the tree - and thereby remove
   * the entire tree. The function referenced by 
   * @a destroy, initially passed as a parameter to 
   * BITREEinit(), will be called exactly once 
   * for each destroyed node - provided @a destroy
   * was not set to NULL - when BITREEinit() was 
   * called.
   * 
   * @param[in] tree - reference to current tree.
   * @param[in] node - reference to a node - or 
   * set to NULL.
   * @return Nothing.
   * @see BITREEinit()
   **/
  void BITREEremright(BiTree tree, BiTreeNode node);

  /**
   * Merge two trees as subtrees to a new root
   * node - containing parameter @a data as root
   * node data.
   *
   * This function merges the two trees referenced by 
   * parameters @a left and @a right - into a new, 
   * single binary tree - returned by the function, if 
   * the call was successful. After merging, the new,
   * merged tree will have a root node containing
   * parameter @a data as node data - and the trees
   * referenced by @a left and @a right, respectively -
   * as left and right subtrees. The size of the new,
   * merged tree will be updated accordingly. Finally,
   * the two trees @a left and @a right are detached from
   * their tree nodes - leaving only the tree headers -
   * and setting tree size to 0. It is the responsability
   * of the caller to care for the future of these two,
   * empty trees - destroying them when appropriate.
   * @param[in] left - reference to a tree, that will
   * serve as the @a left subtree of the new, merged
   * tree - returned by this function.
   * @param[in] right - reference to a tree, that will
   * serve as the @a right subtree of the new, merged
   * tree - returned by this function.
   * @param[in] data - data to be inserted in the root
   * node of the new, merged tree.
   * @return A new, merged tree if the merge was successful -
   * or NULL otherwise.
   **/
  BiTree BITREEmerge(BiTree left, BiTree right, const void *data);

  /**
   * Get the size of the tree
   * 
   * @param[in] tree - a reference to the current tree
   *
   * @return The size, i.e. the number of nodes in
   * the tree.
   **/
  int BITREEsize(BiTree tree);

  /**
   * Get the root node of the tree
   * 
   * @param[in] tree - a reference to the current tree
   *
   * @return A reference to the root node of the tree.
   **/
  BiTreeNode BITREEroot(BiTree tree);

  /**
   * Determines whether the parameter @a node is NULL -
   * i.e. "end-of-branch" - or not
   * 
   * @param[in] node - the node to be tested
   *
   * @return Value 1 if @a node is the end of a 
   * branch in the tree - or 0 otherwise.
   **/
  int BITREEis_eob(BiTreeNode node);

  /**
   * Determines if the the parameter @a node is a leaf
   * node, i.e. has no children - or not
   * 
   * @param[in] node - the node to be tested
   *
   * @return Value 1 if @a node is a leaf node - or
   * 0 otherwise.
   **/
  int BITREEis_leaf(BiTreeNode node);

  /**
   * Get a reference to node data
   * 
   * @param[in] node - reference to current node
   *
   * @return Generic reference to node data.
   **/
  void *BITREEdata(BiTreeNode node);

  /**
   * Get a reference to left child node.
   * 
   * @param[in] node - reference to current node
   *
   * @return A reference to left child of 
   * parameter @a node.
   **/
  BiTreeNode BITREEleft(BiTreeNode node);

  /**
   * Get a reference to right child node.
   * 
   * @param[in] node - reference to current node
   *
   * @return A reference to right child of 
   * parameter @a node.
   **/
  BiTreeNode BITREEright(BiTreeNode node);

  /**
   * Get the tree height(=nr of levels).
   * 
   * @param[in] tree - reference to current tree
   *
   * @return Tree height - i.e. the max. number
   * of levels in the tree.
   **/
  int BITREEheight(BiTree tree);

  /**
   * Print all tree nodes, with their data, on screen.
   *
   * @param[in] tree - reference to current tree.
   * @param[in] callback - reference to user-defined 
   * callback function, that gets read access to element
   * data via its parameter @a data - to do whatever is 
   * relevant. In this case it is a matter of formatting 
   * data for printing on screen. The printed data should 
   * be kept to a minimum (the key value, for example) in 
   * order not to clutter the screen. This function is 
   * primarily for small trees - and educational/debugging
   * purposes.
   * @return Nothing.
   **/
  void BITREEprint(BiTree tree, void (*callback)(const void *data));

  /**
   * Traverse the entire tree - in @a preorder
   * 
   * Preorder traversal means the following, where
   * @a recursion is obvious:\n\n
   * As long as we are visiting a valid(=not NULL) 
   * node - do:\n
   * 1. First - visit the root node...\n
   * 2. ..then goto the left subtree of the root node -
   * and traverse this subtree in @a preorder...\n
   * 3. ..and finally goto the right subtree - and 
   * traverse this subtree, too - in @a preorder.
   *
   * @param[in] tree - reference to current tree.
   * @param[in] callback - reference to user-defined 
   * callback function, that gets read access to element
   * data via its parameter @a data - to do whatever is 
   * relevant. In could be a matter of formatting 
   * data for printing on screen, for example.
   * @return Nothing.
   **/
  void BITREEpreorder(BiTree tree, void (*callback)(const void *data));

  /**
   * Traverse the entire tree - in @a inorder.
   * 
   * Inorder traversal means the following, where
   * @a recursion is obvious:\n\n
   * As long as we are visiting a valid(=not NULL) 
   * node - do:\n
   * 1. First - goto the left subtree of the root
   *  node - and traverse this subtree in 
   * @a inorder...\n
   * 2. ..visit the root node...\n
   * 3. ..and finally goto the right subtree - and 
   * traverse this subtree, too - in @a inorder.
   *
   * @param[in] tree - reference to current tree.
   * @param[in] callback - reference to user-defined 
   * callback function, that gets read access to
   * element data via its parameter @a data - to
   * do whatever is relevant. In could be a matter
   * of formatting data for printing on screen, for
   * example.
   * @return Nothing.
   **/
  void BITREEinorder(BiTree tree, void (*callback)(const void *data));

  /**
   * Traverse the entire tree - in @a postorder.
   * 
   * Postorder traversal means the following, where @a recursion 
   * is obvious:\n\n
   * As long as we are visiting a valid(=not NULL) 
   * node - do:\n
   * 1. First - goto the left subtree of the root
   *  node - and traverse this subtree in 
   * @a postorder...\n
   * 2. ..then goto the right subtree of the root node -
   * and traverse this subtree in @a postorder...\n
   * 3. ..and finally - visit the root node.
   * @param[in] tree - reference to current tree.
   * @param[in] callback - reference to user-defined 
   * callback function, that gets read access to element
   * data via its parameter @a data - to do whatever is 
   * relevant. In could be a matter of formatting 
   * data for printing on screen, for example.
   * @return Nothing.
   **/
  void BITREEpostorder(BiTree tree, void (*callback)(const void *data));

#ifdef __cplusplus
}
#endif 

#endif /* _BITREE_H_ */
