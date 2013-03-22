/**
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: demo6.c
 * Author  : Dan Levin
 * Date    : Wed Feb 20 11:59:40 GMT 2013
 * Version : 0.15
 * ---
 * Description: Usage demo of the AVL tree ADT - in LevAWC. 
 *
 * Revision history: (this is where you document the diffs between versions...)
 * Date   Revision
 * 130312 Created this program the first time..
 *
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "avltree.h"
#include "utils.h"

#ifndef OK
#define OK 0
#endif

#define NR_OF_ITEMS 9

/* Function declarations */
void my_destroy(void *data);
void print(const void *data);
int my_cmp(const void *key1, const void *key2);
int my_random(int start, int stop);
void my_clearscrn(void);
void prompt_and_pause(char *message);

void add_nodes(AvlTree tree, int nr_of_nodes);
void remove_nodes(AvlTree tree);
void insert_nodes(AvlTree tree);

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

/* --- Function: void add_nodes(AvlTree tree, int nr_of_nodes) --- */
void add_nodes(AvlTree tree, int nr_of_nodes)
{
  int i=0, *pi, retval, dupctr=0;

  do
    {
      pi = (int *)malloc(sizeof(int));
      *pi = my_random(1,99);
      
      if ((retval = AVLTREEinsert(tree, pi)) != 0) /* Insertion failed... */
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
    } while (++i < nr_of_nodes);

  AVLTREEprint(tree, print);
  printf("INORDER: ");
  AVLTREEinorder(tree, print);
  printf("\n%d/%d successful insertions -- %d duplicates rejected...", AVLTREEsize(tree), nr_of_nodes, dupctr);
}

/* --- Function: void remove_nodes(AvlTree tree, int nr_of_removes) --- */
void remove_nodes(AvlTree tree)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      AVLTREEprint(tree, print);
      printf("INORDER: ");
      AVLTREEinorder(tree, print);

      printf("\nEnter data for node to be removed (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */
      
      if (tmp == -1)
	break;

      pi = &tmp;
      if ((retval = AVLTREEremove(tree, pi)) != 0) /* Node removal failed.. */
	{
	  /* Removal didn't work - node NOT found... */
	  if (retval == -1)
	    {
	      sprintf(mess, "Element %d not found..!", *(int *)pi);
	      prompt_and_pause(mess);
	    }
	  else /* Serious failure..(-2) */
	    {
	      printf("Fatal failure - bailing out...");
	      getchar();
	      exit(retval);
	    }
	}
      else
	{
	  /* Removal succesful - notify user.. */
	  sprintf(mess, "Element %d successfully removed..!", *(int *)pi);
	  prompt_and_pause(mess);
	  /* Free element - after being removed from tree.. */
	  /*  free(pi); */
	}
    } while (1);
}

/* --- Function: void insert_nodes(AvlTree tree, int nr_of_insertions) --- */
void insert_nodes(AvlTree tree)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      AVLTREEprint(tree, print);
      printf("INORDER: ");
      AVLTREEinorder(tree, print);

      printf("\nEnter data for node to be inserted (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */

      if (tmp == -1)
	break;

      pi = (int *)malloc(sizeof(int));
      *pi = tmp;

      if ((retval = AVLTREEinsert(tree, pi)) != 0) /* Insertion failed... */
	{
	  if (retval == 1) /* Duplicate key value.. */
	    {
	      sprintf(mess, "Element %d already present..!", *pi);
	      prompt_and_pause(mess);
	      free(pi); /* Free element - since being duplicate..  */
	    }
	  else
	    {
	      prompt_and_pause("Fatal error - bailing out..:!\n");
	      exit(-1);
	    }
	}
      else
	{
	  sprintf(mess, "Element %d successfully inserted..", *(int *)pi);
	  prompt_and_pause(mess);
	}
    } while (1);
}

int main(void)
{
  /* Declare YOUR variables here ! */
  AvlTree mytree;
  char msg[BUFSIZ];

  srand((unsigned int)time(NULL));
  my_clearscrn();


  printf("--- INITIALIZING AN AVL TREE, %d ELEMENTS, RANDOM INTEGER DATA ---", NR_OF_ITEMS);

  if ((mytree = AVLTREEinit(my_cmp, my_destroy)) == NULL)
    {
      printf("\nFatal error - bailing out...\n!");
      exit(-1);
    }
  
  /* Initialize - and add elements to the table... */
  add_nodes(mytree, NR_OF_ITEMS);
  
  sprintf(msg, "\nNow - let's DELETE some nodes in the tree...");
  prompt_and_pause(msg);

  /* Do the manual removals... */
  remove_nodes(mytree);
  my_clearscrn();  
  AVLTREEprint(mytree, print);
  printf("INORDER: ");
  AVLTREEinorder(mytree, print);
  
  sprintf(msg, "\nNow - let's ADD some nodes to the tree..");
  prompt_and_pause(msg);

  /* Do the manual insertions... */
  insert_nodes(mytree);
  my_clearscrn();  
  AVLTREEprint(mytree, print);
  printf("INORDER: ");
  AVLTREEinorder(mytree, print);
  
  prompt_and_pause("\n\nLet's tidy up and destroy the search tree - Bye...!");

  AVLTREEdestroy(mytree);
  
  return 0;
}
