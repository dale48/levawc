/**
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: demo10.c
 * Author  : Dan Levin
 * Date    : Thu Jan 08 20:06:18 2015
 * Version : 0.1 
 * ---
 * Description: A demo program testing/showing the Open-addressed Hash Table ADT 
 * 
 * Revision history: (this is where you document the diffs between versions...)
 * Date   Revision
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ohashtbl.h"

#ifndef OK
#define OK 0
#endif

#define NR_OF_ITEMS 7
#define NR_OF_SLOTS 11

/* Some string macros for the main menu... */
#define MAIN_MENU_ROW "\n\nMENU: 0=Exit 1=Add_Node 2=Remove_Node 3=Print_Table"
#define MAIN_PROMPT "\nSelection <0-3>+<Enter>: "

/* FUNCTION DECLARATIONS */
void my_destroy(void *data);
void print(const void *data);
int my_cmp(const void *key1, const void *key2);
int my_random(int start, int stop);
void my_clearscrn(void);
void prompt_and_pause(char *message);
int my_match(const void *k1, const void *k2);
int my_hash1(const void *key);
int my_hash2(const void *key);

void create_nodes(OHtbl tbl, int nr_of_nodes);

/* Functions handling menu selections */
void ins_node(OHtbl tbl);
void rem_node(OHtbl tbl);
void print_table(OHtbl tbl);
void final_status(OHtbl tbl);

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
  printf("\n%02d", *(int *)data);
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

/* --- Function: int my_hash1(const void *key) --- */
int my_hash1(const void *key)
{
  return (*(const int *)key % NR_OF_SLOTS);
}

/* --- Function: int my_hash2(const void *key) --- */
int my_hash2(const void *key)
{
  return 1 + (*(const int *)key % (NR_OF_SLOTS - 2));
}

/* --- Function: void add_nodes(OHtbl tbl, int nr_of_nodes) --- */
void create_nodes(OHtbl tbl, int nr_of_nodes)
{
  int i=0, *pi, retval, dupctr=0;

  do
    {
      pi = (int *)malloc(sizeof(int));
      *pi = my_random(1,99);
      
      if ((retval = OHTBLinsert(tbl, pi)) != OK) /* Insertion failed... */
	{
	  if (retval == 1) /* Duplicate key value.. */
	    {
	      dupctr++;
	      free(pi); /* Free node - since duplicate..  */
	    }
	  else
	    {
	      if (retval == -1)
		{
		  prompt_and_pause("Table full - or fatal error..!\n");
		}
	      else
		{
		  prompt_and_pause("Fatal error - bailing out..!\n");
		  exit(-1);
		}
	    }
	}
    } while (++i < nr_of_nodes);

  printf("\nCurrent table status:");
  print_table(tbl);
  printf("\n%d/%d successful insertions -- %d duplicates rejected...", OHTBLsize(tbl), nr_of_nodes, dupctr);
}

/* --- Function: void ins_node(OHtbl tbl) --- */
void ins_node(OHtbl tbl)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("\nCurrent table status(%d elements): ", OHTBLsize(tbl));
      print_table(tbl);

      printf("\nEnter data for element to be inserted (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */

      if (tmp == -1)
	break;

      pi = (int *)malloc(sizeof(int));
      *pi = tmp;

      if ((retval = OHTBLinsert(tbl, pi)) != OK) /* Insertion failed... */
	{
	  if (retval == 1) /* Duplicate key value.. */
	    {
	      sprintf(mess, "Element %d already present..!", *pi);
	      prompt_and_pause(mess);
	      free(pi); /* Free element - since being duplicate..  */
	    }
	  else if (retval == -1)
	    {
	      prompt_and_pause("Table full - no more insertions possible..!\n");
	      free(pi); /* Free element - since being duplicate..  */
	    }
	  else
	    {
	      prompt_and_pause("Fatal error - bailing out..!\n");
	      exit(-1);
	    }
	}
      else
	{
	  sprintf(mess, "Element %d inserted..", *(int *)pi);
	  prompt_and_pause(mess);
	}
    } while (1);
}

/* --- Function: void remove_nodes(OHtbl tbl) --- */
void rem_node(OHtbl tbl)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("\nCurrent table status(%d elements): ", OHTBLsize(tbl));
      print_table(tbl);

      printf("\nEnter data for element to be removed (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */

      if (tmp == -1)
	break;

      pi = &tmp;
      if ((retval = OHTBLremove(tbl, (void **)&pi)) != OK) /* Node removal failed.. */
	{
	  /* Removal didn't work - node NOT found... */
	  if (retval == -1)
	    {
	      sprintf(mess, "Element %d not found..!", *(int *)pi);
	      prompt_and_pause(mess);
	    }
	  else
	    {
	      printf("Fatal failure - bailing out...");
	      exit(retval);
	    }
	}
      else
	{
	  /* Removal succesful - notify user.. */
	  sprintf(mess, "Element %d removed..!", *(int *)pi);
	  prompt_and_pause(mess);
	  /* Free element - after being removed from table.. */
	  free(pi);
	}
    } while (1);
}

/* --- Function: void print_table(parameter_list) --- */
void print_table(OHtbl tbl)
{
  OHTBLprint(tbl, print);
}

/* --- Function: void final_status(OHtbl tbl) --- */
void final_status(OHtbl tbl)
{
  /* Final list status... */
  my_clearscrn();
  printf("\nFinal list contents(%d nodes): ", OHTBLsize(tbl));
  OHTBLprint(tbl, print);
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
  OHtbl mytbl;
  int menu_choice;

  srand((unsigned int)time(NULL));
  my_clearscrn();


  printf("--- INITIALIZING A OPEN-ADDRESSED HASHTABLE, %d ELEMENTS, RANDOM INTEGER DATA ---", NR_OF_ITEMS);

  if ((mytbl = OHTBLinit(NR_OF_SLOTS, my_hash1, my_hash2, my_match, my_destroy)) == NULL)
    {
      printf("\nFatal error - bailing out...\n!");
      exit(-1);
    }
  
  /* Initialize - and add elements to the table... */
  create_nodes(mytbl, NR_OF_ITEMS);

  /* sprintf(msg, "\n\nNow - let's DELETE some elements from the table"); */
  /* prompt_and_pause(msg); */

  /* /\* Do some manual removals... *\/ */
  /* remove_nodes(mytbl); */
  /* my_clearscrn();   */
  /* printf("\nCurrent table status(%d elements): ", OHTBLsize(mytbl)); */
  /* OHTBLprint(mytbl, print); */

  /* sprintf(msg, "\n\nNow - let's ADD some elements to the table"); */
  /* prompt_and_pause(msg); */

  /* /\* Do the manual insertions... *\/ */
  /* insert_nodes(mytbl); */
  /* my_clearscrn();   */
  /* printf("\nFinal table status(%d elements): ", OHTBLsize(mytbl)); */
  /* OHTBLprint(mytbl, print); */

  /* Enter menu... */
  do
    {
      menu_choice = menu(0, 3);

      switch (menu_choice)
        {
        case 1:
          ins_node(mytbl);
          break;
        case 2:
          rem_node(mytbl);
          break;
        case 3:
          print_table(mytbl);
          break;
        default:
          final_status(mytbl);
          break;
        }
    }
  while (menu_choice); 

  prompt_and_pause("\n\nLet's tidy up and destroy the hashtable - Bye...!");
  OHTBLdestroy(mytbl);

  return 0;
}
