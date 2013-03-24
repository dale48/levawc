/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: bitree.c
 * Author  : Kyle Loudon/Dan Levin
 * Date    : Fri Mar 22 12:40:45 GMT 2013
 * Version : 0.40
 * ---
 * Description : A basic, miminal, binary tree ADT - written in ANSI C.
 * 
 * Date        Revision message
 * 2013-02-17  Created this file
 *
 */
/**
 * @file bitree.c
 **/

#include <stdio.h>
#include <stdlib.h>

#include "bitree.h"

/**
 * Macro for level separation when calling BITREEprint()
 * 
 * This macro sets the distance - measured in column 
 * positions - between node levels of the tree when
 * it is printed on screen as a result of a call to 
 * BITREEprint()
 **/
#define BITREE_PRINT_LEVEL_PADDING 4

struct BiTreeNode_ 
{
  void                 *data;
  struct BiTreeNode_   *left;
  struct BiTreeNode_   *right;
};

struct BiTree_ 
{
  int                 size;
  int                 (*compare)(const void *key1, const void *key2);
  void                (*destroy)(void *data);
  struct BiTreeNode_   *root;
};


/* STATIC FUNCTION DECLARATIONS */
static BiTreeNode remnode(BiTree tree, BiTreeNode node, void **data);
static int treeheight(BiTreeNode node, int depth);
static void print_tree(BiTreeNode node, int level, void (*callback)(const void *data));
static void preorder(BiTreeNode node, void (*callback)(const void *data));
static void inorder(BiTreeNode node, void (*callback)(const void *data));
static void postorder(BiTreeNode node, void (*callback)(const void *data));

/* FUNCTION DEFINITIONS -------------------------------------------------------- */
BiTree BITREEinit(void (*destroy)(void *data))
{
  BiTree tree;

  if ((tree = (BiTree)malloc(sizeof(struct BiTree_)))==NULL)
    return NULL;

  tree->size = 0;
  tree->destroy = destroy;
  tree->root = NULL;

  return tree;
}

void BITREEdestroy(BiTree tree)
{
  /* Remove all nodes */
  BITREEremleft(tree, NULL);

  /* Remove tree header */
  free(tree);
}

void BITREEsetcompare(BiTree tree, int (*compare)(const void *key1, const void *key2))
{
  tree->compare = compare;
}

int BITREEinsleft(BiTree tree, BiTreeNode node, const void *data)
{
  BiTreeNode new_node, *position;

  /* Determine where to insert the node... */
  if (node == NULL)
    {
      /* Allow insertion at the root only in an empty tree */
      if (BITREEsize(tree) > 0)
        return -1;

      position = &tree->root;
    }
  else
    {
      /* Normally allow insertion only at the end of a branch */
      if (BITREEleft(node) != NULL)
        return -1;
      
      position = &node->left;
    }

  /* Allocate storage for the node */
  if ((new_node = (BiTreeNode)malloc(sizeof(struct BiTreeNode_))) == NULL)
    return -1;

  /* Insert the node into the tree */
  new_node->data = (void *)data;
  new_node->left = NULL;
  new_node->right = NULL;
  *position = new_node;

  /* Adjust the size of the tree to account for the inserted node */
  tree->size++;
  
  return 0;
}

int BITREEinsright(BiTree tree, BiTreeNode node, const void *data)
{
  BiTreeNode new_node, *position;

  /* Determine where to insert the node... */
  if (node == NULL)
    {
      /* Allow insertion at the root only in an empty tree */
      if (BITREEsize(tree) > 0)
        return -1;

      position = &tree->root;
    }
  else
    {
      /* Normally allow insertion only at the end of a branch */
      if (BITREEright(node) != NULL)
        return -1;
      
      position = &node->right;
    }

  /* Allocate storage for the node */
  if ((new_node = (BiTreeNode)malloc(sizeof(struct BiTreeNode_))) == NULL)
    return -1;

  /* Insert the node into the tree */
  new_node->data = (void *)data;
  new_node->left = NULL;
  new_node->right = NULL;
  *position = new_node;

  /* Adjust the size of the tree to account for the inserted node */
  tree->size++;
  
  return 0;
}

