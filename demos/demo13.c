/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: tst1.c
 * Author  : Dan Levin
 * Date    : Fri Feb 20 13:23:46 2015
 * Version : 0.51
 * ---
 * Description: A demo program testing/showing some Graph Algorithms 
 * 
 * Revision history: (this is where you document the diffs between versions...)
 * Date   Revision
 * 150331 This code ready for ver. 0.51
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graph.h"
#include "algo.h"
#include "utils.h"

/* --- MACROS --- */
#ifndef OK
#define OK 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* MACRO DEFINITIONS */
#define NR_OF_MST_VERTICES 9
#define NR_OF_DSP_VERTICES 6
#define NR_OF_TSP_VERTICES 7
#define NR_OF_TSP_COORDS 2

#define INITIAL_INFO "--- INITIAL DEMO INFORMATION ---\n\nThis demo contains code from the book - \"K Loudon: Mastering Algoritms with C\".\n\nFor further details - check the following:\n\n - \"Chapter 16: Graph Algorithms\"\n - The \"graphalg\" subfolder - in downloadable example zipfile\n\nTip: Use paper/pencil to draw initial graphs - and their calculated results.\nCould be useful here :-)..\n\n"

#define MAIN_MENU_ROW "--- GRAPH ALGORITHMS DEMO ---\nMENU: 0=Exit 1=MST 2=DSP 3=TSP\nSelection "
/* --- END-MACRO-DEFINITIONS --- */

/* --- GLOBAL-VARIABLES --- */
char mst_vtx_data[NR_OF_MST_VERTICES]={'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'};

double mst_edge_data[NR_OF_MST_VERTICES][NR_OF_MST_VERTICES] =
  {{0,4.0,0,0,0,0,0,8.0,0}, {4.0,0,8.0,0,0,0,0,11.0,0}, {0,8.0,0,7.0,0,4.0,0,0,2.0},
   {0,0,7.0,0,9.0,14.0,0,0,0}, {0,0,0,9.0,0,10.0,0,0,0}, {0,0,4.0,14.0,10.0,0,2.0,0,0},
   {0,0,0,0,0,2.0,0,1.0,6.0}, {8.0,11.0,0,0,0,0,1.0,0,7.0}, {0,0,2.0,0,0,0,6.0,7.0,0}};

char dsp_vtx_data[NR_OF_DSP_VERTICES]={'a', 'b', 'c', 'd', 'e', 'f'};
double dsp_edge_data[NR_OF_DSP_VERTICES][NR_OF_DSP_VERTICES] =
  {{0,8.0,4.0,0,0,0}, {0,0,6.0,2.0,0,4.0}, {0,0,0,0,4.0,1.0},
   {0,0,0,0,0,0}, {0,0,0,0,0,5.0}, {0,2.0,0,7.0,4.0,0}};

double tsp_data[NR_OF_TSP_VERTICES][NR_OF_TSP_COORDS] = {{2.0,1.0},{1.0,3.0},{2.0,4.0},{4.0,3.0},
                                                         {5.0,2.0},{5.0,5.0},{6.0,3.0}};
/* --- END-GLOBAL-VARIABLES --- */

/* FUNCTION-DECLARATIONS */
/* Call-back functions */
void mst_destroy(void *data);
void dsp_destroy(void *data);
void tsp_destroy(void *data);
int mst_match(const void *k1, const void *k2);
int dsp_match(const void *k1, const void *k2);
int tsp_match(const void *k1, const void *k2);
void print_mst_vtx(const void *data);
void print_mst_edge(const void *data);
void print_mst(const void *data);
void print_dsp_vtx(const void *data);
void print_dsp_edge(const void *data);
void print_dsp(const void *data);
void print_tsp_vtx(const void *data);
/* void print_tsp_edge(const void *data); */

/* Menu selections */
int mst(void);
int tsp(void);
int dsp(void);

/* Misc. application functions.. */
int read_mst_vtx(Graph gr, char *vertices, int nr_of_vertices);
int read_mst_edge(Graph gr, double mst_edges[][NR_OF_MST_VERTICES], int nr_of_vertices);
int read_dsp_vtx(Graph gr, char *vertices, int nr_of_vertices);
int read_dsp_edge(Graph gr, double (*dsp_edges)[NR_OF_DSP_VERTICES], int nr_of_vertices);
int read_tsp_vtx(Slist lst, double (*vertices)[NR_OF_TSP_COORDS], int nr_of_vertices);

