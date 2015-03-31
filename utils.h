/*
 *       _____
 * ANSI / ___/
 *     / /__  
 *     \___/  
 *
 * Filename: utils.h
 * Author  : Dan Levin
 * Date    : Sun Feb 08 18:41:33 2015
 * Version : 0.51
 * ---
 * Description: Description: Miscellanenous utility functions
 * 
 * Date   Revision message
 * 150331 This code ready for version 0.51
 *
 */
/**
 * @file utils.h
 **/

#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#ifdef __cplusplus
extern "C" {
#endif

  /* SOME USEFUL MACROS */
  /* Check if memory allocation failed - Usage: MALCHK(string); */
  /**
   * Macro for handling dynamic memory allocation failure
   * 
   * Check if memory allocation failed - Usage: MALCHK(pointer to dynamically allocated memory);
   *
   **/
#define MALCHK(x) if(!x){fprintf(stderr, "Out of memory, file: %s, line: %d", __FILE__, __LINE__); exit(EXIT_FAILURE);}
 
  /**
   * Macro for handling file opening failure
   * 
   * Check if a file was opened properly - Usage: FILCHK(fp);
   *
   **/
#define FILCHK(x) if(!x){fprintf(stderr, "File opening failure - in file: %s, line: %d", __FILE__, __LINE__); exit(EXIT_FAILURE);}

  /* FUNCTION-DECLARATIONS */
  /**
   * Generate random integer values in an interval
   * 
   **/
  int rand_int(int low, int high);

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

  /**
   * Determine if a string is a float..
   * 
   **/
  int isfloat2(char *c);

  /**
   * Determine if a string is a float..
   * 
   **/
  int isfloat(char *str);

  /**
   * Determine if a string is an unsigned integer..
   * 
   **/
  int isunsigned(char *str);

  /**
   * Determine if a string is an unsigned float..
   * 
   **/
  int isunsignedfloat(char *str);

  /**
   * Move a string in memory...
   * 
   **/
  char *strmove (char *to, char *from);


  /**
   * Insert a string into another..
   * 
   **/
  char *strins (char *dest, const char *ins);

  /**
   * Trim both ends of a string 
   * 
   **/
  char *strtrim (char *str, int (*left) (int), int (*right) (int));

  /**
   * Trim left side(=skip blanks) of a string
   * 
   **/
  char *strtriml (char *str);

  /**
   * Trim rigt side(=skip trailing blanks) of a string
   * 
   **/
  char *strtrimr (char *str);

  /**
   * Read integer from stdin - with some error handling
   * 
   **/
  int read_int(const char *prompt, const int lo_val, const int hi_val);

  /**
   * Read character from stdin - with some error handling
   * 
   **/
  int read_char(const char *prompt, const int lo_val, const int hi_val, int (*char_ok)(int ch));

  /**
   * Rudimentary menu function
   * 
   **/
  int menu(const char *menurow, const int lo_sel, const int hi_sel);

  /**
   * Determine if a value is within a specified interval - or not
   * 
   **/
  int is_val_ok(const int val, const int lo_val, const int hi_val);

  /**
   * Clear screen - both in Windows and Linux
   * 
   **/
  void my_clearscrn(void);

  /**
   * Stop execution, show prompt - and wait for <Enter> keypress
   * 
   **/
  void prompt_and_pause(char *message);

  /**
   * Flush input/keyb buffer
   * 
   **/
  void myflush(FILE *in);


  /* END-OF-FUNCTION-DECLARATIONS */

#ifdef __cplusplus
}
#endif 

#endif /* _UTILS_H_ */
