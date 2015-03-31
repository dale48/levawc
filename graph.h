/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: graph.h
 * Author  : Kyle Loudon/Dan Levin
 * Date    : Thu Jan 15 09:25:57 2015
 * Version : 0.51
 * ---
 * Description: A generic graph ADT - written in ANSI C. 
 * 
 * Date   Revision message
 * 150331 This code ready for version 0.51
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
   * This function simply sets up the graph so that it can be used
   * in forthcoming operations, i.e. internal counters are set to 0,
   * and the call-backs given as parameters are encapsulated within
   * the graph structure.
   * 
   * @param[in] match - a reference to a user-defined function that 
   * receives references to vertex key data - and search key data - 
   * via its parameters @a key1 and @a key2. Hence this callback 
   * function can make the actual matching of search key and vertex 
   * key data - and determine if search data is present in the graph. 
   * This callback function shall return 1 - in case of a hit - 
   * or 0 otherwise.
   *
   * @param[in] destroy - A reference to a user-defined function, 
   * reponsible for freeing vertex and edge data, when the graph is 
   * deleted. If @a destroy is NULL - then vertex and edge data will 
   * be left untouched when the graph is destroyed.
   *
   * @return A reference - to a new, empty graph - if dynamic memory
   * allocation for the ADT was successful - or NULL otherwise. Take 
   * really good care of this return value, since it will be needed as
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
   * The new vertex node inserted will contain a pointer to @a vtxdata, so 
   * the memory referenced by @a vtxdata must be valid as long as the vertex 
   * is part of the graph. It's the responsibility of the caller to manage this 
   * memory - referenced by @a vtxdata.
   *
   * @param[in] graph - a reference to current graph.
   * @param[in] vtxdata - reference to data of vertex to be inserted.
   * @return Value 0 - if the insertion was successful\n
   *         Value 1 - if vertex containing @a vtxdata already exists(=duplicate)
   *         in the graph\n
   *         Value -1 - otherwise, indicating some other (possibly fatal) error
   *
   **/
  int GRAPHinsvertex(Graph graph, const void *vtxdata);

  /**
   * Insert an edge, <i>incident from</i> vertex specified by @a vtxdata - and also
   * <i>incident to</i> vertex specified by @a adjdata. 
   *
   * Both vertices making up the edge must, of course, already exist in the graph,
   * that is, have been inserted previously by a call to GRAPHinsvertex(). The newly
   * inserted edge is represented by a node, with a reference to @a adjdata, in the 
   * adjacency(=edge) list of the vertex specified by @a vtxdata. The memory 
   * referenced by @a adjdata should remain valid as long as the edge remains in 
   * the graph. This memory is also where you normally put @a edge @a data @a 
   * specifics - such as @a cost, @a distance, etc.
   * It's the responsibility of the caller to manage the storage referenced by @a adjdata. 
   * To model an edge (@a u,@a v) in an @a undirected graph, you should call this 
   * interface function twice:
   * First time to insert an edge from @a u to @a v - and again to insert an edge
   * in the opposite direction - from @a v to @a u. Thus, in undirected graphs, you
   * have to do 2 edge insertions for each (undirected) edge.
   *
   * @param[in] graph - a reference to current graph.
   * @param[in] vtxdata - a reference to search (key) data of the vertex, which the edge 
   * is incident @a from.
   * @param[in] adjdata - a reference to data of the vertex, which the edge is incident
   * @a to. This referenced data memory must remain valid as long as the edge remains in
   * the graph - and it is the responsibility of the caller to handle it from now on.
   * @return Value 0 - if the insertion was successful\n
   *         Value 1 - if an edge @a incident @a from vertex specified by @a vtxdata - 
   * and @a incident @a to vertex specified by @a adjdata - already exists in the graph - 
   * i.e. you are trying to insert a @a duplicate edge\n
   *         Value -2 - if @a one - or @a both vertices specified by @a vtxdata 
   * and @a adjdata, respectively - is/are @a missing in the graph. 
   * Thus, @a both vertices must exist prior to @a successful insertion of an edge.\n
   *         Value -1 - otherwise, indicating some other (probably fatal) error
   * 
   **/
  int GRAPHinsedge(Graph graph, const void *vtxdata, const void *adjdata);

  /**
   * Removal of vertex - with data referenced by @a vtxdata 
   * 
   * When called, the 2nd parameter of this function, @a vtxdata, should reference a 
   * pointer, that points to search (key)data. After the call - this (external) data 
   * pointer referenced by parameter @a vtxdata, has been @a redirected to point to 
   * data of the removed node - if the call was succesful. The caller is responsible 
   * for the future of this memory - deallocating it, if needed, for example.
   *
   * For successful removal there are 2 preconditions to be met by the target vertex. 
   * The vertex to be removed must be @a isolated - which implies the following:
   * - The vertex specified with parameter @a vtxdata must @a not have any edges 
   * incident @a to it - i.e. it must not be part of @a any adjacency list in the graph,
   * whatsoever.
   * - Moreover, this vertex must not have any edges incident @a from it - i.e. its
   * own adjacency list must be empty.
   * 
   * Under these circumstances, the vertex specified with parameter @a vtxdata is 
   * isolated (edge-free). Now it can be removed - and accessed by the redirected, 
   * external pointer, that was manipulated by parameter @a vtxdata.
   * 
   * @param[in] graph - a reference to current graph.
   * @param[in,out] vtxdata - reference to (external) pointer, that prior to call 
   * should point to vertex search (key) data. After the call, this external pointer
   * is redirected to reference removed vertex data.
   * @return Value 0 - it the removal was successful\n
   *         Value -2 - if vertex referenced by @a vtxdata is missing\n
   *         Value -3 - if the vertex referenced by @a vtxdata is @a not isolated - 
   *         i.e. this vertex has edges, incident @a from - and/or - @a to it.\n
   *         Value -1 - otherwise, indicating some other (maybe fatal) error.
   * @see GRAPHinsvertex(), GRAPHis_isolated()
   **/
  int GRAPHremvertex(Graph graph, void **vtxdata);

  /**
   * Remove edge - <i>incident from</i> vertex specified by @a vtxdata - and also
   * <i>incident to</i> vertex spec. by @a edgedata 
   *
   * When called, the 3rd parameter of this function, @a edgedata, should reference an 
   * (external) pointer, that points to edge search (key) data. After the call - this
   * pointer referenced by parameter @a edgedata, has been @a redirected to point to 
   * data part of the removed (edge) node - if the call was succesful. The caller is 
   * responsible for the future of this memory - deallocating it, if needed, 
   * for example. This removed edge data will typically contain:
   * - search (key) data for the vertex, which this edge is incident @a to..
   * - ..and, moreover, @a edge @a specific @a data, such as @a cost, @a weight, etc.
   * 
   * @param[in] graph - a reference to current graph.
   * @param[in] vtxdata - a reference to vertex search (key) data of the vertex, 
   * which the target edge is incident @a from..
   * @param[in,out] edgedata - a reference to an external pointer, initially pointing 
   * to edge search (key) data. After the call, the external pointer is redirected to
   * reference removed edge data - i.e. search key data for the vertex, which the edge
   * is incident @a to.
   * @return Value 0 - if removal of edge was successful\n
   *         Value -2 - if one or both of the vertices constituting the edge - is/are
   * missing.\n
   *         Value -1 - otherwise, indicating some other (possibly fatal) error.
   * @see GRAPHinsedge()
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
   * adjacent nodes, incident from argument @a vtxnode.
   **/
  EdgeNode GRAPHgetedgehead(VertexNode vtxnode);

  /**
   * Get next vertex node 
   * 
   * @param[in] vtxnode - reference to a valid vertex node in the graph.
   *
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
   * Get data part from a vertex 
   * 
   * @param[in] vtxnode - reference to a valid vertex node in the graph.
   * @return A reference to the data part of parameter @a vtxnode.
   *   
   **/
  void *GRAPHgetvertexdata(VertexNode vtxnode);

  /**
   * Get data from an edge 
   * 
   * @param[in] enode - reference to a valid node in an adjacency
   * list of a vertex node in the graph.
   * @return A reference to the data part of parameter @a enode.
   *
   **/
  void *GRAPHgetedgedata(EdgeNode enode);

  /**
   * Get number of edges <i>incident from</i> a vertex 
   * 
   * Alternate desription: Get the number of nodes in the adjacency list
   * of the vertex node @a vtxdata.
   *
   * @param[in] vtxnode - a reference to a valid vertex node in the graph.
   * @return The total number of edges <i>incident from</i>
   * the vertex specified by @a vtxnode.
   *
   **/
  int GRAPHgetedgecount(VertexNode vtxnode);

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
   * @param[in] vtxcallback -  reference to user-defined callback function, 
   * that gets read access to vertex data via its parameter @a vtxcallback - 
   * to do whatever is relevant. In this case it is a matter of formatting vertex 
   * data for printing on screen. The printed data should be kept to a 
   * minimum (the key value, for example) in order not to clutter the screen. 
   * This function is primarily for small graphs and debugging purposes
   * @param[in] edgecallback - reference to user-defined callback function, 
   * that gets read access to edge data via its parameter @a edgecallback - 
   * to do whatever is relevant. In this case it is a matter of formatting edge 
   * data for printing on screen. The printed data should be kept to a 
   * minimum (the key value, for example) in order not to clutter the screen. 
   * This function is primarily for small graphs and debugging purposes
   * @return Nothing.
   *
   **/
  void GRAPHprint(Graph graph, void (*vtxcallback)(const void *data),
                  void (*edgecallback)(const void *data));

  /**
   * Traverse all vertices/edges of the graph 
   * 
   * 
   * @param[in] graph - a reference to current graph.
   * @param[in] vtxcallback -  reference to user-defined callback function, 
   * that gets read access to vertex data via its parameter @a vtxcallback - 
   * to do whatever is relevant. 
   * @param[in] edgecallback - reference to user-defined callback function, 
   * that gets read access to edge data via its parameter @a edgecallback - 
   * to do whatever is relevant. 
   * @return Nothing.
   **/
  void GRAPHtraverse(Graph graph, void (*vtxcallback)(const void *data), void (*edgecallback)(const void *data));

  /* --- Search functions --- */
  /**
   * Find vertex node containing data referenced by @a vtxdata 
   *
   * Search the graph for a vertex, whose data matches the search key data 
   * referenced by parameter @a vtxdata. A reference to the vertex with a match will 
   * be returned - NULL otherwise. A user-defined callback function, responsible 
   * for doing the matching of vertex data - with data referenced by parameter 
   * @a vtxdata - must exist for this function to work - otherwise NULL will be 
   * returned - always. This user-supplied match-callback was set into the graph 
   * when it was created, with a call to function - GRAPHinit().
   *
   * @param[in] graph - a reference to current graph
   * @param[in] vtxdata - a reference to search key data for the vertex to be
   * found
   * @return A reference to vertex node matching search key data referenced 
   * by parameter @a vtxdata, if found - or NULL otherwise
   **/
  VertexNode GRAPHfindvertex(const Graph graph, const void *vtxdata);

  /**
   * Find edge - <i>incident from</i> vertex specified by @a vtxdata - 
   * and also <i>incident to</i> vertex specfied by @a edgedata 
   * 
   * Search the graph for an edge, whose data matches search key data referenced 
   * by parameter @a edgedata. Moreover, this data should be found in a node,
   * present in the adjacency list of a vertex, matching data referenced by
   * parameter @a vtxdata. Thus, if an edge exists, <i>incident from</i> vertex 
   * specified by @a vtxdata - and also <i>incident to</i> vertex specified by 
   * @a edgedata - then a node @a must exist in the adjacency list of vertex 
   * specified by @a vtxdata, that is matching @a edgedata. 
   * In other words, parameter @a edgedata specifies the vertex, which the target
   * edge is incident @a to. 
   * This is how edges are implemented in our graph.
   * A reference to the node with a match will 
   * be returned - NULL otherwise. A user-defined callback function, responsible 
   * for doing the matching of node data - with data referenced by parameter data - 
   * must exist for this function to work - otherwise NULL will be returned - always. 
   * This user-supplied match-callback is set into the graph when it was created,
   *  with a call to function - GRAPHinit().
   *
   * @param[in] graph - a reference to current graph.
   * @param[in] vtxdata - reference to search key data for the vertex node, which
   * the target edge is incident from.
   * @param[in] edgedata - reference to search key data for the (edge) node -
   * situated in the adjacency list of vertex node specified by parameter @a vtxdata
   * @return A reference to a node in the adjacency list of vertex node specified 
   * by @a vtxdata, that is matching search (key) data referenced by @a edgedata, 
   * if found - or NULL otherwise.
   * @see GRAPHis_adjacent()
   *
   **/
  EdgeNode GRAPHfindedge(const Graph graph, const void *vtxdata, const void *edgedata);

  /* --- Miscellaneous functions --- */
  /**
   * Determine if 2 vertices are adjacent - i.e. if there is an edge connecting them
   *
   * More in detail, this function determines if there is an edge - <i>incident from</i>
   * vertex matching search key data referenced by parameter @a vtxdata - which is 
   * <i>incident to</i> another vertex, matching search key data referenced by parameter
   * @a edgedata. Or - equivalently put - if vertex matching search key
   * data referenced by @a edgedata is @a present in the <i>adjacency list</i> of vertex
   * specified by parameter @a vtxdata. 
   *
   * @param[in] graph - a reference to current graph.
   * @param[in] vtxdata - reference to search key data for vertex, which the edge is <i>incident from</i>.
   * @param[in] edgedata - reference to search key data for vertex, which the edge is <i>incident to</i>.
   * @return  Value 1 - if there is an edge incident from vertex specified by @a vtxdata - 
   * (incident) to a vertex specified by @a edgedata - or 0 otherwise.
   **/
  int GRAPHis_adjacent(const Graph graph, const void *vtxdata, const void *edgedata); 

  /**
   * Determine if a vertex is isolated - i.e. has no (in- or outgoing) edges 
   *
   * If a vertex is to be considered isolated - the following must be true:
   * - The vertex specified with parameter @a vtxdata must <i>not have any edges
   * incident to it</i> - i.e. it must @a not be part of any adjacency list in the 
   * graph, whatsoever.
   * - Moreover, this vertex must <i>not have any edges incident from it</i> - i.e. 
   * its own <i>adjacency list</i> must be <i>empty</i>.
   *
   * @param[in] graph - a reference to current graph.
   * @param[in] vtxdata - reference to search key data for target vertex.
   * @return Value 1 - if target vertex is isolated - 0 otherwise.
   *
   **/
  int GRAPHis_isolated(const Graph graph, const void *vtxdata);

#ifdef __cplusplus
}
#endif 

#endif /* _GRAPH_H_ */
