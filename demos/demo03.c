/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: demo03.c
 * Author  : Dan Levin
 * Date    : Fri Feb 20 10:09:46 2015
 * Version : 0.51
 * ---
 * Description: A demo of stack/queue ADT usage - in LevAWC
 *
 * Revision history: (this is where you document the diffs between versions...)
 * Date   Revision
 * 130122 Created this file - the first time..
 * 130123 Completed the code - and debugged/tested it as well..
 * 150121 Made this demo3.c menu-driven
 * 150220 Moved some utility functions from here - to file ../utils.c
 * 150220 Source ready for version 0.5!
 * 150317 Source ready for version 0.51
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "stack.h"
#include "queue.h"
#include "utils.h"

#define NR_OF_ITEMS 8

#ifndef OK
#define OK 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* Some string macros for the main menu... */
#define MAIN_MENU_ROW "--- QUEUE/STACK DEMO ---\nMENU: 0=Exit 1=Enqueue 2=Dequeue 3=Push 4=Pop 5=Dequeue/Push 6=Print\nSelection "

/* FUNCTION-DECLARATIONS */
/* Application-specific callbacks */
void my_destroy(void *data);
int my_chkch(int ch);
void print(const void *data);

/* Functions handling menu selections */
void enqueue_node(Queue que);
void dequeue_node(Queue que);
void push_node(Stack stk);
void pop_node(Stack stk);
void print_queue_stack(Queue que, Stack stk);
void dequeue_push_node(Queue que, Stack stk);
void final_status(Queue que, Stack stk);

/* Misc. application functions.. */
void enqueue_push_nodes(Queue que, Stack stk, int nr_of_ele);
/* END-OF-FUNCTION-DECLARATIONS */

/* FUNCTION-DEFINITIONS - that is, the rest of the program */
/* --- Function: void my_destroy(void *data) --- */
void my_destroy(void *data)
{
  free(data);
}

/* --- Function: int my_chkch(int ch) --- */
int my_chkch(int ch)
{
  return strchr("YyNn", ch) ? 1 : 0;
}

/* --- Function: void print(const void *data) --- */
void print(const void *data)
{
  printf(" %02d", *(int *)data);
}

/* --- Function: void queue_nodess(Queue que, Stack stk, int nr_of_ele) --- */
void enqueue_push_nodes(Queue que, Stack stk, int nr_of_ele)
{
  int i=0, *pi, retval;

  my_clearscrn();
  printf("--- CREATING QUEUE AND STACK (%d nodes each), RANDOM INTEGER DATA ---\n", NR_OF_ITEMS);

  do
    {
      /* Create dyn. memory, store random nr - and enqueue... */
      pi = (int *)malloc(sizeof(int));
      MALCHK(pi);

      *pi = rand_int(1,50);

      retval = QUEUEenqueue(que, pi);
      assert(retval == OK);

      /* Create dyn. memory, store random nr - and push... */
      pi = (int *)malloc(sizeof(int));
      MALCHK(pi);

      *pi = rand_int(1,50);

      retval = STACKpush(stk, pi);
      assert(retval == OK);

    } while (++i < nr_of_ele);

  printf("\nCurrent queue and stack status: ");
  printf("\nQueue(%d nodes): ", QUEUEsize(que));
  SLISTtraverse(que, print, SLIST_FWD);
  printf("\nStack(%d nodes): ", STACKsize(stk));
  SLISTtraverse(stk, print, SLIST_FWD);
  prompt_and_pause("\n\n");
}

/* --- Function: void enqueue_node(Queue que) --- */
void enqueue_node(Queue que)
{
  int tmp, *pi;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- ENQUEUE NODE TO QUEUE ---\n");
      printf("\nCurrent queue status(%d nodes): ", QUEUEsize(que));
      SLISTtraverse(que, print, SLIST_FWD);

      tmp = read_int("\nEnter integer data of node to be enqueued (-1=Quit): ", 0, 0);

      if (tmp == -1)
        break;

      pi = (int *)malloc(sizeof(int));
      MALCHK(pi);

      *pi = tmp;

      if ((QUEUEenqueue(que, pi)) != OK)
        {
          printf("\nFatal error enqueing data - exiting...!");
          QUEUEdestroy(que);
          exit(-1);
        }
      else
        {
          sprintf(mess, "Node %d will be enqueued!", *pi);
          prompt_and_pause(mess);
        }
    } while (TRUE);
}

