/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: demo5.c
 * Author  : Dan Levin
 * Date    : Fri Feb 20 11:00:53 2015
 * Version : 0.5
 * ---
 * Description: Usage demo for heap and priority queue ADT - in LevAWC. 
 *
 * Revision history: (this is where you document the diffs between versions...)
 * Date   Revision
 * 130220 Created this program the first time..
 * 150124 Converted this file, demo5.c - to be menu-driven.
 * 150220 Moved some utility functions from here - to file ../utils.c
 * 150220 Source ready for version 0.5! 
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "pqueue.h"
#include "slist.h"
#include "utils.h"

#define NR_OF_ITEMS 16
#define NR_OF_REMOVALS 3
#define NR_OF_INSERTS 3

/* Some string macros for the main menu... */
#define MAIN_MENU_ROW "--- HEAP/PRIORITY QUEUE DEMO ---\nMENU: 0=Exit 1=Add_Node 2=Rem_Node 3=Heapsort_Intro 4=Print\nSelection "

#ifndef OK
#define OK 0
#endif

/* FUNCTION DECLARATIONS */
/* Application-specific callbacks */
void my_destroy(void *data);
void print(const void *data);
int my_cmp(const void *key1, const void *key2);

/* Functions handling menu selections */
void add_node(PQueue pq);
void rem_node(PQueue pq);
void heapsort_intro(PQueue pq);
void print_pqueue(PQueue pq);
void final_status(PQueue pq);

/* Misc. application functions.. */
void create_nodes(PQueue pq, int nr_of_ele);
void move_nodes(PQueue pq, Slist mylst, int nr_of_moves);
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

/* --- Function: void create_nodes(PQueue pq, int nr_of_ele) --- */
void create_nodes(PQueue pq, int nr_of_ele)
{
  int i=0, *pi, retval;

  my_clearscrn();
  printf("--- INITIALIZING A PRIORITY QUEUE, %d NODES, RANDOM INTEGER DATA ---", NR_OF_ITEMS);

  do
    {
      /* Get a random integer */
      pi = (int *)malloc(sizeof(int));
      *pi = rand_int(1,99);

      /* Insert an integer into priority queue... */
      retval = PQUEUEinsert(pq, pi);
      /* Defensive programming... */
      assert(retval == OK);

    } while (++i < nr_of_ele);

  printf("\n\nCurrent queue status -- after %d successful insertions...", PQUEUEsize(pq));
  PQUEUEprint(pq, print);
  printf("\nRead \"Tree\" view(above) - columnwise, up-->down - and compare to \"Heap\" view..\n");
  prompt_and_pause("..do you see any pattern..?");
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
      printf("\n\nCurrent priority queue status(%d nodes): ", PQUEUEsize(pq));
      PQUEUEprint(pq, print);

      tmp = read_int("\nEnter integer data for node to be inserted (-1=Quit): ", 0, 0);

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
          sprintf(mess, "Node %d will be inserted!", *pi);
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
                  sprintf(mess, "(Top) node %d will be removed!", *pi);
                  prompt_and_pause(mess);
                  my_destroy(pi);
                }
            }
          else
            tmp = -1;
        }
    } while (1);
}


/* --- Function: void heapsort_intro(PQueue pq) --- */
void heapsort_intro(PQueue pq)
{
  int *pv, retval;
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
    
  do
    {
      my_clearscrn();
      printf("--- HEAPSORT INTRODUCTION ---\n");

      if (PQUEUEpeek(pq))
        {
          /* Print the contents of the priority queue... */
          PQUEUEprint(pq, print);
          /* Print the list - from the beginning... */
          printf("=====");
          printf("\nList: ");
          SLISTtraverse(tmplst, print, SLIST_FWD);
          printf ("\n\nNow - move the high priority node, value=%d, from the priority queue...\n", *(int *)PQUEUEpeek(pq));
          prompt_and_pause("..and insert it - at the front of the list...");
          /* Extract the top priority node from the queue... */
          retval = PQUEUEextract(pq, (void **)&pv);
          /* Defensive programming... */
          assert(retval == OK);
          /* Insert this node at the front of a linked list... */
          retval = SLISTinsnext(tmplst, NULL, pv);
          /* Defensive programming... */
          assert(retval == OK);
        }
      else
        {
          printf("\nHeap:  (empty)");
          printf("\nTree:  (empty)\n");

          /* Print the list - from the beginning... */
          printf("=====");
          printf("\nList: ");
          SLISTtraverse(tmplst, print, SLIST_FWD);
          prompt_and_pause("\n\nEvidently - the priority queue is now empty..!");
          break;
        }

    } while (1);

  printf("\n---\nHey - it looks like the priority queue can be used for sorting things.. :-)!");
  prompt_and_pause("\nTRUE, indeed! Just Google \'Heapsort\' for more..");

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

/* --- Function: void print_pueue(PQueue pq) --- */
void print_pqueue(PQueue pq)
{
  my_clearscrn();
  printf("--- PRINT PRIORITY QUEUE ---");
  printf("\n\nCurrent priority queue contents(%d nodes): ", PQUEUEsize(pq));
  PQUEUEprint(pq, print);
  prompt_and_pause("\n");
}

/* --- Function: void final_status(Slist list) --- */
void final_status(PQueue pq)
{
  my_clearscrn();
  printf("--- FINAL STATUS ---");
  printf("\n\nFinal priority queue contents(%d nodes): ", PQUEUEsize(pq));
  PQUEUEprint(pq, print);
}

int main(void)
{
  /* Declare YOUR variables here ! */
  PQueue mypq;
  int menu_choice;

  srand((unsigned int)time(NULL));
  
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
      menu_choice = menu(MAIN_MENU_ROW, 0, 4);

      switch (menu_choice)
        {
        case 1:
          ins_node(mypq);
          break;
        case 2:
          rem_node(mypq);
          break;
        case 3:
          heapsort_intro(mypq);
          break;
        case 4:
          print_pqueue(mypq);
          break;
        default:
          final_status(mypq);
          break;
        }
    }
  while (menu_choice); 

  prompt_and_pause("\nLet's tidy up and destroy the prio.queue..- Bye!");
  PQUEUEdestroy(mypq);

  return 0;
}
