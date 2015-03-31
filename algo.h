/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: algo.h
 * Author  : Dan Levin
 * Date    : Mon Feb 16 10:06:22 2015
 * Version : 0.51
 * ---
 * Description: Miscellanoeus algorithms 
 * 
 * Date   Revision message
 * 150331 This code ready for version 0.51
 *
 */
/**
 * @file algo.h
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>
#include <float.h>
#include <math.h>

#include "slist.h"
#include "queue.h"
#include "graph.h"

#ifndef _ALGO_H_
#define _ALGO_H_

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * A data structure for finding the shortest path between vertices in a graph
   * 
   **/
  typedef struct DspVertexdata_ 
  {
    /**
     * Pointer to optional vertex data..
     * 
     **/
    void *data;
  
    double weight;
    VertexColor color;
    double distance;

    /* Pointer to parent vertex */
    struct DspVertexdata_ *parent;
  } *DspVertexdata;

  /**
   * A data structure for finding the minimum spanning tree in a graph
   * 
   **/
  typedef struct MstVertexdata_ 
  {
    /**
     * Pointer to optional vertex data..
     * 
     **/
    void *data;

    double weight;
    VertexColor color;
    double key;

    /* Pointer to parent vertex */
    struct MstVertexdata_  *parent;
  } *MstVertexdata;

  /**
   * A data structure for finding the travelling salesmans route in a graph
   * 
   **/
  typedef struct TspVertexdata_ 
  {
    /**
     * Pointer to optional vertex data..
     * 
     **/
    void *data;

    double x, y;
    VertexColor color;
  } *TspVertexdata;

  /**
   * A data structure for vertices in a breadth-first search
   * 
   **/
  typedef struct BfsVertexdata_ 
  {
    /**
     * Pointer to optional vertex data..
     * 
     **/
    void *data;

    int hops;
    VertexColor color;
  } *BfsVertexdata;

  /**
   * A data structure for vertices in a depth-first search
   * 
   **/
  typedef struct DfsVertexdata_ 
  {
    /**
     * Pointer to optional vertex data..
     * 
     **/
    void *data;

    VertexColor color;
  } *DfsVertexdata;


  /* --- Function interface/declarations --- */

  /**
   * Find shortest path with Dijkstra's algorithm
   * 
   **/
  int ALGOdsp(Graph gr, const DspVertexdata start, Slist *spath,
              int (*match)(const void *key1, const void *key2));

  /**
   * Find minimal spanning tree with Prims's algorithm
   * 
   **/
  int ALGOmst(Graph gr, const MstVertexdata start, Slist *span,
              int (*match)(const void *key1, const void *key2));

  /**
   * Find route for Travelling Salesman
   * 
   **/
  int ALGOtsp(Slist vertices, const TspVertexdata start, Slist *tour,
              int (*match)(const void *key1, const void *key2));

  /**
   * Do a breadth-first search in a graph
   * 
   **/ 
  int ALGObfs(Graph gr, const BfsVertexdata start, Slist *hops, 
              int (*match)(const void *key1, const void *key2));

  /**
   * Do a depth-first search in a graph
   * 
   **/ 
  int ALGOdfs(Graph gr, Slist *ordered);

#ifdef __cplusplus
}
#endif 

#endif /* _ALGO_H_ */
