/*
 * $RCSfile: pgMatchOpt.h,v $
 * $Revision: 1.1 $
 * $Date: 1999/04/30 17:43:27 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

/*
**    header file for optimization routine 
**    under branch-width equality (match) constraint
*/
#ifndef __PG_MATCHOPT_
#define __PG_MATCHOPT_


#include "pgCircuit.h"

/*
**    Match set defintion.
**    In each match set, all the branches 
**    are required to be equal.
*/

#define MatchSet struct _matchset

MatchSet {
    Branches *branch;
    MatchSet  *next;
    double   ub;    /* upper bound of resistance value */
    double     lb;    /* lower bound of resistance value */
};

extern MatchSet * pgNewMatch(Branches *);
extern void pgFreeMatchSet(MatchSet *);
extern void pgAddMatch(MatchSet **list, MatchSet *new);
extern void pgCompMatchSetBound(MatchSet *list);
extern void pgSetResistValue(MatchSet *list, double value);

/************************************************************/
/************************************************************/

/*
**    List of MatchSet
*/

#define MatchList struct _matchlist

MatchList {
    MatchSet  *mset;
    MatchList *next;
    int      num; /* number of branches in the matchset */
};

extern MatchList *pgNewMatchList();
extern void pgAddMatchList(MatchList **list, MatchList *new);
extern void pgBuildMatchList(ExtConst *);
extern void pgFreeMatchList(MatchList *);
extern MatchList *pgFindMatchList(Branches *);
extern void pgPrintMatchList(FILE *fp, MatchList *);
extern void pgUpdateMatchValue(MatchSet *, double);

/************************************************************/
/************************************************************/
/* other optimization function */

extern void pgMatchSetOpt();
extern void pgOptmzAllMatch();
extern void pgOptmzMatchSet( MatchSet *, double ); 
extern double pgMatchSetMaxStep( MatchSet *); 
extern double pgMatchSetSens( MatchSet *, int); 

/************************************************************/
/************************************************************/

/* 
**    global variable definition
*/

MatchList    *theMatchList;

#define     MIN_STEP 1e-8 /* minimum step size */

#endif __PG_MATCHOPT_
