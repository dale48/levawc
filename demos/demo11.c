/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: demo11.c
 * Author  : Dan Levin
 * Date    : Wed Mar 11 09:14:04 2015
 * Version : 0.51
 * ---
 * Description: Basic usage of Graph ADT
 * 
 * Revision history: (this is where you document the diffs between versions...)
 * Date   Revision
 * 150318 This source ready for version 0.51
 */

#include <stdio.h>
#include <stdlib.h>

#include "slist.h"
#include "graph.h"
#include "algo.h"
#include "utils.h"

/* --- MACRO DEFINITIONS --- */
#define STRSIZ BUFSIZ/128
#define MESSIZ BUFSIZ/4

#define ERR_DUPL 1
#define ERR_VTX_OR_EDGE_MISSING -2
#define ERR_VTX_NOT_ISOL -3
#define ERR_FATAL -1

#ifndef OK
#define OK 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define NR_OF_TASKNODES 9
#define NR_OF_NETNODES 6

#define INITIAL_INFO "--- INITIAL DEMO INFORMATION ---\n\nThis demo contains code from the book - \"K Loudon: Mastering Algoritms with C\".\n\nFor further details - check the following:\n\n - \"Chapter 11: Graphs\"\n - The \"graph\" subfolder - in downloadable example zipfile\n\nTip: Use paper/pencil to draw initial graphs - and their calculated results.\nCould be useful here :-)..\n\n"

#define MAIN_MENU_ROW "--- GRAPH BASIC USAGE & BFS/DFS DEMO ---\nMENU: 0=Exit 1=Basic_Graph_Usage 2=BFS/Hop_Count 3=DFS/Topological_Sort\nSelection "

/* --- GLOBAL VARIABLES --- */
/* Error indexes for messages */
enum err {err_dupl, err_vtx_or_edge_missing, err_vtx_not_isol, err_fatal, no_err};
char err_mess[][MESSIZ] = {"\n\nError: Duplicate vertex or edge!",
                           "\n\nError: Vertex or edge not found!",
                           "\n\nError: Vertex not isolated - can't be removed!",
                           "\n\nError: Fatal error!",
                           "\n\nEverything is OK"};
/* Bfs Graph Data */
int net_nodes[NR_OF_NETNODES] = {1, 2, 3, 4, 5, 6};
int net_connections[NR_OF_NETNODES][NR_OF_NETNODES] = 
  {{0,1,1,0,0,0}, {1,0,1,1,0,0}, {1,1,0,0,1,0},
   {0,1,0,0,1,0}, {0,0,1,1,0,1}, {0,0,0,0,1,0}};
 
/* Dfs Graph Data */
char task_nodes[NR_OF_TASKNODES] = {'a', 'b', 'c',
                                    'd', 'e', 'f',
                                    'g', 'h', 'i'};
char task_connections[NR_OF_TASKNODES][NR_OF_TASKNODES] = 
  {{0,1,1,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,1}, {0,0,0,0,0,0,0,0,1},
   {0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,1,0,1,0}, {0,0,1,0,0,0,0,1,0},
   {0,0,0,0,0,0,0,1,0}, {0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0}};

/* --- FUNCTION-DECLARATIONS --- */
/* Basic Usage Functions */
void prt_bas_vtx(const void *data);
void prt_bas_edge(const void *data);
static int bas_match(const void *str1, const void *str2);
void print_info(Graph gr, void (*vtxprt)(const void *data), void (*edgprt)(const void *data));
void print_insedge_errmess(int errcode);
void print_remedge_errmess(int errcode);
void print_insvtx_errmess(int errcode);
void print_remvtx_errmess(int errcode);
void print_errmess(int errcode);

/* Bfs Functions */
void bfs_destroy(void *data);
void prt_bfs_vtx(const void *data);
void prt_bfs_edge(const void *data);
int bfs_match(const void *k1, const void *k2);
int read_netnodes(Graph gr, int *pnodes, int nr_of_nodes);
int read_netconnections(Graph gr, int netconn[][NR_OF_NETNODES], int nr_of_nodes); 

/* Dfs Functions */
void dfs_destroy(void *data);
void prt_dfs_vtx(const void *data);
void prt_dfs_edge(const void *data);
int dfs_match(const void *k1, const void *k2);
int read_tasknodes(Graph gr, char *pnodes, int nr_of_nodes);
int read_taskconnections(Graph gr, char taskconn[][NR_OF_TASKNODES], int nr_of_nodes);
/* --- END-FUNCTION-DECLARATIONS --- */

