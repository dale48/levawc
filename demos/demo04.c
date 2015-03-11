/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: demo04.c
 * Author  : Dan Levin
 * Date    : Fri Feb 20 10:32:01 2015
 * Version : 0.5
 * ---
 * Description: Usage demo of the chained hashtable ADT - in LevAWC. 
 *
 * Revision history: (this is where you document the diffs between versions...)
 * Date   Revision
 * 130201 Created this program the first time..
 * 130324 Changed functions "insert_nodes(), remove_nodes()" - and made the loops in them terminated by user interaction.
 * 150123 Converted demo4.c to be menu-driven.
 * 150220 Moved some utility functions from here - to file ../utils.c
 * 150220 Source ready for version 0.5!
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "chashtbl.h"
#include "utils.h"

#ifndef OK
#define OK 0
#endif

/* Some string macros for the main menu... */
#define MAIN_MENU_ROW "--- CHAINED HASH TABLE DEMO ---\nMENU: 0=Exit 1=Add_Node 2=Rem_Node 3=Search 4=Print\nSelection "

#define NR_OF_ITEMS 30
#define NR_OF_BUCKETS 11

/* FUNCTION DECLARATIONS */
/* Application-specific callbacks */
void my_destroy(void *data);
void print(const void *data);
int my_cmp(const void *key1, const void *key2);
int my_match(const void *k1, const void *k2);
int my_hash(const void *key);

/* Functions handling menu selections */
void ins_nodes(CHtbl tbl);
void rem_nodes(CHtbl tbl);
void find_node(CHtbl tbl);
void print_table(CHtbl tbl);
void final_status(CHtbl tbl);

/* Misc. application functions.. */
void create_nodes(CHtbl list, int nr_of_nodes);
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

/* --- Function: int my_hash(const void *key) --- */
int my_hash(const void *key)
{
  return *(int *)key%NR_OF_BUCKETS;
}

/* --- Function: void create_nodes(CHtbl tbl, int nr_of_nodes) --- */
void create_nodes(CHtbl tbl, int nr_of_nodes)
{
  int i=0, *pi, retval, dupctr=0;

  my_clearscrn();
  printf("--- INITIALIZING A CHAINED HASHTABLE, %d NODES, RANDOM INTEGER DATA ---", NR_OF_ITEMS);

  do
    {
      pi = (int *)malloc(sizeof(int));
      *pi = rand_int(1,99);
      
      if ((retval = CHTBLinsert(tbl, pi)) != OK) /* Insertion failed... */
        {
          if (retval == 1) /* Duplicate key value.. */
            {
              dupctr++;
              my_destroy(pi); /* Free node - since duplicate..  */
            }
          else
            {
              prompt_and_pause("Fatal error - bailing out..!\n");
              exit(-1);
            }
        }
    } while (++i < nr_of_nodes);

  printf("\n\nCreated hash table - %d/%d successful insertions -- %d duplicates rejected...", CHTBLsize(tbl), nr_of_nodes, dupctr);

  printf("\nCurrent table status(%d nodes): ", CHTBLsize(tbl));
  CHTBLprint(tbl, print);
  prompt_and_pause("\n\n");
}

