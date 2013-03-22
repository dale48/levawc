/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: avltree.c
 * Author  : Kyle Loudon/Dan Levin
 * Date    : Wed Mar 06 09:40:39 2013
 * Version : 0.1 
 * ---
 * Description: An AVL Tree - with some extensions. 
 * 
 */
/**
 * @file avltree.c
 **/

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "avltree.h"

/**
 * Macro for level separation when calling AVLTREEprint()
 * 
 * This macro sets the distance - measured in column 
 * positions - between node levels of the tree when
 * it is printed on screen as a result of a call to 
 * AVLTREEprint()
 **/
#define HEAP_PRINT_LEVEL_PADDING 4

struct AvlTreeNode_ 
{
  void                  *data;
  int                   hidden;
  int                   factor;
  struct AvlTreeNode_   *left;
  struct AvlTreeNode_   *right;
};

struct AvlTree_
{
  int                   size;
  int                   (*compare)(const void *key1, const void *key2);
  void                  (*destroy)(void *data);
  struct AvlTreeNode_   *root;
};

/* STATIC FUNCTION DECLARATIONS */
static void rotate_left(AvlTreeNode *node);
static void rotate_right(AvlTreeNode *node);
static void destroy_left(AvlTree tree, AvlTreeNode node);
static void destroy_right(AvlTree tree, AvlTreeNode node);
static int insert(AvlTree tree, AvlTreeNode *node, const void *data, int *balanced);
static int hide(AvlTree tree, AvlTreeNode node, const void *data);
static int lookup(AvlTree tree, AvlTreeNode node, void **data);
static void print_tree(AvlTreeNode node, int level, void (*callback)(const void *data));
static void preorder(AvlTreeNode node, void (*callback)(const void *data));
static void inorder(AvlTreeNode node, void (*callback)(const void *data));
static void postorder(AvlTreeNode node, void (*callback)(const void *data));
static int treeheight(AvlTreeNode node, int depth);
static int avltree_insleft(AvlTree tree, AvlTreeNode node, const void *data);
static int avltree_insright(AvlTree tree, AvlTreeNode node, const void *data);

/* FUNCTION DEFINITIONS -------------------------------------------------------- */
AvlTree AVLTREEinit(int (*compare)(const void *key1, const void *key2), void (*destroy)(void *data))
{
  AvlTree tree;

  if ((tree = (AvlTree)malloc(sizeof(struct AvlTree_)))==NULL)
    return NULL;

  tree->size = 0;
  tree->compare = compare;
  tree->destroy = destroy;
  tree->root = NULL;

  return tree;
}

void AVLTREEdestroy(AvlTree tree)
{
  destroy_left(tree, NULL);
  free(tree);
}

int AVLTREEinsert(AvlTree tree, const void *data)
{
  int balanced = 0;

  return insert(tree, &tree->root, data, &balanced);
}

int AVLTREEremove(AvlTree tree, const void *data)
{
  return hide(tree, tree->root, data);
}

int AVLTREElookup(AvlTree tree, void **data)
{
  return lookup(tree, tree->root, data);
}

int AVLTREEsize(AvlTree tree)
{
  return tree->size;
}

AvlTreeNode AVLTREEroot(AvlTree tree)
{
  return tree->root;
}

int AVLTREEis_eob(AvlTreeNode node)
{
  return node == NULL;
}

int AVLTREEis_leaf(AvlTreeNode node)
{
  return node->left == NULL && node->right == NULL;
}

void *AVLTREEdata(AvlTreeNode node)
{
  return node->data;
}

AvlTreeNode AVLTREEleft(AvlTreeNode node)
{
  return node->left;
}

AvlTreeNode AVLTREEright(AvlTreeNode node)
{
  return node->right;
}

int AVLTREEheight(AvlTree tree)
{
  return treeheight(AVLTREEroot(tree), 0);
}

void AVLTREEprint(AvlTree tree, void (*callback)(const void *data))
{
  /* Now - print the entire tree... */
  printf("\nAVL TREE STATUS: Size(%d nodes)/Height(%d levels)/(XX=hidden) ---\n", AVLTREEsize(tree), AVLTREEheight(tree));
  print_tree(AVLTREEroot(tree), 0, callback);
}

void AVLTREEpreorder(AvlTree tree, void (*callback)(const void *data))
{
  preorder(tree->root, callback);
}