/* --- FUNCTION DEFINITIONS --- */
/* --- Function: void prt_bas_vtx(const void *data) --- */
void prt_bas_vtx(const void *data)
{
  printf("%s", (char *)data);
}

/* --- Function: void prt_bas_edge(const void *data) --- */
void prt_bas_edge(const void *data)
{
  printf("%s ", (char *)data);
}
/* --- Function: bas_match(const void *str1, const void *str2) --- */
static int bas_match(const void *str1, const void *str2) 
{
  return !strcmp((const char *)str1, (const char *)str2); /* Determine whether two strings match */
}

/* --- Function: void print_info(Graph gr, void (*vtxprt)(const void *data), void (*edgprt)(const void *data)) --- */
void print_info(Graph gr, void (*vtxprt)(const void *data), void (*edgprt)(const void *data))
{
  my_clearscrn();
  printf("--- BASIC GRAPH OPERATIONS ---\n");
  GRAPHprint(gr, vtxprt, edgprt);
  printf("\n\nNr of vertices/edges: %d/%d ", GRAPHvcount(gr), GRAPHecount(gr));
}

/* --- Function: void print_errmess(int errcode) --- */
void print_errmess(int errcode)
{
  switch (errcode)
    {
    case ERR_DUPL:
      prompt_and_pause(err_mess[err_dupl]);
      break;
    case ERR_VTX_OR_EDGE_MISSING:
      prompt_and_pause(err_mess[err_vtx_or_edge_missing]);
      break;
    case ERR_VTX_NOT_ISOL:
      prompt_and_pause(err_mess[err_vtx_not_isol]);
      break;
    case ERR_FATAL:
      prompt_and_pause(err_mess[err_fatal]);
      break;
    default:
      prompt_and_pause(err_mess[no_err]);
      break;
    }
}

/* --- Function: void print_testmess(int testcode) --- */
void print_testmess(int testcode)
{
  if (testcode)
    printf("\n\nVertices ARE adjacent..");
  else
    printf("\n\nVertices are NOT adjacent..");
}

