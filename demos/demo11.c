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
 * 150204 Made this demo11.c menu-driven.
 * 150206 This source is now ready for version 0.5!
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
#define MAIN_MENU_ROW "\n--- GRAPH DEMO ---\nMENU: 0=Exit 1=Add_Vertex 2=Rem_Vertex 3=Add_Edge 4=Rem_Edge 5=Print"
#define MAIN_PROMPT "\nSelection <0-5>+<Enter>: "

/* FUNCTION DECLARATIONS */
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
void print_graph(Graph gr);
void final_status(Graph gr);
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

/* --- Function: void printvtx(const void *data) --- */
void printvtx(const void *data)
{
  printf("%02d", *(int *)data);
}

/* --- Function: void printvtx(const void *data) --- */
void printedge(const void *data)
{
  printf("%02d ", *(int *)data);
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
              my_destroy(pi); /* Free node - since duplicate..  */
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

  my_clearscrn();
  printf("--- INITIALIZING A GRAPH, %d VERTICES, RANDOM INTEGER DATA ---", NR_OF_VERTICES);
  printf("\n\nCurrent graph status:");
  print_graph(gr);
  printf("\n\n%d/%d successful insertion(s) -- %d duplicates rejected...", GRAPHvcount(gr), nr_of_nodes, dupctr);
  prompt_and_pause("\n\n");
}

/* --- Function: void ins_vertex(Graph gr) --- */
void ins_vertex(Graph gr)
{
  int tmp, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- ADD VERTEX ---");
      printf("\n\nCurrent graph status(%d vertices/%d edges): ", GRAPHvcount(gr), GRAPHecount(gr));
      print_graph(gr);

      printf("\n\nEnter data for vertex to be inserted (-1=Quit): ");
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
              sprintf(mess, "Vertex %d already present..!", *pi);
              prompt_and_pause(mess);
              my_destroy(pi); /* Free node - since being duplicate..  */
            }
          else if (retval == -1) /* Vertex missing - or error.. */
            {
              prompt_and_pause("Vertex missing - or fatal error - insertion failed..!\n");
              my_destroy(pi); /* Free node - due to error..  */
            }
          else
            {
              prompt_and_pause("Fatal error - bailing out..!\n");
              exit(-1);
            }
        }
      else
        {
          sprintf(mess, "Vertex %d will be inserted..", *(int *)pi);
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
      printf("--- REMOVE VERTEX ---");
      printf("\n\nCurrent graph status(%d vertices/%d edges): ", GRAPHvcount(gr), GRAPHecount(gr));
      print_graph(gr);

      printf("\n\nEnter data for vertex to be removed (-1=Quit): ");
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
          sprintf(mess, "Vertex %d will be removed..!", *(int *)pi);
          prompt_and_pause(mess);
          /* Free node - after being removed from graph.. */
          my_destroy(pi);
        }
    } while (1);
}

/* --- Function: void ins_edge(Graph gr) --- */
void ins_edge(Graph gr)
{
  int tmp1, tmp2, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- ADD EDGE ---");
      printf("\n\nCurrent graph status(%d vertices/%d edges): ", GRAPHvcount(gr), GRAPHecount(gr));
      print_graph(gr);

      printf("\n\nEnter data for first vertex of the edge to be created (-1=Quit)   : ");
      scanf("%d", &tmp1);
      getchar(); /* Remove CR from input buffer */
      if (tmp1 == -1)
        break;

      printf("Enter data for adjacent vertex of the edge to be created (-1=Quit): ");
      scanf("%d", &tmp2);
      getchar(); /* Remove CR from input buffer */

      if (tmp2 == -1)
        break;

      pi = (int *)malloc(sizeof(int));
      *pi = tmp2;

      if ((retval = GRAPHinsedge(gr, &tmp1, pi)) != OK) /* Edge insertion failed.. */
        {
          /* Duplicate edge found... */
          if (retval == 1)
            {
              sprintf(mess, "Duplicate edge found - between %d and %d..!", tmp1, *(int *)pi);
              prompt_and_pause(mess);
              my_destroy(pi);
            }
          else
            {
              if (retval == -1)
                {
                  sprintf(mess, "Vertex missing - or fatal error..!");
                  prompt_and_pause(mess);
                  my_destroy(pi);
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
          /* Insertion succesful - notify user.. */
          sprintf(mess, "Edge between vertex %d and %d will inserted..!", tmp1, *(int *)pi);
          prompt_and_pause(mess);
        }
    } while (1);
}

/* --- Function: void rem_edge(Graph gr) --- */
void rem_edge(Graph gr)
{
  int tmp1, tmp2, *pi, retval;
  char mess[BUFSIZ];

  do
    {
      my_clearscrn();
      printf("--- REMOVE EDGE ---");
      printf("\n\nCurrent graph status(%d vertices/%d edges): ", GRAPHvcount(gr), GRAPHecount(gr));
      print_graph(gr);

      printf("\n\nEnter data for first vertex of the edge to be removed (-1=Quit)   : ");
      scanf("%d", &tmp1);
      getchar(); /* Remove CR from input buffer */

      if (tmp1 == -1)
        break;

      printf("Enter data for adjacent vertex of the edge to be removed (-1=Quit): ");
      scanf("%d", &tmp2);
      getchar(); /* Remove CR from input buffer */

      if (tmp2 == -1)
        break;

      pi = &tmp2;

      if ((retval = GRAPHremedge(gr, &tmp1, (void **)&pi)) != OK) /* Edge removal failed.. */
        {
          /* Removal didn't work -  adj vertices not found... */
          if (retval == -1)
            {
              sprintf(mess, "Edge connecting vertices %d and %d not found - or other error..!", tmp1, *(int *)pi);
              prompt_and_pause(mess);
            }
        }
      else
        {
          /* Removal succesful - notify user.. */
          sprintf(mess, "Edge between %d and %d will be removed..!", tmp1, *(int *)pi);
          prompt_and_pause(mess);
          /* Free node - after being removed from graph.. */
          my_destroy(pi);
        }
    } while (1);
}

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
          printf("Invalid input - integer only!");
          getchar();
        }
    } while (retval == EOF || !sel_ok);

  return selection;
}

/* --- Function: void print_graph(Graph gr) --- */
void print_graph(Graph gr)
{
  GRAPHprint(gr, printvtx, printedge);  
}

/* --- Function: void tidy_and_quit(Graph gr) --- */
void final_status(Graph gr)
{
  my_clearscrn();
  printf("--- FINAL STATUS ---");
  printf("\n\nFinal graph status(%d vertices/%d edges): ", GRAPHvcount(gr), GRAPHecount(gr));
  GRAPHprint(gr, printvtx, printedge);
}


int main(void)
{
  /* Declare YOUR variables here ! */
  Graph mygraph;
  int menu_choice;

  srand((unsigned int)time(NULL));

  if ((mygraph = GRAPHinit(my_match, my_destroy)) == NULL)
    {
      printf("\nFatal error - bailing out...\n!");
      exit(-1);
    }
  
  /* Initialize - and add random vertices to the graph... */
  create_rand_vertices(mygraph, NR_OF_VERTICES);

  /* Enter menu loop.. */
  do
    {
      menu_choice = menu(0, 5);

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
        case 5:
          my_clearscrn();
          printf("--- PRINT GRAPH AND EDGES ---\n");
          print_graph(mygraph);
          prompt_and_pause("\n\n");
          break;
        default:
          final_status(mygraph);
          break;
        }
    }
  while (menu_choice); 

  prompt_and_pause("\n\nLet's tidy up and destroy the graph..- Bye!");

  GRAPHdestroy(mygraph);

  return 0;

}