int BITREEinsert(BiTree tree, const void *data)
{
  BiTreeNode node, prev;
  int direction, cmpval;

  if (!tree->compare) /* User-defined compare-callback NOT set... */
    return -2;

  node = BITREEroot(tree); /* Start at the root... */
  direction = 0;

  while (!BITREEis_eob(node)) 
    {
      prev = node;
      cmpval = tree->compare(data, BITREEdata(node));

      if (cmpval == 0) /* Node found - already in the tree! */
        return 1; /* Tell caller, that node is already present... */
      else 
        if (cmpval < 0) /* Go to the left...  */
          {
            node = BITREEleft(node);
            direction = 1;
          }
        else /* Go to the right... */
          {
            node = BITREEright(node);
            direction = 2;
          }
    }

  if (direction == 0) /* Insert a root node in an empty tree... */
    return BITREEinsleft(tree, NULL, data);

  if (direction == 1) /* Insert a new left subnode... */
    return BITREEinsleft(tree, prev, data);

  if (direction == 2) /* Insert a new right subnode... */
    return BITREEinsright(tree, prev, data);

  return -1; /* Should not get here... */
}

int BITREElookup(BiTree tree, void **data)
{
  BiTreeNode node;
  int cmpval;

  if (!tree->compare) /* User-defined compare-callback NOT set... */
    return -2;
  
  node = BITREEroot(tree); /* Start at the root... */

  while (!BITREEis_eob(node)) 
    {
      cmpval = tree->compare(*data, BITREEdata(node));

      if (cmpval == 0) /* Node found! */
        {
          *data = BITREEdata(node);
          return 0;
        }
      else
        if (cmpval < 0)
          node = BITREEleft(node);
        else 
          node = BITREEright(node);
    }

  return -1; /* Node not found... */
}

int BITREEremove(BiTree tree, void **data)
{
  int tmp;

  if ((tmp = BITREElookup(tree, data)) != 0)
    return tmp;
  else
    tree->root =  remnode(tree, tree->root, data);

  return 0; /* Node successfully removed */
}

void BITREEremleft(BiTree tree, BiTreeNode node)
{
  BiTreeNode *position;

  /* Do not allow removal from an empty tree... */
  if (BITREEsize(tree) == 0)
    return;

  /* Determine where to remove nodes... */
  if (node == NULL)
    position = &tree->root;
  else
    position = &node->left;

  /* Remove the nodes... */
  if (*position != NULL) 
    {
      BITREEremleft(tree, *position);
      BITREEremright(tree, *position);

      if (tree->destroy != NULL) 
        {
          /* Call a user-defined function to free dynamically allocated data */
          tree->destroy((*position)->data);
        }

      free(*position);
      *position = NULL;

      /* Adjust the size of the tree to account for the removed node */
      tree->size--;
    }
}

void BITREEremright(BiTree tree, BiTreeNode node)
{
  BiTreeNode *position;

  /* Do not allow removal from an empty tree... */
  if (BITREEsize(tree) == 0)
    return;

  /* Determine where to remove nodes... */
  if (node == NULL)
    position = &tree->root;
  else
    position = &node->right;

  /* Remove the nodes... */
  if (*position != NULL) 
    {
      BITREEremleft(tree, *position);
      BITREEremright(tree, *position);

      if (tree->destroy != NULL) 
        {
          /* Call a user-defined function to free dynamically allocated data */
          tree->destroy((*position)->data);
        }

      free(*position);
      *position = NULL;

      /* Adjust the size of the tree to account for the removed node */
      tree->size--;
    }
}

BiTree BITREEmerge(BiTree left_tree, BiTree right_tree, const void *data)
{
  BiTree merge_tree;
  BiTreeNode node;

  /* Initialize the merged tree */
  if ((merge_tree = BITREEinit(left_tree->destroy)) == NULL)
    return NULL;

  /* Insert the data for the root node of the merged tree */
  if (BITREEinsleft(merge_tree, NULL, data) != 0)
    {
      BITREEdestroy(merge_tree);
      return NULL;
    }

  /* Merge the two binary trees into a single binary tree */
  node = BITREEroot(merge_tree);
  node->left = BITREEroot(left_tree);
  node->right = BITREEroot(right_tree);

  /* Adjust the size of the new binary tree */
  merge_tree->size = merge_tree->size + BITREEsize(left_tree) + BITREEsize(right_tree);

  /* Do not let the original trees access the merged nodes */
  left_tree->root = NULL;
  left_tree->size = 0;
  right_tree->root = NULL;
  right_tree->size = 0;
  
  return merge_tree;
}

int BITREEsize(BiTree tree)
{
  return tree->size;
}

BiTreeNode BITREEroot(BiTree tree)
{
  return tree->root;
}

int BITREEis_eob(BiTreeNode node)
{
  return node == NULL;
}

int BITREEis_leaf(BiTreeNode node)
{
  return node->left == NULL && node->right == NULL;
}

void *BITREEdata(BiTreeNode node)
{
  return node->data;
}

BiTreeNode BITREEleft(BiTreeNode node)
{
  return node->left;
}

BiTreeNode BITREEright(BiTreeNode node)
{
  return node->right;
}

int BITREEheight(BiTree tree)
{
  return treeheight(BITREEroot(tree), 0);
}

