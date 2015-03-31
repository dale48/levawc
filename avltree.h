/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: avltree.h
 * Author  : Kyle Loudon/Dan Levin
 * Date    : Fri Mar 22 12:40:45 GMT 2013
 * Version : 0.51
 * ---
 * Description: An AVLtree - with some extensions.
 * 
 * Date   Revision message
 * 150331 This code ready for version 0.51
 */
/**
 * @file avltree.h
 **/

#ifndef _AVLTREE_H_
#define _AVLTREE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * Balance factor for a "left-heavy" node - in the AVL Tree
   * 
   **/
#define            AVL_LFT_HEAVY         1
  /**
   * Balance factor for a "balanced" node - in the AVL Tree
   * 
   **/
#define            AVL_BALANCED          0
  /**
   * Balance factor for a "right-heavy" node - in the AVL Tree
   * 
   **/
#define            AVL_RGT_HEAVY        -1

  /**
   * Use a @b typedef - to hide the interior of @b AvlTreeNode_ - in the 
   * implementation file. This is how @a data @a hiding can be done in C.
   * 
   **/
  typedef struct AvlTreeNode_ *AvlTreeNode;

  /**
   * Use a @b typedef - to hide the interior of @b AvlTree_ - in the 
   * implementation file. This is how @a data @a hiding can be done in C.
   * 
   **/
  typedef struct AvlTree_ *AvlTree;

  /* FUNCTION DECLARATIONS */

  /**
   * Initialize the tree
   * 
   * @param[in] compare - a reference to a user-defined
   * function, responsible for comparing node data. This 
   * callback function should return a value of -1 if
   * data referenced by key1 is less than data referenced
   * by key2 - or 0 if they are equal - or 1 otherwise.
   * The purpose of this function is to implement node 
   * data searching - which is part of functions like
   * AVLTREEinsert(), AVLTREElookup() and AVLTREEremove().
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
  AvlTree AVLTREEinit(int (*compare)(const void *key1, const void *key2), 
                      void (*destroy)(void *data));

  /**
   * Destroy the tree
   * 
   * The tree is destroyed - that is, all dynamically allocated 
   * memory occupied by the elements - will be destroyed.
   * It is the user-defined callback function @a destroy, given 
   * as an argument to @b AVLTREEinit(), that is responsible for 
   * freeing dynamically allocated element data, when this 
   * function is called. If, on the other hand, @a destroy is 
   * set to NULL when @b AVLTREEinit() is called, all element data 
   * will be left untouched after the tree is dismounted and 
   * destroyed. When all elements and data have been 
   * deallocated - the rest of the tree is freed, too.
   * 
   * @param[in] tree - a reference to current tree.
   * @return Nothing.
   * @see AVLTREEinit()
   **/
  void AVLTREEdestroy(AvlTree tree);

  /**
   * Insert a new node, together with data into 
   * the tree.
   * 
   * In case a node with data equal to what is 
   * referenced by parameter @a data already
   * exists, the function returns
   * with a value of 1. If not, a new node is 
   * created to hold data referenced by parameter
   * @a data - and this node is inserted according
   * to the rules for a binary search tree - 
   * that is, at any given node - all nodes in the
   * @a left subtree hold data @a less than data
   * of the new node - and all nodes of the
   * @a right subtree has data @a larger than 
   * that of the new node. This navigation to find
   * the appropriate place for the new node is 
   * accomplished by recursive calls. After the
   * new node is inserted, the balance of ancestor
   * nodes may have been altered. This is checked
   * on the way up through the recursive calls, and
   * adjusted through rotations if the imbalance is 
   * reaching (+/-)2 levels at a particular node.
   * The end result is an almost evenly balanced
   * tree when the recursion has unwinded all the
   * way up to the root node.
   * @param[in] tree - reference to current tree.
   * @param[in] data - reference to data to be 
   * inserted within the new node. It is 
   * the responsability of the caller to ensure,
   * that this memory is valid as long as it is 
   * present in the tree.
   * @return Value 0 if insertion was successful.\n
   * Value 1, if data already exists in the tree.\n
   * Value -1, indicates fatal error.
   * 
   **/
  int AVLTREEinsert(AvlTree tree, const void *data);

  /**
   * Remove/Hide a node - together with its data
   * from the tree.
   * 
   * Removal of a node is implemented as a kind of
   * @a lazy @a removal - where a node, if found in
   * the tree, is not physically removed - just 
   * hidden. This means, that data in the tree must
   * remain valid even @a after it is removed.
   * Consequently, the size of the tree does not
   * decrease after removal.\n\n
   * @a Pros: Simpler, the tree structure is maintained
   * after removal, no rebalancing needed..\n
   * @a Cons: The tree has a certain amount of
   * redundant data, occupied by all hidden nodes.
   * If removals are frequent and numreous, another
   * removal strategy(=physical removal), might
   * be preferred.. 
   * @return Value 0 - if call was successful.\n
   * Value -1, if searched node not found.
   **/
  int AVLTREEremove(AvlTree tree, const void *data);

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
   * 
   * @param[in] tree - reference to current tree.
   * @param[in,out] data - a reference to an external
   * pointer, pointing at the data to be searched for -
   * at the call. Upon return - this pointer has been
   * redirected by this function - and points instead 
   * to data contained in the node hit - if any.
   * @return Value 0 - if call was successful.\n
   * Value -1, if searched node not found.
   **/
  int AVLTREElookup(AvlTree tree, void **data);

  /**
   * Get the size of the tree
   * 
   * @param[in] tree - a reference to the current tree
   *
   * @return The size, i.e. the number of nodes in
   * the tree.
   **/
  int AVLTREEsize(AvlTree tree);

  /**
   * Get a reference to the root node of the tree
   * 
   * @param[in] tree - a reference to the current tree
   *
   * @return A reference to the root node of the tree.
   **/
  AvlTreeNode AVLTREEroot(AvlTree tree);

  /**p
   * Determines whether the parameter @a node is NULL -
   * i.e. "end-of-branch" - or not
   * 
   * @param[in] node - the node to be tested
   *
   * @return Value 1 if @a node is the end of a 
   * branch in the tree - or 0 otherwise.
   **/
  int AVLTREEis_eob(AvlTreeNode node);

  /**
   * Determines if the the parameter @a node is
   * a leaf node, i.e. lacks children - or not
   * 
   * @param[in] node - the node to be tested
   *
   * @return Value 1 if @a node is a leaf - or
   * 0 otherwise.
   **/
  int AVLTREEis_leaf(AvlTreeNode node);

  /**
   * Get a reference to node data
   * 
   * @param[in] node - reference to current node
   *
   * @return Generic reference to node data.
   **/
  void *AVLTREEdata(AvlTreeNode node);

  /**
   * Get a reference to left child node.
   * 
   * @param[in] node - reference to current node
   *
   * @return A reference to left child of 
   * parameter @a node.
   **/
  AvlTreeNode AVLTREEleft(AvlTreeNode node);

  /**
   * Get a reference to right child node.
   * 
   * @param[in] node - reference to current node
   *
   * @return A reference to right child of 
   * parameter @a node.
   **/
  AvlTreeNode AVLTREEright(AvlTreeNode node);

  /**
   * Get the tree height(=nr of levels).
   * 
   * @param[in] tree - reference to current tree
   *
   * @return Tree height - i.e. the max. number
   * of levels in the tree.
   **/
  int AVLTREEheight(AvlTree tree);

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
  void AVLTREEprint(AvlTree tree, void (*callback)(const void *data));

  /**
   * Traverse the entire tree - in @a preorder.
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
   * callback function, that gets read access to
   * element data via its parameter @a data - to
   * do whatever is  relevant. In could be a matter
   * of formatting data for printing on screen, for
   * example.
   * @return Nothing.
   **/
  void AVLTREEpreorder(AvlTree tree, void (*callback)(const void *data));

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
  void AVLTREEinorder(AvlTree tree, void (*callback)(const void *data));

  /**
   * Traverse the entire tree - in @a postorder.
   * 
   * Postorder traversal means the following, where
   * @a recursion is obvious:\n\n
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
  void AVLTREEpostorder(AvlTree tree, void (*callback)(const void *data));

#ifdef __cplusplus
}
#endif 

#endif /* _AVLTREE_H_ */
