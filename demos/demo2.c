/**
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: demo2.c
 * Author  : Dan Levin
 * Date    : Wed Jan 21 14:09:11 2015
 * Version : 0.5
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
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dlist.h"

#ifndef OK
#define OK 0
#endif

/* Some string macros for the main menu... */
#define MAIN_MENU_ROW "\n\nMENU: 0=Exit 1=Add_Node 2=Remove_Node 3=Sort_List"
#define MAIN_PROMPT "\nSelection <0-3>+<Enter>: "

#define NR_OF_ITEMS 10
#define MINLEN 5

/* FUNCTION DECLARATIONS */
void my_destroy(void *data);
void print(const void *data);
int my_cmp(const void *key1, const void *key2);
int my_random(int start, int stop);
void my_clearscrn(void);
void prompt_and_pause(char *message);
int my_match(const void *k1, const void *k2);

void create_random_nodes(Dlist list, int nr_of_nodes);

/* Functions handling menu selections */
void rem_nodes(Dlist list);
void ins_nodes(Dlist list);

/* Menu (handling) functions */
int is_sel_ok(const int menusel, const int lowsel, const int hisel);
int menu(const int low_sel, const int hi_sel);
/* END-OF-FUNCTION-DECLARATIONS */

/* FUNCTION DEFINITIONS - the rest of the program */
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

/* --- Function: void create_random_nodes(Dlist list, int nr_of_nodes) --- */
void create_random_nodes(Dlist list, int nr_of_nodes)
{
  int i=0, *pi, retval;

  do
    {
      pi = (int *)malloc(sizeof(int));
      *pi = my_random(1,50);

      /* Defensive programming... */
      if (!DLISTsize(list))
        retval=DLISTinsprev(list, NULL, pi);
      else
        retval=DLISTinsprev(list, DLISThead(list), pi);

      assert(retval == OK);

    } while (++i < nr_of_nodes);
}

/* --- Function: void rem_nodes(Dlist list) --- */
void rem_nodes(Dlist list)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- REMOVE SOME NODES/DATA FROM THE LIST ---");
      printf("\nCurrent list status(%d nodes): ", DLISTsize(list));
      printf("\nForwards : ");
      DLISTtraverse(list, print, DLIST_FWD);
      printf("\nBackwards: ");
      DLISTtraverse(list, print, DLIST_BWD);

      printf("\n\nEnter keydata for node to be removed (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */

      if (tmp == -1)
        break;

      /* Remove node - and free memory */
      pi = &tmp;

      if ((retval = DLISTfind_remove(list, (void **)&pi)) != 0)
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

/* --- Function: void ins_nodes(Dlist list) --- */
void ins_nodes(Dlist list)
{
  int tmp, *pi;
  DlistNode node;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- ADD NODES WITH DATA=99 - AFTER USER-SPECIFIED NODES ---");
      printf("\nCurrent list status(%d nodes): ", DLISTsize(list));
      printf("\nForwards : ");
      DLISTtraverse(list, print, DLIST_FWD);
      printf("\nBackwards: ");
      DLISTtraverse(list, print, DLIST_BWD);

      printf("\n\nEnter nodedata of node after which 99 should be inserted (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */

      if (tmp == -1)
        break;

      if ((node = DLISTfindnode(list, &tmp)) != NULL) /* Node found */
        {
          /* Insert node after first occurance of user-specified node */
          pi = (int *)malloc(sizeof(int));
          *pi = 99;

          if ((DLISTinsnext(list, node, pi)) != 0)
            {
              printf("Fatal error - exiting...");
              exit(-1);
            }
          else
            {
              sprintf(mess, "Node 99 inserted after node %d", *(int *)DLISTdata(node));
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

/* --- Function: void sort_list(Dlist list) --- */
void sort_list(Dlist list)
{
  my_clearscrn();
  printf("--- SORTED LIST ---");
  DLISTsort(list, my_cmp);
  printf("\nCurrent list status(%d nodes): ", DLISTsize(list));
  printf("\nForwards : ");
  DLISTtraverse(list, print, DLIST_FWD);
  printf("\nBackwards: ");
  DLISTtraverse(list, print, DLIST_BWD);
}

/* --- Function: void final_status(Dlist list) --- */
void final_status(Dlist list)
{
  /* Final list status... */
  printf("\nFinal list contents(%d nodes): ", DLISTsize(list));
  printf("\nForwards : ");
  DLISTtraverse(list, print, DLIST_FWD);
  printf("\nBackwards: ");
  DLISTtraverse(list, print, DLIST_BWD);
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
  int retval, selection, sel_ok;

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
  Dlist mylist;
  int menu_choice;

  /* Seed to random generator and clear the screen.. */
  srand((unsigned int)time(NULL));
  my_clearscrn();

  /* Initialize the list.. */
  printf("--- CREATED A DOUBLY-LINKED LIST(%d NODES) - WITH RANDOM INTEGER DATA ---", NR_OF_ITEMS);
  if ((mylist = DLISTinit(my_destroy)) == NULL)
    {
      printf("\nFatal error... - bailing out!");
      exit(-1);
    }

  /* Set match-callback into list structure.. */
  DLISTsetmatch(mylist, my_match);

  /* Populate the (empty) list.. */
  create_random_nodes(mylist, NR_OF_ITEMS);

  /* Display the list... */
  printf("\nCurrent list status(%d nodes): ", DLISTsize(mylist));
  printf("\nForwards : ");
  DLISTtraverse(mylist, print, DLIST_FWD);
  printf("\nBackwards: ");
  DLISTtraverse(mylist, print, DLIST_BWD);

  do
    {
      menu_choice = menu(0, 3);

      switch (menu_choice)
        {
        case 1:
          ins_nodes(mylist);
          break;
        case 2:
          rem_nodes(mylist);
          break;
        case 3:
          sort_list(mylist);
          break;
        default:
          final_status(mylist);
          break;
        }
    }
  while (menu_choice); 

  /* ..and finally destroy the list. */
  prompt_and_pause("\n\nLet's tidy up and destroy the list.. - Bye!");
  DLISTdestroy(mylist);

  return 0;
}
