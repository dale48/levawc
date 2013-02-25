/**
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: demo5.c
 * Author  : Dan Levin
 * Date    : Wed Feb 20 11:59:40 GMT 2013
 * Version : 0.15
 * ---
 * Description: Usage demo for heap and priority queue ADT - in LevAWC. 
 *
 * Revision history: (this is where you document the diffs between versions...)
 * Date   Revision
 * 130220 Created this program the first time..
 *
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "pqueue.h"
#include "slist.h"

#define NR_OF_ITEMS 16
#define NR_OF_REMOVALS 3
#define NR_OF_INSERTS 3

#ifndef OK
#define OK 0
#endif

/* Function declarations */
void my_destroy(void *data);
void print(const void *data);
int my_cmp(const void *key1, const void *key2);
int my_random(int start, int stop);
void my_clearscrn(void);
void prompt_and_pause(char *message);

void add_elements(PQueue pq, int nr_of_ele);
void move_elements(PQueue pq, Slist mylst, int nr_of_moves);

/* Function definitions - the rest of the program */
/* --- Function: int my_random(int start, int stop) --- */
int my_random(int start, int stop)
{
  return start+rand()%(stop-start+1);
}

/* --- Function: void my_destroy(void *data) --- */
void my_destroy(void *data)
{
  free(data);
}

/* --- Function: void print(const void *data) --- */
void print(const void *data)
{
  printf(" %02d", *(int *)data);
}

/* --- Function: int my_cmp(const int *key1, const int *key2) --- */
int my_cmp(const void *key1, const void *key2)
{
  return (*(int *)key1 - *(int *)key2);
}

/* --- Function: void my_clearscrn(void) --- */
void my_clearscrn(void)
{
#ifdef __unix__
  system("clear");
#elif _WIN32
  system("cls");
#endif
}

/* --- Function: void prompt_and_pause(char *message) --- */
void prompt_and_pause(char *message)
{
  printf("%s", message);
  printf(" - Hit <Enter> to continue...");
  getchar();
}

/* --- Function: void add_nodes(PQueue pq, int nr_of_nodes) --- */
void add_elements(PQueue pq, int nr_of_ele)
{
  int i=0, *pi, retval;

  do
    {
      /* Get a random integer */
      pi = (int *)malloc(sizeof(int));
      *pi = my_random(1,99);

      /* Insert an integer into priority queue... */
      retval = PQUEUEinsert(pq, pi);
      /* Defensive programming... */
      assert(retval == OK);

    } while (++i < nr_of_ele);

  printf("\nCurrent queue status -- after %d successful insertions...", PQUEUEsize(pq));
  PQUEUEprint(pq, print);
  printf("Read \"Tree\" (above) - columnwise, left to right - and compare to \"Heap\"..\n");
  printf("..do you see any pattern?\n--------");
  printf ("\nNow, let's move the high priority element, value=%d, from the priority queue..\n", *(int *)PQUEUEpeek(pq));
  prompt_and_pause("..and insert it - at the front of a fresh list");
}

/* --- Function: void move_elements(PQueue pq, int nr_of_moves) --- */
void move_elements(PQueue pq, Slist mylst, int nr_of_moves)
{
  int i=0, *pv, retval;

  do
    {
      my_clearscrn();
      /* Extract the top priority element from the queueu... */
      retval = PQUEUEextract(pq, (void **)&pv);
      /* Defensive programming... */
      assert(retval == OK);
      /* Insert this element at the front of a linked list... */
      retval = SLISTinsnext(mylst, NULL, pv);
      /* Defensive programming... */
      assert(retval == OK);
      /* Print the contents of the priority queue... */
      PQUEUEprint(pq, print);
      /* Print the list - from the beginning... */
      printf("\n*******************************************************************************");
      printf("\nList: ");
      SLISTtraverse(mylst, print, SLIST_FWD);
      printf("\n*******************************************************************************\n");
      if (PQUEUEpeek(pq))
	{
	  printf ("Now - move the high priority element, value=%d, from the priority queue...\n", *(int *)PQUEUEpeek(pq));
	  prompt_and_pause("..and insert it - at the front of the list...");
	}
      else
	{
	  printf("\nThe priority queue is now - empty..!");
	}

      i++;
    } while (i < nr_of_moves);
  getchar();
}

int main(void)
{
  /* Declare YOUR variables here ! */
  PQueue mypq;
  Slist mylist;

  srand((unsigned int)time(NULL));
  my_clearscrn();

  printf("--- INITIALIZING A PRIORITY QUEUE, %d ELEMENTS, RANDOM INTEGER DATA ---", NR_OF_ITEMS);
  /* Initialize  the priority queue and linked list - defensive programming...... */
  if ((mypq = PQUEUEinit(my_cmp, my_destroy)) == NULL)
    {
      printf("\nFatal error - bailing out...\n!");
      exit(-1);
    }

  /* Initialize a list - defensive programming... */
  if ((mylist = SLISTinit(my_destroy)) == NULL)
    {
      printf("\nFatal error - bailing out...\n!");
      exit(-1);
    }
 
    
  /* Add elements to the priority queue... */
  add_elements(mypq, NR_OF_ITEMS);

  /* Move all elements from priority queue - to the list... */
  move_elements(mypq, mylist, PQUEUEsize(mypq));

  printf("\nHey - looks like the priority queue can be used for sorting things.. :-)!");
  printf("\n\nThis is, indeed, the case - just Google for \'Heapsort\'...");
  prompt_and_pause("\n\nLet's tidy up and destroy the queue - Bye...!");

  SLISTdestroy(mylist);
  PQUEUEdestroy(mypq);

  return 0;
}
