/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: demo14.c
 * Author  : Dan Levin
 * Date    : Fri Feb 20 13:26:47 2015
 * Version : 0.5 
 * ---
 * Description: A demo program for testing/showing Dijkstra's Shortest Path
 * Algorithm - as "The EU City Criss Cross Tour".
 *
 * Revision history: (this is where you document the diffs between versions...)
 * Date   Revision
 * 150206 This source is created for version 0.5..
 *
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graph.h"
#include "algo.h"
#include "utils.h"

#ifndef OK
#define OK 0
#endif

#define KEYSIZ BUFSIZ/4
#define ERRSIZ BUFSIZ/4
#define COMMENTCHAR '#'
#define MYFILLCHAR '-'

/* Global variables */

int row_ctr;

typedef struct MyCitydata_
{
  int id;
  char name[KEYSIZ];
} *MyCitydata;

/*  String macro for the main menu... */
#define MAIN_MENU_ROW "--- EU CITY CRISS-CROSS TOUR DEMO ---\nMENU: 0=Exit 1=Make_Tour 2=Min_Span_Tree 3=Print_Graph\nSelection"

/* FUNCTION-DECLARATIONS */
/* Application-specifik callbacks */
void my_destroy(void *data);                  /* Callback for deallocating vertex memory */void print(const void *data);
int my_match(const void *, const void *);     /* Callback for matching vertices */
int my_cmp(const void *key1,                  /* Callback for comparing vetices */
           const void *key2);
void vtx_prt(const void *vtxdata);            /* Callback for printing vertex data */
void edge_prt(const void *edgedata);          /* Callback for printing edge data */

/* Menu selection (handling) functions */
void make_tour(Graph gr);                     /* Make a tour - and calculate shortest path.. */
void min_span_tree(Graph gr);                 /* Calculate the Minimum Spanning Tree  */
void final_status(Graph gr);                  /* Final status.. */

/* Misc. application functions.. */
int read_graphdata(Graph gr, char *filename); /* Read graph(=vertex/edge) data from textfile */
int read_vertices(Graph gr, FILE *fp);        /* Read vertex data */
int read_edges(Graph gr, FILE *fp);           /* Read edge data */
void printroute(DspVertexdata spvtx);         /* Print route info */
void error_mess(const char *str);             /* Output error messages on stderr */
void print_vertices(Graph gr);                /* Print all vertices(=destinations) on screen */
void prt_fill(char *str, char fillchar,       /* Right fill a string field up to length 'fieldlen' with 'fillchar'  */
              int fieldlen);

/* END-OF-FUNCTION-DECLARATIONS */

/* FUNCTION DEFINITIONS - the rest of the program */
/* --- Function: void my_destroy(void *data) --- */
void my_destroy(void *data)
{
  DspVertexdata vdata;
  MyCitydata mcd;

  vdata = (DspVertexdata)data;
  mcd = (MyCitydata)(vdata->data);
  free(mcd);
  free(vdata);
}

/* --- Function: void printvtx(const void *data) --- */
void printvtx(const void *data)
{
  DspVertexdata vdata;
  MyCitydata mcd;

  vdata = (DspVertexdata)data;
  mcd = (MyCitydata)(vdata->data);

  printf("%02d", mcd->id);
}

/* --- Function: void printvtx(const void *data) --- */
void printedge(const void *data)
{
  DspVertexdata vdata;
  MyCitydata mcd;

  vdata = (DspVertexdata)data;
  mcd = (MyCitydata)(vdata->data);

  printf("%02d|%-3.3s|", mcd->id, mcd->name);
}

/* --- Function: int my_match(const void *k1, const void *k2) --- */
int my_match(const void *k1, const void *k2)
{
  DspVertexdata vdata;
  MyCitydata mcd1, mcd2;

  vdata = (DspVertexdata)k1;
  mcd1 = (MyCitydata)(vdata->data);
  vdata = (DspVertexdata)k2;
  mcd2 = (MyCitydata)(vdata->data);

  return mcd1->id == mcd2->id;
}

/* --- Function: int my_cmp(const int *key1, const int *key2) --- */
int my_cmp(const void *key1, const void *key2)
{
  DspVertexdata vdata;
  MyCitydata mcd1, mcd2;

  vdata = (DspVertexdata)key1;
  mcd1 = (MyCitydata)(vdata->data);
  vdata = (DspVertexdata)key2;
  mcd2 = (MyCitydata)(vdata->data);

  return mcd1->id - mcd2->id;
}

