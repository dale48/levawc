/**
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: demo3.c
 * Author  : Dan Levin
 * Date    : Thu Jan 22 21:10:38 2015
 * Version : 0.5
 * ---
 * Description: A demo of stack/queue ADT usage - in LevAWC
 *
 * Revision history: (this is where you document the diffs between versions...)
 * Date   Revision
 * 130122 Created this file - the first time..
 * 130123 Completed the code - and debugged/tested it as well..
 * 150121 Made this demo3.c menu-driven
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "stack.h"
#include "queue.h"

#define NR_OF_ITEMS 8

#ifndef OK
#define OK 0
#endif

/* Some string macros for the main menu... */
#define MAIN_MENU_ROW "\n\nMENU: 0=Exit 1=Enqueue 2=Dequeue 3=Push 4=Pop 5=Dequeue/Push"
#define MAIN_PROMPT "\nSelection <0-5>+<Enter>: "


/* FUNCTION DECLARATIONS */
void my_destroy(void *data);
void print(const void *data);
int my_random(int start, int stop);
void my_clearscrn(void);
void prompt_and_pause(char *message);
void enqueue_push_nodes(Queue que, Stack stk, int nr_of_ele);

/* Functions handling menu selections */
void enqueue_node(Queue que);
void dequeue_node(Queue que);
void push_node(Stack stk);
void pop_node(Stack stk);
void dequeue_push_node(Queue que, Stack stk);
void final_status(Queue que, Stack stk);

/* Menu (handling) functions */
int is_sel_ok(const int menusel, const int lowsel, const int hisel);
int menu(const int low_sel, const int hi_sel);
/* END-OF-FUNCTION-DECLARATIONS */

/* FUNCTION DEFINITIONS - that is, the rest of the program */
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

/* --- Function: void queue_elements(Queue que, Stack stk, int nr_of_ele) --- */
void enqueue_push_nodes(Queue que, Stack stk, int nr_of_ele)
{
  int i=0, *pi, retval;

  do
    {
      /* Create dyn. memory, store random nr - and enqueue... */
      pi = (int *)malloc(sizeof(int));
      *pi = my_random(1,50);
      retval = QUEUEenqueue(que, pi);
      assert(retval == OK);
      /* Create dyn. memory, store random nr - and push... */
      pi = (int *)malloc(sizeof(int));
      *pi = my_random(1,50);
      retval = STACKpush(stk, pi);
      assert(retval == OK);
    } while (++i < nr_of_ele);

  printf("\nCurrent queue(%d elements) and stack(%d elements) contents: ", QUEUEsize(que), STACKsize(stk));
  /* The following calls to SLISTtraverse are possible - although this function is NOT a member
     of the queue/stack (public) interfaces - but because a queue/stack is also a list ! */
  printf("\nQueue: ");
  SLISTtraverse(que, print, SLIST_FWD);
  printf("\nStack: ");
  SLISTtraverse(stk, print, SLIST_FWD);
}

/* --- Function: void enqueue_node(Queue que) --- */
void enqueue_node(Queue que)
{
  int tmp, *pi;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- ENQUEUE NODES ---");
      printf("\nCurrent queue status(%d nodes): ", QUEUEsize(que));
      SLISTtraverse(que, print, SLIST_FWD);

      printf("\n\nEnter integer data of node to be enqueued (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */

      if (tmp == -1)
        break;

      pi = (int *)malloc(sizeof(int));
      *pi = tmp;

      if ((QUEUEenqueue(que, pi)) != OK)
        {
          printf("\nFatal error enqueing data - exiting...!");
          exit(-1);
        }
      else
        {
          sprintf(mess, "Node %d enqueued!", *pi);
          prompt_and_pause(mess);
        }
    } while (1);
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
      my_clearscrn();
      printf("--- DEQUEUE NODES ---");
      printf("\nCurrent queue status(%d nodes): ", QUEUEsize(que));
      SLISTtraverse(que, print, SLIST_FWD);

      if (tmp == -1)
        break;

      ptmp = (int *)QUEUEpeek(que);

      if (ptmp  == NULL)
        {
          prompt_and_pause("\n\nQueue is EMPTY - nothing to do..!");
          tmp = -1;
        }
      else
        {
          sprintf(mess, "\nAbout to dequeue node %d..", *ptmp);
          printf("\n%s - Continue? (y/n+Enter): ", mess); 
          ans = getchar();
          getchar(); /* Remove '\n' from keyb. buffer */
          
          if (ans == 'y' || ans == 'Y')
            {
              if ((QUEUEdequeue(que, (void **)&pi)) != OK)
                {
                  printf("\nFatal error dequeing data - exiting...!");
                  exit(-1);
                }
              else
                {
                  sprintf(mess, "Node %d dequeued!", *pi);
                  prompt_and_pause(mess);
                  free(pi);
                }
            }
          else
            tmp = -1;
        }
    } while (1);
}

