/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: heap.c
 * Author  : Kyle Loudon/Dan Levin
 * Date    : Fri Mar 22 12:40:45 GMT 2013
 * Version : 0.40
 * ---
 * Description: A heap ADT - written in ANSI C. 
 * 
 */

/**
 * @file heap.c
 **/

#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

/**
 * Macro for level separation when calling HEAPprint()
 * 
 * This macro sets the distance - measured in column 
 * positions - between node levels of the tree when
 * it is printed on screen as a result of a call to 
 * HEAPprint()
 **/
#define HEAP_PRINT_LEVEL_PADDING 4

struct Heap_ {
  int   size;
  int  (*compare)(const void *key1, const void *key2);
  void (*destroy)(void *data);
  void **tree;
}; 

/* STATIC FUNCTION DECLARATIONS */
static void print_tree(Heap hp, int ele_idx, int level, void (*callback)(const void *data));
static int HEAPparent(int npos);
static int HEAPleft(int npos);
static int HEAPright(int npos);


/* --- PUBLIC FUNCTION DEFINITIONS --- */
/* --- Function:   Heap HEAPinit(int (*compare)(const void *key1, const void* key2), void (*destroy)(void *data)) --- */
Heap HEAPinit(int (*compare)(const void *key1, const void* key2), void (*destroy)(void *data))
{
  Heap hp;

  if ((hp = (Heap)malloc(sizeof(struct Heap_)))==NULL)
    return NULL;

  hp->size = 0;
  hp->compare = compare;
  hp->destroy = destroy;
  hp->tree = NULL;
 
  return hp;
}

/* --- Function: void HEAPdestroy(Heap hp) --- */
void HEAPdestroy(Heap hp)
{
  int i;

  if (hp->destroy != NULL)
    {
      for (i = 0; i < HEAPsize(hp); i++)
        hp->destroy(hp->tree[i]);
    }

  free(hp->tree);
  free(hp);
}
  
/* --- Function:   int HEAPinsert(Heap hp, const void *data) --- */
int HEAPinsert(Heap hp, const void *data)
{
  void *tmp;
  int currpos, parentpos;

  /* Allocate storage for the new node */
  if ((tmp = realloc(hp->tree, (HEAPsize(hp) + 1) * sizeof(void *))) == NULL)
    return -1;
  else
    hp->tree = (void **)tmp;

  /* Insert the node after the last node */
  hp->tree[HEAPsize(hp)] = (void *)data;

  /* Heapify the tree by pushing the contents of the new node upward */
  currpos = HEAPsize(hp);
  parentpos = HEAPparent(currpos);

  while (currpos > 0 && hp->compare(hp->tree[parentpos], hp->tree[currpos]) < 0)
    {
      /* Swap the contents of the current node and its parent */
      tmp = hp->tree[parentpos];
      hp->tree[parentpos] = hp->tree[currpos];
      hp->tree[currpos] = tmp;

      /* Move up one level in the tree to continue heapifying */
      currpos = parentpos;
      parentpos = HEAPparent(currpos);
    }

  /* Adjust the size of the heap to account for the inserted node */
  hp->size++;
  
  return 0;
}

/* --- Function: const void *HEAPpeek(Heap hp) --- */
const void *HEAPpeek(Heap hp)
{
  return hp->size ? hp->tree[0] : NULL;
}