void AVLTREEinorder(AvlTree tree, void (*callback)(const void *data))
{
  inorder(tree->root, callback);
}

void AVLTREEpostorder(AvlTree tree, void (*callback)(const void *data))
{
  postorder(tree->root, callback);
}

/* STATIC FUNCTION DEFINITIONS ------------------------------------------ */

/* --- Function: static void rotate_left(AvlTreeNode *node) --- */
static void rotate_left(AvlTreeNode *node)
{
  AvlTreeNode left, grandchild;

  left = (*node)->left;

  if (left->factor == AVL_LFT_HEAVY) 
    {
      /* Perform an LL rotation... */
      (*node)->left = left->right;
      left->right = *node;
      (*node)->factor = AVL_BALANCED;
      left->factor = AVL_BALANCED;
      *node = left;
    }
  else 
    {
      /* Perform an LR rotation... */
      grandchild = left->right;
      left->right = grandchild->left;
      grandchild->left = left;
      (*node)->left = grandchild->right;
      grandchild->right = *node;

      switch (grandchild->factor) 
        {
        case AVL_LFT_HEAVY:
          (*node)->factor = AVL_RGT_HEAVY;
          left->factor = AVL_BALANCED;
          break;

        case AVL_BALANCED:
          (*node)->factor = AVL_BALANCED;
          left->factor = AVL_BALANCED;
          break;
        case AVL_RGT_HEAVY:
          (*node)->factor = AVL_BALANCED;
          left->factor = AVL_LFT_HEAVY;
          break;
        }

      grandchild->factor = AVL_BALANCED;
      *node = grandchild;
    }
}

/* --- Function: static void rotate_right(AvlTreeNode *node) --- */
static void rotate_right(AvlTreeNode *node)
{
  AvlTreeNode right, grandchild;
  
  right = (*node)->right;

  if (right->factor == AVL_RGT_HEAVY) 
    {
      /* Perform an RR rotation... */
      (*node)->right = right->left;
      right->left = *node;
      (*node)->factor = AVL_BALANCED;
      right->factor = AVL_BALANCED;
      *node = right;
    }
  else
    {
      /* Perform an RL rotation... */
      grandchild = right->left;
      right->left = grandchild->right;
      grandchild->right = right;
      (*node)->right = grandchild->left;
      grandchild->left = *node;

      switch (grandchild->factor)
        {
        case AVL_LFT_HEAVY:
          (*node)->factor = AVL_BALANCED;
          right->factor = AVL_RGT_HEAVY;
          break;
        case AVL_BALANCED:
          (*node)->factor = AVL_BALANCED;
          right->factor = AVL_BALANCED;
          break;
        case AVL_RGT_HEAVY:
          (*node)->factor = AVL_LFT_HEAVY;
          right->factor = AVL_BALANCED;
          break;
        }

      grandchild->factor = AVL_BALANCED;
      *node = grandchild;
    }
}

/* --- Function: static void destroy_left(AvlTree tree, AvlTreeNode node) --- */
static void destroy_left(AvlTree tree, AvlTreeNode node)
{
  AvlTreeNode *position;

  /* Destruction of an empty tree is not allowed.. */
  if (tree->size == 0)
    return;

  /* Determine where to destroy nodes... */
  if (node == NULL)
    position = &tree->root;
  else
    position = &node->left;

  /* Destroy the nodes... */
  if (*position != NULL)
    {
      destroy_left(tree, *position);
      destroy_right(tree, *position);

      if (tree->destroy != NULL)
        {
          /* Call a user-defined function to free dynamically allocated data */
          tree->destroy((*position)->data);
        }
      /* Now, free the node itself... */
      free(*position);
      *position = NULL;

      /* Adjust the size of the tree to account for the destroyed node... */
      tree->size--;
    }
}

/* --- Function: static void destroy_right(AvlTree tree, AvlTreeNode node) --- */
static void destroy_right(AvlTree tree, AvlTreeNode node)
{
  AvlTreeNode *position;

  /* Destruction of an empty tree is not allowed.. */
  if (tree->size == 0)
    return;

  /* Determine where to destroy nodes... */
  if (node == NULL)
    position = &tree->root;
  else
    position = &node->right;

  /* Destroy the nodes... */
  if (*position != NULL)
    {
      destroy_left(tree, *position);
      destroy_right(tree, *position);

      if (tree->destroy != NULL)
        {
          /* Call a user-defined function to free dynamically allocated data */
          tree->destroy((*position)->data);
        }
      /* Now, free the node itself... */
      free(*position);
      *position = NULL;

      /* Adjust the size of the tree to account for the destroyed node... */
      tree->size--;
    }
}