/* --- Function: void push_node(Stack stk) --- */
void push_node(Stack stk)
{
  int tmp, *pi;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- PUSH NODES ---");
      printf("\nCurrent stack status(%d nodes): ", STACKsize(stk));
      SLISTtraverse(stk, print, SLIST_FWD);

      printf("\n\nEnter integer data of node to be pushed (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */

      if (tmp == -1)
        break;

      pi = (int *)malloc(sizeof(int));
      *pi = tmp;

      if ((STACKpush(stk, pi)) != OK)
        {
          printf("\nFatal error pushing data - exiting...!");
          exit(-1);
        }
      else
        {
          sprintf(mess, "Node %d pushed on stack!", *pi);
          prompt_and_pause(mess);
        }
    } while (1);
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
      my_clearscrn();
      printf("--- POP NODES ---");
      printf("\nCurrent stack status(%d nodes): ", STACKsize(stk));
      SLISTtraverse(stk, print, SLIST_FWD);

      if (tmp == -1)
        break;

      ptmp = (int *)STACKpeek(stk);

      if (ptmp  == NULL)
        {
          prompt_and_pause("\n\nStack is EMPTY - nothing to do..!");
          tmp = -1;
        }
      else
        {
          sprintf(mess, "\nAbout to pop node %d..", *ptmp);
          printf("\n%s - Continue? (y/n+Enter): ", mess); 
          ans = getchar();
          getchar(); /* Remove '\n' from keyb. buffer */
          
          if (ans == 'y' || ans == 'Y')
            {
              if ((STACKpop(stk, (void **)&pi)) != OK)
                {
                  printf("\nFatal error popping data - exiting...!");
                  exit(-1);
                }
              else
                {
                  sprintf(mess, "Node %d popped!", *pi);
                  prompt_and_pause(mess);
                  free(pi);
                }
            }
          else
            tmp = -1;
        }
    } while (1);
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
      my_clearscrn();
      printf("--- DEQUEUE/PUSH NODES ---");
      printf("\nCurrent queue/stack status: ");
      printf("\nQueue: ");
      SLISTtraverse(que, print, SLIST_FWD);
      printf(" (%d nodes)", QUEUEsize(que));
      printf("\nStack: ");
      SLISTtraverse(stk, print, SLIST_FWD);
      printf(" (%d nodes)", STACKsize(stk));

      if (tmp == -1)
        break;

      ptmp = (int *)QUEUEpeek(que);

      if (ptmp  == NULL)
        {
          prompt_and_pause("\n\nQueue is EMPTY - nothing to do..!");
          tmp = -1;
        }
      else
        {
          sprintf(mess, "\nAbout to dequeue node %d - and push it onto stack..", *ptmp);
          printf("\n%s - Continue? (y/n+Enter): ", mess); 
          ans = getchar();
          getchar(); /* Remove '\n' from keyb. buffer */
          
          if (ans == 'y' || ans == 'Y')
            {
              if ((QUEUEdequeue(que, (void **)&pi)) != OK)
                {
                  printf("\nFatal error dequeing data - exiting...!");
                  exit(-1);
                }
              else
                {
                  if (STACKpush(stk, pi) != OK)
                    {
                      printf("\nFatal error pushing data - exiting...!");
                      exit(-1);
                    }

                  sprintf(mess, "Node %d dequeued - and pushed..!", *pi);
                  prompt_and_pause(mess);
                }
            }
          else
            tmp = -1;
        }
    } while (1);
}

/* --- Function: void final_status(Queue que, Stack stk) --- */
void final_status(Queue que, Stack stk)
{
  /* Final list status... */
  printf("\n\nFinal list contents: ");
  printf("\nQueue: ");
  SLISTtraverse(que, print, SLIST_FWD);
  printf(" (%d nodes)", QUEUEsize(que));
  printf("\nStack: ");
  SLISTtraverse(stk, print, SLIST_FWD);
  printf(" (%d nodes)", STACKsize(stk));
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
  int retval, selection, sel_ok;

  selection = EOF;

  do
    {
      printf("%s", MAIN_MENU_ROW);
      printf("%s", MAIN_PROMPT);
      retval = scanf("%d", &selection);

      if (retval == 1)
        {
          sel_ok = is_sel_ok(selection, low_sel, hi_sel);

          if (!sel_ok)
            printf("Invalid selection/input - use <%d> to <%d>...!", low_sel, hi_sel);        
          getchar();   
        }
      else
        {
          printf("Invalid input - use integer only!");
          getchar();
        }
    } while (retval != 1 || !sel_ok);

  return selection;
}

int main(void)
{
  /* Declare YOUR variables here ! */
  Stack mystack;
  Queue myqueue;
  int menu_choice;

  srand((unsigned int)time(NULL));
  my_clearscrn();

  if ((myqueue = QUEUEinit(my_destroy)) == NULL) /* Create new queue... */
    {
      printf("\nFatal error - bailing out...!");
      exit(-1);
    }

  if ((mystack = STACKinit(my_destroy)) == NULL) /* Create new stack... */
    {
      printf("\nFatal error - bailing out...!");
      exit(-1);
    }

  /* Create and initialize queue and stack... */
  printf("--- CREATING QUEUE AND STACK (%d nodes each), RANDOM INTEGER DATA ---", NR_OF_ITEMS);
  enqueue_push_nodes(myqueue, mystack, NR_OF_ITEMS);

  do
    {
      menu_choice = menu(0, 5);

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
        default:
          final_status(myqueue, mystack);
          break;
        }
    }
  while (menu_choice); 

  prompt_and_pause("\n\nLet's tidy up (destroy queue/stack) - Bye!");

  STACKdestroy(mystack);
  QUEUEdestroy(myqueue);

  return 0;
}
