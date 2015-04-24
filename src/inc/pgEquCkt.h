/*
 * $RCSfile: pgEquCkt.h,v $
 * $Revision: 1.1 $
 * $Date: 2000/10/10 01:00:04 $, (c) Copyright 1999, 2000 by X.-D. Sheldon Tan
 */

#ifndef __EQUCKT_
#define __EQUCKT_

#include "pgCommon.h"
#include "pgCircuit.h"

void pgOptimizeEqnCircuit(Circuit *oldCkt);
void pgConstructEquCkt(Circuit *ckt, Circuit *newCkt);
int isSuppressedNode(Circuit *ckt, int idx);
void markAllNodeUnvisited(Circuit *ckt);
void copyBranch(Branches *bch, Circuit *ckt);
int suppressBranches(Branches *bch, Circuit *ckt, Circuit *newCkt);
void computeEqnPiCircuit(BranchLink *bchlist,
			 double *eqnResist,
			 double *equLength,
			 double *current1,
			 double *current2);
#endif __EQUCKT_