/* --- Function: void basics(void) --- */
int basics(void)
{
  Graph gr;
  VertexNode vnode;
  EdgeNode enode;
  char data[STRSIZ], *pdata1, *pdata2;
  int retval;
  char mess[MESSIZ];

  /* Initialize the graph. */
  gr = GRAPHinit(bas_match, free);
  my_clearscrn();
  printf("--- BASIC GRAPH USAGE ---\n\nGraph created.. - let's do some basic graph editing..");
  /* --- Perform some graph operations --- */
  /* --- Start with inserting some vertices.. --- */

  /* Insert first vertex.. */
  if ((pdata1 = (char *)malloc(STRSIZ)) == NULL)
    return 1;
  strcpy(pdata1, "a");
  sprintf(mess, "\n\nInsert first vertex: %s", pdata1);
  prompt_and_pause(mess);
  if (GRAPHinsvertex(gr, pdata1) != 0) /* Do the insertion.. */
    return 1;
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);
  
  /* Insert next vertex.. */
  if ((pdata1 = (char *)malloc(STRSIZ)) == NULL)
    return 1;
  strcpy(pdata1, "b");
  sprintf(mess, "\n\nInsert vertex: %s", pdata1);
  prompt_and_pause(mess);
  if (GRAPHinsvertex(gr, pdata1) != 0) /* Do the insertion.. */
    return 1;
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);

  /* Insert next vertex.. */
  if ((pdata1 = (char *)malloc(STRSIZ)) == NULL)
    return 1;
  strcpy(pdata1, "c");
  sprintf(mess, "\n\nInsert vertex: %s", pdata1);
  prompt_and_pause(mess);
  if (GRAPHinsvertex(gr, pdata1) != 0) /* Do the insertion.. */
    return 1;
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);

  /* Insert next vertex.. */
  if ((pdata1 = (char *)malloc(STRSIZ)) == NULL)
    return 1;
  strcpy(pdata1, "d");
  sprintf(mess, "\n\nInsert vertex: %s", pdata1);
  prompt_and_pause(mess);
  if (GRAPHinsvertex(gr, pdata1) != 0) /* Do the insertion.. */
    return 1;
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);

  /* Insert next vertex.. */
  if ((pdata1 = (char *)malloc(STRSIZ)) == NULL)
    return 1;
  strcpy(pdata1, "e");
  sprintf(mess, "\n\nInsert vertex: %s", pdata1);
  prompt_and_pause(mess);
  if (GRAPHinsvertex(gr, pdata1) != 0) /* Do the insertion.. */
    return 1;
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);
  
  prompt_and_pause("\n\nCheck out all vertex insertions above..");

  /* --- Now, let's insert some edges.. --- */
  /* Insert first edge.. */
  if ((pdata2 = (char *)malloc(STRSIZ)) == NULL)
    return 1;
  strcpy(data, "a");
  strcpy(pdata2, "b");
  sprintf(mess, "\nNow - let's insert edge: %s --> %s", data, pdata2);
  prompt_and_pause(mess);
  if (GRAPHinsedge(gr, data, pdata2) != 0) /* Do the insertion.. */
    return 1;
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);

  /* Insert next edge.. */
  if ((pdata2 = (char *)malloc(STRSIZ)) == NULL)
    return 1;
  strcpy(data, "a");
  strcpy(pdata2, "c");
  sprintf(mess, "\n\nInsert edge: %s --> %s", data, pdata2);
  prompt_and_pause(mess);
  if (GRAPHinsedge(gr, data, pdata2) != 0)
    return 1;
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);

  /* Insert next edge.. */
  if ((pdata2 = (char *)malloc(STRSIZ)) == NULL)
    return 1;
  strcpy(data, "b");
  strcpy(pdata2, "c");
  sprintf(mess, "\n\nInsert edge: %s --> %s", data, pdata2);
  prompt_and_pause(mess);
  if (GRAPHinsedge(gr, data, pdata2) != 0) /* Do the insertion.. */
    return 1;
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);

  /* Insert next edge.. */
  if ((pdata2 = (char *)malloc(STRSIZ)) == NULL)
    return 1;
  strcpy(data, "b");
  strcpy(pdata2, "d");
  sprintf(mess, "\n\nInsert edge: %s --> %s", data, pdata2);
  prompt_and_pause(mess);
  if (GRAPHinsedge(gr, data, pdata2) != 0) /* Do the insertion.. */
    return 1;
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);

  /* Insert next edge.. */
  if ((pdata2 = (char *)malloc(STRSIZ)) == NULL)
    return 1;
  strcpy(data, "c");
  strcpy(pdata2, "b");
  sprintf(mess, "\n\nInsert edge: %s --> %s", data, pdata2);
  prompt_and_pause(mess);
  if (GRAPHinsedge(gr, data, pdata2) != 0) /* Do the insertion.. */
    return 1;
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);

  /* Insert next edge.. */
  if ((pdata2 = (char *)malloc(STRSIZ)) == NULL)
    return 1;
  strcpy(data, "c");
  strcpy(pdata2, "c");
  sprintf(mess, "\n\nInsert edge: %s --> %s", data, pdata2);
  prompt_and_pause(mess);
  if (GRAPHinsedge(gr, data, pdata2) != 0)/* Do the insertion.. */
    return 1;
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);

  /* Insert next edge.. */
  if ((pdata2 = (char *)malloc(STRSIZ)) == NULL)
    return 1;
  strcpy(data, "c");
  strcpy(pdata2, "d");
  sprintf(mess, "\n\nInsert edge: %s --> %s", data, pdata2);
  prompt_and_pause(mess);
  if (GRAPHinsedge(gr, data, pdata2) != 0) /* Do the insertion.. */
    return 1;
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);

  /* Insert next edge.. */
  if ((pdata2 = (char *)malloc(STRSIZ)) == NULL)
    return 1;
  strcpy(data, "d");
  strcpy(pdata2, "a");
  sprintf(mess, "\n\nInsert edge: %s --> %s", data, pdata2);
  prompt_and_pause(mess);
  if (GRAPHinsedge(gr, data, pdata2) != 0) /* Do the insertion.. */
    return 1;
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);

  /* Insert next edge.. */
  if ((pdata2 = (char *)malloc(STRSIZ)) == NULL)
    return 1;
  strcpy(data, "e");
  strcpy(pdata2, "c");
  sprintf(mess, "\n\nInsert edge: %s --> %s", data, pdata2);
  prompt_and_pause(mess);
  if (GRAPHinsedge(gr, data, pdata2) != 0) /* Do the insertion.. */
    return 1;
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);

  /* Insert next edge.. */
  if ((pdata2 = (char *)malloc(STRSIZ)) == NULL)
    return 1;
  strcpy(data, "e");
  strcpy(pdata2, "d");
  sprintf(mess, "\n\nInsert edge: %s --> %s", data, pdata2);
  prompt_and_pause(mess);
  if (GRAPHinsedge(gr, data, pdata2) != 0) /* Do the insertion.. */
    return 1;
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);

  /* Now, let's remove some edges.. */
  /* Remove first edge.. */
  if ((pdata2 = (char *)malloc(STRSIZ)) == NULL)
    return 1;
  strcpy(data, "a");
  strcpy(pdata2, "c");
  pdata1 = pdata2;
  sprintf(mess, "\n\nRemove edge %s --> %s", data, pdata2);
  prompt_and_pause(mess);
  if (GRAPHremedge(gr, data, (void **)&pdata1) != 0) /* Do the removal.. */
    {
      free(pdata2);
      return 1;
    }
  free(pdata1); /* Deallocate mem. held by removed edge.. */
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);

  /* Remove next edge.. */
  strcpy(data, "c");
  strcpy(pdata2, "c");
  pdata1 = pdata2;
  sprintf(mess, "\n\nRemove edge %s --> %s", data, pdata2);
  prompt_and_pause(mess);
  if (GRAPHremedge(gr, data, (void **)&pdata1) != 0)  /* Do the removal.. */
    {
      free(pdata2);
      return 1;
    }
  free(pdata1); /* Deallocate mem. held by removed edge.. */
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);

  /* Remove next edge.. */
  strcpy(data, "e");
  strcpy(pdata2, "c");
  pdata1 = pdata2;
  sprintf(mess, "\n\nRemove edge %s --> %s", data, pdata2);
  prompt_and_pause(mess);
  if (GRAPHremedge(gr, data, (void **)&pdata1) != 0)  /* Do the removal.. */
    {
      free(pdata2);
      return 1;
    }
  free(pdata1); /* Deallocate mem. held by removed edge.. */
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);

  /* Remove next edge.. */
  strcpy(data, "a");
  strcpy(pdata2, "b");
  pdata1 = pdata2;
  sprintf(mess, "\n\nRemove edge %s --> %s", data, pdata2);
  prompt_and_pause(mess);
  if (GRAPHremedge(gr, data, (void **)&pdata1) != 0)  /* Do the removal.. */
    {
      free(pdata2);
      return 1;
    }
  free(pdata1); /* Deallocate mem. held by removed edge.. */
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);

  /* Remove next edge.. */
  strcpy(data, "d");
  strcpy(pdata2, "a");
  pdata1 = pdata2;
  sprintf(mess, "\n\nRemove edge %s --> %s", data, pdata2);
  prompt_and_pause(mess);
  if (GRAPHremedge(gr, data, (void **)&pdata1) != 0)  /* Do the removal.. */
    {
      free(pdata2);
      return 1;
    }
  free(pdata1); /* Deallocate mem. held by removed edge.. */
  free(pdata2); /* Deallocate mem. held by search key value(=a).. */
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);

  /* Now, lets remove a vertex.. */
  strcpy(data, "a");
  pdata1 = data;
  sprintf(mess, "\n\nRemove vertex %s", data);
  prompt_and_pause(mess);
  if (GRAPHremvertex(gr, (void **)&pdata1) != 0)  /* Do the removal.. */
    return 1;
  free(pdata1); /* Deallocate mem. held by removed vertex.. */
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);

  /* --- Do some invalid insertions/removals --- */
  /* First invalid operation.. */
  if ((pdata2 = (char *)malloc(STRSIZ)) == NULL)
    return 1;
  strcpy(data, "f");
  strcpy(pdata2, "a");
  sprintf(mess,"\n\nNow - try to insert an invalid edge from %s --> %s", data, pdata2);
  prompt_and_pause(mess);
  retval = GRAPHinsedge(gr, data, pdata2); /* Do the insertion.. */
  if (retval != 0)
    free(pdata2); /* Deallocte mem. held by search key value(=a).. */
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);
  print_errmess(retval);

  /* Next invalid operation.. */
  if ((pdata2 = (char *)malloc(STRSIZ)) == NULL)
    return 1;
  strcpy(data, "c");
  strcpy(pdata2, "b");
  sprintf(mess,"\nInsert an existing edge from %s --> %s", data, pdata2);
  prompt_and_pause(mess);
  retval = GRAPHinsedge(gr, data, pdata2); /* Do the insertion.. */
  if (retval != 0)
    free(pdata2); /* Deallocte mem. held by search key value(=b).. */
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);
  print_errmess(retval);

  /* Next invalid operation.. */
  if ((pdata2 = (char *)malloc(STRSIZ)) == NULL)
    return 1;
  strcpy(data, "f");
  strcpy(pdata2, "a");
  pdata1 = pdata2;
  retval = GRAPHremedge(gr, data, (void **)&pdata2);  /* Do the removal.. */
  sprintf(mess, "\nRemove an invalid edge from %s --> %s", data, pdata2);
  prompt_and_pause(mess);
  if (retval == 0)
    {
      free(pdata2); /* Deallocte mem. held by removed edge.. */
      free(pdata1); /* Deallocte mem. held by search key value(=b).. */
      return 1;
    }
  free(pdata2); /* Deallocte mem. held by search key value(=b).. */
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);
  print_errmess(retval);

  /* Next invalid operation.. */
  if ((pdata2 = (char *)malloc(STRSIZ)) == NULL)
    return 1;
  strcpy(data, "c");
  strcpy(pdata2, "e");
  pdata1 = pdata2;
  retval = GRAPHremedge(gr, data, (void **)&pdata2);  /* Do the removal.. */
  sprintf(mess, "\nRemoving an invalid edge from %s --> %s", data, pdata2);
  prompt_and_pause(mess);
  if (retval == 0)
    {
      free(pdata2); /* Deallocte mem. held by removed edge.. */
      free(pdata1); /* Deallocte mem. held by search key value(=e).. */
      return 1;
    }
  free(pdata2); /* Deallocte mem. held by search key value(=e).. */
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);
  print_errmess(retval);

  /* Next invalid operation.. */
  if ((pdata2 = (char *)malloc(STRSIZ)) == NULL)
    return 1;
  strcpy(pdata2, "c");
  retval = GRAPHinsvertex(gr, pdata2);  /* Do the insertion.. */
  sprintf(mess, "\nInsert an existing vertex %s", data);
  prompt_and_pause(mess);
  if (retval != 0) /* Insertion failed - which is the case here.. */
    free(pdata2); /* Deallocte mem. held by search key value(=c).. */
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);
  print_errmess(retval);

  /* Next invalid operation.. */
  if ((pdata2 = (char *)malloc(STRSIZ)) == NULL)
    return 1;
  strcpy(pdata2, "c");
  pdata1 = pdata2;
  retval = GRAPHremvertex(gr, (void **)&pdata2);  /* Do the insertion.. */
  sprintf(mess, "\nRemove an existing vertex %s", pdata1);
  prompt_and_pause(mess);
  if (retval == 0)
    {
      free(pdata2); /* Deallocte mem. held by removed vertex.. */
      free(pdata1); /* Deallocte mem. held by search key value(=c).. */
      return 1;
    }
  free(pdata2); /* Deallocte mem. held by search key value(=c).. */
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);
  print_errmess(retval);

  /* Next invalid operation.. */
  if ((pdata2 = (char *)malloc(STRSIZ)) == NULL)
    return 1;
  strcpy(pdata2, "d");
  pdata1 = pdata2;
  retval = GRAPHremvertex(gr, (void **)&pdata2);  /* Do the insertion.. */
  sprintf(mess, "\nRemove an existing vertex %s", pdata1);
  prompt_and_pause(mess);
  if (retval == 0)
    {
      free(pdata2); /* Deallocte mem. held by removed vertex.. */
      free(pdata1); /* Deallocte mem. held by search key value(=d).. */
      return 1;
    }
  free(pdata2); /* Deallocte mem. held by search key value(=d).. */
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);
  print_errmess(retval);

  /* --- Testing GRAPHis_adjacent() --- */
  /* First test of GRAPHis_adjacent().. */
  if ((pdata2 = (char *)malloc(STRSIZ)) == NULL)
    return 1;
  strcpy(data, "b");
  strcpy(pdata2, "d");
  retval = GRAPHis_adjacent(gr, data, pdata2); /* Do the test.. */
  sprintf(mess, "\nTest if (%s, %s) are adjacent...", data, pdata2);
  prompt_and_pause(mess);
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);
  print_testmess(retval);

  /* Next test of GRAPHis_adjacent().. */
  strcpy(data, "a");
  strcpy(pdata2, "e");
  retval = GRAPHis_adjacent(gr, data, pdata2); /* Do the test.. */
  sprintf(mess, "\n\nTest if (%s, %s) are adjacent...", data, pdata2);
  prompt_and_pause(mess);
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);
  print_testmess(retval);

  /* Next test of GRAPHis_adjacent().. */
  strcpy(data, "e");
  strcpy(pdata2, "d");
  retval = GRAPHis_adjacent(gr, data, pdata2); /* Do the test.. */
  sprintf(mess, "\n\nTest if (%s, %s) are adjacent...", data, pdata2);
  prompt_and_pause(mess);
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);
  print_testmess(retval);

  /* Next test of GRAPHis_adjacent().. */
  strcpy(data, "c");
  strcpy(pdata2, "a");
  retval = GRAPHis_adjacent(gr, data, pdata2); /* Do the test.. */
  sprintf(mess, "\n\nTest if (%s, %s) are adjacent...", data, pdata2);
  prompt_and_pause(mess);
  /* Print the result.. */
  print_info(gr, prt_bas_vtx, prt_bas_edge);
  print_testmess(retval);

  /* Free temporary dyn. memory - no longer used.. */
  free(pdata2);

  /* Print all adjacent vertices - to a certain vertex.. */
  strcpy(data, "c");
  sprintf(mess, "\n\nFinally - print vertices adjacent to %s: ", data);
  prompt_and_pause(mess);

  vnode = GRAPHfindvertex(gr, data);
  printf("\nVertices adjacent(=incident) from %s (%d pcs): ", data, GRAPHgetedgecount(vnode));

  for (enode = GRAPHgetedgehead(vnode); enode != NULL; enode = GRAPHgetedgenext(enode))
    {
      printf("%s ", (char *)GRAPHgetedgedata(enode));
    }

  /* Destroy the graph. */
  sprintf(mess, "\n\nTime to tidy up..");
  prompt_and_pause(mess);

  GRAPHdestroy(gr);

  return OK;
}

