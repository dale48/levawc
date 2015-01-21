/**
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: demo11.c
 * Author  : Dan Levin
 * Date    : Thu Jan 08 20:06:18 2015
 * Version : 0.5
 * ---
 * Description: A demo program testing/showing the Graph ADT 
 * 
 * Revision history: (this is where you document the diffs between versions...)
 * Date   Revision
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graph.h"

#ifndef OK
#define OK 0
#endif

#define NR_OF_VERTICES 7
#define NR_OF_EDGES 20

/* Some string macros for the main menu... */
#define MAIN_MENU_ROW "\nMENU: 0=Exit 1=Add_Vertex 2=Remove_Vertex 3=Add_Edge 4=Remove_Edge"
#define MAIN_PROMPT "\nSelection <0-4>+<Enter>: "

/* Function declarations */
void my_destroy(void *data);
void print(const void *data);
int my_cmp(const void *key1, const void *key2);
int my_random(int start, int stop);
void my_clearscrn(void);
void prompt_and_pause(char *message);
int my_match(const void *k1, const void *k2);

/* Menu functions */
int is_sel_ok(const int sel, const int lowsel, const int hisel);
int menu(const int low_sel, const int hi_sel);

void create_rand_vertices(Graph gr, int nr_of_nodes);
/* Menu selections */
void ins_vertex(Graph gr);
void rem_vertex(Graph gr);
void ins_edge(Graph gr);
void rem_edge(Graph gr);
void final_status(Graph gr);

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

/* --- Function: void printvtx(const void *data) --- */
void printvtx(const void *data)
{
  printf("%02d", *(int *)data);
}

/* --- Function: void printvtx(const void *data) --- */
void printedge(const void *data)
{
  printf("%02d", *(int *)data);
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

int is_sel_ok(const int menusel, const int lowsel, const int hisel)
{
  int retval;

  return (retval = menusel>=lowsel && menusel<=hisel) ? 1 : 0;
}

/* --- Function: void add_nodes(Graph gr, int nr_of_nodes) --- */
void create_rand_vertices(Graph gr, int nr_of_nodes)
{
  int i=0, *pi, retval, dupctr=0;

  do
    {
      pi = (int *)malloc(sizeof(int));
      *pi = my_random(1,99);
      
      if ((retval = GRAPHinsvertex(gr, pi)) != OK) /* Insertion failed... */
	{
	  if (retval == 1) /* Duplicate key value.. */
	    {
	      dupctr++;
	      free(pi); /* Free node - since duplicate..  */
	    }
	  else
	    {
	      if (retval == -1)
		{
		  prompt_and_pause("Insertion of vertex failed..!\n");
		}
	      else
		{
		  prompt_and_pause("Fatal error - bailing out..!\n");
		  exit(-1);
		}
	    }
	}
    } while (++i < nr_of_nodes);

  printf("\n\nCurrent graph status:");
  GRAPHprint(gr, printvtx, printedge);
  printf("\n%d/%d successful insertions -- %d duplicates rejected...\n", GRAPHvcount(gr), nr_of_nodes, dupctr);
}

/* --- Function: void ins_vertex(Graph gr) --- */
void ins_vertex(Graph gr)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("\nCurrent graph status(%d vertices/%d edges): ", GRAPHvcount(gr), GRAPHecount(gr));
      GRAPHprint(gr, printvtx, printedge);

      printf("\nEnter data for vertex to be inserted (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */

      if (tmp == -1)
	break;

      pi = (int *)malloc(sizeof(int));
      *pi = tmp;

      if ((retval = GRAPHinsvertex(gr, pi)) != OK) /* Insertion failed... */
	{
	  if (retval == 1) /* Duplicate key value.. */
	    {
	      sprintf(mess, "Element %d already present..!", *pi);
	      prompt_and_pause(mess);
	      free(pi); /* Free element - since being duplicate..  */
	    }
	  else if (retval == -1)
	    {
	      prompt_and_pause("Fatal error - insertion failed..!\n");
	      free(pi); /* Free element - since being duplicate..  */
	    }
	  else
	    {
	      prompt_and_pause("Fatal error - bailing out..!\n");
	      exit(-1);
	    }
	}
      else
	{
	  sprintf(mess, "Element %d inserted..", *(int *)pi);
	  prompt_and_pause(mess);
	}
    } while (1);
}