/* --- Function: void ins_nodes(CHtbl tbl) --- */
void ins_nodes(CHtbl tbl)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- ADD NODE TO HASH TABLE ---\n");
      printf("\nCurrent table status(%d nodes): ", CHTBLsize(tbl));
      CHTBLprint(tbl, print);

      /* printf("\n\nEnter data for node to be inserted (-1=Quit): "); */
      /* scanf("%d", &tmp); */
      /* getchar(); /\* Remove CR from input buffer *\/ */
      
      tmp = read_int("\nEnter data for node to be inserted (-1=Quit): ", 0, 0);

      if (tmp == -1)
        break;

      pi = (int *)malloc(sizeof(int));
      *pi = tmp;

      if ((retval = CHTBLinsert(tbl, pi)) != OK) /* Insertion failed... */
        {
          if (retval == 1) /* Duplicate key value.. */
            {
              sprintf(mess, "Node %d already present (bucket %d)..!", *pi, (*pi)%NR_OF_BUCKETS);
              prompt_and_pause(mess);
              my_destroy(pi); /* Free node - since being duplicate..  */
            }
          else
            {
              prompt_and_pause("Fatal error - bailing out..:!\n");
              exit(-1);
            }
        }
      else
        {
          sprintf(mess, "Node will be %d inserted - in bucket %d..", *(int *)pi, (*pi)%NR_OF_BUCKETS);
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
      printf("--- REMOVE NODE FROM HASH TABLE ---\n");
      printf("\nCurrent table status(%d nodes): ", CHTBLsize(tbl));
      CHTBLprint(tbl, print);

      /* printf("\n\nEnter data for node to be removed (-1=Quit): w"); */
      /* scanf("%d", &tmp); */
      /* getchar(); /\* Remove CR from input buffer *\/ */
      
      tmp = read_int("\nEnter data for node to be removed (-1=Quit): ", 0, 0);

      if (tmp == -1)
        break;

      pi = &tmp;
      if ((retval = CHTBLremove(tbl, (void **)&pi)) != OK) /* Node removal failed.. */
        {
          /* Removal didn't work - node NOT found... */
          if (retval == 1)
            {
              sprintf(mess, "Node %d not found..!", *(int *)pi);
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
          sprintf(mess, "Node %d will be removed - from bucket %d..!", *(int *)pi, (*pi)%NR_OF_BUCKETS);
          prompt_and_pause(mess);
          /* Free node - after being removed from table.. */
          my_destroy(pi);
        }
    } while (1);
}

/* --- Function: void find_node(CHtbl tbl) --- */
void find_node(CHtbl tbl)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- SEARCH NODE IN HASH TABLE ---\n");
      printf("\nCurrent table status(%d nodes): ", CHTBLsize(tbl));
      CHTBLprint(tbl, print);

      tmp = read_int("\nEnter data for node to be found (-1=Quit): ", 0, 0);

      if (tmp == -1)
        break;

      pi = &tmp;

      if ((retval = CHTBLlookup(tbl, (void **)&pi)) != OK) /* Node not found failed... */
        {
          sprintf(mess, "\nNode %d NOT FOUND!", *(int *)pi);
          prompt_and_pause(mess);
          /* prompt_and_pause("\nNode NOT FOUND!"); */
        }
      else
        {
          sprintf(mess, "\nNode %d FOUND - in bucket %d..", *(int *)pi, (*pi)%NR_OF_BUCKETS);
          prompt_and_pause(mess);
        }
    } while (1);
}

/* --- Function: void print_table(CHtbl tbl) --- */
void print_table(CHtbl tbl)
{
  /* Print table status... */
  my_clearscrn();
  printf("--- PRINT TABLE ---");
  printf("\n\nCurrent table status(%d nodes): ", CHTBLsize(tbl));
  CHTBLprint(tbl, print);
  prompt_and_pause("\n\n");
}

/* --- Function: void final_status(CHtbl tbl) --- */
void final_status(CHtbl tbl)
{
  my_clearscrn();
  printf("--- FINAL STATUS ---\n");
  /* Final table status... */
  printf("\nFinal table status(%d nodes): ", CHTBLsize(tbl));
  CHTBLprint(tbl, print);
}

int main(void)
{
  /* Declare YOUR variables here ! */
  CHtbl mytbl;
  int menu_choice;

  srand((unsigned int)time(NULL));

  if ((mytbl = CHTBLinit(NR_OF_BUCKETS, my_hash, my_match, my_destroy)) == NULL)
    {
      printf("\nFatal error - bailing out...\n!");
      exit(-1);
    }
  
  /* Initialize - and add nodes to the table... */
  create_nodes(mytbl, NR_OF_ITEMS);

  /* Enter menu loop */
  do
    {
      menu_choice = menu(MAIN_MENU_ROW, 0, 4);

      switch (menu_choice)
        {
        case 1:
          ins_nodes(mytbl);
          break;
        case 2:
          rem_nodes(mytbl);
          break;
        case 3:
          find_node(mytbl);
          break;
        case 4:
          print_table(mytbl);
          break;
        default:
          final_status(mytbl);
          break;
        }
    }
  while (menu_choice); 

  prompt_and_pause("\n\nLet's tidy up and destroy the hashtable..- Bye!");
  CHTBLdestroy(mytbl);

  return 0;
}
