/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: set.c
 * Author  : Kyle Loudon/Dan Levin
 * Date    : Mon Apr 08 12:32:13 2013
 * Version : 0.1 
 * ---
 * Description: A pure, generic set ADT - written in ANSI C 
 * 
 */
/**
 * @file set.c
 **/

#include <stdio.h>
#include <stdlib.h>

#include "set.h"


/* --- Function:   Set SETinit(int (*match)(const void *key1, const void *key2), void (*destroy)(void *data)) --- */
Set SETinit(int (*match)(const void *key1, const void *key2), void (*destroy)(void *data))
{
  Set set;

  set = SLISTinit(destroy);

  if (set)
    SLISTsetmatch(set, match);
  
  return set;
}

/* --- Furnction:   void SETdestroy(Set set) --- */
void SETdestroy(Set set)
{
  SLISTdestroy(set);
}

/* --- Function:   int SETinsert(Set set, const void *data) --- */
int SETinsert(Set set, const void *data)
{
  /* Do not allow the insertion of duplicates */
  if (SETis_member(set, data))
    return 1;

  /* Insert the data */
  return SLISTinsnext(set, SLISTtail(set), data);
}

/* --- Function:   int SETremove(Set set, void **data) --- */
int SETremove(Set set, void **data)
{
  return SLISTfind_remove(set, data);
}

/* --- Function:   Set SETunion(Set set1, Set set2) --- */
Set SETunion(Set set1, Set set2)
{
  Set setu;
  SlistNode member;
  void *data;

  /* Initialize the set for the union */
  setu = SETinit(SLISTgetmatch(set1), NULL);

  /* Insert the members of the first set */
  for (member = SLISThead(set1); member != NULL; member = SLISTnext(member)) 
    {
      data = SLISTdata(member);

      if (SLISTinsnext(setu, SLISTtail(setu), data) != 0)
        {
          SETdestroy(setu);
          return NULL;
        }

    }

  /* Insert the members of the second set */
  for (member = SLISThead(set2); member != NULL; member = SLISTnext(member))
    {
      if (SETis_member(set1, SLISTdata(member)))
        {
          /* Do not allow the insertion of duplicates */
          continue;
        }
      else
        {
          data = SLISTdata(member);

          if (SLISTinsnext(setu, SLISTtail(setu), data) != 0)
            {
              SETdestroy(setu);
              return NULL;
            }
        }
    }

  return setu;
}
  
/* --- Function:   Set SETintersection(Set set1, Set set2) --- */
Set SETintersection(Set set1, Set set2)
{
  Set seti;
  SlistNode member;
  void *data;

  /* Initialize the set for the intersection */
  seti = SETinit(SLISTgetmatch(set1), NULL);

  /* Insert the members present in both sets */
  for (member = SLISThead(set1); member != NULL; member = SLISTnext(member)) 
    {
      if (SETis_member(set2, SLISTdata(member))) 
        {
          data = SLISTdata(member);

          if (SLISTinsnext(seti, SLISTtail(seti), data) != 0)
            {
              SETdestroy(seti);
              return NULL;
            }
        }
    }

  return seti;
}
  
/* --- Function:   Set SETdifference(Set set1, Set set2) --- */
Set SETdifference(Set set1, Set set2)
{
  Set setd;
  SlistNode member;
  void *data;

  /* Initialize the set for the difference */
  setd = SETinit(SLISTgetmatch(set1), NULL);

  /* Insert the members from set1 not in set2 */
  for (member = SLISThead(set1); member != NULL; member = SLISTnext(member))
    {
      if (!SETis_member(set2, SLISTdata(member)))
        {
          data = SLISTdata(member);

          if (SLISTinsnext(setd, SLISTtail(setd), data) != 0)
            {
              SETdestroy(setd);
              return NULL;
            }
        }
    }

  return setd;
}
  
/* --- Function:   int SETis_member(Set set, const void *data) --- */
int SETis_member(Set set, const void *data)
{
  if (SLISTfindnode(set, data) != NULL)
    return 1;
  else
    return 0;
}
  
/* --- Function:   int SETis_subset(const Set set1, const Set set2) --- */
int SETis_subset(const Set set1, const Set set2)
{
  SlistNode tmp;

  /* Do a quick test to rule out some cases */
  if (SETsize(set1) > SETsize(set2))
    return 0;

  /* Determine if set1 is a subset of set2 */
  for (tmp = SLISThead(set1); tmp != NULL; tmp = SLISTnext(tmp)) 
    {
      if (!SETis_member(set2, SLISTdata(tmp)))
        return 0;
    }

  return 1;
}

/* --- Function:   int SETis_equal(Set set1, Set set2) --- */
int SETis_equal(Set set1, Set set2)
{
  /* Do a quick test to rule out some cases */
  if (SETsize(set1) != SETsize(set2))
    return 0;

  /* Sets of the same size are equal if they are subsets */
  return SETis_subset(set1, set2);
}

/* --- Function: int SETsize(Set set) --- */
int SETsize(Set set)
{
  return SLISTsize(set);
}

/* --- Function:   void SETsort(Set set, int (*cmp)(const void *key1, const void *key2)) --- */
void SETsort(Set set, int (*cmp)(const void *key1, const void *key2))
{
  SLISTsort(set, cmp);
}

/* --- Function:   void SETtraverse(Set set, void (*callback)(const void *data), int direction) --- */
void SETtraverse(Set set, void (*callback)(const void *data), int direction)
{
  SLISTtraverse(set, callback, direction);
}