int read_graphdata(Graph gr, char *filename)
{
  FILE *fp;
  char errmess[ERRSIZ];

  FILCHK((fp = fopen(filename, "rt")));

  if (read_vertices(gr, fp) != OK)
    {
      sprintf(errmess, "\nError when reading data(=vertices) from file %s!", filename);
      error_mess(errmess); /* Print error message */
      fclose(fp);          /* Close data file */
      exit(EXIT_FAILURE);
    }

  rewind(fp);

  if (read_edges(gr, fp) != OK)
    {
      sprintf(errmess, "\nError when reading data(=edges) from file %s!", filename);
      error_mess(errmess); /* Print error message */
      fclose(fp);          /* Close data file */
      exit(EXIT_FAILURE);
    }

  fclose(fp);

  /* Everything is OK */
  return 0;
}

int read_vertices(Graph gr, FILE *fp)
{
  char buf[BUFSIZ], bufcopy[BUFSIZ], tmp[KEYSIZ];
  char *commentptr, *pc;
  char errmess[ERRSIZ];
  DspVertexdata vtxdata;
  MyCitydata my_city;
  
  int retval;

  /* Read rows from datafile into row buffer 'buf'.. */
  while (fgets(buf, BUFSIZ, fp) != NULL)
    {
      /* Remove comments from buffer */
      if ((commentptr = strchr(buf, COMMENTCHAR)) != NULL)
        *commentptr = '\0';
      
      /* Get rid of starting or trailing blank characters in buffer */
      strtrim(buf, isspace, isspace);

      /* If buffer is NOT empty... */
      if (*buf)
        {
          /* Save a copy of input buffer */
          strcpy(bufcopy, buf);

          /* Read the first field.. */
          pc = strtok(buf, ",;");

          /* Allocate memory for a vertex - with error handling.. */
          /* Data type 'struct DspVertexdata_' - defined in file 'algo.h'.. */
          MALCHK((vtxdata = malloc(sizeof(struct DspVertexdata_))));
          /* Data type 'struct MyCitydata_' - defined above.. */
          MALCHK((my_city = malloc(sizeof(struct MyCitydata_))));
          /* Link the structures together.. */
          vtxdata->data = my_city;

          /* Copy field data into temp. field buffer... */
          strcpy(tmp, pc);
          strtrim(tmp, isspace, isspace);

          /* If wrong format - OR - temp. buffer empty..  */
          if (!isunsignedfloat(tmp) || *tmp=='\0') /* Bailing out... */
            {
              sprintf(errmess, "\nData error - when reading key data(=%s) - on this line:\n%s",
                      tmp, bufcopy);
              error_mess(errmess); /* Print error message */
              /* Free city memory */
              free(my_city);
              /* Free vertex memory */
              free(vtxdata);
              exit(EXIT_FAILURE);
            }
          
          /* Copy and convert temp. field buffer into vertex structure... */
          my_city->id = atoi(tmp);

          /* Read next field... */
          pc = strtok(NULL, ",;");

          /* If field data exists.. */
          if (*pc)
            {
              /* Copy & trim field into vertex structure.. */
              strcpy(my_city->name, pc);
              strtrim(my_city->name, isspace, isspace);
            }
          else /* Bailing out... */
            {
              sprintf(errmess, "\nData missing - when reading data(=%s) - on this line:\n%s",
                      pc, bufcopy);
              error_mess(errmess); /* Print error message */
              /* Free dyn. allocated  memory */
              my_destroy(vtxdata);
              exit(EXIT_FAILURE);
            }

          /* Insert vertex into graph - with error control */
          if ((retval = GRAPHinsvertex(gr, vtxdata)) != OK)
            {
              /* Prepare and output error message */
              sprintf(errmess, "\nError when inserting vertex data into graph - from this line:\n%s", bufcopy);
              error_mess(errmess);
              /* Free dyn. allocated  memory */
              my_destroy(vtxdata);
              /* Return error code.. */
              return retval; 
            }
        }
    }
  /* Everything is OK */
  return 0;
}