/* --- Function: void bfs_destroy(void *data) --- */
void bfs_destroy(void *data)
{
  free(((BfsVertexdata)data)->data);
  free((BfsVertexdata)data);
}

/* --- Function: void prt_bfs_vtx(const void *data) --- */
void prt_bfs_vtx(const void *data)
{
  printf("Node%02d", *(int *)((BfsVertexdata)data)->data);
}

/* --- Function: void prt_bfs_edge(const void *data) --- */
void prt_bfs_edge(const void *data)
{
  printf("Node%02d ", *(int *)((BfsVertexdata)data)->data);
}

/* --- Function: int bfs_match(const void *k1, const void *k2) --- */
int bfs_match(const void *k1, const void *k2)
{
  return *(int *)((BfsVertexdata)k1)->data == *(int *)((BfsVertexdata)k2)->data;
}

/* --- Function: int read_netnodes(Graph gr, int *pnodes, int nr_of_nodes) --- */
int read_netnodes(Graph gr, int *pnodes, int nr_of_nodes)
{
  int i, retval;
  BfsVertexdata bfs;

  for (i = 0; i < nr_of_nodes; ++i)
    {
      bfs = (BfsVertexdata)malloc(sizeof(struct BfsVertexdata_));
      MALCHK(bfs);
      
      bfs->data = (int *)malloc(sizeof(int));
      MALCHK(bfs->data);

      /* Copy data to vertex.. */
      *((int *)bfs->data) = *(pnodes+i);

      if ((retval = GRAPHinsvertex(gr, bfs)) != OK)
        {
          bfs_destroy(bfs);
          return retval;
        }
    }

  /* Everything OK */
  return OK;
}

