/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: demo06.c
 * Author  : Dan Levin
 * Date    : Fri Feb 20 11:17:57 2015
 * Version : 0.5
 * ---
 * Description: Usage demo of the binary search tree ADT - in LevAWC. 
 *
 * Revision history: (this is where you document the diffs between versions...)
 * Date   Revision
 * 130312 Created this program the first time..
 * 150206 Made this demo6.c menu-driven.
 * 150220 Moved some utility functions from here - to file ../utils.c
 * 150220 Source reade for version 0.5!
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bitree.h"
#include "utils.h"

#ifndef OK
#define OK 0
#endif

#define NR_OF_ITEMS 9

/* Some string macros for the main menu... */
#define MAIN_MENU_ROW "--- BINARY SEARCH TREE DEMO ---\nMENU: 0=Exit 1=Add_Node 2=Rem_Node 3=Search 4=Print\nSelection "

/* FUNCTION DECLARATIONS */
/* Application-specific callbacks */
void my_destroy(void *data);
void print(const void *data);
int my_cmp(const void *key1, const void *key2);

/* Functions handling menu selections */
void ins_node(BiTree tree);
void rem_node(BiTree tree);
void search_node(BiTree tree);
void print_tree(BiTree tree);
void final_status(BiTree tree);

/* Misc. application functions.. */
void create_nodes(BiTree tree, int nr_of_nodes);
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

/* --- Function: void create_nodes(BiTree tree, int nr_of_nodes) --- */
void create_nodes(BiTree tree, int nr_of_nodes)
{
  int i=0, *pi, retval, dupctr=0;

  do
    {
      pi = (int *)malloc(sizeof(int));
      *pi = rand_int(1,99);
      
      if ((retval = BITREEinsert(tree, pi)) != 0) /* Insertion failed... */
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

  my_clearscrn();
  printf("--- INITIALIZING A BINARY SEARCH TREE, %d NODES, RANDOM INTEGER DATA ---\n", NR_OF_ITEMS);
  print_tree(tree);
  printf("\n\n%d/%d successful insertions -- %d duplicate(s) rejected..", BITREEsize(tree), nr_of_nodes, dupctr);
  prompt_and_pause("\n\n");
}

/* --- Function: void insert_nodes(BiTree tree, int nr_of_insertions) --- */
void ins_node(BiTree tree)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- INSERT NODE ---\n");
      print_tree(tree);

      tmp = read_int("\nEnter integer data for node to be inserted (-1=Quit): ", 0, 0);

      if (tmp == -1)
        break;

      pi = (int *)malloc(sizeof(int));
      *pi = tmp;

      if ((retval = BITREEinsert(tree, pi)) != 0) /* Insertion failed... */
        {
          if (retval == 1) /* Duplicate key value.. */
            {
              sprintf(mess, "Node %d already present..!", *pi);
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
          sprintf(mess, "Node %d will be inserted..", *(int *)pi);
          prompt_and_pause(mess);
        }
    } while (1);
}

/* --- Function: void remove_nodes(BiTree tree, int nr_of_removes) --- */
void rem_node(BiTree tree)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- REMOVE NODE ---\n");
      print_tree(tree);

      tmp = read_int("\nEnter data for node to be removed (-1=Quit): ", 0, 0);

      if (tmp == -1)
        break;

      pi = &tmp;
      if ((retval = BITREEremove(tree, (void **)&pi)) != 0) /* Node removal failed.. */
        {
          /* Removal didn't work - node NOT found... */
          if (retval == -1)
            {
              sprintf(mess, "Node %d not found..!", *(int *)pi);
              prompt_and_pause(mess);
            }
          else /* Serious failure..(-1 or -2) */
            {
              printf("Fatal failure - bailing out...");
              getchar();
              exit(retval);
            }
        }
      else
        {
          /* Removal succesful - notify user.. */
          sprintf(mess, "Node %d will be removed..!", *(int *)pi);
          prompt_and_pause(mess);
          /* Free node - after being removed from tree.. */
          my_destroy(pi);
        }
    } while (1);
}

/* --- Function: void search_node(BiTree tree) --- */
void search_node(BiTree tree)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- SEARCH NODE ---\n");
      print_tree(tree);

      tmp = read_int("\nEnter data for node to be found (-1=Quit): ", 0, 0);

      if (tmp == -1)
        break;

      pi = &tmp;
      if ((retval = BITREElookup(tree, (void **)&pi)) != 0) /* Node searching failed.. */
        {
          /* The search didn't work - node NOT found... */
          if (retval == -1)
            {
              sprintf(mess, "Node %d NOT found..!", *(int *)pi);
              prompt_and_pause(mess);
            }
          else /* Compare-callback not set - or serious failure..(-2) */
            {
              printf("Compare-callback not set - or other fatal failure - bailing out...");
              getchar();
              exit(retval);
            }
        }
      else
        {
          /* Searching succesful - notify user.. */
          sprintf(mess, "Node %d FOUND..!", *(int *)pi);
          prompt_and_pause(mess);
        }
    } while (1);
}

/* --- Function: void print_tree(BiTree tree) --- */
void print_tree(BiTree tree)
{
  BITREEprint(tree, print);
  printf("INORDER: ");
  BITREEinorder(tree, print);
}

/* --- Function: void final_status(Slist list) --- */
void final_status(BiTree tree)
{
  /* Final list status... */
  my_clearscrn();
  printf("--- FINAL TREE STATUS ---\n");
  BITREEprint(tree, print);
  printf("INORDER: ");
  BITREEinorder(tree, print);
}

int main(void)
{
  /* Declare YOUR variables here ! */
  BiTree mytree;
  int menu_choice;

  srand((unsigned int)time(NULL));

  if ((mytree = BITREEinit(my_destroy)) == NULL)
    {
      printf("\nFatal error - bailing out...\n!");
      exit(-1);
    }
  
  /* Don't forget to set the compare callback..! */
  BITREEsetcompare(mytree, my_cmp);

  /* Initialize - and add nodes to the table... */
  create_nodes(mytree, NR_OF_ITEMS);
  
  do
    {
      menu_choice = menu(MAIN_MENU_ROW, 0, 4);

      switch (menu_choice)
        {
        case 1:
          ins_node(mytree);
          break;
        case 2:
          rem_node(mytree);
          break;
        case 3:
          search_node(mytree);
          break;
        case 4:
          my_clearscrn();
          printf("--- PRINT TREE ---\n");
          print_tree(mytree);
          prompt_and_pause("\n\n");
          break;
        default:
          final_status(mytree);
          break;
        }
    }
  while (menu_choice); 

  prompt_and_pause("\n\nLet's tidy up and destroy the tree..- Bye!");
  BITREEdestroy(mytree);
  
  return 0;
}