int read_edges(Graph gr, FILE *fp)
{
  int state, fldctr;
  char buf[BUFSIZ], bufcopy[BUFSIZ], tmp[KEYSIZ];
  char *commentptr, *pc;
  char errmess[ERRSIZ];
  struct DspVertexdata_ tmpvtxdata, tmpedgedata;
  struct MyCitydata_ cty_data, cty_data2;
  VertexNode vtxnode;
  /* EdgeNode enode; */
  DspVertexdata edata, tmpdata;
  MyCitydata mycitydata;
  int retval;

  /* Link 2 tmp structures together... */
  tmpvtxdata.data = &cty_data;
  tmpedgedata.data = &cty_data2;

  /* Read rows from datafile into row buffer 'buf'.. */
  while (fgets(buf, BUFSIZ, fp) != NULL)
    {
      /* Remove comments from buffer */
      if ((commentptr = strchr(buf, COMMENTCHAR)) != NULL)
        *commentptr = '\0';
      
      /* Get rid of starting or trailing blank characters in buffer */
      strtrim(buf, isspace, isspace);

      /* If buffer is NOT empty... */
      if (*buf)
        {
          /* Save a copy of input row buffer */
          strcpy(bufcopy, buf);

          /* Read the first field.. */
          pc = strtok(buf, ",;");

          /* Copy field data into temp. field buffer... */
          strcpy(tmp, pc);
          strtrim(tmp, isspace, isspace);

          /* If wrong format - OR - temp. buffer empty..  */
          if (!isunsigned(tmp) || *tmp=='\0') /* Bailing out... */
            {
              sprintf(errmess, "\nData error - when reading key data(=%s) - on this line:\n%s",
                      tmp, bufcopy);
              error_mess(errmess); /* Print error message */
              exit(EXIT_FAILURE);
            }
          
          /* Copy and convert temp. field buffer into temp. vertex structure... */
          cty_data.id = atoi(tmp);

          /* Read next field - and dump it... */
          pc = strtok(NULL, ",;");

          /* If field data is missing.. */
          if (pc == NULL)
            {
              sprintf(errmess, "\nData missing - when reading data - on this line:\n%s", bufcopy);
              error_mess(errmess); /* Print error message */
              exit(EXIT_FAILURE);
            }

          /* Now - start reading edge data.. */
          state = 1;   /* Set state to 1 initially */
          fldctr = 1;  /* Set field counter to 1 inintially */

          while ((pc = strtok(NULL, ",;")) != NULL)
            {
              switch (state)
                {
                case 1: /* Read edge key */
                  /* Allocate memory for edge - with error handling.. */
                  MALCHK((edata = malloc(sizeof(struct DspVertexdata_))));
                  MALCHK((mycitydata = malloc(sizeof(struct DspVertexdata_))));

                  /* Link the 2 dyn. allocated structures together */
                  edata->data = mycitydata;

                  /* Copy field data into temp. field buffer.. */
                  strcpy(tmp, pc);
                  strtrim(tmp, isspace, isspace); /* Trimming.. */

                  /* If wrong format - OR - temp. buffer empty..  */
                  if (!isunsigned(tmp) || *tmp=='\0') /* Bailing out... */
                    {
                      sprintf(errmess, "\nData error - when reading key data(=%s) - on this line:\n%s",
                              tmp, bufcopy);
                      error_mess(errmess); /* Print error message */
                      /* Free dyn. allocated  memory */
                      my_destroy(edata);
                      exit(EXIT_FAILURE);
                    }
          
                  /* Copy and convert temp. field buffer into edge structure... */
                  mycitydata->id = atoi(tmp);

                  /* Get additional data from vertex, which is one part of the current edge.. */
                  /* edgetmp.key = pedge->key; */
                  cty_data2.id = mycitydata->id;

                  /* Look it up in the graph.. */
                  vtxnode = GRAPHfindvertex(gr, &tmpedgedata);
                  assert(vtxnode!=NULL);

                  /* If found - copy field data - from vertexdata to edgedata.. */
                  if ((tmpdata = (DspVertexdata)GRAPHgetvertexdata(vtxnode)) != NULL)
                    strcpy(mycitydata->name, ((MyCitydata)(tmpdata->data))->name);

                  state = 2;
                  break;

                case 2: /* Read weight */
                  strcpy(tmp, pc);
                  strtrim(tmp, isspace, isspace);

                  if (!isunsignedfloat(tmp) || *tmp=='\0')
                    {
                      sprintf(errmess, "\nData error - when reading edge weight data(=%s) - on this line:\n%s", tmp, bufcopy);
                      error_mess(errmess); /* Print error message */
                      /* Free dyn. allocated  memory */
                      my_destroy(edata);
                      exit(EXIT_FAILURE);
                    }
                  edata->weight = atof(tmp);

                  /* Insert edge into graph... */
                  if ((retval = GRAPHinsedge(gr, &tmpvtxdata, edata)) != OK)
                    {
                      sprintf(errmess, "\nError when inserting edge from %d to %d - on this line:\n%s",
                              cty_data.id, mycitydata->id, bufcopy);
                      error_mess(errmess);
                      /* Free dyn. allocated  memory */
                      my_destroy(edata);
                      return retval;
                    }
                  state = 1;
                  break;
                }
              fldctr++; /* Increment the field counter */
            }

          if (!(fldctr%2)) /* If field counter is even.. */
            {
              sprintf(errmess, "\nData corrupt (wrong number of fields?) - on this line:\n%s", bufcopy);
              error_mess(errmess);
              /* Free dyn. allocated  memory */
              my_destroy(edata);
              exit(EXIT_FAILURE);
            }
        }
    }

  /* Everything is OK */
  return 0;
}