/* END-OF-FUNCTION-DECLARATIONS */

/* FUNCTION DEFINITIONS - the rest of the program */
/* --- Function: void mst_destroy(void *data) --- */
void mst_destroy(void *data)
{
  free(((MstVertexdata)data)->data);
  free(data);
}

/* --- Function: void dsp_destroy(void *data) --- */
void dsp_destroy(void *data)
{
  free(((DspVertexdata)data)->data);
  free(data);
}

/* --- Function: void tsp_destroy(void *data) --- */
void tsp_destroy(void *data)
{
  free(((TspVertexdata)data)->data);
  free(data);
}

/* --- Function: void print_mst_vtx(const void *data) --- */
void print_mst_vtx(const void *data)
{
  printf("%c ", *(char *)((MstVertexdata)data)->data);
}

/* --- Function: void print_mst_edge(const void *data) --- */
void print_mst_edge(const void *data)
{
  printf("%c %4.1lf ", *(char *)((MstVertexdata)data)->data, 
         ((MstVertexdata)data)->weight);
}

/* --- Function: void print_mst(const void *data) --- */
void print_mst(const void *data)
{
  MstVertexdata mst = (MstVertexdata)data;

  printf("\nVertex = %c, parent = %c", *(char *)mst->data,
         mst->parent != NULL ? *(char *)mst->parent->data : '-');
}

/* --- Function: void print_dsp_vtx(const void *data) --- */
void print_dsp_vtx(const void *data)
{
  printf("%c ", *(char *)((DspVertexdata)data)->data);
}

/* --- Function: void print_dsp_edge(const void *data) --- */
void print_dsp_edge(const void *data)
{
  printf("%c %4.1lf ", *(char *)((DspVertexdata)data)->data,
         ((DspVertexdata)data)->weight);
}

/* --- Function: void print_dsp(const void *data) --- */
void print_dsp(const void *data)
{
  DspVertexdata dsp = (DspVertexdata)data;

  printf("\nVertex = %c, parent = %c, distance=%.1lf", *(char *)dsp->data, 
         dsp->parent != NULL ? *(char *)dsp->parent->data : '-', dsp->distance);
}

/* --- Function: void print_tsp_vtx(const void *data) --- */
void print_tsp_vtx(const void *data)
{
  TspVertexdata tsp = (TspVertexdata)data;

  printf("\nVertex: %c, x: %.1lf, y: %.1lf", *(char *)tsp->data, tsp->x, tsp->y);
}

/* --- Function: int mst_match(const void *k1, const void *k2) --- */
int mst_match(const void *k1, const void *k2)
{
  return *(char *)((MstVertexdata)k1)->data == *(char *)((MstVertexdata)k2)->data;
}

/* --- Function: int dsp_match(const void *k1, const void *k2) --- */
int dsp_match(const void *k1, const void *k2)
{
  return *(char *)((DspVertexdata)k1)->data == *(char *)((DspVertexdata)k2)->data;
}

/* --- Function: int tsp_match(const void *k1, const void *k2) --- */
int tsp_match(const void *k1, const void *k2)
{
  return *(char *)((DspVertexdata)k1)->data == *(char *)((DspVertexdata)k2)->data;
}

/* --- Function: int mst(void) --- */
int mst(void)
{
  Graph gr;
  Slist scan;
  struct MstVertexdata_ mst_tmp;
  char start;

  my_clearscrn();
  printf("--- MINIMAL SPANNING TREE ---");

  gr = GRAPHinit(mst_match, mst_destroy);

  if ((read_mst_vtx(gr, mst_vtx_data, NR_OF_MST_VERTICES)) != OK)
    {
      GRAPHdestroy(gr);
      return -2;
    }

  if ((read_mst_edge(gr, mst_edge_data, NR_OF_MST_VERTICES)) != OK)
    {
      GRAPHdestroy(gr);
      return -3;
    }

  prompt_and_pause("\n\nGraph initialized and graph data read..");
  printf("\nGRAPH:");
  
  /* Display graph.. */
  GRAPHprint(gr, print_mst_vtx, print_mst_edge);
  printf("\n--------------------------------------\nNr of vertices/edges: %d/%d", 
         GRAPHvcount(gr), GRAPHecount(gr));

  start = read_char("\nStart node ", 'a', 'i', isalpha);
  mst_tmp.data = &start;

  /* Now - call appropriate algorithm to compute Minimal Spanning Tree.. */
  if (ALGOmst(gr, &mst_tmp, &scan, mst_match) != OK)
    {
      GRAPHdestroy(gr);
      return -4;
    }

  /* Display Minimal Spanning Tree.. */
  printf("\nMinimal spanning tree:\n");
  SLISTtraverse(scan, print_mst, SLIST_FWD);
  prompt_and_pause("\n\n");

  /* Tidy up.. */
  SLISTdestroy(scan);
  GRAPHdestroy(gr);

  /* Everything is OK */
  return OK;
}

