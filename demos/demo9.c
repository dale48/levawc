/**
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: demo9.c
 * Author  : Dan Levin
 * Date    : Wed Apr 17 14:37:59 2013
 * Version : 0.1 
 * ---
 * Description: A usage demo program - showing/testing the Set ADT 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "set.h"

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

void add_random_nodes(Set list, int nr_of_nodes);
void remove_nodes(Set list);
void insert_nodes(Set list);

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

/* --- void add_random_nodes(Set set, int nr_of_nodes) --- */
void add_random_nodes(Set set, int nr_of_nodes)
{
  int i=0, *pi, retval, dupctr=0;

  do
    {
      pi = (int *)malloc(sizeof(int));
      *pi = my_random(1,50);

      if ((retval = SETinsert(set, pi)) != 0) /* Insertion failed... */
	{
	  if (retval == 1) /* Duplicate key value.. */
	    {
	      dupctr++;
	      free(pi); /* Free node - since duplicate..  */
	    }
	  else
	    {
	      prompt_and_pause("Fatal error - bailing out..!\n");
	      exit(-1);
	    }
	}
      else
	i++; /* Insertion successful! */

    } while (i < nr_of_nodes);
}

/* --- Function: void remove_nodes(Set set) --- */
void remove_nodes(Set set)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- REMOVE SOME NODES/DATA FROM THE SET ---");
      printf("\nCurrent set status(%d nodes): ", SETsize(set));
      SETtraverse(set, print, SET_FWD);

      printf("\nEnter (key)data for node to be removed (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */

      if (tmp == -1)
	break;

      /* Remove node - and free memory */
      pi = &tmp;

      if ((retval = SETremove(set, (void **)&pi)) != 0)
	{
	  if (retval == 1)
	    {
	      sprintf(mess, "Element %d not found..!", *(int *)pi);
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
	  /* Free element - after being removed from set.. */
	  free(pi);
	}

    } while (1);
}

/* --- Function: void insert_nodes(Set set) --- */
void insert_nodes(Set set)
{
  int tmp, *pi;
  SlistNode node;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- ADD NODES WITH DATA=99 - AFTER USER-SPECIFIED NODES ---");
      printf("\nCurrent set status(%d nodes): ", SETsize(set));
      SETtraverse(set, print, SET_FWD);

      printf("\nEnter nodedata of node after which 99 should be inserted (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */

      if (tmp == -1)
	break;

      if ((SETis_member(set, &tmp)) == 1) /* Node found */
	{
	  /* Insert node after first occurance of user-specified node */
	  pi = (int *)malloc(sizeof(int));
	  *pi = 99;

	  if ((SLISTinsnext(set, node, pi)) != 0)
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
  Set myset;
  char mess[BUFSIZ];
  int sz;

  srand((unsigned int)time(NULL));
  my_clearscrn();

  printf("--- INITIALIZING A SET, %d NODES, INTEGER DATA ---", NR_OF_ITEMS);
  if ((myset = SETinit(my_match, my_destroy)) == NULL) /* Initialize the set */
    {
      printf("\nFatal error... - bailing out!");
      exit(-1);
    }

  /* Populate the (empty) set.. */
  add_random_nodes(myset, NR_OF_ITEMS);
  printf("\nCurrent set status(%d nodes): ", SETsize(myset));
  SETtraverse(myset, print, SET_FWD);

  /* Let the user remove some specified nodes... */
  /* prompt_and_pause("\nNext - let's REMOVE some nodes.."); */
  /* SETsetmatch(myset, my_match); */
  /* remove_nodes(myset); */

  /* Let the user add some nodes - with value=99 - at specified locations.. */
  /* prompt_and_pause("\nFinally - let's ADD some nodes..."); */
  /* if ((sz = SETsize(myset)) < MINLEN) */
  /*   { */
  /*     sprintf(mess, "\nSet too short - adding %d random nodes..", (MINLEN-sz)); */
  /*     prompt_and_pause(mess); */
  /*     add_random_nodes(myset, (MINLEN-sz)); /\* Populate the set *\/ */
  /*     my_clearscrn(); */
  /*   } */
  /* insert_nodes(myset); */

  /* Final set status... */
  printf("\nFinal set contents(%d nodes): ", SETsize(myset));
  SETsort(myset, my_cmp);
  SETtraverse(myset, print, SET_FWD);

  prompt_and_pause("\n\nLet's tidy up and destroy the set.. - Bye!");

  SETdestroy(myset);

  return 0;
}
