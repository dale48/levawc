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

  typedef SlistNode VertexNode;
  typedef SlistNode EdgeNode;

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

  /* --- Data manipulation functions --- */
  /**
   * Insert a vertex into the graph
   * 
   * @param[in] graph - a reference to current graph.
   * @param[in] vtxdata - reference to data of vertex to be inserted.
   * @return Value 0 - if the insertion was successful\n
   *         Value 1 - if vertex containing @a vtxdata already exists in the graph\n
   *         Value -1 - otherwise, indicating some other (fatal) error
   *
   **/
  int GRAPHinsvertex(Graph graph, const void *vtxdata);

  /**
   * Insert vertex data, referenced by 'adjdata'
   * into the adjacent vertices of vertex, referenced by 'vtxdata'
   *
   * @param[in] graph - a reference to current graph.
   * @param[in] vtxdata - a reference to data of the vert
   * @param[in] adjdata - 
   * @return Value 0 - if the insertion was successful\n
   *         Value 1 - if edge containing vertices ref. by @a vtxdata and @a adjdata 
   *  already exists in the graph - i.e. you are trying to insert a duplicate edge\n
   *         Value -2 - one - or both vertices holding @a vtxdata and @a adjdata
   * is/are missing in the graph. Both must exist prior to successful insertion of 
   * an edge.\n
   *         Value -1 - otherwise, indicating some other (fatal) error
   * 
   **/
  int GRAPHinsedge(Graph graph, const void *vtxdata, const void *adjdata);

  /**
   * Remove vertex with data referenced by 'vtxdata'
   * 
   * @param[in] graph - a reference to current graph.
   * @param[in,out] vtxdata - 
   * @return Value 0 - it the removal was successful\n
   *         Value -2 - if vertex referenced by @a vtxdata is
   * missing\n
   *         Value -3 - if the vertex referenced by @a vtxdata is not isolated, i.e. \n
   *         Value -1 - otherwise, indicating some other (fatal) error
   **/
  int GRAPHremvertex(Graph graph, void **vtxdata);

  /**
   * Remove edge from vertex referenced by 'vtxdata'  - to vertex ref. by 'edgedata'
   * 
   * @param[in] graph - a reference to current graph.
   * @param[in] vtxdata - 
   * @param[in,out] edgedata - 
   * @return Value 0 - if removal of edge was successful\n
   *         Value -2 - if one or both of the vertices constituting the edge are
   * missing.\n
   *         Value -1 - otherwise, indicating some other (fatal) errror.
   *
   **/
  int GRAPHremedge(Graph graph, void *vtxdata, void **edgedata);

  /* --- Getter functions --- */
  /**
   * Get the first vertex node
   * 
   * @param[in] graph - a reference to current graph.
   * @return A reference to the first vertex node in the graph.
   * 
   **/
  VertexNode GRAPHgetvertexhead(Graph graph);

  /**
   * Get the first edge node
   *
   * @param[in] vtxnode - reference to a valid vertex node in the
   * graph. 
   * @return A reference to the first node in the collection of
   * adjacent nodes - of argument @a vtxnode.
   **/
  EdgeNode GRAPHgetedgehead(VertexNode vtxnode);

  /**
   * Get next vertex node
   * 
   * @param[in] vtxnode - reference to a valid vertex node in the
   * graph.
   * @return A reference to the next vertex node in the graph.
   **/
  VertexNode GRAPHgetvertexnext(VertexNode vtxnode);

  /**
   * Get next edge node
   * 
   * @param[in] enode - reference to a valid node in an adjacency
   * list of a vertex node in the graph.
   * @return A reference to the next node in the adjacency list.
   *
   **/
  EdgeNode GRAPHgetedgenext(EdgeNode enode);

  /**
   * Get data from a vertex
   * 
   * @param[in] vtxnode - reference to a valid vertex node in
   * the graph.
   * @return A reference to the data part of argument @a vtxnode.
   *   
   **/
  void *GRAPHgetvertexdata(VertexNode vtxnode);

  /**
   * Get data from an edge
   * 
   * @param[in] enode - reference to a valid node in an adjacency
   * list of a vertex node in the graph.
   * @return A reference to the data part of argument @a enode.
   *
   **/
  void *GRAPHgetedgedata(EdgeNode enode);

  /**
   * Get total number of vertices
   * 
   * @param[in] graph - a reference to current graph.
   * @return The total number of vertices contained in @a graph.
   **/
  int GRAPHvcount(Graph graph);

  /**
   * Get total number of edges
   * 
   * @param[in] graph - a reference to current graph.
   * @return The total number of edges contained in @a graph.
   *
   **/
  int GRAPHecount(Graph graph);

  /* --- Traversal functions --- */
  /**
   * Print all vertex and edge data of the graph - on screen
   * 
   * @param[in] graph - a reference to current graph.
   * @param[in] vtxcallback - a reference to current graph.
   * @param[in] edgecallback - a reference to current graph.
   * @return Nothing.
   *
   **/
  void GRAPHprint(Graph graph, void (*vtxcallback)(const void *data),
                  void (*edgecallback)(const void *data));

  /**
   * Traverse all vertices/edges of the graph
   * 
   * @param[in] graph - a reference to current graph.
   * @param[in] vtxcallback - a reference to current graph.
   * @param[in] edgecallback - a reference to current graph.
   * @return Nothing.
   **/
  void GRAPHtraverse(Graph graph, void (*vtxcallback)(const void *data), void (*edgecallback)(const void *data));

  /* --- Search functions --- */
  /**
   * Find vertex node containing data referenced by @a vtxdata
   *
   * @param[in] graph - a reference to current graph. 
   * @param[in] vtxdata - 
   * @return A reference to vertex node containing data referenced by @a vtxdata, if found - or NULL otherwise.
   **/
  VertexNode GRAPHfindvertex(const Graph graph, const void *vtxdata);

  /**
   * Find edge of vertex referenced by @a vtxdata to vertex ref. by @a edgedata - if any
   * 
   * @param[in] graph - a reference to current graph.
   * @param[in] vtxdata - 
   * @param[in] edgedata - 
   * @return A reference to a node in the adjacency list of vertex node ref. by @a vtxdata, if found - or NULL otherwise.
   *
   **/
  EdgeNode GRAPHfindedge(const Graph graph, const void *vtxdata, const void *edgedata);

  /* --- Miscellaneous functions --- */
  /**
   * Determine if 2 vertices are adjacent - i.e. an edge connecting them 
   *
   * @param[in] graph - a reference to current graph.
   * @param[in] vtxdata - 
   * @param[in] edgedata - 
   * @return 
   **/
  int GRAPHis_adjacent(const Graph graph, const void *vtxdata, const void *edgedata); 

  /**
   * Determine if a vertex is isolated - i.e. has no edges 
   *
   * @param[in] graph - a reference to current graph.
   * @param[in] vtxdata - 
   * @return 
   **/
  int GRAPHis_isolated(const Graph graph, const void *vtxdata);

#ifdef __cplusplus
}
#endif 

#endif /* _GRAPH_H_ */
