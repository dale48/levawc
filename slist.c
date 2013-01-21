/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: slist.c
 * Author  : Kyle Loudon/Dan Levin
 * Date    : Mon Dec 03 15:20:15 2012
 * Version : 0.1 
 * ---
 * Description: A singly-linked list - implemented as a pure, generic ADT.
 *
 * Revision history - coming up below:
 *
 * Date        Revision message
 * 2012-12-03  Created this file
 *
 */

/**
 * @file slist.c
 * 
 **/

#include "slist.h"

struct SListElmt_
{
  void               *data;
  struct SListElmt_  *next;
};

struct SList_
{
  int                 size;
  int                 (*match)(const void *key1, const void *key2);
  void                (*destroy)(void *data);
  struct SListElmt_    *head;
  struct SListElmt_    *tail;
};


static void revert(SlistNode node);
static void bwd_traverse(SlistNode node, void (*callback)(const void *data));

/* FUNCTION DEFINITIONS --------------------------------------------------- */

Slist SLISTinit(void (*destroy)(void *data))
{
  Slist list;

  if ((list = (Slist)malloc(sizeof(struct SList_)))==NULL)
    return NULL;

  list->size = 0;
  list->match = NULL;
  list->destroy = destroy;
  list->head = NULL;
  list->tail = NULL;

  return list;
}

void SLISTdestroy(Slist list)
{
  void *data;

  while (SLISTsize(list) > 0) 
    {

      if (SLISTremnext(list, NULL, (void **)&data) == 0 && list->destroy !=
          NULL) 
        list->destroy(data);
    }
  free(list);
}

int SLISTinsnext(Slist list, SlistNode node, const void *data)
{
  SlistNode newelement;

  if ((newelement = (SlistNode)malloc(sizeof(struct SListElmt_))) == NULL)
    return -1;

  newelement->data = (void *)data;

  if (node == NULL) 
    {
      if (list->size == 0)
        list->tail = newelement;

      newelement->next = list->head;
      list->head = newelement;
    }

  else 
    {
      if (node->next == NULL)
        list->tail = newelement;

      newelement->next = node->next;
      node->next = newelement;
    }

  list->size++;

  return 0;
}

int SLISTremnext(Slist list, SlistNode node, void **data)
{

  SlistNode oldelement;

  if (list->size == 0)
    return -1;

  if (node == NULL)
    {
      *data = list->head->data;
      oldelement = list->head;
      list->head = list->head->next;

      if (list->size == 1)   /* Changed according to errata 010607 */
        list->tail = NULL;
    }
  else 
    {
      if (node->next == NULL)
        return -1;

      *data = node->next->data;
      oldelement = node->next;
      node->next = node->next->next;

      if (node->next == NULL)
        list->tail = node;
    }

  free(oldelement);

  list->size--;

  return 0;
}

int SLISTsize(Slist list)
{
  return list->size;
}

SlistNode SLISThead(Slist list)
{
  return list->head;
}

SlistNode SLISTtail(Slist list)
{
  return list->tail;
}

int  SLISTishead(Slist list, SlistNode node)
{
  return list->head==node ? 1 : 0;
}

int  SLISTistail(Slist list, SlistNode node)
{
  return list->tail==node ? 1 : 0;
}

void *SLISTdata(SlistNode node)
{
  return node->data;
}

SlistNode SLISTnext(SlistNode node)
{
  return node->next;
}

SlistNode SLISTfindnode(Slist list, const void *data)
{
  SlistNode member = NULL;

  /* Match callback not set */
  if (list->match == NULL)
    return NULL;
  
  for (member = list->head; member != NULL; member = member->next) 
    {
      if (list->match(data, SLISTdata(member)))
        break;
    }

  return member;
}

int SLISTremnode(Slist list, void **data)
{
  SlistNode member, prev;

  prev = NULL;

  /* If match-callback not set */
  if (list->match == NULL)
    return -1;

  for (member = list->head; member != NULL; member = member->next) 
    {
      if (list->match(*data, member->data))
        break;

      prev = member;
    }

  if (member == NULL)
    return -1;

  return SLISTremnext(list, prev, data);
}

void SLISTsetmatch(Slist list, int (*match)(const void *key1, const void *key2))
{
  list->match = match;
}

void SLISTreverse(Slist list)
{
  SlistNode tmp;

  /* If listsize > 1... */
  if (list->size > 1)
    {
      /* Call 'revert()' to reverse the list physically */
      revert(list->head);
      list->head->next = NULL;

      /* Swap head- and tailpointers */
      tmp = list->head;
      list->head = list->tail;
      list->tail = tmp;
    }
}

static void revert(SlistNode node)
{
  if (!(node->next))
    return;

  revert(node->next);
  node->next->next=node;
}

/* Selection sort for linked list */
void SLISTsort(Slist list, int (*cmp)(const void *key1, const void *key2))
{
  SlistNode curr, maxmin, tmp;
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

static void bwd_traverse(SlistNode node, void (*callback)(const void *data))
{
  if (!(node))
    return;

  bwd_traverse(node->next, callback);
  callback(node->data);
}

void SLISTtraverse(Slist list, void (*callback)(const void *data), int direction)
{
  SlistNode curr;

  if (direction == SLIST_FWD)
    for (curr=list->head; curr != NULL; curr=curr->next)
      callback(curr->data); 

  if (direction == SLIST_BWD)
    {
      curr = list->head;
      bwd_traverse(curr, callback);
    }
}
