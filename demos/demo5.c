/**
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: demo5.c
 * Author  : Dan Levin
 * Date    : Sat Jan 24 15:56:52 2015
 * Version : 0.5
 * ---
 * Description: Usage demo for heap and priority queue ADT - in LevAWC. 
 *
 * Revision history: (this is where you document the diffs between versions...)
 * Date   Revision
 * 130220 Created this program the first time..
 * 150124 Converted this file, demo5.c - to be menu-driven.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "pqueue.h"
#include "slist.h"

#define NR_OF_ITEMS 16
#define NR_OF_REMOVALS 3
#define NR_OF_INSERTS 3

/* Some string macros for the main menu... */
#define MAIN_MENU_ROW "\n\nMENU: 0=Exit 1=Add_Node 2=Remove_Node 3=Heapsort_intro"
#define MAIN_PROMPT "\nSelection <0-3>+<Enter>: "

#ifndef OK
#define OK 0
#endif

/* FUNCTION DECLARATIONS */
void my_destroy(void *data);
void print(const void *data);
int my_cmp(const void *key1, const void *key2);
int my_random(int start, int stop);
void my_clearscrn(void);
void prompt_and_pause(char *message);

void create_nodes(PQueue pq, int nr_of_ele);
void move_elements(PQueue pq, Slist mylst, int nr_of_moves);

/* Functions handling menu selections */
void add_node(PQueue pq);
void rem_node(PQueue pq);
void heapsort_intro(PQueue pq, int nr_of_ele);
void final_status(PQueue pq);

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

/* --- Function: void create_nodes(PQueue pq, int nr_of_ele) --- */
void create_nodes(PQueue pq, int nr_of_ele)
{
  int i=0, *pi, retval;

  do
    {
      /* Get a random integer */
      pi = (int *)malloc(sizeof(int));
      *pi = my_random(1,99);

      /* Insert an integer into priority queue... */
      retval = PQUEUEinsert(pq, pi);
      /* Defensive programming... */
      assert(retval == OK);

    } while (++i < nr_of_ele);

  printf("\nCurrent queue status -- after %d successful insertions...", PQUEUEsize(pq));
  PQUEUEprint(pq, print);
  printf("\nRead the \"Tree\" view(above) - columnwise, up -> down - and compare to \"Heap\" view..\n");
  printf("..do you see any pattern?\n--------");
}

/* --- Function: void ins_node(PQueue pq) --- */
void ins_node(PQueue pq)
{
  int tmp, *pi;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- INSERT NODES ---");
      printf("\nCurrent priority queue status(%d nodes): ", PQUEUEsize(pq));
      PQUEUEprint(pq, print);

      printf("\n\nEnter integer data of node to be inserted (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */

      if (tmp == -1)
        break;

      pi = (int *)malloc(sizeof(int));
      *pi = tmp;

      if ((PQUEUEinsert(pq, pi)) != OK)
        {
          printf("\nFatal error enqueing data - exiting...!");
          exit(-1);
        }
      else
        {
          sprintf(mess, "Node %d inserted!", *pi);
          prompt_and_pause(mess);
        }
    } while (1);
}

/* --- void rem_node(PQueue pq) --- */
void rem_node(PQueue pq)
{
  int tmp, *pi, *ptmp;
  char mess[BUFSIZ], ans;

  /* Initialize 'tmp'... */
  tmp = 0;

  do
    {
      my_clearscrn();
      printf("--- REMOVE NODES ---");
      printf("\nCurrent priority queue status(%d nodes): ", PQUEUEsize(pq));
      PQUEUEprint(pq, print);

      if (tmp == -1)
        break;

      ptmp = (int *)PQUEUEpeek(pq);

      if (ptmp  == NULL)
        {
          prompt_and_pause("\n\nPriority queue is EMPTY - nothing to do..!");
          tmp = -1;
        }
      else
        {
          sprintf(mess, "\nAbout to remove (top) node %d..", *ptmp);
          printf("\n%s - Continue? (y/n+Enter): ", mess); 
          ans = getchar();
          getchar(); /* Remove '\n' from keyb. buffer */
          
          if (ans == 'y' || ans == 'Y')
            {
              if ((PQUEUEextract(pq, (void **)&pi)) != OK)
                {
                  printf("\nFatal error removing data - exiting...!");
                  exit(-1);
                }
              else
                {
                  sprintf(mess, "(Top) node %d removed!", *pi);
                  prompt_and_pause(mess);
                  free(pi);
                }
            }
          else
            tmp = -1;
        }
    } while (1);
}


