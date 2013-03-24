/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: utils.h
 * Author  : Dan Levin
 * Date    : Fri Mar 22 12:40:45 GMT 2013
 * Version : 0.40
 * ---
 * Description: Description: Miscellanenous utility functions
 * 
 */
/**
 * @file utils.h
 **/

#include <stdio.h>
#include <stdlib.h>

/* SOME USEFUL MACROS */
/* Check if memory allocation failed - Usage: MALCHK(string); */
/**
 * Macro for handling dynamic memory allocation failure
 * 
 **/
#define MALCHK(x) if(!x){puts("Out of memory"); exit(EXIT_FAILURE);}
 
/* Check if a file was opened properly - Usage: FILCHK(fp); */
/**
 * Macro for handling file opening failure
 * 
 **/
#define FILCHK(x) if(!x){printf("Cannot open file %s\n", x); exit(EXIT_FAILURE);}

/* FUNCTION DECLARATIONS */

/**
 * Generate random integer values in an interval
 * 
 **/
int randint(int low, int high);

/**
 * Set both a lower and upper bounds for an integer value - i.e. force it into a given interval
 * 
 **/
int dn_up_lim(int minval, int maxval, int val);

/**
 * Set a lower bound for an integer value
 * 
 **/
int dn_lim(int minval, int val);

/**
 * Set an upper bound for an integer value
 * 
 **/
int up_lim(int maxval, int val);

/**
 * Return the largest of two integer values
 * 
 **/
int maxval(int val1, int val2);

/**
 * Return the smallest of two integer values
 * 
 **/
int minval(int val1, int val2);
