/*
 * $RCSfile: pgStddefs.h,v $
 * $Revision: 1.1 $
 * $Date: 1999/04/30 17:43:27 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

/*
 *
 * stddefs.h : standard header file
 * This file should be compatable with c and c++
 *
 */

#ifndef STDDEFS_H
#define STDDEFS_H

#include <time.h>

/* The folowing is for portability between IBM PC's and SUN.
   'integer' is a 32 bit number. */

#define BITS_PER_INTEGER 32
typedef int                integer;  /* subst long for PC's */   
typedef unsigned int       uinteger;  /* subst unsigned long for PC's */   
typedef unsigned int       boolean;  
typedef char*              string; 
/*typedef unsigned long       ulong;*/

#ifndef TRUE
#define TRUE            ((boolean)1)
#endif
#ifndef FALSE
#define FALSE           ((boolean)0)
#endif
#define YES             1
#define NO              0
#define RAD_CODE        YES

#define NIL(X)   ((X)0)
#ifndef MAX
#define MAX(X,Y) (((X)>(Y))?(X):(Y))
#endif
#ifndef MIN
#define MIN(X,Y) (((X)>(Y))?(Y):(X))
#endif
#ifndef ABS
#define ABS(X) (((X)>0)?(X):(X)*-1)
#endif

#ifndef Sign
#define Sign(X) (((X)>0)?1:-1)
#endif

/*
#ifndef SWAP
#define SWAP(T,X,Y) {T S; S=X; X=Y; Y=S;}
#endif
*/
#endif