/* --- Function: static int insert(AvlTree tree, AvlTreeNode *node, const void *data, int *balanced) --- */
static int insert(AvlTree tree, AvlTreeNode *node, const void *data, int *balanced)
{
  //  AvlTreeNode avl_data;
  int cmpval, retval;

  /* Insert the data into the tree... */
  if ((*node) == NULL)
    {
      return avltree_insleft(tree, *node, data);
    }
  else
    {
      /* Handle insertion into a tree that is not empty... */
      cmpval = tree->compare(data, (*node)->data);

      if (cmpval < 0)
        {
          /* Move to the left... */
          if ((*node)->left == NULL)
            {
              if (avltree_insleft(tree, *node, data) != 0)
                return -1;

              *balanced = 0;
            }
          else
            {
              if ((retval = insert(tree, &(*node)->left, data, balanced)) != 0) 
                {
                  return retval;
                }
            }

          /* Ensure that the tree remains balanced... */
          if (!(*balanced))
            {
              switch ((*node)->factor) 
                {
                case AVL_LFT_HEAVY:
                  rotate_left(node);
                  *balanced = 1;
                  break;
                case AVL_BALANCED:
                  (*node)->factor = AVL_LFT_HEAVY;
                  break;
                case AVL_RGT_HEAVY:
                  (*node)->factor = AVL_BALANCED;
                  *balanced = 1;
                }
            }
        } /* if (cmpval < 0) - end */
      else if (cmpval > 0) 
        {
          /* Move to the right... */
          if ((*node)->right == NULL)
            {
              if (avltree_insright(tree, *node, data) != 0)
                return -1;

              *balanced = 0;
            }
          else 
            {
              if ((retval = insert(tree, &(*node)->right, data, balanced)) != 0)
                {
                  return retval;
                }
            }
         
          /* Ensure that the tree remains balanced... */
          if (!(*balanced)) 
            {
              switch ((*node)->factor) 
                {
                case AVL_LFT_HEAVY:
                  (*node)->factor = AVL_BALANCED;
                  *balanced = 1;
                  break;
                case AVL_BALANCED:
                  (*node)->factor = AVL_RGT_HEAVY;
                  break;
                case AVL_RGT_HEAVY:
                  rotate_right(node);
                  *balanced = 1;
                }
            }
        } /* if (cmpval > 0) - end */
      else
        {
          /* Handle finding a copy of the data... */
          if (!((*node)->hidden))
            {
              /* Do nothing since the data is in the tree - and not hidden... */
              return 1;
            }
          else
            {
              /* Insert the new data - and mark it as not hidden... */
              if (tree->destroy != NULL) 
                {
                  /* Destroy the hidden data since it is being replaced.. */
                  tree->destroy((*node)->data);
                }

              (*node)->data = (void *)data;
              (*node)->hidden = 0;

              /* Do not rebalance because the tree structure is unchanged.. */
              *balanced = 1;
            }
        }
    }

  return 0; /* Successful insertion completed! */
}

/* --- Function: static int hide(AvlTree tree, AvlTreeNode node, const void *data) --- */
static int hide(AvlTree tree, AvlTreeNode node, const void *data)
{
  int cmpval, retval;

  if (node == NULL) 
    {
      /* Return that the data was not found... */
      return -1;
    }

  cmpval = tree->compare(data, node->data);

  if (cmpval < 0) 
    {
      /* Move to the left... */
      retval = hide(tree, node->left, data);
    }
  else if (cmpval > 0) 
    {
      /* Move to the right... */
      retval = hide(tree, node->right, data);
    }
  else /* Node found - hidden or not..! */
    {
      if (!(node->hidden))
        {
          /* Mark the node as hidden... */
          node->hidden = 1;
          /* Return success... */
          retval = 0;
        }
      else
        return -1;
    }

  return retval;  
}