void BITREEprint(BiTree tree, void (*callback)(const void *data))
{
  /* Now - print the entire tree... */
  printf("\nTREE STATUS: Size(%d nodes)/Height(%d levels) ---\n", BITREEsize(tree), BITREEheight(tree));
  print_tree(BITREEroot(tree), 0, callback);
}

void BITREEpreorder(BiTree tree, void (*callback)(const void *data))
{
  preorder(tree->root, callback);
}

void BITREEinorder(BiTree tree, void (*callback)(const void *data))
{
  inorder(tree->root, callback);
}

void BITREEpostorder(BiTree tree, void (*callback)(const void *data))
{
  postorder(tree->root, callback);
}

/* STATIC FUNCTION DEFINITIONS ------------------------------------------ */

static BiTreeNode remnode(BiTree tree, BiTreeNode node, 
                          void **data)
{
  BiTreeNode ptmp;
  void *datatmp;
  int cmpval;

  if (node != NULL)
    {
      /* Compare data */
      cmpval = tree->compare(*data, node->data);
    
      if(cmpval < 0)
        node->left = remnode(tree, node->left, data);
      else if (cmpval > 0)
        node->right = remnode(tree, node->right, data);
      else if(node->left && node->right) /* Node to be deleted was found - and has 2 children..! */
        {
          ptmp = node->right;
          /* Search for "smallest" node in right subtree - i.e. turn constantly left.. */
          while (ptmp->left)
            ptmp = ptmp->left;
          /* Swap data */
          datatmp = node->data;
          node->data = ptmp->data;
          ptmp->data = datatmp;
            
          /* Find and remove the matching node (recursively).. */
          node->right = remnode(tree, node->right, data);
        }
      else /* Matching node has less than 2 children.. */
        {
          ptmp = node;
          assert(ptmp);

          /* Link over node 'ptmp' */
          if (node->left == NULL)
            node = node->right;
          else if (node->right == NULL)
            node = node->left;
          /* Hand node data back to caller */
          assert(ptmp->data);
          *data = ptmp->data;

          free(ptmp);
          tree->size--;
        }
    }
  return node;
}

/* --- Function: static void print_tree(BiTree tree, BiTreeNode node, int level, void (*callback)(const void *data)) --- */
static void print_tree(BiTreeNode node, int level, void (*callback)(const void *data))
{
  char *p_msk;

  /* Print current element data */
  p_msk = (char *)malloc((BITREE_PRINT_LEVEL_PADDING*level+1)*sizeof(char));
  assert(p_msk);
  memset(p_msk, '-', BITREE_PRINT_LEVEL_PADDING*level);
  p_msk[BITREE_PRINT_LEVEL_PADDING*level] = '\0';
  printf("%s", p_msk);
  free(p_msk);
  /* Recursion condition */
  if (BITREEis_eob(node))
    {
      printf("NIL");
      printf("\n");      
      return;
    }
  else
    callback(BITREEdata(node));
  printf("\n");      

  /* Recursively traverse and print both "subtrees"... */
  print_tree(BITREEleft(node), level+1, callback);
  print_tree(BITREEright(node), level+1, callback);
}

/* --- Function: void preorder(BiTreeNode node, void (*callback)(const void *data)) --- */
static void preorder(BiTreeNode node, void (*callback)(const void *data))
{
  if (node)
    {
      /* Handle current node... */
      callback(node->data);
      /* Traverse left subtree - recursively in preorder... */
      preorder(node->left, callback);
      /* Traverse right subtree - recursively in preorder... */
      preorder(node->right, callback);
    }
}

/* --- Function: void inorder(BiTreeNode node, void (*callback)(const void *data)) --- */
static void inorder(BiTreeNode node, void (*callback)(const void *data))
{
  if (node)
    {
      /* Traverse left subtree - recursively in inorder... */
      inorder(node->left, callback);
      /* Handle current node... */
      callback(node->data);
      /* Traverse right subtree - recursively in inorder... */
      inorder(node->right, callback);
    }
}

/* --- Function: void postorder(BiTreeNode node, void (*callback)(const void *data)) --- */
static void postorder(BiTreeNode node, void (*callback)(const void *data))
{
  if (node)
    {
      /* Traverse left subtree - recursively in postorder... */
      postorder(node->left, callback);
      /* Traverse right subtree - recursively in postorder... */
      postorder(node->right, callback);
      /* Handle current node... */
      callback(node->data);
    }
}

/* --- Function: int treeheight(BiTreeNode node, int depth) --- */
static int treeheight(BiTreeNode node, int depth)
{
  if (!node)
    return depth;
  else
    return maxval(treeheight(node->left, depth+1),
                  treeheight(node->right, depth+1));
}
