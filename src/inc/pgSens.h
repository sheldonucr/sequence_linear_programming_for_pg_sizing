/*
 * $RCSfile: pgSens.h,v $
 * $Revision: 1.2 $
 * $Date: 2000/10/10 01:00:04 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

/*
    Parsing and building of MNA matrix from SPICE netlist
*/

#ifndef __PG_SENSE_
#define __PG_SENSE_

#include "pgCircuit.h"

/* data structure definition */
#define DEV_SENS struct _dev_sens
#define VOL_SENS struct _vol_sens

DEV_SENS {
    Branches    *pdev;
    double        sens;
    DEV_SENS    *next;
};

VOL_SENS {
    DEV_SENS    *dslist; /* device sensitivity list */
    int        node; /* correspoding node */
    VOL_SENS    *next; 
};

/* some global variables declaration */

VOL_SENS *theVolSenList;

/* function declaration */

extern void pgDCSensAnalysis(Circuit *ckt);
extern void pgFreeVolSensList(VOL_SENS *);
extern void pgFreeDevSensList(DEV_SENS *);
extern void pgComputeNodeSens(Circuit *ckt, int node);
extern void pgComputeDeviceSens(Circuit *ckt, int node, double **sens_matrix);
extern void pgAddVolSens(VOL_SENS *pdev_sens);
extern void pgAddDevSens(VOL_SENS *pvol_sens, DEV_SENS *pdev_sens);
extern void pgPrintNodeSens( int node );

#endif __PG_SENSE_