/* --- Function: int read_mst_vtx(Graph gr, char *vertices, int nr_of_vertices) --- */
int read_mst_vtx(Graph gr, char *vertices, int nr_of_vertices)
{
  int i, retval;
  MstVertexdata mst;

  for (i = 0; i < nr_of_vertices; ++i)
    {
      mst = (MstVertexdata)malloc(sizeof(struct MstVertexdata_));
      MALCHK(mst);
      
      mst->data = (char *)malloc(sizeof(char));
      MALCHK(mst->data);

      /* Copy data to vertex - by pointer arithmetic.. */
      *((char *)mst->data) = *(vertices+i);
      
      /* Insert vertex data into graph.. */
      if ((retval = GRAPHinsvertex(gr, mst)) != OK)
        {
          mst_destroy(mst);
          return retval;
        }
    }

  /* Everything OK */
  return OK;   
}

/* --- Function: int read_mst_edge(Graph gr, double mst_edges[][NR_OF_MST_VERTICES], int nr_of vertices) --- */
int read_mst_edge(Graph gr, double mst_edges[][NR_OF_MST_VERTICES], int nr_of_vertices)
{
  int i, j, retval;
  char tmp;
  struct MstVertexdata_ mst_tmp;
  MstVertexdata mst;

  for (i = 0; i < nr_of_vertices; ++i)
    {
      for (j = 0; j < nr_of_vertices; ++j)
        {
          if (mst_edges[i][j] != 0)
            {
              mst = (MstVertexdata)malloc(sizeof(struct MstVertexdata_));
              MALCHK(mst);            

              mst->data = (char *)malloc(sizeof(char));
              MALCHK(mst->data);              

              *(char *)mst->data = 'a'+j;
              /* Distribute data - via indexing into 2-dim. array.. */
              mst->weight = mst_edges[i][j];
              
              /* Temporary vertex (search) data.. */
              tmp = 'a'+i;
              mst_tmp.data = &tmp;

              if ((retval = GRAPHinsedge(gr, &mst_tmp, mst)) != OK)
                {
                  mst_destroy(mst);
                  return retval;
                }
            }
        }
    }
  /* Everything OK */
  return OK;   
}

/* --- Function: int dsp(void) --- */
int dsp(void)
{
  Graph gr;
  Slist spaths;
  struct DspVertexdata_ dsp_tmp;
  char start;

  my_clearscrn();
  printf("--- DIJKSTRA'S SHORTEST PATH ---");

  gr = GRAPHinit(dsp_match, dsp_destroy);

  if ((read_dsp_vtx(gr, dsp_vtx_data, NR_OF_DSP_VERTICES)) != OK)
    {
      GRAPHdestroy(gr);
      return -2;
    }

  if ((read_dsp_edge(gr, dsp_edge_data, NR_OF_DSP_VERTICES)) != OK)
    {
      GRAPHdestroy(gr);
      return -3;
    }

  prompt_and_pause("\n\nGraph initialized and graph data read..");
  printf("\nGRAPH:");

  /* Display graph.. */
  GRAPHprint(gr, print_dsp_vtx, print_dsp_edge);
  printf("\n-------------------------------\nNr of vertices/edges: %d/%d", GRAPHvcount(gr), GRAPHecount(gr));

  start = read_char("\nStart node ", 'a', 'f', isalpha);
  dsp_tmp.data = &start;

  /* Now - call appropriate algorithm to compute Shortest Paths.. */
  if (ALGOdsp(gr, &dsp_tmp, &spaths, dsp_match) != OK)
    {
      GRAPHdestroy(gr);
      return -4;
    }

  /* Display Dijkstra's Shortest Path.. */
  printf("\nDijkstra's Shortest Paths:\n");
  SLISTtraverse(spaths, print_dsp, SLIST_FWD);
  prompt_and_pause("\n\n");

  /* Tidy up.. */
  SLISTdestroy(spaths);
  GRAPHdestroy(gr);

  /* Everything is OK */
  return OK;
}

