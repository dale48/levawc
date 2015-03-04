/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: algo.c
 * Author  : Dan Levin
 * Date    : Mon Feb 16 10:08:33 2015
 * Version : 0.5 
 * ---
 * Description: Miscellaneous  algorithms 
 * 
 */
/**
 * @file algo.c
 **/

#include <stdio.h>
#include <stdlib.h>
#include "algo.h"

#ifndef OK
#define OK 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* --- STATIC FUNCTION DECLARATIONS --- */

static void relax(DspVertexdata u, DspVertexdata v, double weight);
static int dfs_main(Graph gr, VertexNode vtx, Slist *ordered);

/* --- FUNCTION DEFINITIONS --- */

int ALGOdsp(Graph gr, const DspVertexdata start, Slist *spath, int (*match)(const void *key1, const void *key2))
{
  DspVertexdata vtxdata, edgedata, mindata;
  VertexNode vtxnode, minnode;
  EdgeNode edgenode;
  double minimum;
  int found, i;

  /* --- Initialize all of the vertices in the graph --- */
  found = FALSE;

  /* Loop over list of vertices... */
  for (vtxnode = GRAPHgetvertexhead(gr); vtxnode != NULL; vtxnode = GRAPHgetvertexnext(vtxnode)) 
    {
      vtxdata = (DspVertexdata)GRAPHgetvertexdata(vtxnode);
      
      if (match(vtxdata, start))
        {
          /* --- Initialize the start vertex --- */
          vtxdata->color = white;
          vtxdata->distance = 0;
          vtxdata->parent = NULL;
          found = TRUE;
        }
      else
        {
          /* --- Initialize vertices other than the start vertex --- */
          vtxdata->color = white;
          vtxdata->distance = DBL_MAX;
          vtxdata->parent = NULL;
        }
    }

  /* --- Return if the start vertex was not found --- */
  if (found == FALSE)
    return -1;

  /* --- Use Dijkstra's algorithm to compute shortest paths from start vertex --- */
  i = 0;

  while (i < GRAPHvcount(gr)) 
    {
      /* --- Select the white vertex with the smallest shortest-path estimate --- */
      minimum = DBL_MAX;

      /* Loop over list of vertices... */
      for (vtxnode = GRAPHgetvertexhead(gr); vtxnode != NULL; vtxnode = GRAPHgetvertexnext(vtxnode)) 
        {
          /* Extract vertex data from current node.. */
          vtxdata = (DspVertexdata)GRAPHgetvertexdata(vtxnode);
          /* If current vertex is white - and its distance < minimum.. */
          if (vtxdata->color == white && vtxdata->distance < minimum) 
            {
              minimum = vtxdata->distance; /* Set minimum to distance field of vertex */
              minnode = vtxnode; /* Save node with smallest distance value - so far.. */
            }
        }

      /* --- Color the selected(=minimum)vertex black --- */
      mindata = (DspVertexdata)GRAPHgetvertexdata(minnode);
      mindata->color = black;

      /* --- Traverse each vertex adjacent to the selected vertex node(='minnode') above --- */
      for (edgenode = GRAPHgetedgehead(minnode); edgenode != NULL; edgenode = GRAPHgetedgenext(edgenode)) 
        {
          /* Extract data from edge node */
          edgedata = GRAPHgetedgedata(edgenode);

          /* --- Find the matching vertex for current edge node in the list of vertices --- */
          if ((vtxnode = GRAPHfindvertex(gr, edgedata)) != NULL)
            {
              /* Extract data from vertex found */
              vtxdata = (DspVertexdata)GRAPHgetvertexdata(vtxnode);
              /* Relax data for 2 vertices - 'minnode' and 'vtxnode' */
              relax(mindata, vtxdata, edgedata->weight);
            }
        }
      /* --- Prepare to select the next vertex --- */
      i++;
    }
  
  /* --- Load the vertices with their path information into a list ---  */
  *spath = SLISTinit(NULL);

  /* Loop over list of vertices... */
  for (vtxnode = GRAPHgetvertexhead(gr); vtxnode != NULL; vtxnode = GRAPHgetvertexnext(vtxnode)) 
    {
      /* --- Load each black vertex from graph into the list --- */
      /* Extract data of current vertex node */
      vtxdata = (DspVertexdata)GRAPHgetvertexdata(vtxnode);

      if (vtxdata->color == black) /* If current vertex is black */
        {
          if (SLISTinsnext(*spath, SLISTtail(*spath), vtxdata) != OK) 
            {
              SLISTdestroy(*spath);
              *spath = NULL; 
              return -1; /* Return error.. */
            }
        }
    }

  /* Everything is OK */
  return OK;
}

