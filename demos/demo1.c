/**
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: demo1.c
 * Author  : Dan Levin
 * Date    : Tue Jan 20 20:41:54 2015
 * Version : 0.5
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
 * 150120 Started making demo1.c menu-driven. 
 * 150205 Source ready for version 0.5!
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

/* Some string macros for the main menu... */
#define MAIN_MENU_ROW "\n--- SINGLY-LINKED LIST DEMO ---\nMENU: 0=Exit 1=Add_Node 2=Rem_Node 3=Search 4=Sort 5=Print"
#define MAIN_PROMPT "\nSelection <0-5>+<Enter>: "

/* FUNCTION DECLARATIONS */
void my_destroy(void *data);
void print(const void *data);
int my_cmp(const void *key1, const void *key2);
int my_random(int start, int stop);
void my_clearscrn(void);
void prompt_and_pause(char *message);
int my_match(const void *k1, const void *k2);

void create_random_nodes(Slist list, int nr_of_nodes);

/* Functions handling menu selections */
void ins_nodes(Slist list);
void rem_nodes(Slist list);
void search_node(Slist lst);
void sort_list(Slist list);
void print_list(Slist lst);
void final_status(Slist list);

/* Menu (handling) functions */
int is_sel_ok(const int menusel, const int lowsel, const int hisel);
int menu(const int low_sel, const int hi_sel);
/* END-OF-FUNCTION-DECLARATIONS */

/* FUNCTION DEFINITIONS - that is, the rest of the program */
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

/* --- Function: void create_random_nodes(Slist list, int nr_of_nodes) --- */
void create_random_nodes(Slist list, int nr_of_nodes)
{
  int i=0, *pi, retval;

  my_clearscrn();
  /* Initialize the list.. */
  printf("--- CREATED A SINGLY-LINKED LIST(%d NODES)- RANDOM INTEGER DATA ---", NR_OF_ITEMS);

  do
    {
      pi = (int *)malloc(sizeof(int));
      *pi = my_random(1,50);

      retval=SLISTinsnext(list, NULL, pi);
      assert(retval == OK);

    } while (++i < nr_of_nodes);

  /* Display the list... */
  printf("\n\nCurrent list status(%d nodes): ", SLISTsize(list));
  SLISTtraverse(list, print, SLIST_FWD);
  prompt_and_pause("\n\n");
}

/* --- Function: void ins_nodes(Slist list) --- */
void ins_nodes(Slist list)
{
  int tmp, *pi;
  SlistNode node;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- ADD NODE - WITH DATA=99 - AFTER USER-SPECIFIED NODE ---\n");
      printf("\nCurrent list status(%d nodes): ", SLISTsize(list));
      SLISTtraverse(list, print, SLIST_FWD);

      printf("\n\nEnter nodedata, after which new node(data=99) to be inserted (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */

      if (tmp == -1)
        break;

      if ((node = SLISTfindnode(list, &tmp)) != NULL) /* Node found */
        {
          /* Insert node after first occurance of user-specified node */
          pi = (int *)malloc(sizeof(int));
          *pi = 99;

          if ((SLISTinsnext(list, node, pi)) != OK)
            {
              printf("\nFatal error - exiting...!");
              exit(-1);
            }
          else
            {
              sprintf(mess, "Node 99 will be inserted after node %d", *(int *)SLISTdata(node));
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

/* --- Function: void rem_nodes(Slist list) --- */
void rem_nodes(Slist list)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- REMOVE NODE FROM LIST ---\n");
      printf("\nCurrent list status(%d nodes): ", SLISTsize(list));
      SLISTtraverse(list, print, SLIST_FWD);

      printf("\n\nEnter keydata for node to be removed (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */

      if (tmp == -1)
        break;

      /* Remove node - and free memory */
      pi = &tmp;

      if ((retval = SLISTfind_remove(list, (void **)&pi)) != OK)
        {
          if (retval == 1)
            {
              sprintf(mess, "Node %d not found..!", tmp);
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
          sprintf(mess, "Node %d will be removed..!", *(int *)pi);
          prompt_and_pause(mess);
          /* Free node - after being removed from list.. */
          my_destroy(pi);
        }
    } while (1);
}

/* --- Function: void search_node(Slist lst) --- */
void search_node(Slist lst)
{
  int tmp;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- SEARCH NODE ---\n");
      printf("\nCurrent list status(%d nodes): ", SLISTsize(lst));
      SLISTtraverse(lst, print, SLIST_FWD);

      printf("\n\nEnter keydata for node to be found (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */

      if (tmp == -1)
        break;

      if (SLISTfindnode(lst, &tmp) == NULL) /* Node not found.. */
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
    } while (1);
}

/* --- Function: void sort_list(Slist list) --- */
void sort_list(Slist list)
{
  my_clearscrn();
  printf("--- SORT LIST ---\n");
  SLISTsort(list, my_cmp);
  printf("\nCurrent list status(%d nodes, ascending) : ", SLISTsize(list));
  SLISTtraverse(list, print, SLIST_FWD);

  printf("\nCurrent list status(%d nodes, descending): ", SLISTsize(list));
  SLISTtraverse(list, print, SLIST_BWD);
  prompt_and_pause("\n\n");
}

/* --- Function: void print_list(Slist lst) --- */
void print_list(Slist lst)
{
  my_clearscrn();
  printf("--- PRINT LIST ---\n");
  /* List status... */
  printf("\nCurrent list status(%d nodes): ", SLISTsize(lst));
  SLISTtraverse(lst, print, SLIST_FWD);
  prompt_and_pause("\n\n");
}

/* --- Function: void final_status(Slist list) --- */
void final_status(Slist list)
{
  my_clearscrn();
  printf("--- FINAL STATUS ---\n");
  /* Final list status... */
  printf("\nFinal list contents(%d nodes): ", SLISTsize(list));
  SLISTtraverse(list, print, SLIST_FWD);
}

/* --- Function: int is_sel_ok(const int menusel, const int lowsel, const int hisel) --- */
int is_sel_ok(const int menusel, const int lowsel, const int hisel)
{
  int retval;

  return (retval = menusel>=lowsel && menusel<=hisel) ? 1 : 0;
}

/* --- Function: int menu(const int low_sel, const int hi_sel) --- */
int menu(const int low_sel, const int hi_sel)
{
  int retval, selection, sel_ok=0;

  my_clearscrn();

  do
    {
      printf("%s", MAIN_MENU_ROW);
      printf("%s", MAIN_PROMPT);
      retval = scanf("%d", &selection);

      if (retval == 1)
        {
          sel_ok = is_sel_ok(selection, low_sel, hi_sel);
          if (!sel_ok)
            printf("Invalid selection - use <%d> to <%d>...!", low_sel, hi_sel);              
          getchar();   
        }
      else
        {
          printf("Invalid input - use integer only!");
          getchar();
        }

    } while (retval == EOF || !sel_ok);

  return selection;
}

int main(void)
{
  /* Declare YOUR variables here ! */
  Slist mylist;
  int menu_choice;

  /* Seed to random generator and clear the screen.. */
  srand((unsigned int)time(NULL));

  if ((mylist = SLISTinit(my_destroy)) == NULL)
    {
      printf("\nFatal error... - bailing out!");
      exit(-1);
    }

  /* Set match-callback into list structure.. */
  SLISTsetmatch(mylist, my_match);

  /* Populate the (empty) list.. */
  create_random_nodes(mylist, NR_OF_ITEMS);

  /* Enter menu loop... */
  do
    {
      menu_choice = menu(0, 5);

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
  SLISTdestroy(mylist);

  return 0;
}