/* --- Function: int read_netconnections(Graph gr, int netconn[][NR_OF_NETNODES], int nr_of_nodes) --- */
int read_netconnections(Graph gr, int netconn[][NR_OF_NETNODES], int nr_of_nodes)
{
  int i, j, tmp, retval;
  struct BfsVertexdata_ bfstmp;
  BfsVertexdata bfs;

  for (i = 0; i < nr_of_nodes; ++i)
    {
      for (j = 0; j < nr_of_nodes; ++j)
        {
          if (netconn[i][j] != 0)
            {
              bfs = (BfsVertexdata)malloc(sizeof(struct BfsVertexdata_));
              MALCHK(bfs);            

              bfs->data = (int *)malloc(sizeof(int));
              MALCHK(bfs->data);              

              *(int *)bfs->data = j+1;
              
              /* Temporary vertex (search) data.. */
              tmp = i+1;
              bfstmp.data = &tmp;

              if ((retval = GRAPHinsedge(gr, &bfstmp, bfs)) != OK)
                {
                  bfs_destroy(bfs);
                  return retval;
                }
            }
        }
    }
  /* Everything OK */
  return OK;
} 
 
int bfs(void)
{
  Graph gr;
  Slist network;
  struct BfsVertexdata_ bfstmp;
  BfsVertexdata netdata;
  int tmpid, retval;
  SlistNode listnode;

  my_clearscrn();
  printf("--- NETWORK HOPS/BFS DEMO ---");

  gr = GRAPHinit(bfs_match, bfs_destroy);

  /* Read net node(=vertex) data into graph.. */
  if ((read_netnodes(gr, net_nodes, NR_OF_NETNODES)) != OK)
    {
      fprintf(stderr, "Fatal error when reading netnode data - bailing out..");
      GRAPHdestroy(gr);
      exit(-1);
    }
  /* Read net connection(=edge) data into graph.. */
  if ((read_netconnections(gr, net_connections, NR_OF_NETNODES)) != OK)
    {
      fprintf(stderr, "Fatal error when reading netconnections data - bailing out..");
      GRAPHdestroy(gr);
      exit(-1);
    }

  prompt_and_pause("\n\nGraph initialized and graph data read..");
  printf("\nGRAPH:");

  /* Display graph.. */
  GRAPHprint(gr, prt_bfs_vtx, prt_bfs_edge);
  printf("\nNr of vertices/edges: %d/%d", GRAPHvcount(gr), GRAPHecount(gr));

  tmpid = read_int("\nEnter startnode id ", 1, 6);
  bfstmp.data = &tmpid;

  if ((retval = ALGObfs(gr, &bfstmp, &network, bfs_match)) != OK)
    {
      fprintf(stderr, "\nFatal error when calling 'ALGObfs()'(Return value: %d) - Bailing out..", retval);
      GRAPHdestroy(gr);
      exit(-1);
    }

  printf("\nNetwork Hops(BFS Analysis)\n--------------------------");
  for (listnode = SLISThead(network); listnode != NULL; listnode = SLISTnext(listnode))
    {
      netdata = (BfsVertexdata)SLISTdata(listnode);
      printf("\nNode%02d, color=%d, hops=%d", *(int *)netdata->data, netdata->color, netdata->hops);
    }
    
  prompt_and_pause("\n\nTime to tidy up..");

  SLISTdestroy(network);
  GRAPHdestroy(gr);
 
  return OK;
}