static void relax(DspVertexdata u, DspVertexdata v, double weight)
{ 
  /* --- Relax an edge between two vertices u and v --- */
  if (v->distance > u->distance + weight) 
    {
      v->distance = u->distance + weight; /* Update the 'distance' field of 'v' */
      v->parent = u; /* Record the parent of 'v' */
    } 
}

int ALGOmst(Graph gr, const MstVertexdata start, Slist *span, int (*match)(const void *key1, const void *key2))
{
  MstVertexdata vtxdata, edgedata, mindata;
  VertexNode vtxnode, minnode;
  EdgeNode edgenode;
  double minimum;
  int found, i;

  /* --- Initialize all of the vertices in the graph --- */
  found = FALSE;

  /* Loop over list of vertices... */
  for (vtxnode = GRAPHgetvertexhead(gr); vtxnode != NULL; vtxnode = GRAPHgetvertexnext(vtxnode)) 
    {
      /* Extract data of current vertex.. */
      vtxdata = (MstVertexdata)GRAPHgetvertexdata(vtxnode);
      
      if (match(vtxdata, start)) /* If current vertex equals start node.. */
        {
          /* --- Initialize the start vertex --- */
          vtxdata->color = white;
          vtxdata->key = 0;
          vtxdata->parent = NULL;
          found = TRUE;
        }
      else
        {
          /* --- Initialize vertices other than the start vertex --- */
          vtxdata->color = white;
          vtxdata->key = DBL_MAX;
          vtxdata->parent = NULL;
        }
    }

  /* --- Return if the start vertex was not found --- */
  if (found == FALSE)
    return -1;

  /* --- Use Prim's algorithm to compute Minimal Spanning Tree --- */
  i = 0;

  while (i < GRAPHvcount(gr)) 
    {
      /* --- Select the white vertex with the smallest key value --- */
      minimum = DBL_MAX;

      /* Loop over list of vertices... */
      for (vtxnode = GRAPHgetvertexhead(gr); vtxnode != NULL; vtxnode = GRAPHgetvertexnext(vtxnode)) 
        {
          /* Extract data of current vertex.. */
          vtxdata = (MstVertexdata)GRAPHgetvertexdata(vtxnode);
          
          /* If color of current vertex is white - and - 'key' < 'minimum'.. */
          if (vtxdata->color == white && vtxdata->key < minimum) 
            {
              minimum = vtxdata->key; /* Set 'minimum' to 'key' value of current vertex */
              minnode = vtxnode; /* Save node with smallest key value - so far.. */
            }
        }

      /* --- Color the selected(=minimum)vertex black --- */
      mindata = (MstVertexdata)GRAPHgetvertexdata(minnode);
      mindata->color = black;

      /* --- Traverse each vertex adjacent to the selected vertex node(='minnode') above --- */
      for (edgenode = GRAPHgetedgehead(minnode); edgenode != NULL; edgenode = GRAPHgetedgenext(edgenode)) 
        {
          /* Extract data of current (adjacent) edge node */
          edgedata = GRAPHgetedgedata(edgenode);

          /* --- Find the matching vertex for current edge node in the list of vertices --- */
          if ((vtxnode = GRAPHfindvertex(gr, edgedata)) != NULL)
            {
              /* Extract data of vertex found.. */
              vtxdata = (MstVertexdata)GRAPHgetvertexdata(vtxnode);
              /* If color of vertex found is white - and - 
                 'key' value is > 'weight' of current edge node */
              if (vtxdata->color == white && edgedata->weight < vtxdata->key)
                {
                  vtxdata->key = edgedata->weight; /* Set 'key' field of vertex found to weight of current edge */
                  vtxdata->parent = mindata; /* Record parent in vertex found - to data of 'minnode' selected above */
                }
            }
        }
      /* --- Prepare to select the next vertex --- */
      i++;
    }
  
  /* --- Load the vertices with their path information into a list ---  */
  *span = SLISTinit(NULL);

  /* Loop over list of vertices... */
  for (vtxnode = GRAPHgetvertexhead(gr); vtxnode != NULL; vtxnode = GRAPHgetvertexnext(vtxnode)) 
    {
      /* --- Load each black vertex from graph into the list --- */
      vtxdata = (MstVertexdata)GRAPHgetvertexdata(vtxnode);

      if (vtxdata->color == black) 
        {
          if (SLISTinsnext(*span, SLISTtail(*span), vtxdata) != OK) 
            {
              SLISTdestroy(*span);
              *span = NULL; 
              return -1; /* Return error.. */
            }
        }
    }

  return OK;
}

