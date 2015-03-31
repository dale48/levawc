/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: demo07.c
 * Author  : Dan Levin
 * Date    : Fri Feb 20 11:33:43 2015
 * Version : 0.51
 * ---
 * Description: Usage demo of the AVL tree ADT - in LevAWC. 
 *
 * Revision history: (this is where you document the diffs between versions...)
 * Date   Revision
 * 130312 Created this program the first time..
 * 150127 Converted this program, demo7.c, to be menu-driven.
 * 150220 Moved some utility functions from here - to file ../utils.c
 * 150220 Source ready for version 0.5!
 * 150318 Source ready for version 0.51
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "avltree.h"
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

#define NR_OF_ITEMS 9

/* Some string macros for the main menu... */
#define MAIN_MENU_ROW "--- AVL SEARCH TREE DEMO ---\nMENU: 0=Exit 1=Add_Node 2=Rem_Node 3=Search 4=Print\nSelection "

/* FUNCTION-DECLARATIONS */
/* Application-specific callbacks */
void my_destroy(void *data);
void print(const void *data);
int my_cmp(const void *key1, const void *key2);

/* Functions handling menu selections */
void rem_node(AvlTree tree);
void ins_node(AvlTree tree);
void search_node(AvlTree tree);
void print_tree(AvlTree tree);
void final_status(AvlTree tree);

/* Misc. application functions.. */
void create_nodes(AvlTree tree, int nr_of_nodes);
/* END-OF-FUNCTION-DECLARATIONS */

/* FUNCTION-DEFINITIONS - the rest of the program */
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

/* --- Function: void create_nodes(AvlTree tree, int nr_of_nodes) --- */
void create_nodes(AvlTree tree, int nr_of_nodes)
{
  int i=0, *pi, retval, dupctr=0;

  do
    {
      pi = (int *)malloc(sizeof(int));
      MALCHK(pi);

      *pi = rand_int(1,99);
      
      if ((retval = AVLTREEinsert(tree, pi)) != OK) /* Insertion failed... */
        {
          if (retval == 1) /* Duplicate key value.. */
            {
              dupctr++;
              my_destroy(pi); /* Free node - since duplicate..  */
            }
          else
            {
              prompt_and_pause("Fatal error - bailing out..!\n");
              AVLTREEdestroy(tree);
              exit(-1);
            }
        }
    } while (++i < nr_of_nodes);

  my_clearscrn();
  printf("--- INITIALIZING AN AVL TREE, %d NODES, RANDOM INTEGER DATA ---\n", NR_OF_ITEMS);
  print_tree(tree);
  printf("\n\n%d/%d successful insertions -- %d duplicate(s) rejected...", AVLTREEsize(tree), nr_of_nodes, dupctr);
  prompt_and_pause("\n\n");
}

/* --- Function: void ins_node(AvlTree tree, int nr_of_insertions) --- */
void ins_node(AvlTree tree)
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
      MALCHK(pi);

      *pi = tmp;

      if ((retval = AVLTREEinsert(tree, pi)) != OK) /* Insertion failed... */
        {
          if (retval == 1) /* Duplicate key value.. */
            {
              sprintf(mess, "Error: Duplicate - node %d already present..!", *pi);
              prompt_and_pause(mess);
              my_destroy(pi); /* Free node - since being duplicate..  */
            }
          else
            {
              prompt_and_pause("\nFatal error - bailing out..:!\n");
              AVLTREEdestroy(tree);
              exit(-1);
            }
        }
      else
        {
          sprintf(mess, "Node %d will be inserted..", *(int *)pi);
          prompt_and_pause(mess);
        }
    } while (TRUE);
}

/* --- Function: void rem_node(AvlTree tree, int nr_of_removes) --- */
void rem_node(AvlTree tree)
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
      if ((retval = AVLTREEremove(tree, pi)) != OK) /* Node removal failed.. */
        {
          /* Removal didn't work - node NOT found... */
          if (retval == -1)
            {
              sprintf(mess, "Error: Node %d not found..!", *(int *)pi);
              prompt_and_pause(mess);
            }
          else /* Serious failure..(-2) */
            {
              printf("\nFatal failure - bailing out...");
              AVLTREEdestroy(tree);
              exit(retval);
            }
        }
      else
        {
          /* Removal succesful - notify user.. */
          sprintf(mess, "Node %d will be removed(=hidden)..!", *(int *)pi);
          prompt_and_pause(mess);
          /* Attention - don't have to free node space here - it will be hidden.. */
        }
    } while (TRUE);
}

/* --- Function: void search_node(AvlTree tree) --- */
void search_node(AvlTree tree)
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

      if ((retval = AVLTREElookup(tree, (void **)&pi)) != OK) /* Node search failed.. */
        {
          /* Searching didn't work - node NOT found... */
          if (retval == -1)
            {
              sprintf(mess, "Node %d NOT found..!", *(int *)pi);
              prompt_and_pause(mess);
            }
          else /* Serious failure..(-2) */
            {
              printf("Fatal failure - bailing out...");
              AVLTREEdestroy(tree);
              exit(retval);
            }
        }
      else
        {
          /* Removal succesful - notify user.. */
          sprintf(mess, "Node %d FOUND..!", *(int *)pi);
          prompt_and_pause(mess);
        }
    } while (TRUE);
}

/* --- Function: void print_tree(AvlTree tree) --- */
void print_tree(AvlTree tree)
{
  AVLTREEprint(tree, print);
  printf("INORDER: ");
  AVLTREEinorder(tree, print);
}

/* --- Function: void final_status(Slist list) --- */
void final_status(AvlTree tree)
{
  /* Final list status... */
  my_clearscrn();
  printf("--- FINAL AVL TREE STATUS---\n");
  print_tree(tree);
}

int main(void)
{
  /* Declare YOUR variables here ! */
  AvlTree mytree;
  int menu_choice;

  srand((unsigned int)time(NULL));

  if ((mytree = AVLTREEinit(my_cmp, my_destroy)) == NULL)
    {
      printf("\nFatal error - bailing out...\n!");
      AVLTREEdestroy(mytree);
      exit(-1);
    }
  
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
  AVLTREEdestroy(mytree);
  
  return 0;
}
