/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: graph.c
 * Author  : Kyle Loudon/Dan Levin
 * Date    : Thu Jan 15 09:27:26 2015
 * Version : 0.51
 * ---
 * Description: A generic graph ADT - written in ANSI C. 
 * 
 * Date   Revision message
 * 150331 This code ready for version 0.51
 *
 */
/**
 * @file graph.c
 **/

#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

struct Vertex_
{
  void  *vertexdata;
  /* --- Collection of adjacent vertices of a vertex - i.e. its edges.. --- */
  Slist adj_vertices;
};

struct Graph_
{
  int vcount;
  int ecount;

  int (*match)(const void *key1, const void *key2);
  void (*destroy)(void *data);
  /* --- Collection of vertices within the graph --- */
  Slist vertices;
}; 

/* --- FUNCTION DEFINITIONS --- */
Graph GRAPHinit(int (*match)(const void *key1, const void *key2),
                void (*destroy)(void *data))
{
  Graph grtmp;

  /* Allocate memory for the graph structure */
  if ((grtmp = (Graph)malloc(sizeof(struct Graph_))) == NULL)
    return NULL;

  grtmp->match = match;
  grtmp->destroy = destroy;
  grtmp->vcount = 0;
  grtmp->ecount = 0;

  /* Initialize the vertex list */
  if ((grtmp->vertices = SLISTinit(NULL)) == NULL)
    return NULL;

  /* Set the match-callback function into the vertex list */
  SLISTsetmatch(grtmp->vertices, grtmp->match);

  return grtmp;
} 

void GRAPHdestroy(Graph graph)
{
  Vertex vtx;

  /* Remove each vertex, destroy its data and edges - and finally the vertex itself */
  while (SLISTsize(graph->vertices) > 0)
    {
      /* Remove the first vertex from the vertex collection */
      if (SLISTremnext(graph->vertices, NULL, (void **)&vtx) == 0)
        {
          /* Destroy the corresponding edges for current vertex */
          SLISTdestroy(vtx->adj_vertices);

          /* Destroy the vertex data */
          if (graph->destroy != NULL)
            graph->destroy(vtx->vertexdata);

          /* Destroy the vertex structure */
          free(vtx);
        }
    }

  /* Destroy the vertex collection structure, which is now empty of vertices */
  SLISTdestroy(graph->vertices);

  /* Destroy the graph structure */
  free(graph);
} 

int GRAPHinsvertex(Graph graph, const void *data)
{
  Vertex vtx;
  int retval;

  /* Do not allow the insertion of duplicate vertices */
  if (GRAPHfindvertex(graph, data) != NULL)
    return 1;

  /* Create the new vertex */
  if ((vtx = (Vertex)malloc(sizeof(struct Vertex_))) == NULL)
    return -1;

  /* Insert vertex data into the new vertex */
  vtx->vertexdata = (void *)data;
  
  /* Initialize/insert the adjacent vertices(=edges) for the new vertex */
  if ((vtx->adj_vertices = SLISTinit(graph->destroy)) == NULL)
    return -1;

  /* Set the match-callback function into the adjacent vertices(=edges) collection */
  SLISTsetmatch(vtx->adj_vertices, graph->match);

  /* Insert the new vertex into the vertex collection */
  if ((retval = SLISTinsnext(graph->vertices, SLISTtail(graph->vertices), vtx)) != 0)
    return retval;

  /* Update the vertex count */
  graph->vcount++;

  /* Everything OK */
  return 0;
}

int GRAPHinsedge(Graph graph, const void *vtxdata, const void *adjdata)
{
  SlistNode node;
  int retval;

  /* Do not allow insertion of an edge without both its vertices present.. */
  /* Therefore - check for presence of 'adjdata' in the vertex collection.. */
  if (GRAPHfindvertex(graph, adjdata) == NULL) /* --- Vertex not found --- */
    return -2;

  /* Check for presence of 'vtxdata' among vertices.. */
  if ((node = GRAPHfindvertex(graph, vtxdata)) == NULL) /* --- Vertex not found --- */
    return -2;

  /* Do not allow insertion of duplicate edges for 'node'.. */
  if (SLISTfindnode(((Vertex)SLISTdata(node))->adj_vertices, adjdata) != NULL) /* --- If duplicate.. --- */
    return 1;    

  /* Insert 'adjdata' into the edge collection of the vertex containing data 'vtxdata'.. */
  if ((retval = SLISTinsnext(((Vertex)SLISTdata(node))->adj_vertices, NULL, adjdata)) !=0)
    return retval;

  /* Update the edge count */
  graph->ecount++;

  /* Everything OK */
  return 0;
}

int GRAPHremvertex(Graph graph, void **data)
{
  SlistNode node, tmp, prev;
  Vertex vtx;

  /* Search for vertex matching 'data'... */
  if ((node = GRAPHfindvertex(graph, *data)) == NULL) /* Vertex not found */
    return -2;   /* Return -2 if NOT found */

  /* --- Return -3 if searched vertex is NOT isolated.. --- */
  if (GRAPHis_isolated(graph, *data) == 0)
    return -3;

  prev = NULL;

  for (tmp = SLISThead(graph->vertices); tmp != NULL; tmp = SLISTnext(tmp))
    {
      /* --- Test for a hit - if so - leave the loop.. --- */
      if (graph->match(*data, ((Vertex)SLISTdata(tmp))->vertexdata))
        break;

      /* Keep a pointer to the vertex BEFORE the vertex to be removed.. */
      prev = tmp;
    }

  /* Remove the vertex */
  if (SLISTremnext(graph->vertices, prev, (void **)&vtx) != 0)
    return -1;

  /* Return vertex data to caller */
  *data = vtx->vertexdata;
  
  /* Destroy the (empty) edge collection.. */
  SLISTdestroy(vtx->adj_vertices);

  /* Destroy the vertex structure */
  free(vtx);

  /* Adjust the vertex count to account for the removed vertex */
  graph->vcount--;

  /* Everything is OK */
  return 0;
}