int ALGOtsp(Slist *vertices, const TspVertexdata start, Slist *tour, int (*match)(const void *key1, const void *key2))
{
  TspVertexdata tsp_vtx, tsp_start, selection;
  SlistNode node;
  double min, distance, x, y;
  int found, i;

  /* Initialize the list for the tour */
  *tour = SLISTinit(NULL);

  /* --- Initialize all of the vertices in the (complete) graph --- */
  found = FALSE;

  /* Loop over the list of vertices.. */
  for (node = SLISThead(*vertices); node != NULL; node = SLISTnext(node))
    {
      /* Extract data of current node.. */
      tsp_vtx = SLISTdata(node);

      if (match(tsp_vtx, start))
        {

          /* Start the tour at 'start' vertex.. */
          if (SLISTinsnext(*tour, SLISTtail(*tour), tsp_vtx) != OK )
            {
              SLISTdestroy(*tour);
              return -1;
            }
          
          /* Save the start vertex - and its coordinates.. */
          tsp_start = tsp_vtx;
          x = tsp_vtx->x;
          y = tsp_vtx->y;

          /* Color the vertex black */
          tsp_vtx->color = black;
          found = TRUE;
        }
      else
        {
          /* Color all other vertices white */
          tsp_vtx->color = white;
        }
    } /* Initialization done! */

  /* Return if start vertex not found.. */
  if (found == FALSE)
    {
      SLISTdestroy(*tour);
      return -1;
    }


  /* --- Use the nearest-neighbor heuristic to compute the tour --- */
  i = 0;

  while (i < SLISTsize(*vertices)-1)
    {
      /* --- Select the white vertex closest to the previous tour vertex --- */
      min = DBL_MAX;
      /* Loop over list of vertices.. */
      for (node = SLISThead(*vertices); node != NULL; node = SLISTnext(node))
        {
          /* Extract data from current node.. */
          tsp_vtx = SLISTdata(node);

          /* If current vertex is white.. */
          if (tsp_vtx->color == white)
            {
              /* Compute the distance between current vertex and previous one on tour.. */
              distance = sqrt(pow((tsp_vtx->x - x), 2.0) + pow((tsp_vtx->y - y), 2.0));

              /* If closer - save it.. */
              if (distance < min)
                {
                  min = distance;
                  selection = tsp_vtx;
                }
            }
        } /* End-while. Shortest distance to previous vertex computed.. */
      
      /* Save the coordinates of selected(=closest) vertex */
      x = selection->x;
      y = selection->y;
      /* Color selected vertex black.. */
      selection->color = black;

      /* Insert selected vertex into the tour.. */
      if (SLISTinsnext(*tour, SLISTtail(*tour), selection) != OK)
        {
          SLISTdestroy(*tour);
          return -1;
        }

      /* Increment loop variable 'i' - to prepare for next tour vertex */
      i++;
    }

  /* Insert the start vertex again - to complete the tour.. */
  if (SLISTinsnext(*tour, SLISTtail(*tour), tsp_start) != OK)
    {
      SLISTdestroy(*tour);
      return -1;
    }

  return OK;
}