/* --- Function: void heapsort_intro(PQueue pq, int nr_of_ele) --- */
void heapsort_intro(PQueue pq, int nr_of_ele)
{
  int i=0, *pv, retval;
  Slist tmplst;

  if (PQUEUEsize(pq) == 0)
    {
      prompt_and_pause("Priority queue is empty - nothing to do..");
      return;
    }

  /* Initialize a list - defensive programming... */
  if ((tmplst = SLISTinit(my_destroy)) == NULL)
    {
      printf("\nFatal error - bailing out...\n!");
      exit(-1);
    }
    
  printf ("\nNow, let's move the high priority element, value=%d, from the priority queue..\n", *(int *)PQUEUEpeek(pq));
  prompt_and_pause("..and insert it - at the front of a fresh list");

  do
    {
      my_clearscrn();
      /* Extract the top priority element from the queueu... */
      retval = PQUEUEextract(pq, (void **)&pv);
      /* Defensive programming... */
      assert(retval == OK);
      /* Insert this element at the front of a linked list... */
      retval = SLISTinsnext(tmplst, NULL, pv);
      /* Defensive programming... */
      assert(retval == OK);
      /* Print the contents of the priority queue... */
      PQUEUEprint(pq, print);
      /* Print the list - from the beginning... */
      printf("\n*******************************************************************************");
      printf("\nList: ");
      SLISTtraverse(tmplst, print, SLIST_FWD);
      printf("\n*******************************************************************************\n");

      if (PQUEUEpeek(pq))
        {
          printf ("Now - move the high priority element, value=%d, from the priority queue...\n", *(int *)PQUEUEpeek(pq));
          prompt_and_pause("..and insert it - at the front of the list...");
        }
      else
        {
          printf("\nThe priority queue is now - empty..!");
        }

      i++;
    } while (i < nr_of_ele);

  printf("\n\n---\nHey - it looks like the priority queue can be used for sorting things.. :-)!");
  prompt_and_pause("\n\nThis is, indeed, the case - just Google for \'Heapsort\'...");

  /* Move data back from list - to priority queue.. */
  while (SLISTsize(tmplst))
    {
      retval = SLISTremnext(tmplst, NULL, (void **)&pv);
      assert(retval == OK);
      retval = PQUEUEinsert(pq, pv);
      assert(retval == OK);
    }

  SLISTdestroy(tmplst);
}

/* --- Function: void final_status(Slist list) --- */
void final_status(PQueue pq)
{
  printf("\nFinal priority queue contents(%d nodes): ", PQUEUEsize(pq));
  PQUEUEprint(pq, print);
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
  PQueue mypq;
  int menu_choice;

  srand((unsigned int)time(NULL));
  my_clearscrn();

  printf("--- INITIALIZING A PRIORITY QUEUE, %d ELEMENTS, RANDOM INTEGER DATA ---", NR_OF_ITEMS);
  
  /* Initialize  the priority queue and linked list - defensive programming...... */
  if ((mypq = PQUEUEinit(my_cmp, my_destroy)) == NULL)
    {
      printf("\nFatal error - bailing out...\n!");
      exit(-1);
    }

  /* Add initial nodes to the priority queue... */
  create_nodes(mypq, NR_OF_ITEMS);

  do
    {
      menu_choice = menu(0, 3);

      switch (menu_choice)
        {
        case 1:
          ins_node(mypq);
          break;
        case 2:
          rem_node(mypq);
          break;
        case 3:
          heapsort_intro(mypq, PQUEUEsize(mypq));
          break;
        default:
          final_status(mypq);
          break;
        }
    }
  while (menu_choice); 

  prompt_and_pause("\nLet's tidy up and destroy the priority queue - Bye...!");
  PQUEUEdestroy(mypq);

  return 0;
}
