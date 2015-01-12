/**
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: demo1.c
 * Author  : Dan Levin
 * Date    : Wed Feb 20 11:59:40 GMT 2013
 * Version : 0.15
 * ---
 * Description: A first demo of the library LevAWC - singly-linked lists. 
 *
 * Revision history: (this is where you document the diffs between versions...)
 * Date   Revision
 * 121212 Created this program the first time..
 * 121218 After some editing I consider this hack ready for going public :-)
 * 130205 Used the new function 'int SLISTfind_remove()' (instead of the old 'int SLISTremnode()'..)
 * 130205 Further editing - more extensive error handling than before..
 * 130411 Extended user interaction when inserting/removing nodes. User determines the number of operations..
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "slist.h"

#ifndef OK
#define OK 0
#endif

#define NR_OF_ITEMS 10
#define MINLEN 5

/* Function declarations */
void my_destroy(void *data);
void print(const void *data);
int my_cmp(const void *key1, const void *key2);
int my_random(int start, int stop);
void my_clearscrn(void);
void prompt_and_pause(char *message);
int my_match(const void *k1, const void *k2);

void add_random_nodes(Slist list, int nr_of_nodes);
void remove_nodes(Slist list);
void insert_nodes(Slist list);

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

/* --- Function: int my_match(const void *k1, const void *k2) --- */
int my_match(const void *k1, const void *k2)
{
  return *(int *)k1 == *(int *)k2;
}

/* --- void add_random_nodes(Slist list, int nr_of_nodes) --- */
void add_random_nodes(Slist list, int nr_of_nodes)
{
  int i=0, *pi, retval;

  do
    {
      pi = (int *)malloc(sizeof(int));
      *pi = my_random(1,50);

      retval=SLISTinsnext(list, NULL, pi);
      assert(retval == OK);

    } while (++i < nr_of_nodes);
}

/* --- Function: void remove_nodes(Slist list) --- */
void remove_nodes(Slist list)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- REMOVE SOME NODES/DATA FROM THE LIST ---");
      printf("\nCurrent list status(%d nodes): ", SLISTsize(list));
      SLISTtraverse(list, print, SLIST_FWD);

      printf("\nEnter keydata for node to be removed (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */

      if (tmp == -1)
	break;

      /* Remove node - and free memory */
      pi = &tmp;

      if ((retval = SLISTfind_remove(list, (void **)&pi)) != 0)
	{
	  if (retval == 1)
	    {
	      sprintf(mess, "Element %d not found..!", tmp);
	      prompt_and_pause(mess);
	    }
	  else 
	    {
	      if (retval == -2)
		printf("\nMatch-callback is missing... - bailing out!");
	      else
 		printf("\nFatal error... - bailing out!");
	      exit(retval);
	    }
	}
      else
	{
	  /* Removal succesful - notify user.. */
	  sprintf(mess, "Node %d removed..!", *(int *)pi);
	  prompt_and_pause(mess);
	  /* Free element - after being removed from list.. */
	  free(pi);
	}

    } while (1);
}

/* --- Function: void insert_nodes(Slist list) --- */
void insert_nodes(Slist list)
{
  int tmp, *pi;
  SlistNode node;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- ADD NODES WITH DATA=99 - AFTER USER-SPECIFIED NODES ---");
      printf("\nCurrent list status(%d nodes): ", SLISTsize(list));
      SLISTtraverse(list, print, SLIST_FWD);

      printf("\nEnter nodedata of node after which 99 should be inserted (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */

      if (tmp == -1)
	break;

      if ((node = SLISTfindnode(list, &tmp)) != NULL) /* Node found */
	{
	  /* Insert node after first occurance of user-specified node */
	  pi = (int *)malloc(sizeof(int));
	  *pi = 99;

	  if ((SLISTinsnext(list, node, pi)) != 0)
	    {
	      printf("\nFatal error - exiting...!");
	      exit(-1);
	    }
	  else
	    {
	      sprintf(mess, "Node 99 inserted after node %d", *(int *)SLISTdata(node));
	      prompt_and_pause(mess);
	    }
	}
      else
	{
	  sprintf(mess, "Node %d not found...!", tmp);
	  prompt_and_pause(mess);
	}
    } while (1);
}

int main(void)
{
  /* Declare YOUR variables here ! */
  Slist mylist;
  char mess[BUFSIZ];
  int sz;

  /* Seed to random generator and clear the screen.. */
  srand((unsigned int)time(NULL));
  my_clearscrn();

  /* Initialize the list.. */
  printf("--- INITIALIZING A SINGLY-LINKED LIST, %d NODES, RANDOM INTEGER DATA ---", NR_OF_ITEMS);
  if ((mylist = SLISTinit(my_destroy)) == NULL)
    {
      printf("\nFatal error... - bailing out!");
      exit(-1);
    }

  /* Populate the (empty) list.. */
  add_random_nodes(mylist, NR_OF_ITEMS);
  printf("\nCurrent list status(%d nodes): ", SLISTsize(mylist));
  SLISTtraverse(mylist, print, SLIST_FWD);

  /* Sort the list.. */
  prompt_and_pause("\nNext - let's SORT the list...");
  printf("--- SORTED LIST ---");
  SLISTsort(mylist, my_cmp);
  printf("\nCurrent list status(%d nodes): ", SLISTsize(mylist));
  SLISTtraverse(mylist, print, SLIST_FWD);

  /* Traverse the list - backwards.. */
  prompt_and_pause("\nNow - let's TRAVERSE the list - backwards...");
  printf("--- LIST TRAVERSED - BACKWARDS ---");
  printf("\nCurrent list status(%d nodes): ", SLISTsize(mylist));
  SLISTtraverse(mylist, print, SLIST_BWD);

  /* Let the user remove some specified nodes... */
  prompt_and_pause("\n\nNext - let's REMOVE some nodes..");
  SLISTsetmatch(mylist, my_match);
  remove_nodes(mylist);

  /* Let the user add some nodes - with value=99 - at specified locations.. */
  prompt_and_pause("\nFinally - let's ADD some nodes...");
  if ((sz = SLISTsize(mylist)) < MINLEN)
    {
      sprintf(mess, "\nList too short - adding %d random nodes..", (MINLEN-sz));
      prompt_and_pause(mess);
      add_random_nodes(mylist, (MINLEN-sz)); /* Populate the list */
    }
  insert_nodes(mylist);

  /* Final list status... */
  printf("\nFinal list contents(%d nodes): ", SLISTsize(mylist));
  SLISTtraverse(mylist, print, SLIST_FWD);

  /* ..and finally destroy the list. */
  prompt_and_pause("\n\nLet's tidy up and destroy the list.. - Bye!");
  SLISTdestroy(mylist);

  return 0;
}