/* --- Function: void dfs_destroy(void *data) --- */
void dfs_destroy(void *data)
{
  free(((DfsVertexdata)data)->data);
  free((DfsVertexdata)data);
}

/* --- Function: void prt_dfs_vtx(const void *data) --- */
void prt_dfs_vtx(const void *data)
{
  printf("%c", *(char *)((DfsVertexdata)data)->data);
}

/* --- Function: void prt_dfs_edge(const void *data) --- */
void prt_dfs_edge(const void *data)
{
  printf("%c ", *(char *)((DfsVertexdata)data)->data);
}

/* --- Function: int dfs_match(const void *k1, const void *k2) --- */
int dfs_match(const void *k1, const void *k2)
{
  return *(char *)((DfsVertexdata)k1)->data == *(char *)((DfsVertexdata)k2)->data;
}

/* --- Function: int read_tasknodes(Graph gr, char *pnodes, int nr_of_nodes) --- */
int read_tasknodes(Graph gr, char *pnodes, int nr_of_nodes)
{
  int i, retval;
  DfsVertexdata dfs;

  for (i = 0; i < nr_of_nodes; ++i)
    {
      dfs = (DfsVertexdata)malloc(sizeof(struct DfsVertexdata_));
      MALCHK(dfs);
      
      dfs->data = (char *)malloc(sizeof(char));
      MALCHK(dfs->data);

      /* Copy data to vertex.. */
      *((char *)dfs->data) = *(pnodes+i);
      /* *pi = *(pnodes+i); */

      if ((retval = GRAPHinsvertex(gr, dfs)) != OK)
        {
          dfs_destroy(dfs);
          return retval;
        }
    }

  /* Everything OK */
  return OK;
}