void printroute(DspVertexdata vtxdata)
{
  MyCitydata cty_data;

  if (vtxdata == NULL)
    return;
  printroute(vtxdata->parent);

  cty_data = (MyCitydata)(vtxdata->data);
  printf("%02d %-10.10s --> %.1f km\n", cty_data->id, cty_data->name, vtxdata->distance);
}

void error_mess(const char *str)
{
  fprintf(stderr, "%s\n", str);
}

void vtx_prt(const void *vtxdata)
{
  DspVertexdata vdata;
  MyCitydata cty_data;

  vdata = (DspVertexdata)vtxdata;
  cty_data = (MyCitydata)(vdata->data);

  if (row_ctr%15 == 0)
    {
      prompt_and_pause("\n\n");
      my_clearscrn();
    }
  printf("%02d %-12.12s", cty_data->id, cty_data->name);
  row_ctr++;
}

void edge_prt(const void *edgedata)
{
  DspVertexdata edata;
  MyCitydata cty_data;

  edata = (DspVertexdata)edgedata;
  cty_data = (MyCitydata)(edata->data);

  printf("%02d|%-3.3s|%4.1f|", cty_data->id, cty_data->name, edata->weight);
} 

/* --- Function: void prt_fill(char *str, char fillchar, int fieldlen) --- */
void prt_fill(char *str, char fillchar, int fieldlen)
{
  int  i=0, slen;
  char *pc;

  pc = str;
  slen = strlen(str);

  while (i < fieldlen)
    {
      (i - slen) < 0 ? putchar(*pc) : putchar(fillchar);
      i++;
      pc++;
    }
}

/* --- Function: void print_vertices(Graph gr) --- */
void print_vertices(Graph gr)
{
  VertexNode vtxnode;
  DspVertexdata vdata;
  MyCitydata cty_data;
  int i;

  for (i = 0, vtxnode = GRAPHgetvertexhead(gr); vtxnode != NULL; ++i, vtxnode = GRAPHgetvertexnext(vtxnode))
    {
      vdata = (DspVertexdata)GRAPHgetvertexdata(vtxnode);
      cty_data = (MyCitydata)(vdata->data);

      if (i%5 == 0)
        {
          printf("\n   %02d ", cty_data->id);
        }
      else
        {
          printf(" %02d ", cty_data->id);
        }
      prt_fill(cty_data->name, MYFILLCHAR, 11);
    }
}

