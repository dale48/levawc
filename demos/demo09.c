/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: demo09.c
 * Author  : Dan Levin
 * Date    : Fri Feb 20 12:02:43 2015
 * Version : 0.5
 * ---
 * Description: A usage demo program - showing/testing the Set ADT
 * 
 * Revision history: (this is where you document the diffs between versions...)
 * Date   Revision
 * 150127 Created this program the first time - and made it menu-driven..
 * 150220 Moved some utility functions from here - to file ../utils.c
 * 150220 This source ready for version 0.5!
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "set.h"
#include "utils.h"

#ifndef OK
#define OK 0
#endif

#define NR_OF_ITEMS 10

/* Some string macros for the main menu... */
#define MAIN_MENU_ROW "--- SET DEMO ---\nMENU: 0=Exit 1=Set1_Add 2=Set2_Add 3=Set1_Rem 4=Set2_Rem 5=Print\nSelection "

/* FUNCTION DECLARATIONS */
/* Application-specific callbacks */
void my_destroy(void *data);
void print(const void *data);
int my_cmp(const void *key1, const void *key2);
int my_match(const void *k1, const void *k2);

/* Functions handling menu selections */
void ins_node(Set set);
void rem_node(Set set);
void print_sets(Set set1, Set set2);
void print_union_diff_intersec(Set s1, Set s2);
void final_status(Set set1, Set set2);

/* Misc. application functions.. */
void create_random_nodes(Set set, int nr_of_nodes);
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

/* --- void add_random_nodes(Set set, int nr_of_nodes) --- */
void create_random_nodes(Set set, int nr_of_nodes)
{
  int i=0, *pi, retval, dupctr=0;

  do
    {
      pi = (int *)malloc(sizeof(int));
      *pi = rand_int(1,50);

      if ((retval = SETinsert(set, pi)) != OK) /* Insertion failed... */
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
      else
        i++; /* Insertion successful! */

    } while (i < nr_of_nodes);
}

/* --- Function: void ins_node(Set set) --- */
void ins_node(Set set)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- ADD NODE TO SET ---\n");
      printf("\nCurrent set status(%d nodes): ", SETsize(set));
      SETtraverse(set, print, SET_FWD);

      tmp = read_int("\nEnter nodedata for node to be inserted (-1=Quit): ", 0, 0);

      if (tmp == -1)
        break;

      pi = (int *)malloc(sizeof(int));
      *pi = tmp;

      if ((retval = SETinsert(set, pi)) != OK) /* Insertion failed... */
        {
          if (retval == 1) /* Duplicate key value.. */
            {
              sprintf(mess, "Node %d already present..!", *pi);
              prompt_and_pause(mess);
              my_destroy(pi); /* Free node - since being duplicate..  */
            }
          else
            {
              if (retval == -2)
                {
                  printf("\nMatch-callback is missing... - bailing out!");
                  prompt_and_pause(mess);
                }
              else
                {
                  prompt_and_pause("Fatal error - bailing out..:!");
                }
              my_destroy(pi);
              exit(-1); /* Bail out - since serious error..  */
            }
        }
      else
        {
          sprintf(mess, "Node %d will be inserted..", *(int *)pi);
          prompt_and_pause(mess);
        }
    } while (1);
}

/* --- Function: void rem_node(Set set) --- */
void rem_node(Set set)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- REMOVE NODE FROM SET ---\n");
      printf("\nCurrent set status(%d nodes): ", SETsize(set));
      SETtraverse(set, print, SET_FWD);

      tmp = read_int("\nEnter (key)data for node to be removed (-1=Quit): ", 0, 0);

      if (tmp == -1)
        break;

      /* Remove node - and free memory */
      pi = &tmp;

      if ((retval = SETremove(set, (void **)&pi)) != OK)
        {
          if (retval == 1)
            {
              sprintf(mess, "Node %d not found..!", *(int *)pi);
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
          /* Free node - after being removed from set.. */
          my_destroy(pi);
        }

    } while (1);
}

