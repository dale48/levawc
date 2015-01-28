/**
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: demo4.c
 * Author  : Dan Levin
 * Date    : Fri Jan 23 14:04:10 2015
 * Version : 0.5
 * ---
 * Description: Usage demo of the chained hashtable ADT - in LevAWC. 
 *
 * Revision history: (this is where you document the diffs between versions...)
 * Date   Revision
 * 130201 Created this program the first time..
 * 130324 Changed functions "insert_nodes(), remove_nodes()" - and made the loops in them terminated by user interaction.
 * 150123 Converted demo4.c to be menu-driven.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "chashtbl.h"

#ifndef OK
#define OK 0
#endif

/* Some string macros for the main menu... */
#define MAIN_MENU_ROW "\n\nMENU: 0=Exit 1=Add_Node 2=Remove_Node"
#define MAIN_PROMPT "\nSelection <0-2>+<Enter>: "

#define NR_OF_ITEMS 30
#define NR_OF_BUCKETS 11

/* FUNCTION DECLARATIONS */
void my_destroy(void *data);
void print(const void *data);
int my_cmp(const void *key1, const void *key2);
int my_random(int start, int stop);
void my_clearscrn(void);
void prompt_and_pause(char *message);
int my_match(const void *k1, const void *k2);
int my_hash(const void *key);

void create_nodes(CHtbl list, int nr_of_nodes);

/* Functions handling menu selections */
void ins_nodes(CHtbl list);
void rem_nodes(CHtbl list);

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

/* --- Function: int my_hash(const void *key) --- */
int my_hash(const void *key)
{
  return *(int *)key%NR_OF_BUCKETS;
}

/* --- Function: void create_nodes(CHtbl tbl, int nr_of_nodes) --- */
void create_nodes(CHtbl tbl, int nr_of_nodes)
{
  int i=0, *pi, retval, dupctr=0;

  do
    {
      pi = (int *)malloc(sizeof(int));
      *pi = my_random(1,99);
      
      if ((retval = CHTBLinsert(tbl, pi)) != 0) /* Insertion failed... */
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

  printf("\n\nCreated hash table - %d/%d successful insertions -- %d duplicates rejected...", CHTBLsize(tbl), nr_of_nodes, dupctr);
}

/* --- Function: void ins_nodes(CHtbl tbl) --- */
void ins_nodes(CHtbl tbl)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("\nCurrent table status(%d elements): ", CHTBLsize(tbl));
      CHTBLprint(tbl, print);

      printf("\nEnter data for element to be inserted (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */

      if (tmp == -1)
        break;

      pi = (int *)malloc(sizeof(int));
      *pi = tmp;

      if ((retval = CHTBLinsert(tbl, pi)) != 0) /* Insertion failed... */
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
          sprintf(mess, "Element %d inserted - in bucket %d..", *(int *)pi, (*pi)%NR_OF_BUCKETS);
          prompt_and_pause(mess);
        }
    } while (1);
}

/* --- Function: void rem_nodes(CHtbl tbl) --- */
void rem_nodes(CHtbl tbl)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("\nCurrent table status(%d elements): ", CHTBLsize(tbl));
      CHTBLprint(tbl, print);

      printf("\nEnter data for element to be removed (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */

      if (tmp == -1)
        break;

      pi = &tmp;
      if ((retval = CHTBLremove(tbl, (void **)&pi)) != 0) /* Node removal failed.. */
        {
          /* Removal didn't work - node NOT found... */
          if (retval == 1)
            {
              sprintf(mess, "Element %d not found..!", *(int *)pi);
              prompt_and_pause(mess);
            }
          else /* Serious failure..(-1 or -2) */
            {
              printf("Fatal failure - bailing out...");
              exit(retval);
            }
        }
      else
        {
          /* Removal succesful - notify user.. */
          sprintf(mess, "Element %d removed - from bucket %d..!", *(int *)pi, (*pi)%NR_OF_BUCKETS);
          prompt_and_pause(mess);
          /* Free element - after being removed from table.. */
          free(pi);
        }
    } while (1);
}
/* --- Function: void final_status(CHtbl tbl) --- */
void final_status(CHtbl tbl)
{
  /* Final table status... */
  printf("\nFinal table status(%d elements): ", CHTBLsize(tbl));
  CHTBLprint(tbl, print);
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
  CHtbl mytbl;
  int menu_choice;

  srand((unsigned int)time(NULL));
  my_clearscrn();


  printf("--- INITIALIZING A CHAINED HASHTABLE, %d ELEMENTS, RANDOM INTEGER DATA ---", NR_OF_ITEMS);

  if ((mytbl = CHTBLinit(NR_OF_BUCKETS, my_hash, my_match, my_destroy)) == NULL)
    {
      printf("\nFatal error - bailing out...\n!");
      exit(-1);
    }
  
  /* Initialize - and add elements to the table... */
  create_nodes(mytbl, NR_OF_ITEMS);

  /* Enter menu loop */
  do
    {
      menu_choice = menu(0, 2);

      switch (menu_choice)
        {
        case 1:
          ins_nodes(mytbl);
          break;
        case 2:
          rem_nodes(mytbl);
          break;
        default:
          final_status(mytbl);
          break;
        }
    }
  while (menu_choice); 

  prompt_and_pause("\n\nLet's tidy up and destroy the hashtable - Bye...!");

  CHTBLdestroy(mytbl);

  return 0;
}
