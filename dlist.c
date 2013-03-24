/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: dlist.c
 * Author  : Kyle Loudon/Dan Levin
 * Date    : Fri Mar 22 12:40:45 GMT 2013
 * Version : 0.40
 * ---
 * Description: A doubly-linked list - implemented as a pure, generic ADT.
 *
 * Revision history - coming up below:
 *
 * Date        Revision message
 * 2012-12-03  Created this file
 * 2013-02-05  Created a new function 'int DLISTfind_remove(Dlist list, void **data)'
 * 2013-02-19  Made some revision to the Doxygen documentation. Enhanced the description of
 *             in/out parameters - i.e. double-pointers.             
 *
 */

/**
 * @file dlist.c
 * 
 **/

#include "dlist.h"

struct DListElmt_
{
  void             *data;
  struct DListElmt_  *prev;
  struct DListElmt_  *next;
};

struct DList_
{
  int                size;
  int                (*match)(const void *key1, const void *key2);
  void               (*destroy)(void *data);
  struct DListElmt_    *head;
  struct DListElmt_    *tail;
};

/* FUNCTION DEFINITIONS ------------------------------------------------------ */

Dlist DLISTinit(void (*destroy)(void *data))
{
  Dlist list;

  if ((list = (Dlist)malloc(sizeof(struct DList_))) == NULL)
    return NULL;

  list->size = 0;
  list->match = NULL;
  list->destroy = destroy;
  list->head = NULL;
  list->tail = NULL;

  return list;
}

void DLISTdestroy(Dlist list)
{
  void *data;

  while (DLISTsize(list) > 0)
    {
      if (DLISTremove(list, DLISTtail(list), (void **)&data) == 0 && 
          list->destroy != NULL)
        {
          list->destroy(data);
        }
    }
  
  free(list);
}

int DLISTinsnext(Dlist list, DlistNode node, const void *data)
{
  DlistNode newnode;
  
  /* If trying to insert a NULL node into a non-empty list.. */
  if (node == NULL && DLISTsize(list) != 0)
    return -1; /* --- Return error --- */
  
  /* --- Allocate space for a new node --- */
  if ((newnode = (DlistNode)malloc(sizeof(struct DListElmt_))) == NULL)
    return -1;

  newnode->data = (void *)data;

  if (DLISTsize(list) == 0) /* --- List is empty --- */
    {
      /* --- Handle insertion of new node accordingly --- */
      list->head = newnode;
      list->head->prev = NULL;
      list->head->next = NULL;
      list->tail = newnode;
    }
  else /* --- List is not empty --- */
    {
      newnode->next = node->next;
      newnode->prev = node;

      if (node->next == NULL) /* --- Node 'node' is tail.. --- */
        list->tail = newnode;
      else
        node->next->prev = newnode;

      node->next = newnode;
    }
  
  list->size++;

  return 0;
}

int DLISTinsprev(Dlist list, DlistNode node, const void *data)
{
  DlistNode newnode;
  
  /* If trying to insert a NULL node into a non-empty list.. */
  if (node == NULL && DLISTsize(list) != 0)
    return -1; /* --- Return error --- */
  
  /* --- Allocate space for a new node --- */
  if ((newnode = (DlistNode)malloc(sizeof(struct DListElmt_))) == NULL)
    return -1;

  newnode->data = (void *)data;

  if (DLISTsize(list) == 0) /* --- If list is empty --- */
    {
      /* --- Handle insertion of new node accordingly --- */
      list->head = newnode;
      list->head->prev = NULL;
      list->head->next = NULL;
      list->tail = newnode;
    }
  else /* --- List is not empty --- */
    {
      newnode->next = node;
      newnode->prev = node->prev;

      if (node->prev == NULL) /* --- Node 'node' is head.. --- */
        list->head = newnode;
      else
        node->prev->next = newnode;

      node->prev = newnode;
    }
  
  list->size++;

  return 0;
}

int DLISTremove(Dlist list, DlistNode node, void **data)
{
  /* --- If trying to remove a NULL node or if list is empty --- */
  if (node == NULL || DLISTsize(list) == 0)
    return -1;

  /* --- Remove and return data --- */
  *data = node->data;

  if (node == DLISThead(list)) /* --- If 'node' is head of list.. --- */
    {
      /* --- Handle removal of 'node' accordingly --- */
      list->head = node->next;

      if (list->head == NULL) /* --- 'list->head' now pointing at NULL --- */
        list->tail = NULL;
      else
        node->next->prev = NULL;
    }
  else /* --- Handle removal when 'node' is inside list --- */
    {
      node->prev->next = node->next;
      
      if (node->next == NULL) /* --- If 'node' is tail.. --- */
        list->tail = node->prev;
      else
        node->next->prev = node->prev;
    }
  
  /* --- Free memory occupied by 'node' --- */
  free(node);
  
  /* --- Decrease number of nodes in the list --- */
  list->size--;

  return 0;
}

int DLISTsize(Dlist list)
{
  return list->size;
}

DlistNode DLISThead(Dlist list)
{
  return list->head;
}

DlistNode DLISTtail(Dlist list)
{
  return list->tail;
}

int DLISTishead(Dlist list, DlistNode node)
{
  return list->head == node ? 1 : 0;
}

int DLISTistail(Dlist list, DlistNode node)
{
  return list->tail == node ? 1 : 0;
}

void *DLISTdata(DlistNode node)
{
  return node->data;
}

DlistNode DLISTnext(DlistNode node)
{
  return node->next;
}

DlistNode DLISTprev(DlistNode node)
{
  return node->prev;
}

DlistNode DLISTfindnode(Dlist list, const void *data)
{
  DlistNode member = NULL;

  /* If match callback not set */
  if (list->match == NULL)
    return NULL;

  for (member = list->head; member != NULL; member = member->next)
    {
      if (list->match(data, DLISTdata(member)))
        break;
    }
  
  return member;
}

void DLISTsetmatch(Dlist list, int (*match)(const void *key1, const void *key2))
{
  list->match = match;
}

int DLISTfind_remove(Dlist list, void **data)
{
  DlistNode member;

  /* If match-callback not set */
  if (list->match == NULL)
    return -2;

  /* Search list sequentially.. */
  member = DLISTfindnode(list, *data);

  if (member == NULL) /* Node not found */
    return 1;

  /* Perform the removal.. */
  return DLISTremove(list, member, data);
}

void DLISTsort(Dlist list, int (*cmp)(const void *key1, const void *key2))
{
  DlistNode curr, maxmin, tmp;
  void *tmpdata;

  if (list->size > 1)
    {
      curr = list->head;

      while (curr->next)
        {
          maxmin = curr;
          tmp = curr->next;

          while(tmp)
            {
              if (cmp(tmp->data, maxmin->data) < 0)
                maxmin = tmp;
              tmp = tmp->next;
            }

          if (maxmin != curr)
            {
              tmpdata = curr->data;
              curr->data = maxmin->data;
              maxmin->data = tmpdata;
            }
          curr = curr->next;
        }
    }
}

void DLISTtraverse(Dlist list, void (*callback)(const void *data), int direction)
{
  DlistNode curr;

  if (direction == DLIST_FWD)
    for (curr=list->head; curr != NULL; curr=curr->next)
      callback(curr->data); 

  if (direction == DLIST_BWD)
    for (curr=list->tail; curr != NULL; curr=curr->prev)
      callback(curr->data); 
}
