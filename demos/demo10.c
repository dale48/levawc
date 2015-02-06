/**
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: demo10.c
 * Author  : Dan Levin
 * Date    : Thu Jan 08 20:06:18 2015
 * Version : 0.5
 * ---
 * Description: A demo program testing/showing the Open-addressed Hash Table ADT 
 * 
 * Revision history: (this is where you document the diffs between versions...)
 * Date   Revision
 * 150104 Made this demo10.c menu-driven.
 * 150206 This source is ready for version 0.5!
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
#define MAIN_MENU_ROW "\n--- OPEN-ADDRESSED HASH TABLE DEMO ---\nMENU: 0=Exit 1=Add_Node 2=Rem_Node 3=Search 4=Print"
#define MAIN_PROMPT "\nSelection <0-4>+<Enter>: "

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
void search_node(OHtbl tbl);
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

  my_clearscrn();
  printf("--- INITIALIZING A OPEN-ADDRESSED HASHTABLE, %d NODES, RANDOM INTEGER DATA ---", NR_OF_ITEMS);

  do
    {
      pi = (int *)malloc(sizeof(int));
      *pi = my_random(1,99);
      
      if ((retval = OHTBLinsert(tbl, pi)) != OK) /* Insertion failed... */
        {
          if (retval == 1) /* Duplicate key value.. */
            {
              dupctr++;
              my_destroy(pi); /* Free node - since duplicate..  */
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

  printf("\n\nCurrent table status:");
  print_table(tbl);
  printf("\n\n%d/%d successful insertions -- %d duplicate(s) rejected...", OHTBLsize(tbl), nr_of_nodes, dupctr);
  prompt_and_pause("\n\n");
}

/* --- Function: void ins_node(OHtbl tbl) --- */
void ins_node(OHtbl tbl)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- INSERT NODE ---");
      printf("\n\nCurrent table status(%d nodes): ", OHTBLsize(tbl));
      print_table(tbl);

      printf("\n\nEnter data for node to be inserted (-1=Quit): ");
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
              sprintf(mess, "Node %d already present..!", *pi);
              prompt_and_pause(mess);
              my_destroy(pi); /* Free node - since being duplicate..  */
            }
          else if (retval == -1)
            {
              prompt_and_pause("Table full - no more insertions possible..!\n");
              my_destroy(pi); /* Free node - since being duplicate..  */
            }
          else
            {
              prompt_and_pause("Fatal error - bailing out..!\n");
              exit(-1);
            }
        }
      else
        {
          sprintf(mess, "Node %d will be inserted..", *(int *)pi);
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
      printf("--- REMOVE NODE ---");
      printf("\n\nCurrent table status(%d nodes): ", OHTBLsize(tbl));
      print_table(tbl);

      printf("\n\nEnter data for node to be removed (-1=Quit): ");
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
              sprintf(mess, "Node %d not found..!", *(int *)pi);
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
          sprintf(mess, "Node %d will be removed..!", *(int *)pi);
          prompt_and_pause(mess);
          /* Free node - after being removed from table.. */
          my_destroy(pi);
        }
    } while (1);
}

/* --- Function: void search_node(OHtbl tbl) --- */
void search_node(OHtbl tbl)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- SEARCH NODE ---\n");
      print_table(tbl);
      printf("\n\nEnter data for node to be found (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */
      
      if (tmp == -1)
        break;

      pi = &tmp;

      if ((retval = OHTBLlookup(tbl, (void **)&pi)) != OK) /* Node search failed.. */
        {
          /* Searching didn't work - node NOT found... */
          if (retval == -1)
            {
              sprintf(mess, "Node %d NOT found..!", *(int *)pi);
              prompt_and_pause(mess);
            }
          else /* Should not get here.. */
            {
              printf("Fatal failure - bailing out...");
              getchar();
              exit(retval);
            }
        }
      else
        {
          /* Removal succesful - notify user.. */
          sprintf(mess, "Node %d FOUND..!", *(int *)pi);
          prompt_and_pause(mess);
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
  printf("--- FINAL STATUS ---");
  printf("\n\nFinal table contents(%d nodes): ", OHTBLsize(tbl));
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
  OHtbl mytbl;
  int menu_choice;

  srand((unsigned int)time(NULL));

  if ((mytbl = OHTBLinit(NR_OF_SLOTS, my_hash1, my_hash2, my_match, my_destroy)) == NULL)
    {
      printf("\nFatal error - bailing out...\n!");
      exit(-1);
    }
  
  /* Initialize - and add nodes to the table... */
  create_nodes(mytbl, NR_OF_ITEMS);

  /* Enter menu loop... */
  do
    {
      menu_choice = menu(0, 4);

      switch (menu_choice)
        {
        case 1:
          ins_node(mytbl);
          break;
        case 2:
          rem_node(mytbl);
          break;
        case 3:
          search_node(mytbl);
          break;
        case 4:
          my_clearscrn();
          printf("--- PRINT TABLE ---");
          printf("\n\nFinal table contents(%d nodes): ", OHTBLsize(mytbl));
          print_table(mytbl);
          prompt_and_pause("\n\n");
          break;
        default:
          final_status(mytbl);
          break;
        }
    }
  while (menu_choice); 

  prompt_and_pause("\n\nLet's tidy up and destroy the hashtable..- Bye!");
  OHTBLdestroy(mytbl);

  return 0;
}