/* --- Function: static int lookup(AvlTree tree, AvlTreeNode node, void **data) --- */
static int lookup(AvlTree tree, AvlTreeNode node, void **data)
{
  int cmpval, retval;

  if (node == NULL)
    {
      /* Return that the data was not found... */
      return -1;
    }

  cmpval = tree->compare(*data, node->data);

  if (cmpval < 0)
    {
      /* Move to the left... */
      retval = lookup(tree, node->left, data);
    }
  else if (cmpval > 0)
    {
      /* Move to the right... */
      retval = lookup(tree, node->right, data);
    }
  else
    {
      /* Node found - or hidden..! */
      if (!(node->hidden) )
        {
          /* Pass back the data from the tree... */
          *data = node->data;
          retval = 0;
        }
      else
        {
          /* Return that the data was not found! */
          return -1;
        }
    }

  return retval;
}

static void print_tree(AvlTreeNode node, int level, void (*callback)(const void *data))
{
  char *p_msk;

  /* Print current element data */
  p_msk = (char *)malloc((HEAP_PRINT_LEVEL_PADDING*level+1)*sizeof(char));
  assert(p_msk);
  memset(p_msk, '-', HEAP_PRINT_LEVEL_PADDING*level);
  p_msk[HEAP_PRINT_LEVEL_PADDING*level] = '\0';
  printf("%s", p_msk);
  free(p_msk);
  /* Recursion condition */
  if (AVLTREEis_eob(node))
    {
      printf("NIL");
      printf("\n");      
      return;
    }
  else
    {
      if (node->hidden)
        printf(" XX");
      else 
        callback(AVLTREEdata(node));
      printf("\n");      
    }

  /* Recursively traverse and print both "subtrees"... */
  print_tree(AVLTREEleft(node), level+1, callback);
  print_tree(AVLTREEright(node), level+1, callback);
}

/* --- Function: void preorder(AvlTreeNode node, void (*callback)(const void *data)) --- */
static void preorder(AvlTreeNode node, void (*callback)(const void *data))
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

/* --- Function: void inorder(AvlTreeNode node, void (*callback)(const void *data)) --- */
static void inorder(AvlTreeNode node, void (*callback)(const void *data))
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

/* --- Function: void postorder(AvlTreeNode node, void (*callback)(const void *data)) --- */
static void postorder(AvlTreeNode node, void (*callback)(const void *data))
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

/* --- Function: int treeheight(AvlTreeNode node, int depth) --- */
static int treeheight(AvlTreeNode node, int depth)
{
  if (!node)
    return depth;
  else
    return maxval(treeheight(node->left, depth+1),
                  treeheight(node->right, depth+1));
}

static int avltree_insleft(AvlTree tree, AvlTreeNode node, const void *data)
{
  AvlTreeNode new_node, *position;

  /* Determine where to insert the node... */
  if (node == NULL)
    {
      /* Allow insertion at the root only in an empty tree */
      if (tree->size > 0)
        return -1;

      position = &tree->root;
    }
  else
    {
      /* Normally allow insertion only at the end of a branch */
      if (node->left != NULL)
        return -1;
      
      position = &node->left;
    }

  /* Allocate storage for the node */
  if ((new_node = (AvlTreeNode)malloc(sizeof(struct AvlTreeNode_))) == NULL)
    return -1;

  /* Insert the node into the tree */
  new_node->data = (void *)data;
  new_node->factor = AVL_BALANCED;
  new_node->hidden = 0;
  new_node->left = NULL;
  new_node->right = NULL;
  *position = new_node;

  /* Adjust the size of the tree to account for the inserted node */
  tree->size++;
  
  return 0;
}

static int avltree_insright(AvlTree tree, AvlTreeNode node, const void *data)
{
  AvlTreeNode new_node, *position;

  /* Determine where to insert the node... */
  if (node == NULL)
    {
      /* Allow insertion at the root only in an empty tree */
      if (tree->size > 0)
        return -1;

      position = &tree->root;
    }
  else
    {
      /* Normally allow insertion only at the end of a branch */
      if (node->right != NULL)
        return -1;
      
      position = &node->right;
    }

  /* Allocate storage for the node */
  if ((new_node = (AvlTreeNode)malloc(sizeof(struct AvlTreeNode_))) == NULL)
    return -1;

  /* Insert the node into the tree */
  new_node->data = (void *)data;
  new_node->factor = AVL_BALANCED;
  new_node->hidden = 0;
  new_node->left = NULL;
  new_node->right = NULL;
  *position = new_node;

  /* Adjust the size of the tree to account for the inserted node */
  tree->size++;
  
  return 0;
}