/* --- Function: void rem_vertex(Graph gr) --- */
void rem_vertex(Graph gr)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("\nCurrent graph status(%d vertices/%d edges): ", GRAPHvcount(gr), GRAPHecount(gr));
      GRAPHprint(gr, printvtx, printedge);

      printf("\nEnter data for vertex to be removed (-1=Quit): ");
      scanf("%d", &tmp);
      getchar(); /* Remove CR from input buffer */

      if (tmp == -1)
	break;

      pi = &tmp;

      if ((retval = GRAPHremvertex(gr, (void **)&pi)) != OK) /* Vertex removal failed.. */
	{
	  /* Removal didn't work - node NOT found... */
	  if (retval == 1)
	    {
	      sprintf(mess, "Vertex %d not found..!", *(int *)pi);
	      prompt_and_pause(mess);
	    }
	  else
	    {
	      if (retval == -2)
		{
		  sprintf(mess, "Vertex %d not isolated - can't be removed..!", *(int *)pi);
		  prompt_and_pause(mess);
		}
	      else
		{
		  printf("Fatal failure - bailing out...");
		  exit(retval);
		}
	    }
	}
      else
	{
	  /* Removal succesful - notify user.. */
	  sprintf(mess, "Vertex %d removed..!", *(int *)pi);
	  prompt_and_pause(mess);
	  /* Free element - after being removed from graph.. */
	  free(pi);
	}
    } while (1);
}

/* --- Function: void ins_edge(Graph gr) --- */
void ins_edge(Graph gr)
{
  
}

/* --- Function: void rem_edge(Graph gr) --- */
void rem_edge(Graph gr)
{
  
}

int menu(const int low_sel, const int hi_sel)
{
  int retval, selection, sel_ok;

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
	  printf("Invalid input - integer only!");
	  getchar();
	}

    } while (retval == EOF || !sel_ok);

  return selection;
}

/* --- Function: void tidy_and_quit(Graph gr) --- */
void final_status(Graph gr)
{
  my_clearscrn();
  printf("\nFinal graph status(%d vertices/%d edges): ", GRAPHvcount(gr), GRAPHecount(gr));
  GRAPHprint(gr, printvtx, printedge);
}


int main(void)
{
  /* Declare YOUR variables here ! */
  Graph mygraph;
  char msg[BUFSIZ];
  int menu_choice;

  srand((unsigned int)time(NULL));
  my_clearscrn();

  printf("--- INITIALIZING A GRAPH, %d VERTICES, RANDOM INTEGER DATA ---", NR_OF_VERTICES);

  if ((mygraph = GRAPHinit(my_match, my_destroy)) == NULL)
    {
      printf("\nFatal error - bailing out...\n!");
      exit(-1);
    }
  
  /* Initialize - and add random vertices to the graph... */
  create_rand_vertices(mygraph, NR_OF_VERTICES);

  do
    {
      menu_choice = menu(0, 4);

      switch (menu_choice)
	{
        case 1:
	  ins_vertex(mygraph);
	  break;
        case 2:
	  rem_vertex(mygraph);
	  break;
        case 3:
	  ins_edge(mygraph);
	  break;
        case 4:
	  rem_edge(mygraph);
	  break;
        default:
	  final_status(mygraph);
	  break;
	}
    }
  while (menu_choice); 

  prompt_and_pause("\n\nLet's tidy up and destroy the graph - Bye...!");

  GRAPHdestroy(mygraph);

  return 0;

}