/* --- Function: int read_dsp_vtx(Graph gr, char *vertices, int nr_of_vertices) --- */
int read_dsp_vtx(Graph gr, char *vertices, int nr_of_vertices)
{
  int i, retval;
  DspVertexdata dsp;

  for (i = 0; i < nr_of_vertices; ++i)
    {
      dsp = (DspVertexdata)malloc(sizeof(struct DspVertexdata_));
      MALCHK(dsp);
      
      dsp->data = (char *)malloc(sizeof(char));
      MALCHK(dsp->data);

      /* Copy data to vertex - by pointer arithmetic.. */
      *((char *)dsp->data) = *(vertices+i);
      
      /* Insert vertex data into graph.. */
      if ((retval = GRAPHinsvertex(gr, dsp)) != OK)
        {
          dsp_destroy(dsp);
          return retval;
        }
    }

  /* Everything OK */
  return OK;   
}

/* --- Function: int read_dsp_edge(Graph gr, double (*dsp_edges)[NR_OF_DSP_VERTICES], int nr_of_vertices) --- */
int read_dsp_edge(Graph gr, double (*dsp_edges)[NR_OF_DSP_VERTICES], int nr_of_vertices)
{
  int i, j, retval;
  char tmp;
  struct DspVertexdata_ dsp_tmp;
  DspVertexdata dsp;
  double *pdbl;

  for (i = 0; i < nr_of_vertices; ++i)
    {
      for (j = 0; j < nr_of_vertices; ++j)
        {
          
          /* Using pointer arithmetic here - just as an (uglier) alternative to indexing... */
          /* Get the adress of double numbers in 2-dim array - using pointer arithmetic */
          pdbl = *(dsp_edges+i)+j; 

          if ( *pdbl != 0) /* If current dbl nr not equal to 0.. */
            {
              dsp = (DspVertexdata)malloc(sizeof(struct DspVertexdata_));
              MALCHK(dsp);            

              dsp->data = (char *)malloc(sizeof(char));
              MALCHK(dsp->data);              

              *(char *)dsp->data = 'a'+j;

              /* Copy read nr into relevant variable.. */
              dsp->weight = *pdbl;
              /* Temporary vertex (search) data.. */
              tmp = 'a'+i;
              dsp_tmp.data = &tmp;
              /* Now - insert current edge into graph.. */
              if ((retval = GRAPHinsedge(gr, &dsp_tmp, dsp)) != OK)
                {
                  dsp_destroy(dsp);
                  return retval;
                }
            }
        }
    }
  /* Everything OK */
  return OK;   
}

/* --- Function: int tsp(void) --- */
int tsp(void)
{
  Slist tsp_list, tour;
  SlistNode node;
  struct TspVertexdata_ tsp_start;
  TspVertexdata tsp_rcd;
  char id;
  double x, y, total=0, distance;

  my_clearscrn();
  printf("--- TRAVELING SALESMAN'S PATH ---");

  tsp_list = SLISTinit(tsp_destroy);

  /* Read tsp data.. */
  if ((read_tsp_vtx(tsp_list, tsp_data, NR_OF_TSP_VERTICES)) != OK)
    {
      SLISTdestroy(tsp_list);
      return -2;
    }

  prompt_and_pause("\n\nTour list created - and data read..");  

  /* Display tsp data read.. */
  printf("\nTour Vertex Data:");
  SLISTtraverse(tsp_list, print_tsp_vtx, SLIST_FWD);
  
  /* Set up start vertex.. */
  id = read_char("\nStart node ", 'a', 'g', isalpha);
  tsp_start.data = &id;

  /* Now - call appropriate algorithm to compute Traveling Salesman Path.. */
  if (ALGOtsp(tsp_list, &tsp_start, &tour, tsp_match) != OK)
    {
      SLISTdestroy(tsp_list);
      return -4;
    }

  /* Print the optimized tour.. */
  printf("\nTraveling Salesman Path:");
  for (node = SLISThead(tour); node != NULL; node = SLISTnext(node))
    {
      tsp_rcd = SLISTdata(node);

      if (!SLISTishead(tour, node))
        {
          distance = sqrt(pow(tsp_rcd->x-x, 2.0) + pow(tsp_rcd->y-y, 2.0));
          total = total + distance;     
        }

      x = tsp_rcd->x;
      y = tsp_rcd->y;

      if (!SLISTishead(tour, node))
        printf("\nVertex=%c, distance=%.2lf", *(char *)tsp_rcd->data, distance);
      else
        printf("\nVertex=%c", *(char *)tsp_rcd->data);
    }

  printf("\n------------------------\nTotal = %.2lf", total);

  prompt_and_pause("\n\n");

  /* Tidy up.. */
  SLISTdestroy(tsp_list);

  /* Everything OK */
  return OK;
}