/* --- Function: void dequeue_node(Queue que) --- */
void dequeue_node(Queue que)
{
  int tmp, *pi, *ptmp;
  char mess[BUFSIZ], ans;

  /* Initialize 'tmp'... */
  tmp = 0;

  do
    {
      if (tmp == -1)
        break;

      my_clearscrn();
      printf("--- DEQUEUE NODE FROM QUEUE ---\n");
      printf("\nCurrent queue status(%d nodes): ", QUEUEsize(que));
      SLISTtraverse(que, print, SLIST_FWD);

      ptmp = (int *)QUEUEpeek(que);

      if (ptmp  == NULL)
        {
          prompt_and_pause("\n\nQueue is EMPTY - nothing to do..!");
          tmp = -1;
        }
      else
        {
          sprintf(mess, "\nAbout to dequeue node %d.. - Continue? (y/n): ", *ptmp);
          ans = read_char(mess, 0, 0, my_chkch);
          
          if (ans == 'y' || ans == 'Y')
            {
              if ((QUEUEdequeue(que, (void **)&pi)) != OK)
                {
                  printf("\nFatal error dequeing data - exiting...!");
                  QUEUEdestroy(que);
                  exit(-1);
                }
              else
                {
                  sprintf(mess, "Node %d will be dequeued!", *pi);
                  prompt_and_pause(mess);
                  my_destroy(pi);
                }
            }
          else
            tmp = -1;
        }
    } while (TRUE);
}

/* --- Function: void push_node(Stack stk) --- */
void push_node(Stack stk)
{
  int tmp, *pi;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- PUSH NODE ON STACK ---\n");
      printf("\nCurrent stack status(%d nodes): ", STACKsize(stk));
      SLISTtraverse(stk, print, SLIST_FWD);

      tmp = read_int("\nEnter integer data of node to be pushed (-1=Quit): ", 0, 0);

      if (tmp == -1)
        break;

      pi = (int *)malloc(sizeof(int));
      MALCHK(pi);

      *pi = tmp;

      if ((STACKpush(stk, pi)) != OK)
        {
          printf("\nFatal error pushing data - exiting...!");
          STACKdestroy(stk);
          exit(-1);
        }
      else
        {
          sprintf(mess, "Node %d will be pushed on stack!", *pi);
          prompt_and_pause(mess);
        }
    } while (TRUE);
}

/* --- Function: void pop_node(Stack stk) --- */
void pop_node(Stack stk)
{
  int tmp, *pi, *ptmp;
  char mess[BUFSIZ], ans;

  /* Initialize 'tmp'... */
  tmp = 0;

  do
    {
      if (tmp == -1)
        break;

      my_clearscrn();
      printf("--- POP NODE FROM STACK ---\n");
      printf("\nCurrent stack status(%d nodes): ", STACKsize(stk));
      SLISTtraverse(stk, print, SLIST_FWD);

      ptmp = (int *)STACKpeek(stk);

      if (ptmp  == NULL)
        {
          prompt_and_pause("\n\nStack is EMPTY - nothing to do..!");
          tmp = -1;
        }
      else
        {
          sprintf(mess, "\nAbout to pop node %d.. - Continue? (y/n): ", *ptmp);
          ans = read_char(mess, 0, 0, my_chkch);

          if (ans == 'y' || ans == 'Y')
            {
              if ((STACKpop(stk, (void **)&pi)) != OK)
                {
                  printf("\nFatal error popping data - exiting...!");
                  STACKdestroy(stk);
                  exit(-1);
                }
              else
                {
                  sprintf(mess, "Node %d will be popped!", *pi);
                  prompt_and_pause(mess);
                  my_destroy(pi);
                }
            }
          else
            tmp = -1;
        }
    } while (TRUE);
}

