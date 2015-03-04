/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: utils.c
 * Author  : Dan Levin
 * Date    : Mon Feb 09 07:58:29 2015
 * Version : 0.5
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

#ifndef OK
#define OK 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* FUNCTION DEFINITIONS */
/* --- Function: int randint(int low, int high) --- */
int rand_int(int low, int high)
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

/* --- Function: int isfloat2(char *c) --- */
int isfloat2(char *c)
{
  int isnum=0, wfe=1;
  int i=0;

  if (strlen(c)==0)
    return(FALSE);

  if ((c[i]=='+') || (c[i]=='-'))
    i++;
  while ((c[i] >= '0') && (c[i] <= '9')) {
    isnum = TRUE;
    i++;
  }
  if (c[i]=='.') {
    i++;
    while ((c[i] >= '0') && (c[i] <= '9')) {
      isnum = TRUE;
      i++;
    }
  }
  if ((c[i]=='e') || (c[i]=='E')) {
    wfe = FALSE;
    i++;
    if ((c[i]=='+') || (c[i]=='-'))
      i++;
    while ((c[i] >= '0') && (c[i] <= '9')) {
      wfe = TRUE;
      i++;
    }
  }
  if ((c[i]=='f') || (c[i]=='F') || (c[i]=='l') || (c[i]=='L'))
    i++;

  if ((c[i]=='\0') && isnum && wfe)
    return(TRUE);
  else
    return(FALSE);
}

/* --- Function: int isfloat(char *str) --- */
int isfloat(char *str) 
{
  int i = 0;

  if ( str[i] == '-' )
    i++;

  while( str[i] >= '0' && str[i] <= '9' ) 
    i++;

  if (!str[i])
    return TRUE;

  if (str[i] == '.' ) 
    i++;

  while( str[i] >= '0' && str[i] <= '9' )
    i++;

  if (!str[i])
    return TRUE;

  return FALSE;
} 

/* --- Function: int isunsigned(char *str) --- */
int isunsigned(char *str)
{
  int i = 0;

  if ( str[i] == '+' )
    i++;

  while( str[i] >= '0' && str[i] <= '9' ) 
    i++;

  if (!str[i])
    return TRUE;

  return FALSE;
}

/* --- Function: int isunsignedfloat(char *str) --- */
int isunsignedfloat(char *str) 
{
  int i = 0;

  if ( str[i] == '+' )
    i++;

  while( str[i] >= '0' && str[i] <= '9' ) 
    i++;

  if (!str[i])
    return TRUE;

  if (str[i] == '.' ) 
    i++;

  while( str[i] >= '0' && str[i] <= '9' )
    i++;

  if (!str[i])
    return TRUE;

  return FALSE;
} 

/* --- Function: char *strmove(char *to, char *from) --- */
char *strmove (char *to, char *from)
{
  return memmove (to, from, strlen (from) + 1); /* + 1 because of termination */
}

/* --- Function: char *strins (char *dest, const char *ins) --- */
char *strins (char *dest, const char *ins)
{
  strmove (dest + strlen (ins), dest);
  memcpy (dest, ins, strlen (ins));

  return dest;
}

/* --- Function: char *strtrim(char *str, int (*left) (int), int (*right) (int)) --- */
char *strtrim(char *str, int (*left) (int), int (*right) (int))
{
  if (left)
    {
      char *p = str;

      while (*p && left ((int) *p))
        p++;

      if (p - str)
        strmove (str, p);
    }

  if (right)
    {
      char *p = strchr (str, 0);

      while ((p - 1) - str && right ((int) *(p - 1)))
        p--;

      *p = 0;
    }

  return str;
}

/* --- Function: char *strtriml(char *str) --- */
char *strtriml (char *str)
{
  return strtrim(str, isspace, NULL);
}

/* --- Function: char *strtrimr(char *str) --- */
char *strtrimr(char *str)
{
  return strtrim (str, NULL, isspace);
}

/* --- Function: int menu(const int lo_sel, const int hi_sel) --- */
int menu(const char *menurow, const int lo_sel, const int hi_sel)
{
  int selection;

  my_clearscrn();

  selection = read_int(menurow, lo_sel, hi_sel);

  return selection;
}

/* --- Function: int read_int(char *prompt, int lo_val, int hi_val) --- */
int read_int(const char *prompt, const int lo_val, const int hi_val)
{
  int retval, input, val_ok;

  /* Initialize 'val_ok' - this is crucial.. */
  val_ok = FALSE;

  do
    {
      printf("\n%s", prompt);
      /* Print tail to prompt - if 'lo_val' differ from 'hi_val'.. */
      if (lo_val != hi_val)
        printf("<%d-%d>+<Enter>: ", lo_val, hi_val);

      /* Read user input stdin.. */
      retval = scanf("%d", &input);
      
      if (retval == 1) /* If valid integer input.. */
        {
          if (lo_val != hi_val)
            { 
              /* Check whether input is within interval 'lo_val' to 'hi_val'.. */
              val_ok = is_val_ok(input, lo_val, hi_val);
              if (!val_ok)
                printf("Invalid selection - use <%d> to <%d>...!", lo_val, hi_val);
            }
          else
            val_ok =TRUE;
        }
      else
        printf("Invalid input - integer only!");

      /* Remove '\n' from keyb. buffer */
      getchar();
    } while (retval == EOF || !val_ok);

  return input;
}

/* --- Function: int is_val_ok(const int val, const int lo_val, const int hi_val) --- */
int is_val_ok(const int val, const int lo_val, const int hi_val)
{
  return (val>=lo_val && val<=hi_val) ? TRUE : FALSE;
}

/* --- Function: void my_clearscrn(void) --- */
void my_clearscrn(void)
{
#ifdef __unix__
  system("clear");
#elif _WIN32
  system("cls");
#endif
}

/* --- Function: void prompt_and_pause(char *message) --- */
void prompt_and_pause(char *message)
{
  printf("%s", message);
  printf(" - Hit <Enter> to continue...");
  getchar();
}
