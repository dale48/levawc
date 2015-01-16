/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: graph.h
 * Author  : Kyle Loudon/Dan Levin
 * Date    : Thu Jan 15 09:25:57 2015
 * Version : 0.5
 * ---
 * Description: A generic graph ADT - written in ANSI C. 
 * 
 */
/**
 * @file graph.h
 **/

#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>

#include "slist.h"
#include "set.h"
#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * Define colors for vertices in graphs
   **/
  typedef enum VertexColor_ {white, gray, black} VertexColor;

  /**
   * Use a @b typedef - to hide the interior of @b Vertex_ - in 
   * the implementation file. This is how @a data @a 
   * hiding can be done in C.
   * 
   **/
  typedef struct Vertex_ *Vertex;
  /**
   * Use a @b typedef - to hide the interior of @b Graph_ - in 
   * the implementation file. This is how @a data @a 
   * hiding can be done in C.
   * 
   **/
  typedef struct Graph_ *Graph;

  typedef Slist Vertexcollection;
  typedef Slist Edgecollection;

  /* FUNCTION DECLARATIONS/PUBLIC INTERFACE */
  /* --- Maintenance functions --- */
  /**
   * Initialize the graph
   * 
   * @param[in] match - a reference to a user-defined function that 
   * receives references to vertex data - and search key data - via its 
   * parameters @a key1 and @a key2. Hence this callback function
   * can make the actual matching of search key and vertex data and 
   * determine if search data is present in the graph. This callback 
   * function shall return 1 - in case of a hit - or 0 otherwise.
   * @param[in] destroy - A reference to a user-made function, reponsible
   * for freeing vertex data, when the set is deleted. If @a destroy is 
   * NULL - then vertex data will be left untouched when the graph is 
   * destroyed.
   * @return A reference - to a new, empty graph - if dynamic memory
   * allocation for the ADT was successful - or NULL otherwise. Take 
   * really good care  of this return value, since it will be needed as
   * a parameter in subsequent calls - to the majority of other graph 
   * handling functions in this function interface - i.e. a sort of
   * "handle" to the graph.
   * @see GRAPHdestroy()
   **/
  Graph GRAPHinit(int (*match)(const void *key1, const void *key2),
                  void (*destroy)(void *data));

  /**
   * Destroy the graph
   * 
   * The graph is destroyed - that is, all the memory occupied by the vertices 
   * and edges is deallocated. The user-defined callback function @a destroy, 
   * given as an argument to @b GRAPHinit(), is responsible for freeing 
   * dynamically allocated vertex/edge data, when this function is called. When
   * all vertices/edges and corresponding data have been deallocated - the graph 
   * header is deallocated, too.
   * 
   * @param[in] graph - a reference to current graph.
   * @return Nothing.
   * @see GRAPHinit()
   **/
  void GRAPHdestroy(Graph graph);

  /**
   * Insert a vertex into the graph
   * 
   **/
  int GRAPHinsvertex(Graph graph, const void *data);

  /**
   * Insert vertex data, referenced by 'adjdata'
   * among the adjacent vertices of vertex referenced by 'vtxdata'
   * 
   **/
  int GRAPHinsedge(Graph graph, const void *vtxdata, const void *adjdata);

  /**
   * Remove vertex with data referenced by 'vtxdata'
   * 
   **/
  int GRAPHremvertex(Graph graph, void **vtxdata);

  /**
   * Remove edge from vertex referenced by 'vtxdata'  - to vertex ref. by 'edgedata'
   * 
   **/
  int GRAPHremedge(Graph graph, void *vtxdata, void **edgedata);

  /**
   * Determine whether 2 vertices are adjacent
   * 
   **/
  int GRAPHis_adjacent(const Graph graph, const void *vtxdata, const void *adjdata);

  /**
   * Get a collection of vertices constituting the graph
   * 
   **/
  Vertexcollection GRAPHgetvertices(Graph graph);

  /**
   * Get a collection of edges connected to a vertex referenced by
   * 'vtxdata'
   * 
   **/
  Edgecollection GRAPHgetedges(Graph graph, const void *vtxdata);

  /**
   * Get the vertex count of the graph
   * 
   **/
  int GRAPHvcount(Graph graph);

  /**
   * Get the edge count of the graph
   * 
   **/
  int GRAPHecount(Graph graph);

  /**
   * Print all vertex and edge data of the graph - on screen
   * 
   **/
  void GRAPHprint(Graph graph, void (*vtxcallback)(const void *data),
                  void (*edgecallback)(const void *data));

  /**
   * Traverse all vertices/edges of the graph
   * 
   **/
  void GRAPHtraverse(Graph graph, void (*vtxcallback)(const void *data), void (*edgecallback)(const void *data));

  /* --- Search functions --- */
  /**
   * Find vertex with its data referenced by 'vtxdata'
   * 
   **/
  SlistNode GRAPHfindvertex(const Graph graph, const void *vtxdata);

  /**
   * Find edge from vertex ref. by 'vtxdata' to vertex ref. by 'edgedata' - if any
   * 
   **/
  SlistNode GRAPHfindedge(const Graph graph, const void *vtxdata, const void *edgedata);

  /* --- Setter/Getter functions --- */


  /* --- Miscellaneous functions --- */

  /**
   * Determine if 2 vertices are adjacent - i.e. an edge connecting them 
   *
   **/
  int GRAPHis_adjacent(const Graph graph, const void *vtxdata, const void *edgedata); 

  /**
   * Determine if a vertex is isolated - i.e. has no edges 
   *
   **/
  int GRAPHis_isolated(const Graph graph, const void *vtxdata);

#ifdef __cplusplus
}
#endif 

#endif /* _GRAPH_H_ */