/* --- Function: void make_tour(Graph gr) --- */
void make_tour(Graph gr)
{
  int start, dest, nr_of_vtcs, retval;
  char reply;
  struct DspVertexdata_ tmpvtxdata;
  struct MyCitydata_ tmpctydata;
  DspVertexdata vtxdata;
  MyCitydata mcd;
  char startname[KEYSIZ];
  char destname[KEYSIZ];
  VertexNode vtxnode;
  Slist my_destpaths = NULL;

  my_clearscrn();
  printf("--- MAKE A TOUR - ENTER START & DESTINATION ---\n");
  printf("\nDestinations:");
  print_vertices(gr); /* Print target destination in graph.. */
  printf("\n");
  tmpvtxdata.data = &tmpctydata;   /* Link temp. structures together.. */
  nr_of_vtcs = GRAPHvcount(gr);

  /* Read start nr.. */
  start = read_int("Start      ", 1, nr_of_vtcs);
  /* Get corresponding city name from graph */
  tmpctydata.id = start;
  vtxnode = GRAPHfindvertex(gr, &tmpvtxdata);
  assert(vtxnode != NULL);
  /* Get the city name.. */
  vtxdata = (DspVertexdata)GRAPHgetvertexdata(vtxnode);
  mcd = (MyCitydata)vtxdata->data;
  strcpy(startname, mcd->name);
  printf("Start      : %d %-11.11s", start, startname);

  /* Make call to ALGOdsp() - this is where the work is done... */
  if ((retval = ALGOdsp(gr, &tmpvtxdata, &my_destpaths, my_match)) != OK)
    {
      error_mess("Something went wrong in call to 'ALGOdsp()' - bailing out..!");
      GRAPHdestroy(gr);
      exit(-1);
    }
  assert(my_destpaths != NULL);

  do
    {
      dest = read_int("Destination", 1, nr_of_vtcs);
      /* Get corresponding city name from graph */
      tmpctydata.id = dest;
      vtxnode = GRAPHfindvertex(gr, &tmpvtxdata);
      assert(vtxnode != NULL);
      /* Get the city name.. */
      vtxdata = (DspVertexdata)GRAPHgetvertexdata(vtxnode);
      mcd = (MyCitydata)vtxdata->data;
      strcpy(destname, mcd->name);
      printf("Destination: %d %-11.11s", dest, destname);

      /* Call function 'print_route()' here... */
      printf("\nOptimized route:\n");
      printroute(vtxdata);

      printf("\nTry another destination (y/n)? ");
      reply = getchar();
      getchar(); /* Remove '\n' from keyb. buffer */
      if (reply == 'y' || reply == 'y')
        {
          my_clearscrn();
          printf("--- MAKE A TOUR - ENTER NEW DESTINATION ---\n");
          printf("\nDestinations:");
          print_vertices(gr);
          printf("\n\n");
          printf("Start: %d %-11.11s", start, startname);
        }
    } while (reply == 'y' || reply == 'Y');

  SLISTdestroy(my_destpaths);
}

/* --- Calculate the Minimum Spanning Tree --- */
void min_span_tree(Graph gr)
{
  my_clearscrn();
  printf("--- MIMIMAL SPANNING TREE ---");
  prompt_and_pause("\n\nComing up in next version..");
}

/* --- Function: void final_status(Graph gr) --- */
void final_status(Graph gr)
{
  /* my_clearscrn(); */
  /* printf("--- FINAL STATUS ---"); */
  /* printf("\n\nFinal graph status(%d vertices/%d edges): ", GRAPHvcount(gr), GRAPHecount(gr)); */
  /* GRAPHprint(gr, printvtx, printedge); */
  prompt_and_pause("\n\nThat's all folks...- Bye!");
}

int main(void)
{
  Graph mygraph;
  char grdatafile[] = "dsp_data.txt";
  int menu_choice;


  /* Enter menu loop.. */
  do
    {
      if ((mygraph = GRAPHinit(my_match, my_destroy)) == NULL)
        {
          printf("\nFatal error - bailing out...\n!");
          exit(EXIT_FAILURE);
        }

      if (read_graphdata(mygraph, grdatafile) != OK)
        {
          printf("\nFailed to read data from file: %s", grdatafile);
          exit(EXIT_FAILURE);
        }

      menu_choice = menu(MAIN_MENU_ROW, 0, 3);

      switch (menu_choice)
        {
        case 1:
          make_tour(mygraph);
          break;
        case 2:
          min_span_tree(mygraph);
          break;
        case 3:
          my_clearscrn();
          printf("--- PRINT VERTICES(=%d) AND EDGES(=%d) ---\n", GRAPHvcount(mygraph), GRAPHecount(mygraph));
          row_ctr=1; /* Initialize row counter.. */
          GRAPHprint(mygraph, vtx_prt, edge_prt);         
          prompt_and_pause("\n\n");
          break;
        default:
          final_status(mygraph);
          break;
        }
      GRAPHdestroy(mygraph);
    }
  while (menu_choice); 

  return 0;
}
