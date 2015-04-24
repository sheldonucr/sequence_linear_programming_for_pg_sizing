/*
 * $RCSfile: pgEMopt.h,v $
 * $Revision: 1.2 $
 * $Date: 2000/10/10 01:00:04 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

/*
**    optimization s.t. electromigration constraint
*/

/* function declaration */
#include "pgCircuit.h"

extern void pgEMoptmz(Circuit *ckt); 
extern void pgCompMaxBrchCurrent(Circuit *ckt);
extern void pgCompMinWidthByEM(Circuit *ckt);
extern void pgEMMatchSetOpt(Circuit *ckt);
