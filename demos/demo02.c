/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: demo02.c
 * Author  : Dan Levin
 * Date    : Fri Feb 20 10:11:42 2015
 * Version : 0.51
 * ---
 * Description: A short C demo program testing the function interface of library LevAWC, doubly-linked list. 
 * 
 * Revision history: (this is where you document the diffs between versions...)
 * Date   Revision
 * 121212 Created this program the first time..
 * 121218 After some editing I consider this hack ready for going public :-)
 * 130205 Used the new function 'int DLISTfind_remove()'...
 * 130205 Further editing - more extensive error handling than before..
 * 130411 Extended user interaction when inserting/removing nodes. User determines the number of operations..
 * 150121 Converted demo2.c to be menu-driven. 
 * 150220 Moved some utility functions from here - to file ../utils.c
 * 150220 Source ready for version 0.5!
 * 150317 Source ready for version 0.51
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dlist.h"
#include "utils.h"

#ifndef OK
#define OK 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* Some string macros for the main menu... */
#define MAIN_MENU_ROW "--- DOUBLY-LINKED LIST DEMO ---\nMENU: 0=Exit 1=Add_Node 2=Rem_Node 3=Search 4=Sort 5=Print\nSelection "

#define NR_OF_ITEMS 10
#define MINLEN 5

/* FUNCTION-DECLARATIONS */
/* Application-specific callbacks.. */
void my_destroy(void *data);
void print(const void *data);
int my_cmp(const void *key1, const void *key2);
int my_match(const void *k1, const void *k2);

/* Functions handling menu selections */
void rem_nodes(Dlist list);
void ins_nodes(Dlist list);
void search_node(Dlist lst);
void sort_list(Dlist list);
void print_list(Dlist lst);
void final_status(Dlist list);

/* Misc. application functions.. */
void create_random_nodes(Dlist list, int nr_of_nodes);
/* END-OF-FUNCTION-DECLARATIONS */

/* FUNCTION DEFINITIONS - the rest of the program */
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

/* --- Function: int my_match(const void *k1, const void *k2) --- */
int my_match(const void *k1, const void *k2)
{
  return *(int *)k1 == *(int *)k2;
}

/* --- Function: void create_random_nodes(Dlist list, int nr_of_nodes) --- */
void create_random_nodes(Dlist list, int nr_of_nodes)
{
  int i=0, *pi, retval;

  my_clearscrn();

  /* Initialize the list.. */
  printf("--- CREATED A DOUBLY-LINKED LIST(%d NODES) - RANDOM INTEGER DATA ---", NR_OF_ITEMS);

  do
    {
      pi = (int *)malloc(sizeof(int));
      MALCHK(pi);

      *pi = rand_int(1,50);

      /* Defensive programming... */
      if (!DLISTsize(list))
        retval=DLISTinsprev(list, NULL, pi);
      else
        retval=DLISTinsprev(list, DLISThead(list), pi);

      assert(retval == OK);

    } while (++i < nr_of_nodes);

  /* Display the list... */
  printf("\n\nCurrent list status(%d nodes): ", DLISTsize(list));
  printf("\nAscending : ");
  DLISTtraverse(list, print, DLIST_FWD);
  printf("\nDescending: ");
  DLISTtraverse(list, print, DLIST_BWD);
  prompt_and_pause("\n\n");
}

/* --- Function: void ins_nodes(Dlist list) --- */
void ins_nodes(Dlist list)
{
  int tmp, *pi;
  DlistNode node;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- ADD NODE WITH DATA=99 - AFTER USER-SPECIFIED NODE ---\n");
      printf("\nCurrent list status(%d nodes): ", DLISTsize(list));
      printf("\nAscending : ");
      DLISTtraverse(list, print, DLIST_FWD);

      tmp = read_int("\nEnter (key)data, after which new node(key=99) will be inserted (-1=Quit): ", 0, 0);

      if (tmp == -1)
        break;

      if ((node = DLISTfindnode(list, &tmp)) != NULL) /* Node found */
        {
          /* Insert node after first occurance of user-specified node */
          pi = (int *)malloc(sizeof(int));
          MALCHK(pi);

          *pi = 99;

          if ((DLISTinsnext(list, node, pi)) != OK)
            {
              printf("\nFatal error - exiting...");
              DLISTdestroy(list);
              exit(-1);
            }
          else
            {
              sprintf(mess, "Node 99 will be inserted after node %d", *(int *)DLISTdata(node));
              prompt_and_pause(mess);
            }
        }
      else
        {
          sprintf(mess, "Error: Node %d not found...!", tmp);
          prompt_and_pause(mess);
        }
    } while (TRUE);
}

