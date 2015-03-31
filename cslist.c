/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: clist.c
 * Author  : Kyle Loudon/Dan Levin
 * Date    : Mon Apr 08 12:24:12 2013
 * Version : 0.51
 * ---
 * Description: A circular, singly-linked list - implemented as a pure, generic ADT
 * 
 * Date   Revision message
 * 130413 Created this file
 * 150331 This code ready for version 0.51
 *
 */
/**
 * @file cslist.c
 **/

#include <stdio.h>
#include <stdlib.h>

#include "cslist.h"

struct CSListElmt_
{
  void               *data;
  struct CSListElmt_  *next;
};

struct CSList_
{
  int                size;
  int                (*match)(const void *key1, const void *key2);
  void               (*destroy)(void *data);
  struct CSListElmt_  *head;
};

/* FUNCTION DEFINITIONS --------------------------------------------------- */

CSlist CSLISTinit(void (*destroy)(void *data))
{
  CSlist clist;

  if ((clist = (CSlist)malloc(sizeof(struct CSList_)))==NULL)
    return NULL;

  clist->size = 0;
  clist->match = NULL;
  clist->destroy = destroy;
  clist->head = NULL;

  return clist;
}

void CSLISTdestroy(CSlist clist)
{
  void *data;

  while (CSLISTsize(clist) > 0) 
    {
      if (CSLISTremnext(clist, clist->head, (void **)&data) == 0 && clist->destroy !=
          NULL) 
        clist->destroy(data);
    }
  free(clist);
}

int CSLISTinsnext(CSlist clist, CSlistNode node, const void *data)
{
  CSlistNode newnode;

  /* Allocate storage for new node */
  if ((newnode = (CSlistNode)malloc(sizeof(struct CSListElmt_))) == NULL)
    return -1;

  newnode->data = (void *)data;

  if (CSLISTsize(clist) == 0) 
    {
      /* Handle insertion when the list is empty */
      newnode->next = newnode;
      clist->head = newnode;
    }
  else
    {
      /* Handle insertion when the list is not empty */
      newnode->next = node->next;
      node->next = newnode;
    }

  /* Adjust list size to account for the new node */
  clist->size++;

  return 0;
}

int CSLISTremnext(CSlist clist, CSlistNode node, void **data)
{

  CSlistNode oldnode;

  /* Don't allow removal from an empty list */
  if (clist->size == 0)
    return -1;

  /* Remove the node from the list */
  *data = node->next->data;

  if (node->next == node) 
    {
      /* Handle removing the last element */
      oldnode = node->next;
      clist->head = NULL;
    }
  else
    {
      /* Handle removing other than the last element */
      oldnode = node->next;
      node->next = node->next->next;
      
      if (oldnode == clist->head)
        clist->head = oldnode->next;
    }

  /* Free storage occupied by the old element */
  free(oldnode);

  /* Adjust list size to account for the removal */
  clist->size--;

  return 0;
}

int CSLISTsize(CSlist clist)
{
  return clist->size;
}

CSlistNode CSLISThead(CSlist clist)
{
  return clist->head;
}

int CSLISTishead(CSlist clist, CSlistNode node)
{
  return clist->head==node ? 1 : 0;
}

void *CSLISTdata(CSlistNode node)
{
  return node->data;
}

CSlistNode CSLISTnext(CSlistNode node)
{
  return node->next;
}

CSlistNode CSLISTfindnode(CSlist clist, const void *data)
{
  CSlistNode member = NULL;
  int head_hit;

  /* Match callback not set - or empty list - not allowed.. */
  if (clist->match == NULL || clist->size == 0)
    return NULL;
  
  member = clist->head;
  head_hit = 0;

  do
    {
      if (clist->match(data, CSLISTdata(member)))
        {
          if (member == clist->head)
            head_hit = 1;
          break;
        }
      member = member->next;
    } while (member != clist->head);
 
  if (member == clist->head && !head_hit)  /* Node not found */
    return NULL;
  else
    return member;   /* We have a hit - somewhere in the list... */
}

void CSLISTsetmatch(CSlist clist, int (*match)(const void *key1, const void *key2))
{
  clist->match = match;
}

int CSLISTfind_remove(CSlist clist, void **data)
{
  CSlistNode member, prev;
  int head_hit;

  /* If match-callback not set */
  if (clist->match == NULL)
    return -2;

  /* If list empty.. - node can not be found! */
  if (!clist->size)
    return 1;

  /* Initialize 'prev' with node prior to head node... */
  prev = clist->head;
  while (prev->next != clist->head)
    prev = prev->next;

  member = clist->head;
  head_hit = 0;
  
  do
    {
      if (clist->match(*data, CSLISTdata(member)))
        {
          if (member == clist->head)
            head_hit = 1;
          break;
        }
      prev = member;
      member = member->next;
    } while (member != clist->head);

  /* Node not found */
  if (member == clist->head && !head_hit)
    return 1;

  /* Perform the removal.. */
  if (clist->size == 1)
    {
      *data = clist->head->data;
      free(clist->head);
      clist->head = NULL;
      clist->size--;
      return 0;
    }
  else
    return CSLISTremnext(clist, prev, data);
}

void CSLISTtraverse(CSlist clist, void (*callback)(const void *data))
{
  CSlistNode curr;

  if (clist->size)
    {
      curr = clist->head;

      do
        {
          callback(curr->data);
          curr = curr->next;
        } while (curr != clist->head);
    }
}