/* --- Function: void dequeue_push_node(Queue que, Stack stk) --- */
void dequeue_push_node(Queue que, Stack stk)
{
  int tmp, *pi, *ptmp;
  char mess[BUFSIZ], ans;

  /* Initialize 'tmp'... */
  tmp = 0;

  do
    {
      if (tmp == -1)
        break;

      my_clearscrn();
      printf("--- DEQUEUE NODE FROM QUEUE AND PUSH ON STACK ---\n");
      printf("\nCurrent queue/stack status: ");
      printf("\nQueue: ");
      SLISTtraverse(que, print, SLIST_FWD);
      printf(" (%d nodes)", QUEUEsize(que));
      printf("\nStack: ");
      SLISTtraverse(stk, print, SLIST_FWD);
      printf(" (%d nodes)", STACKsize(stk));

      ptmp = (int *)QUEUEpeek(que);

      if (ptmp  == NULL)
        {
          prompt_and_pause("\n\nQueue is EMPTY - nothing to do..!");
          tmp = -1;
        }
      else
        {
          sprintf(mess, "\nAbout to dequeue node %d - and push it on stack.. - Continue? (y/n): ", *ptmp);
          ans = read_char(mess, 0, 0, my_chkch);
          
          if (ans == 'y' || ans == 'Y')
            {
              if ((QUEUEdequeue(que, (void **)&pi)) != OK)
                {
                  printf("\nFatal error dequeing data - exiting...!");
                  QUEUEdestroy(que);
                  exit(-1);
                }
              else
                {
                  if (STACKpush(stk, pi) != OK)
                    {
                      printf("\nFatal error pushing data - exiting...!");
                      STACKdestroy(stk);
                      exit(-1);
                    }

                  sprintf(mess, "Node %d will be dequeued - and pushed on stack..!", *pi);
                  prompt_and_pause(mess);
                }
            }
          else
            tmp = -1;
        }
    } while (TRUE);
}

/* --- Function: void print_queue_stack(Queue que, Stack stk) --- */
void print_queue_stack(Queue que, Stack stk)
{
  my_clearscrn();
  printf("--- PRINT QUEUE AND STACK ---\n");
  printf("\nCurrent contents: ");
  printf("\nQueue: ");
  SLISTtraverse(que, print, SLIST_FWD);
  printf(" (%d nodes)", QUEUEsize(que));
  printf("\nStack: ");
  SLISTtraverse(stk, print, SLIST_FWD);
  printf(" (%d nodes)", STACKsize(stk));
  prompt_and_pause("\n\n");
}

/* --- Function: void final_status(Queue que, Stack stk) --- */
void final_status(Queue que, Stack stk)
{
  /* Final list status... */
  my_clearscrn();
  printf("--- FINAL STATUS ---\n");
  printf("\nFinal list contents: ");
  printf("\nQueue: ");
  SLISTtraverse(que, print, SLIST_FWD);
  printf(" (%d nodes)", QUEUEsize(que));
  printf("\nStack: ");
  SLISTtraverse(stk, print, SLIST_FWD);
  printf(" (%d nodes)", STACKsize(stk));
}

int main(void)
{
  /* Declare YOUR variables here ! */
  Stack mystack;
  Queue myqueue;
  int menu_choice;

  srand((unsigned int)time(NULL));

  if ((myqueue = QUEUEinit(my_destroy)) == NULL) /* Create new queue... */
    {
      printf("\nFatal error - bailing out...!");
      QUEUEdestroy(myqueue);
      exit(-1);
    }

  if ((mystack = STACKinit(my_destroy)) == NULL) /* Create new stack... */
    {
      printf("\nFatal error - bailing out...!");
      STACKdestroy(mystack);
      exit(-1);
    }

  /* Create and initialize queue and stack... */
  enqueue_push_nodes(myqueue, mystack, NR_OF_ITEMS);

  do
    {
      menu_choice = menu(MAIN_MENU_ROW, 0, 6);

      switch (menu_choice)
        {
        case 1:
          enqueue_node(myqueue);
          break;
        case 2:
          dequeue_node(myqueue);
          break;
        case 3:
          push_node(mystack);
          break;
        case 4:
          pop_node(mystack);
          break;
        case 5:
          dequeue_push_node(myqueue, mystack);
          break;
        case 6:
          print_queue_stack(myqueue, mystack);
          break;
        default:
          final_status(myqueue, mystack);
          break;
        }
    }
  while (menu_choice); 

  prompt_and_pause("\n\nLet's tidy up (destroy queue/stack)..- Bye!");

  STACKdestroy(mystack);
  QUEUEdestroy(myqueue);

  return 0;
}
