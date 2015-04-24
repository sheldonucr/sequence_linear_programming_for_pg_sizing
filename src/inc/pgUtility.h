/*
 * $RCSfile: pgUtility.h,v $
 * $Revision: 1.1 $
 * $Date: 1999/04/30 17:43:27 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

/*
** The header file for the utility functions
*/

#ifndef UTILITY_H
#define UTILITY_H

extern char    *CopyStr(char *str);
extern char *    ToUpper(char *str);
extern char *    ToLower(char *str);
extern char *    NextField(char **str);
extern double     TransValue(char *value);
extern double    RandVar(double lbound, double ubound);

#endif UTILITY_H