/* --- Function: void print_sets(Set set1, Set set2) --- */
void print_sets(Set set1, Set set2)
{
  printf("\n\nSet 1 (%d nodes): ", SETsize(set1));
  SETsort(set1, my_cmp);
  SETtraverse(set1, print, SET_FWD);
  printf("\nSet 2 (%d nodes): ", SETsize(set2));
  SETsort(set2, my_cmp);
  SETtraverse(set2, print, SET_FWD);
}

/* --- Function: void print_union_diff_intersec(Set s1, Set s2) --- */
void print_union_diff_intersec(Set s1, Set s2)
{
  Set set_union, set_diff1, set_diff2, set_intersec;

  set_union = SETunion(s1, s2);
  set_diff1 = SETdifference(s1, s2);
  set_diff2 = SETdifference(s2, s1);
  set_intersec = SETintersection(s1, s2);

  SETsort(set_union, my_cmp);
  printf("\nSet1 UNION Set2(%02d nodes) : ", SETsize(set_union));
  SETtraverse(set_union, print, SET_FWD);
  printf("\nSet1 DIFF Set2(%02d nodes)  : ", SETsize(set_diff1));
  SETtraverse(set_diff1, print, SET_FWD);
  printf("\nSet2 DIFF Set1(%02d nodes)  : ", SETsize(set_diff2));
  SETtraverse(set_diff2, print, SET_FWD);
  printf("\nSet1 INTSEC Set2(%02d nodes): ", SETsize(set_intersec));
  SETtraverse(set_intersec, print, SET_FWD);

  SETdestroy(set_union);
  SETdestroy(set_diff1);
  SETdestroy(set_diff2);
  SETdestroy(set_intersec);

  prompt_and_pause("\n\n");
}

/* --- Function: void final_status(CSlist list) --- */
void final_status(Set s1, Set s2)
{
  /* Final set status... */
  my_clearscrn();
  printf("--- FINAL STATUS ---\n");
  printf("\nFinal set contents:");
  print_sets(s1, s2);
}

int main(void)
{
  /* Declare YOUR variables here ! */
  Set myset1, myset2;
  int menu_choice;

  srand((unsigned int)time(NULL));

  if ((myset1 = SETinit(my_match, my_destroy)) == NULL) /* Initialize the set1... */
    {
      printf("\nFatal error... - bailing out!");
      exit(-1);
    }

  if ((myset2 = SETinit(my_match, my_destroy)) == NULL) /* Initialize the set2... */
    {
      printf("\nFatal error... - bailing out!");
      exit(-1);
    }

  my_clearscrn();
  printf("--- INITIALIZING 2 SETS, %d NODES, RANDOM INTEGER DATA ---", NR_OF_ITEMS);

  /* Populate the (empty) sets.. */
  create_random_nodes(myset1, NR_OF_ITEMS);
  create_random_nodes(myset2, NR_OF_ITEMS);
  /* Print set status... */
  printf("\n\nCurrent set status: ");
  print_sets(myset1, myset2);
  prompt_and_pause("\n\n");

  /* Enter menu loop... */
  do
    {
      menu_choice = menu(MAIN_MENU_ROW, 0, 5);

      switch (menu_choice)
        {
        case 1:
          ins_node(myset1);
          break;
        case 2:
          ins_node(myset2);
          break;
        case 3:
          rem_node(myset1);
          break;
        case 4:
          rem_node(myset2);
          break;
        case 5:
          {
            my_clearscrn();
            printf("--- PRINT Set1 & Set2 - AND THEIR CORRESPONDING UNION, DIFF AND INTERSECTION ---");
            print_sets(myset1, myset2);
            printf("\n---");
            print_union_diff_intersec(myset1, myset2);
          }
          break;
        default:
          final_status(myset1, myset2);
          break;
        }
    }
  while (menu_choice); 

  /* ..and finally destroy the list. */
  prompt_and_pause("\n\nLet's tidy up and destroy the set..- Bye!");
  SETdestroy(myset1);
  SETdestroy(myset2);

  return 0;
}