int GRAPHremedge(Graph graph, void *vtxdata, void **edgedata)
{
  SlistNode vtxnode;
  int retval;

  /* If edge is NOT found.. */
  if ((vtxnode = GRAPHfindvertex(graph, vtxdata)) == NULL)
    return -2;

  /* Remove the second vertex(='edgedata') - from the adjacency list of the first(='vtxdata') */
  if ((retval = SLISTfind_remove(((Vertex)SLISTdata(vtxnode))->adj_vertices, edgedata)) != 0)
    {
      if (retval == 1)
        return -2;
      else
        return -1;
    }

  /* Adjust the edge count to account for the removed edge */
  graph->ecount--;

  /* Everything is OK */
  return 0;
}

void GRAPHprint(Graph graph, void (*vtxcallback)(const void *data), void (*edgecallback)(const void *data))
{
  SlistNode element;
  int nr = 1;
  
  for (element = SLISThead(graph->vertices); element != NULL; element = SLISTnext(element))
    {
      printf("\nVertex#%02d: ", nr);
      (*vtxcallback)(((Vertex)SLISTdata(element))->vertexdata);
      printf("\nEdges #%02d: ", nr);
      SLISTtraverse(((Vertex)SLISTdata(element))->adj_vertices, edgecallback, SLIST_FWD);
      nr++;
    }
}

void GRAPHtraverse(Graph graph, void (*vtxcallback)(const void *data), void (*edgecallback)(const void *data))
{
  SlistNode element;

  for (element = SLISThead(graph->vertices); element != NULL; element = SLISTnext(element))
    {
      (*vtxcallback)(((Vertex)SLISTdata(element))->vertexdata);
      SLISTtraverse(((Vertex)SLISTdata(element))->adj_vertices, edgecallback, SLIST_FWD);
    }
}

void *GRAPHgetvertexdata(VertexNode vtxnode)
{
  return ((Vertex)SLISTdata(vtxnode))->vertexdata;
}

void *GRAPHgetedgedata(EdgeNode edgenode)
{
  return SLISTdata(edgenode);
}

VertexNode GRAPHgetvertexhead(Graph graph)
{
  return SLISThead(graph->vertices);
}

EdgeNode GRAPHgetedgehead(VertexNode vtxnode)
{
  if (vtxnode != NULL)
    return SLISThead(((Vertex)SLISTdata(vtxnode))->adj_vertices);
  return NULL;
}

VertexNode GRAPHgetvertexnext(VertexNode node)
{
  return SLISTnext(node);
}

EdgeNode GRAPHgetedgenext(EdgeNode node)
{
  return SLISTnext(node);
}

int GRAPHgetedgecount(VertexNode vtxnode)
{
  return SLISTsize(((Vertex)SLISTdata(vtxnode))->adj_vertices);
}

int GRAPHvcount(Graph graph)
{
  return graph->vcount;
}

int GRAPHecount(Graph graph)
{
  return graph->ecount;
} 

int GRAPHis_adjacent(const Graph graph, const void *vtxdata, const void *adjdata)
{
  return GRAPHfindedge(graph, vtxdata, adjdata) == NULL ? 0 : 1;
}

/* --- Function: int GRAPHis_isolated(const Graph graph, const void *vtxdata) --- */
int GRAPHis_isolated(const Graph graph, const void *data)
{
  SlistNode node;

  /* --- Loop over all vertices --- */
  for (node = SLISThead(graph->vertices); node != NULL; node = SLISTnext(node))
    {
      /* --- If 'data' is member of ANY adjacent vertex collection.. --- */
      if (SLISTfindnode(((Vertex)SLISTdata(node))->adj_vertices, data) != NULL)
        break;
    }

  if (node != NULL) /* --- Edges directed TO 'node' exist! --- */
    return 0;

  /* --- Search for vertex containing 'data' --- */
  node = GRAPHfindvertex(graph, data);

  /* --- If 'node' does NOT exist.. --- */
  if (node == NULL)
    return 0;

  /* --- If any edges directed FROM 'node' exist.. --- */
  if (SLISTsize(((Vertex)SLISTdata(node))->adj_vertices) != 0)
    return 0;
  
  /* Vertex containing 'data' is hereby ISOLATED - return TRUE! */
  return 1;
}

/* --- Function: SlistNode GRAPHfindvertex(const Graph graph, const void *data) --- */
VertexNode GRAPHfindvertex(const Graph graph, const void *data)
{
  SlistNode vtx = NULL;
  Slist list;

  list = graph->vertices;

  for (vtx = SLISThead(list); vtx != NULL; vtx = SLISTnext(vtx))
    {
      if (graph->match(data, ((Vertex)SLISTdata(vtx))->vertexdata))
        break;
    }
 
  return vtx;
}

/* --- Function: SlistNode GRAPHfindedge(const Graph graph, const void *vtxdata, const void *edgedata) --- */
EdgeNode GRAPHfindedge(const Graph graph, const void *vtxdata, const void *edgedata)
{
  SlistNode node;
  
  /* --- Find the vertex containing 'vtxdata'.. --- */
  node = GRAPHfindvertex(graph, vtxdata);

  if (node == NULL)
    return node;

  /* Return whether the adjacent vertices of 'node'
     - contains a vertex corresponding to 'edgedata'.. */
  return SLISTfindnode(((Vertex)SLISTdata(node))->adj_vertices, edgedata);
}