/* --- Function: int read_taskconnections(Graph gr, char taskconn[][NR_OF_TASKNODES], int nr_of_nodes) --- */
int read_taskconnections(Graph gr, char taskconn[][NR_OF_TASKNODES], int nr_of_nodes)
{
  int i, j, tmp, retval;
  struct DfsVertexdata_ dfstmp;
  DfsVertexdata dfs;

  for (i = 0; i < nr_of_nodes; ++i)
    {
      for (j = 0; j < nr_of_nodes; ++j)
        {
          if (taskconn[i][j] != 0)
            {
              dfs = (DfsVertexdata)malloc(sizeof(struct DfsVertexdata_));
              MALCHK(dfs);            

              dfs->data = (char *)malloc(sizeof(char));
              MALCHK(dfs->data);              

              *(char *)dfs->data = 'a'+j;
              
              /* Temporary vertex (search) data.. */
              tmp = 'a'+i;
              dfstmp.data = &tmp;

              if ((retval = GRAPHinsedge(gr, &dfstmp, dfs)) != OK)
                {
                  dfs_destroy(dfs);
                  return retval;
                }
            }
        }
    }
  /* Everything OK */
  return OK;
}

int dfs(void)
{
  Graph gr;
  Slist tasks;
  DfsVertexdata taskdata;
  int retval;
  SlistNode listnode;

  my_clearscrn();
  printf("--- TOPOLOGICAL SORTING/DFS DEMO ---");

  gr = GRAPHinit(dfs_match, dfs_destroy);

  /* Read task node(=vertex) data into graph.. */
  if ((read_tasknodes(gr, task_nodes, NR_OF_TASKNODES)) != OK)
    {
      fprintf(stderr, "Fatal error when reading task node data - bailing out..");
      GRAPHdestroy(gr);
      exit(-1);
    }
  
  /* Read task connection(=edge) data into graph.. */
  if ((read_taskconnections(gr, task_connections, NR_OF_TASKNODES)) != OK)
    {
      fprintf(stderr, "Fatal error when reading taskconnections data - bailing out..");
      GRAPHdestroy(gr);
      exit(-1);
    }

  prompt_and_pause("\n\nGraph created and graph data read..");
  printf("\nGRAPH:");

  /* Display graph.. */
  GRAPHprint(gr, prt_dfs_vtx, prt_dfs_edge);
  printf("\nNr of vertices/edges: %d/%d", GRAPHvcount(gr), GRAPHecount(gr));

  if ((retval = ALGOdfs(gr, &tasks)) != OK)
    {
      fprintf(stderr, "\nFatal error when calling 'ALGOdfs()'(Return value: %d) - Bailing out..", retval);
      GRAPHdestroy(gr);
      exit(-1);
    }

  printf("\n\nTopological Sort(DFS Analysis):\n-------------------------------");
  for (listnode = SLISThead(tasks); listnode != NULL; listnode = SLISTnext(listnode))
    {
      taskdata = (DfsVertexdata)SLISTdata(listnode);
      printf("\nNode %c, color=%d", *(char *)taskdata->data, taskdata->color);
    }

  prompt_and_pause("\n\nTime to tidy up..");

  SLISTdestroy(tasks);
  GRAPHdestroy(gr);

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
          if ((retval = basics()) != OK)
            {
              exit(EXIT_FAILURE);
            }
          break;
        case 2:
          if ((retval = bfs()) != OK)
            {
              exit(EXIT_FAILURE);
            }
          break;
        case 3:
          if ((retval = dfs()) != OK)
            {
              exit(EXIT_FAILURE);
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
