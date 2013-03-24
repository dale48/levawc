/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: utils.c
 * Author  : Dan Levin
 * Date    : Fri Mar 22 12:40:45 GMT 2013
 * Version : 0.40
 * ---
 * Description: Miscellanenous utility functions
 * 
 */
/**
 * @file utils.c
 **/

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

/* FUNCTION DEFINITIONS */
/* --- Function: int randint(int low, int high) --- */
int randint(int low, int high)
{
  return low+rand()%(high-low+1);
}

/* --- Function: int dn_up_lim(int maxval, int minval, int val) --- */
int dn_up_lim(int minval, int maxval, int val)
{
  val = val < minval ? minval : val;
  val = val > maxval ? maxval : val;

  return val;
}

/* --- Function: int dn_lim(int minval, int val) --- */
int dn_lim(int minval, int val)
{
  return val < minval ? minval : val;
}


/* --- Function: int up_lim(int maxval, int val) --- */
int up_lim(int maxval, int val)
{
  return val > maxval ? maxval : val;
}

/* --- Function: int maxval(int val1, int val2) --- */
int maxval(int val1, int val2)
{
  return val1>val2 ? val1 : val2;
}

/* --- Function: int minval(int val1, int val2) --- */
int minval(int val1, int val2)
{
  return val1<val2 ? val1 : val2;
}