/* --- Function: int read_tsp_vtx(Slist lst, double (*vertices)[NR_OF_TSP_COORDS], int nr_of_vertices) --- */
int read_tsp_vtx(Slist lst, double (*vertices)[NR_OF_TSP_COORDS], int nr_of_vertices)
{
  int i, j, retval;
  TspVertexdata tsp;
  double *pdbl;

  for (i = 0; i < nr_of_vertices; ++i)
    {
      /* Allocate dyn. memory for a tsp record.. */
      tsp = (TspVertexdata)malloc(sizeof(struct TspVertexdata_));
      MALCHK(tsp);            

      tsp->data = (char *)malloc(sizeof(char));
      MALCHK(tsp->data);

      /* Copy current vertex id into record.. */
      *(char *)tsp->data = 'a'+i;

      for (j = 0; j < NR_OF_TSP_COORDS; ++j)
        {
          /* Get the adress of double numbers in 2-dim array - using pointer arithmetic */
          pdbl = *(vertices+i)+j; 

          /* Copy current nr read - into relevant variable.. */
          (j == 0) ? (tsp->x = *pdbl) : (tsp->y = *pdbl);
        }

      /* Now - insert current tsp data record into list.. */
      if ((retval = SLISTinsnext(lst, SLISTtail(lst), tsp)) != OK)
        {
          tsp_destroy(tsp);
          return retval;
        }
    }

  /* Everything OK */
  return OK;   
}

int main(void)
{
  /* Declare YOUR variables here ! */
  int menu_choice, retval;

  my_clearscrn();
  prompt_and_pause(INITIAL_INFO);

  /* Enter menu loop.. */
  do
    {
      menu_choice = menu(MAIN_MENU_ROW, 0, 3);

      switch (menu_choice)
        {
        case 1:
          if ((retval = mst()) != OK)
            {
              if (retval == -2)
                fprintf(stderr, "Error reading vertex data (errcode=%d).. - bailing out!", retval);
              else if (retval == -3)
                fprintf(stderr, "Error reading edge data (errcode=%d).. - bailing out!", retval);
              else
                fprintf(stderr, "Error: Fatal error (errcode=%d).. - bailing out!", retval);
              exit(-1);
            }
          break;
        case 2:
          if ((retval = dsp()) != OK)
            {
              if (retval == -2)
                fprintf(stderr, "Error reading vertex data (errcode=%d).. - bailing out!", retval);
              else if (retval == -3)
                fprintf(stderr, "Error reading edge data (errcode=%d).. - bailing out!", retval);
              else
                fprintf(stderr, "Error: Fatal error (errcode=%d).. - bailing out!", retval);
              exit(-1);
            }
          break;
        case 3:
          if ((retval = tsp()) != OK)
            {
              if (retval == -2)
                fprintf(stderr, "Error reading vertex data (errcode=%d).. - bailing out!", retval);
              else if (retval == -3)
                fprintf(stderr, "Error reading edge data (errcode=%d).. - bailing out!", retval);
              else
                fprintf(stderr, "Error: Fatal error (errcode=%d).. - bailing out!", retval);
              exit(-1);
            }
          break;
        default:
          prompt_and_pause("\nThat's all folks - Bye..!");
          break;
        }
    }
  while (menu_choice); 

  return 0;
}