int ALGObfs(Graph gr, const BfsVertexdata start, Slist *hops, int (*match)(const void *key1, const void *key2))
{
  Queue queue;
  BfsVertexdata vtxdata, adjvtxdata, edgedata;
  VertexNode vtxnode, vtxnode2;
  EdgeNode edgenode;

  /* --- Initialize all of the vertices in the graph --- */
  /* Loop over list of vertices... */
  for (vtxnode = GRAPHgetvertexhead(gr); vtxnode != NULL; vtxnode = GRAPHgetvertexnext(vtxnode))
    {
      /* Extract data of current vertex node.. */
      vtxdata = (BfsVertexdata)GRAPHgetvertexdata(vtxnode);
      
      if (match(vtxdata, start)) /* If current vertex node matches start node */
        {
          /* --- Initialize the start vertex --- */
          vtxdata->color = gray;
          vtxdata->hops = 0;
        }
      else
        {
          /* --- Initialize vertices other than the start vertex --- */
          vtxdata->color = white;
          vtxdata->hops = -1;
        }
    }

  /* Initialize the queue with the start vertex */
  queue = QUEUEinit(NULL); /* Create the queue.. */

  /* Find the vertex node matching start vertex data */
  if ((vtxnode = GRAPHfindvertex(gr, start)) == NULL)
    {
      QUEUEdestroy(queue);
      return -1;
    }
  
  /* Enqueue this vertex node found.. */
  if (QUEUEenqueue(queue, vtxnode) != OK )
    {
      QUEUEdestroy(queue);
      return -1;
    }
  
  /* Now, we start the Breadth-First Search */
  while (QUEUEsize(queue) > 0)
    {
      /* Peek at the front of the queue.. */
      vtxnode = (VertexNode)QUEUEpeek(queue);
      /* ..and extract corresponding data */
      vtxdata = GRAPHgetvertexdata(vtxnode);

      /* Traverse each node in current edge(=adjacency)list.. */
      for (edgenode = GRAPHgetedgehead(vtxnode); edgenode != NULL; edgenode = GRAPHgetedgenext(edgenode))
        {
          /* Extract data from current edge node.. */
          edgedata = GRAPHgetedgedata(edgenode);
          
          /* Find the matching vertex of edge node - in list of vertices.. */
          if ((vtxnode2 = GRAPHfindvertex(gr, edgedata)) == NULL)
            {
              QUEUEdestroy(queue);
              return -1;
            }
          
          /* Determine the color of current (adjacent) vertex found.. */
          adjvtxdata = GRAPHgetvertexdata(vtxnode2);
          
          if (adjvtxdata->color == white) /* If color is white.. */
            {
              adjvtxdata->color = gray; /* Update color to gray.. */
              adjvtxdata->hops = vtxdata->hops+1; /* Increment 'hops' by 1 - rel. 'hops' in front node in 'queue' */

              /* Enqueue this newly updated node - into the (end of) queue  */
              if (QUEUEenqueue(queue, vtxnode2) != OK)
                {
                  QUEUEdestroy(queue);
                  return -1;
                }
            }
        } /* End of for-loop. Traversal of current adjacency list done.. */
    
      /* Dequeue the front node from queue - and color its vertex black.. */
      if (QUEUEdequeue(queue, (void **)&vtxnode) == OK)
        {
          vtxdata->color = black;         
        }
      else
        {
          QUEUEdestroy(queue);
          return -1;
        }    
    } /* End-while */

  /* Our queue is no longer needed - i.e. destroy it! */
  QUEUEdestroy(queue);

  /* Pass back the hop count for each vertex in a list */
  *hops = SLISTinit(NULL);

  for (vtxnode = GRAPHgetvertexhead(gr); vtxnode != NULL; vtxnode = GRAPHgetvertexnext(vtxnode))
    {
      vtxdata = GRAPHgetvertexdata(vtxnode);

      /* Skip the vertices with hop counts of -1 - i.e. vertices not visited.. */
      if (vtxdata->hops != -1)
        {
          if (SLISTinsnext(*hops, SLISTtail(*hops), vtxdata) != OK)
            {
              SLISTdestroy(*hops);
              return -1;
            }
        }
    }

  return OK;
}

int ALGOdfs(Graph gr, Slist *ordered)
{
  DfsVertexdata vtxdata;
  VertexNode vtxnode;

  /* Initialize all the vertices in the graph */
  for (vtxnode = GRAPHgetvertexhead(gr); vtxnode != NULL; vtxnode = GRAPHgetvertexnext(vtxnode))
    {
      /* Extract data of current node */
      vtxdata = GRAPHgetvertexdata(vtxnode);
      /* Set color to white */
      vtxdata->color = white;
    }
    
  /* Initialize a list.. */
  *ordered = SLISTinit(NULL);

  /* --- Perform the Depth-First Search --- */ 
  for (vtxnode = GRAPHgetvertexhead(gr); vtxnode != NULL; vtxnode = GRAPHgetvertexnext(vtxnode))
    {
      /* Extract data of current node.. */
      vtxdata = GRAPHgetvertexdata(vtxnode);

      /* Ensure that all components of an unconnected graph is searched.. */
      if (vtxdata->color == white)
        {
          if (dfs_main(gr, vtxnode, ordered) != OK)
            {
              SLISTdestroy(*ordered);
              return -1;
            }
        }
    }

  return OK;
}

static int dfs_main(Graph gr, VertexNode vtxnode, Slist *ordered)
{
  VertexNode vnode;
  EdgeNode enode;
  DfsVertexdata vtxdata, vtxdata2, edgedata;

  /* Extract data of vertex node received in arg. 2 - and color the vertex gray.. */
  vtxdata = GRAPHgetvertexdata(vtxnode);
  vtxdata->color = gray;
  /* ..and traverse its adjacency list */
  for (enode = GRAPHgetedgehead(vtxnode); enode != NULL; enode = GRAPHgetvertexnext(enode))
    {
      /* Extract data of current edge node (- in current adjacency list).. */
      edgedata = GRAPHgetedgedata(enode);
      /* Find the matching vertex node in list of vertices.. */
      if ((vnode = GRAPHfindvertex(gr, edgedata)) == NULL)
        return -1;
      /* Extract data of this vertex node found.. */
      vtxdata2 = GRAPHgetvertexdata(vnode);

      /* If this node is white.. */
      if (vtxdata2->color == white)
        {
          /* Make a recursive call.. */
          if (dfs_main(gr, vnode, ordered) != OK)
            return -1;
        }
    }

  /* Color the argument vertex black.. */
  vtxdata->color = black;
  /* ..and insert it as the first into the list */
  if (SLISTinsnext(*ordered, NULL, vtxdata) != OK)
    return -1;

  return OK;
}
