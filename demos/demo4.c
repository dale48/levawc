/**
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: demo4.c
 * Author  : Dan Levin
 * Date    : Wed Feb 20 11:59:40 GMT 2013
 * Version : 0.15
 * ---
 * Description: Usage demo of the chained hashtable ADT - in LevAWC. 
 *
 * Revision history: (this is where you document the diffs between versions...)
 * Date   Revision
 * 130201 Created this program the first time..
 * 130324 Changed functions "insert_nodes(), remove_nodes()" - and made the loops in them terminated by user interaction.
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "chashtbl.h"

#ifndef OK
#define OK 0
#endif

#define NR_OF_ITEMS 30
#define NR_OF_BUCKETS 11

/* Function declarations */
void my_destroy(void *data);
void print(const void *data);
int my_cmp(const void *key1, const void *key2);
int my_random(int start, int stop);
void my_clearscrn(void);
void prompt_and_pause(char *message);
int my_match(const void *k1, const void *k2);
int my_hash(const void *key);

void add_nodes(CHtbl list, int nr_of_nodes);
void remove_nodes(CHtbl list);
void insert_nodes(CHtbl list);

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

/* --- Function: void add_nodes(CHtbl tbl, int nr_of_nodes) --- */
void add_nodes(CHtbl tbl, int nr_of_nodes)
{
  int i=0, *pi, retval, dupctr=0;

  do
    {
      pi = (int *)malloc(sizeof(int));
      *pi = my_random(1,99);
      
      if ((retval = CHTBLinsert(tbl, pi)) != 0) /* Insertion failed... */
	{
	  if (retval == 1) /* Duplicate key value.. */
	    {
	      dupctr++;
	      free(pi); /* Free node - since duplicate..  */
	    }
	  else
	    {
	      prompt_and_pause("Fatal error - bailing out..!\n");
	      exit(-1);
	    }
	}
    } while (++i < nr_of_nodes);

  printf("\nCurrent table status:");
  CHTBLprint(tbl, print);
  printf("\n%d/%d successful insertions -- %d duplicates rejected...", CHTBLsize(tbl), nr_of_nodes, dupctr);
}

/* --- Function: void remove_nodes(CHtbl tbl, int nr_of_removes) --- */
void remove_nodes(CHtbl tbl)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("\nCurrent table status(%d elements): ", CHTBLsize(tbl));
      CHTBLprint(tbl, print);

      printf("\nEnter data for element to be removed (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */

      if (tmp == -1)
	break;

      pi = &tmp;
      if ((retval = CHTBLremove(tbl, (void **)&pi)) != 0) /* Node removal failed.. */
	{
	  /* Removal didn't work - node NOT found... */
	  if (retval == 1)
	    {
	      sprintf(mess, "Element %d not found..!", *(int *)pi);
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
	  sprintf(mess, "Element %d removed - from bucket %d..!", *(int *)pi, (*pi)%NR_OF_BUCKETS);
	  prompt_and_pause(mess);
	  /* Free element - after being removed from table.. */
	  free(pi);
	}

    } while (1);
}

/* --- Function: void insert_nodes(CHtbl tbl, int nr_of_insertions) --- */
void insert_nodes(CHtbl tbl)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("\nCurrent table status(%d elements): ", CHTBLsize(tbl));
      CHTBLprint(tbl, print);

      printf("\nEnter data for element to be inserted (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */

      if (tmp == -1)
	break;

      pi = (int *)malloc(sizeof(int));
      *pi = tmp;

      if ((retval = CHTBLinsert(tbl, pi)) != 0) /* Insertion failed... */
	{
	  if (retval == 1) /* Duplicate key value.. */
	    {
	      sprintf(mess, "Element %d already present..!", *pi);
	      prompt_and_pause(mess);
	      free(pi); /* Free element - since being duplicate..  */
	    }
	  else
	    {
	      prompt_and_pause("Fatal error - bailing out..:!\n");
	      exit(-1);
	    }
	}
      else
	{
	  sprintf(mess, "Element %d inserted - in bucket %d..", *(int *)pi, (*pi)%NR_OF_BUCKETS);
	  prompt_and_pause(mess);
	}

    } while (1);
}

int main(void)
{
  /* Declare YOUR variables here ! */
  CHtbl mytbl;
  char msg[BUFSIZ];

  srand((unsigned int)time(NULL));
  my_clearscrn();


  printf("--- INITIALIZING A CHAINED HASHTABLE, %d ELEMENTS, RANDOM INTEGER DATA ---", NR_OF_ITEMS);

  if ((mytbl = CHTBLinit(NR_OF_BUCKETS, my_hash, my_match, my_destroy)) == NULL)
    {
      printf("\nFatal error - bailing out...\n!");
      exit(-1);
    }
  
  /* Initialize - and add elements to the table... */
  add_nodes(mytbl, NR_OF_ITEMS);

  sprintf(msg, "\n\nNow - let's ADD some elements to the table");
  prompt_and_pause(msg);
  
  /* Do the manual insertions... */
  insert_nodes(mytbl);
  my_clearscrn();  
  printf("\nCurrent table status(%d elements): ", CHTBLsize(mytbl));
  CHTBLprint(mytbl, print);

  sprintf(msg, "\n\nNow - let's DELETE some elements from the table");
  prompt_and_pause(msg);

  /* Do the manual removals... */
  remove_nodes(mytbl);
  my_clearscrn();  
  printf("\nFinal table status(%d elements): ", CHTBLsize(mytbl));
  CHTBLprint(mytbl, print);

  prompt_and_pause("\n\nLet's tidy up and destroy the hashtable - Bye...!");

  CHTBLdestroy(mytbl);

  return 0;
}
