/**
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: demo8.c
 * Author  : Dan Levin
 * Date    : Tue Jan 27 10:13:10 2015
 * Version : 0.15
 * ---
 * Description: Demo of the library LevAWC - circular singly-linked lists. 
 *
 * Revision history: (this is where you document the diffs between versions...)
 * Date   Revision
 * 130410 Created this program the first time..
 * 150127 Converted this program, demo8.c, to be menu-driven.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cslist.h"

#ifndef OK
#define OK 0
#endif

#define NR_OF_ITEMS 10

/* Some string macros for the main menu... */
#define MAIN_MENU_ROW "\n\nMENU: 0=Exit 1=Add_Node 2=Remove_Node 3=Print_List"
#define MAIN_PROMPT "\nSelection <0-3>+<Enter>: "

/* FUNCTION DECLARATIONS */
void my_destroy(void *data);
void print(const void *data);
int my_cmp(const void *key1, const void *key2);
int my_random(int start, int stop);
void my_clearscrn(void);
void prompt_and_pause(char *message);
int my_match(const void *k1, const void *k2);

void create_random_nodes(CSlist list, int nr_of_nodes);

/* Functions handling menu selections */
void rem_node(CSlist list);
void ins_node(CSlist list);
void print_cslist(CSlist list);
void final_status(CSlist list);

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

/* --- void create_random_nodes(CSlist list, int nr_of_nodes) --- */
void create_random_nodes(CSlist list, int nr_of_nodes)
{
  int i=0, *pi, retval;

  do
    {
      pi = (int *)malloc(sizeof(int));
      *pi = my_random(1,50);

      if (!CSLISTsize(list))
	retval=CSLISTinsnext(list, NULL, pi);
      else
	retval=CSLISTinsnext(list, CSLISThead(list), pi);

      assert(retval == OK);

    } while (++i < nr_of_nodes);
}

/* --- Function: void ins_node(CSlist list) --- */
void ins_node(CSlist list)
{
  int tmp, *pi;
  CSlistNode node;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- ADD NODES WITH DATA=99 - AFTER USER-SPECIFIED NODES ---");
      printf("\nCurrent list status(%d nodes): ", CSLISTsize(list));
      print_cslist(list);

      printf("\n\nEnter nodedata of node after which 99 should be inserted (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */

      if (tmp == -1)
	break;

      if ((node = CSLISTfindnode(list, &tmp)) != NULL) /* Node found */
	{
	  /* Insert node after first occurance of user-specified node */
	  pi = (int *)malloc(sizeof(int));
	  *pi = 99;

	  if ((CSLISTinsnext(list, node, pi)) != 0)
	    {
	      printf("\nFatal error - exiting...!");
	      exit(-1);
	    }
	  else
	    {
	      sprintf(mess, "Node 99 inserted after node %d", *(int *)CSLISTdata(node));
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

/* --- Function: void rem_node(CSlist list) --- */
void rem_node(CSlist list)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- REMOVE SOME NODES/DATA FROM THE LIST ---");
      printf("\nCurrent list status(%d nodes): ", CSLISTsize(list));
      print_cslist(list);

      printf("\n\nEnter (key)data for node to be removed (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */

      if (tmp == -1)
	break;

      /* Remove node - and free memory */
      pi = &tmp;

      if ((retval = CSLISTfind_remove(list, (void **)&pi)) != 0)
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
	  /* Free element - after being removed from list.. */
	  free(pi);
	}

    } while (1);
}

/* --- Function: void print_cslist(CSlist list) --- */
void print_cslist(CSlist list)
{
  CSLISTtraverse(list, print);
}

/* --- Function: void final_status(CSlist list) --- */
void final_status(CSlist list)
{
  /* Final list status... */
  printf("\nFinal list contents(%d nodes): ", CSLISTsize(list));
  print_cslist(list);
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
  CSlist mycslist;
  int menu_choice;

  srand((unsigned int)time(NULL));
  my_clearscrn();

  printf("--- INITIALIZING A CIRCULAR, SINGLY-LINKED LIST, %d NODES, INTEGER DATA ---", NR_OF_ITEMS);
  if ((mycslist = CSLISTinit(my_destroy)) == NULL) /* Initialize the list */
    {
      printf("\nFatal error... - bailing out!");
      exit(-1);
    }

  /* Populate the (empty) list.. */
  create_random_nodes(mycslist, NR_OF_ITEMS);
  printf("\nCurrent list status(%d nodes): ", CSLISTsize(mycslist));
  print_cslist(mycslist);

  CSLISTsetmatch(mycslist, my_match);

  /* Enter menu... */
  do
    {
      menu_choice = menu(0, 3);

      switch (menu_choice)
        {
        case 1:
          ins_node(mycslist);
          break;
        case 2:
          rem_node(mycslist);
          break;
        case 3:
	  my_clearscrn();
	  printf("--- PRINT CIRCULAR LIST ---\n\nCurrent list status(%d nodes): ", CSLISTsize(mycslist));
	  print_cslist(mycslist);
          break;
        default:
          final_status(mycslist);
          break;
        }
    }
  while (menu_choice); 

  /* ..and finally destroy the list. */
  prompt_and_pause("\n\nLet's tidy up and destroy the list.. - Bye!");
  CSLISTdestroy(mycslist);

  return 0;
}
