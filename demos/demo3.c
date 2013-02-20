/**
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: demo3.c
 * Author  : Dan Levin
 * Date    : Wed Feb 20 11:59:40 GMT 2013
 * Version : 0.15
 * ---
 * Description: A demo of stack/queue ADT usage - in LevAWC
 *
 * Revision history: (this is where you document the diffs between versions...)
 * Date   Revision
 * 130122 Created this file - the first time..
 * 130123 Completed the code - and debugged/tested it as well..
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stack.h"
#include "queue.h"

#define NR_OF_ITEMS 10

/* Function declarations */
void my_destroy(void *data);
void print(const void *data);
int my_random(int start, int stop);
void my_clearscrn(void);
void prompt_and_pause(char *message);
void queue_elements(Queue que, int nr_of_ele);

/* Function definitions - the rest of the program */
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

/* --- void queue_elements(Queue que, int nr_of_ele) --- */
void queue_elements(Queue que, int nr_of_ele)
{
  int i=0, *pi;

  do
    {
      pi = (int *)malloc(sizeof(int));
      *pi = my_random(1,50);
      QUEUEenqueue(que, pi);
    } while (++i < nr_of_ele);

  printf("\nCurrent queue content(%d elements): ", QUEUEsize(que));
  /* This call to SLISTtraverse is possible - although this function is NOT a member
     of the queue (public) interface - because a queue is also a list ! */
  SLISTtraverse(que, print, SLIST_FWD);
}

int main(void)
{
  /* Declare YOUR variables here ! */
  Stack mystk;
  Queue myqueue;
  char mess[BUFSIZ];
  int i, nr;

  srand((unsigned int)time(NULL));
  my_clearscrn();

  printf("--- INITIALIZING A QUEUE, %d ELEMENTS, RANDOM INTEGER DATA ---", NR_OF_ITEMS);
  if ((myqueue = QUEUEinit(my_destroy)) == NULL) /* Initialize the queue... */
    {
      printf("\nFatal error - bailing out...!");
      exit(-1);
    }

  queue_elements(myqueue, NR_OF_ITEMS); /* Populate the queue... */

  nr = QUEUEsize(myqueue)/2;  /* Save half the size of the queue... */
  sprintf(mess, "\nNext - let's DEQUEUE %d elements from our queue...", nr);
  prompt_and_pause(mess);
  prompt_and_pause("...and now PUSH them - on a brand, new STACK...!!");

  if ((mystk = STACKinit(my_destroy)) == NULL) /* Set up a new stack... */
    {
      printf("\nFatal error - bailing out...!");
      exit(-1);
    }

  for (i = 0; i < nr; ++i)
    {
      void *piq, *pis;
      int tmp;

      QUEUEdequeue(myqueue, &piq);
      sprintf(mess, "QUEUE: Dequeued: %02d (new frontvalue: %02d)", *(int *)piq, *(int *)QUEUEpeek(myqueue));
      prompt_and_pause(mess);

      /* Check current stack top... */
      pis = STACKpeek(mystk);
      /* Push the value just dequeued - from our queue... */
      STACKpush(mystk, piq);

      if (pis == NULL) /* If this is the FIRST stack push... */
	sprintf(mess, "STACK: Pushed  : %02d (old stacktop  : none)", *(int *)STACKpeek(mystk));
      else
	sprintf(mess, "STACK: Pushed  : %02d (old stacktop  : %02d)", *(int *)STACKpeek(mystk), *(int *)pis);

      /* Print the message assembled above... */
      prompt_and_pause(mess);
    }

  printf("\n--- CURRENT STATUS OF STACK AND QUEUE ---");
  printf("\nStack: ");
  SLISTtraverse(mystk, print, SLIST_FWD);
  printf("\nQueue: ");
  SLISTtraverse(myqueue, print, SLIST_FWD);
  
  prompt_and_pause("\n\nLet's tidy up (destroy queue/stack) - Bye!");

  SLISTdestroy(mystk);
  SLISTdestroy(myqueue);

  return 0;
}
