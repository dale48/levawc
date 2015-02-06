/**
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: demo6.c
 * Author  : Dan Levin
 * Date    : Mon Jan 26 14:53:39 2015
 * Version : 0.5
 * ---
 * Description: Usage demo of the binary search tree ADT - in LevAWC. 
 *
 * Revision history: (this is where you document the diffs between versions...)
 * Date   Revision
 * 130312 Created this program the first time..
 * 150206 Made this demo6.c menu-driven.
 * 150206 Source reade for version 0.5!
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
#define MAIN_MENU_ROW "\n--- BINARY SEARCH TREE DEMO ---\nMENU: 0=Exit 1=Add_Node 2=Rem_Node 3=Search 4=Print"
#define MAIN_PROMPT "\nSelection <0-4>+<Enter>: "

/* FUNCTION DECLARATIONS */
void my_destroy(void *data);
void print(const void *data);
int my_cmp(const void *key1, const void *key2);
int my_random(int start, int stop);
void my_clearscrn(void);
void prompt_and_pause(char *message);

void create_nodes(BiTree tree, int nr_of_nodes);

/* Functions handling menu selections */
void ins_node(BiTree tree);
void rem_node(BiTree tree);
void search_node(BiTree tree);
void print_tree(BiTree tree);
void final_status(BiTree tree);

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

/* --- Function: void create_nodes(BiTree tree, int nr_of_nodes) --- */
void create_nodes(BiTree tree, int nr_of_nodes)
{
  int i=0, *pi, retval, dupctr=0;

  do
    {
      pi = (int *)malloc(sizeof(int));
      *pi = my_random(1,99);
      
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

      printf("\n\nEnter data for node to be inserted (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */

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

      printf("\n\nEnter data for node to be removed (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */
      
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

      printf("\n\nEnter data for node to be searched for (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */
      
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
      menu_choice = menu(0, 4);

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