/* --- Function:   int HEAPextract(Heap hp, void **data) --- */
int HEAPextract(Heap hp, void **data)
{
  void *save, *temp;
  int currpos, leftpos, rightpos, tmppos;

  /* Do not allow extraction from an empty heap */
  if (HEAPsize(hp) == 0)
    return -1;

  /* Extract and return node data at the top of the heap */
  *data = hp->tree[0];

  /* Store/Backup data of the last noded of the heap */
  save = hp->tree[HEAPsize(hp) - 1];

  /* Adjust(=Shrink) the size of the heap */
  if (HEAPsize(hp) - 1 > 0)
    {
      if ((temp = (void **)realloc(hp->tree, (HEAPsize(hp) - 1) * sizeof(void *))) == NULL)
        return -1;
      else
        hp->tree = temp;
      
      /* Adjust the size of the heap to account for the extracted node */
      hp->size--;
    }
  else 
    {
      /* Manage the heap when extracting the last node - and return from call */
      free(hp->tree);
      hp->tree = NULL;
      hp->size = 0;

      return 0;
    }

  /* Copy the (earlier saved), last node - to the top */
  hp->tree[0] = save;

  /* Heapify the tree by pushing the contents of the new top - downwards... */
  currpos = 0;

  /* leftpos = HEAPleft(currpos); */
  /* rightpos = HEAPright(currpos); */

  while (1) 
    {
      /* Select the child to swap with the current node */
      leftpos = HEAPleft(currpos);
      rightpos = HEAPright(currpos);

      if (leftpos < HEAPsize(hp) && hp->compare(hp->tree[leftpos], hp->tree[currpos]) > 0) 
        tmppos = leftpos;
      else
        tmppos = currpos;

      if (rightpos < HEAPsize(hp) && hp->compare(hp->tree[rightpos], hp->tree[tmppos]) > 0) 
        tmppos = rightpos;

      /* When tmppos is equal to currpos, the heap property has been restored */
      if (tmppos == currpos) 
        {
          /* Time to return to caller... */
          break;
        }
      else 
        {
          /* Swap the contents of the current node and the selected child */
          temp = hp->tree[tmppos];
          hp->tree[tmppos] = hp->tree[currpos];
          hp->tree[currpos] = temp;

          /* Move down one level in the tree to continue heapifying */
          currpos = tmppos;
        }
    }

  return 0;     
}

/* --- Function: int HEAPsize(Heap hp) --- */
int HEAPsize(Heap hp)
{
  return hp->size;
}

/* --- Function: void HEAPprint(Heap hp, void (*callback)(const void *data)) --- */
void HEAPprint(Heap hp, void (*callback)(const void *data))
{
  int i;

  if (hp->size)
    {
      printf("\nHeap: ");

      /* Print the heap - in array format.. */
      for (i = 0; i < hp->size; ++i)
        callback(hp->tree[i]);

      /* Now - print the "heap tree"... */
      printf("\nTree:\n");
      print_tree(hp, 0, 0, callback);
    } 
}

/* --- STATIC FUNCTION DEFINITIONS --- */
/* --- Function: static void print_tree(Heap hp, int ele_idx, int level, void (*callback)(const void *data)) --- */
static void print_tree(Heap hp, int ele_idx, int level, void (*callback)(const void *data))
{
  char *p_msk;

  /* Recursion condition */
  if (ele_idx >= hp->size)
    return;

  /* Print current element data */
  p_msk = (char *)malloc((HEAP_PRINT_LEVEL_PADDING*level+1)*sizeof(char));
  assert(p_msk);
  memset(p_msk, '-', HEAP_PRINT_LEVEL_PADDING*level);
  p_msk[HEAP_PRINT_LEVEL_PADDING*level] = '\0';
  printf("%s", p_msk);
  callback(hp->tree[ele_idx]);
  printf("\n");
  free(p_msk);

  /* Recursively print "subtrees" of the heap... */
  print_tree(hp, HEAPleft(ele_idx), level+1, callback);
  print_tree(hp, HEAPright(ele_idx), level+1, callback);
}

/* --- Function: static int HEAPparent(int npos) --- */
static int HEAPparent(int npos)
{
  return (npos-1)/2;
}

/* --- Function: static int HEAPleft(int npos) --- */
static int HEAPleft(int npos)
{
  return npos*2+1;
}

/* --- Function: static int HEAPright(int npos) --- */
static int HEAPright(int npos)
{
  return npos*2+2;
}