/* --- Function: void rem_nodes(Dlist list) --- */
void rem_nodes(Dlist list)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- REMOVE NODE FROM LIST ---\n");
      printf("\nCurrent list status(%d nodes): ", DLISTsize(list));
      printf("\nAscending : ");
      DLISTtraverse(list, print, DLIST_FWD);
      printf("\nDescending: ");
      DLISTtraverse(list, print, DLIST_BWD);

      tmp = read_int("\nEnter keydata for node to be removed (-1=Quit): ", 0, 0);

      if (tmp == -1)
        break;

      /* Remove node - and free memory */
      pi = &tmp;

      if ((retval = DLISTfind_remove(list, (void **)&pi)) != OK)
        {
          if (retval == 1)
            {
              sprintf(mess, "Error: Node %d not found..!", tmp);
              prompt_and_pause(mess);
            }
          else 
            {
              if (retval == -2)
                printf("\nError: Match-callback is missing... - bailing out!");
              else
                printf("\nFatal error... - bailing out!");
              DLISTdestroy(list);
              exit(retval);
            }
        }
      else
        {
          /* Removal succesful - notify user.. */
          sprintf(mess, "Node %d will be removed..!", *(int *)pi);
          prompt_and_pause(mess);
          /* Free node - after being removed from list.. */
          my_destroy(pi);
        }
    } while (TRUE);
}
/* --- Function: void search_node(Dlist lst) --- */
void search_node(Dlist lst)
{
  int tmp;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- SEARCH NODE ---\n");
      printf("\nCurrent list status(%d nodes): ", DLISTsize(lst));
      DLISTtraverse(lst, print, DLIST_FWD);

      tmp = read_int("\nEnter keydata for node to be found (-1=Quit): ", 0, 0);

      if (tmp == -1)
        break;

      if (DLISTfindnode(lst, &tmp) == NULL) /* Node not found.. */
        {
          sprintf(mess, "Node %d NOT found..!", tmp);
          prompt_and_pause(mess);
        }
      else
        {
          /* Search succesful - notify user.. */
          sprintf(mess, "Node %d FOUND!", tmp);
          prompt_and_pause(mess);
        }
    } while (TRUE);
}

/* --- Function: void sort_list(Dlist list) --- */
void sort_list(Dlist list)
{
  my_clearscrn();
  printf("--- SORT LIST ---\n");
  DLISTsort(list, my_cmp);
  printf("\nCurrent list status(%d nodes): ", DLISTsize(list));
  printf("\nAscending : ");
  DLISTtraverse(list, print, DLIST_FWD);
  printf("\nDescending: ");
  DLISTtraverse(list, print, DLIST_BWD);
  prompt_and_pause("\n\n");
}

/* --- Function: void print_list(Dlist lst) --- */
void print_list(Dlist lst)
{
  my_clearscrn();
  printf("--- PRINT LIST ---\n");
  /* Print list status... */
  printf("\nCurrent list contents(%d nodes): ", DLISTsize(lst));
  printf("\nAscending : ");
  DLISTtraverse(lst, print, DLIST_FWD);
  printf("\nDescending: ");
  DLISTtraverse(lst, print, DLIST_BWD);
  prompt_and_pause("\n\n");
}

/* --- Function: void final_status(Dlist list) --- */
void final_status(Dlist list)
{
  my_clearscrn();
  printf("--- FINAL STATUS ---\n");
  /* Final list status... */
  printf("\nFinal list contents(%d nodes): ", DLISTsize(list));
  printf("\nAscending : ");
  DLISTtraverse(list, print, DLIST_FWD);
  printf("\nDescending: ");
  DLISTtraverse(list, print, DLIST_BWD);
}

int main(void)
{
  /* Declare YOUR variables here ! */
  Dlist mylist;
  int menu_choice;

  /* Seed to random generator and clear the screen.. */
  srand((unsigned int)time(NULL));

  if ((mylist = DLISTinit(my_destroy)) == NULL)
    {
      printf("\nFatal error... - bailing out!");
      DLISTdestroy(mylist);
      exit(-1);
    }

  /* Set match-callback into list structure.. */
  DLISTsetmatch(mylist, my_match);

  /* Populate the (empty) list.. */
  create_random_nodes(mylist, NR_OF_ITEMS);

  do
    {
      menu_choice = menu(MAIN_MENU_ROW, 0, 5);

      switch (menu_choice)
        {
        case 1:
          ins_nodes(mylist);
          break;
        case 2:
          rem_nodes(mylist);
          break;
        case 3:
          search_node(mylist);
          break;
        case 4:
          sort_list(mylist);
          break;
        case 5:
          print_list(mylist);
          break;
        default:
          final_status(mylist);
          break;
        }
    }
  while (menu_choice); 

  /* ..and finally destroy the list. */
  prompt_and_pause("\n\nLet's tidy up and destroy the list..- Bye!");
  DLISTdestroy(mylist);

  return 0;
}
