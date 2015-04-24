/*
 * $RCSfile: pgMPS.h,v $
 * $Revision: 1.3 $
 * $Date: 2000/10/10 01:00:04 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

/* 
**    Head file for the MPS I/O operation
*/

#include "pgCircuit.h"
#include "pgOptmz.h"

#ifndef __PG_MPS_
#define __PG_MPS_

/************************************************************/
/************************************************************/

#define ColumnEntry struct _column_entry
ColumnEntry {
    long    row; /* the row this column is located */
    double    coeff;
    ColumnEntry *next;
};

extern ColumnEntry * pgNewColEntry(long i, double d);
extern void pgFreeColEntry(ColumnEntry *);
extern void pgAddColEntry(ColumnEntry **, ColumnEntry *);
extern void pgMPSBuildVarColumn(IntConst *);

/************************************************************
/************************************************************/

/* each variable corresponds to a column */

#define VariableList struct _variable_list
VariableList {
    long    index;
    ColumnEntry *head;
    Branches *bch;
    Nodes  *node;
};

void pgMPSBuildBchVarList(Circuit *ckt);
extern void pgFreeVarList(Circuit *ckt);

extern void pgMPSPrintRows(FILE *, IntConst *);
extern void pgMPSPrintRowsR(FILE *, IntConst *);

extern void pgMPSPrintColumns(Circuit *ckt,FILE *);

extern void pgMPSPrintRHS(FILE *, IntConst *);

extern void pgMPSBuildCurVarList();
extern void pgMPSBuildVolVarList();

extern void pgMPSReadCurResult(char *);
extern void pgMPSReadVolResult(char *);

extern void pgMPSBuildCurVarColumn(Circuit *ckt,IntConst *ilist);
extern void pgMPSBuildVolVarColumn(Circuit *ckt,IntConst *ilist);

VariableList *theVarArray;

#endif __PG_MPS_
