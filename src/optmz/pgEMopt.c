
/*
 * $RCSfile: pgEMopt.c,v $
 * $Revision: 1.2 $
 * $Date: 2000/10/10 01:01:12 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

/*******************************************************************************
*                                                                              *
* Module name:                              Prefix: pg                        *
*                                            Author: XiangDong Tan           *
* Description:                                                                 *
*    optimize the G/P network such that the
*    electromigration constraints are satisfied

*    Beacuse the maximum current doese not occurs 
*    when leaf node currents are at their respective
*    maximums. So we compute and accumulate the 
*    positive and negative current for each branches
*    take the larger one the maximum for the branch
*    current and further compute the width requirement
*    according to the allowed current density. This
*    method is pessimistic so that the optimized P/G
*    are guarantteed to satisfy the EM constraint.


*    We assume that the network has been optimized
*    and equal branches have heen matched.
*******************************************************************************/

#include "pgCircuit.h"
#include "pgOptmz.h"
#include "pgMatchOpt.h"
#include "pgEMopt.h"

#include <math.h>

extern double theEMConst;

void pgEMoptmz(Circuit *ckt)
{
  pgCompMaxBrchCurrent(ckt);

  pgCompMinWidthByEM(ckt);

  pgEMMatchSetOpt(ckt);
}       

/*
**    Compute the maximum branch currents which
**    is summation of postive current or nagative
**    currents contributed from each independent
**    current source.
*/

void
pgCompMaxBrchCurrent(Circuit *ckt)
{
  int i;
  double current;
  Branches *baux, *baux11;

  assert(ckt->theDeviceList);

  /* build the MNA matrix and assume the resistance value
     has been optimized and written back */
  if( pgBuildMNAEquation(ckt->theDeviceList) == -1)
    return;

  /* LU decomposition */
  if( spFactor(theMatrix) != spOKAY ){
    error_mesg(INT_ERROR,"Matrix factorization error.");
    return;
  }

  /* initialization */
  for( baux= ckt->theDeviceList; baux; baux = baux->next ){
    if(baux->type == dRES && baux->stat == sNormal)
      baux->pI = baux->nI = 0.0;
  }

  /* calculate the constribuation from each current source */
  for( baux= ckt->theDeviceList; baux; baux = baux->next ){
    if(baux->stat != sNormal || baux->type != dCUR)
      continue;
        
    printf("IS: %s\n",baux->name);
    memset(theRhs, 0, (nMatrixSize + 1)*sizeof(double));
    memset(theSol, 0, (nMatrixSize + 1)*sizeof(double));

    if(baux->n1 != 0)
      theRhs[baux->n1] = -baux->value;
    if(baux->n2 !=0)
      theRhs[baux->n2] = +baux->value;

    spSolve(theMatrix, theRhs, theSol);

    /* calculate the branch currents due to the
       independent current source */
    for(baux11 = ckt->theDeviceList; baux11; baux11 = baux11->next){
      if(baux11->type != dRES)
	continue;
      current = (theSol[baux11->n1] - theSol[baux11->n2])/
	(baux11->value + FTINY);
      if(current >= 0.0)    
	baux11->pI += current;
      else
	baux11->nI += current;
    }
  }
           
  /* release all the memory */
  spDestroy(theMatrix);
  free(theRhs);
  free(theSol);
  free(orgSol);
}


/*
**    Compute the minimum width required by EM constraints
**    we assume the postive and nagtive current has been 
**    caculated.

**    we also change the resistance value whose 
**    width violates the minimum width
**    requirement.
*/

void
pgCompMinWidthByEM(Circuit *ckt)
{
  Branches *baux, *baux11;

  assert(ckt->theDeviceList);
    
  if(theEMConst == 0.0){
    error_mesg(PARSE_ERROR,"Invalid current density constant(=0).");
    return;
  }

  /* first calculate the current width */
  /*
    pgCompWidthFromRes();
    */

  for( baux= ckt->theDeviceList; baux; baux = baux->next ){
    if(baux->type != dRES)
      continue;
    if(baux->pI > fabs(baux->nI))
      baux->wmin = baux->pI/theEMConst;
    else
      baux->wmin = -baux->nI/theEMConst;

    printf("%s, wmin = %lf, width = %lf.\n",baux->name,
	   baux->wmin, baux->width);

    if(baux->wmin > baux->width)
      baux->width = baux->wmin;
  }
}

/*
**    Set the resistance value of a matchset
**    to the largest value of the set. 
**    We assume that the matchset optimization
**    has been performed before this EM optimization.
*/

void
pgEMMatchSetOpt(Circuit *ckt)
{
  MatchList *mlist;
  MatchSet  *maux;
  double    max_val;
  Branches  *baux, *baux11;

  assert(ckt->theDeviceList);
    
  /* first, we build the match set list */
  pgBuildMatchList(ckt->theExtConstList);

  /* then we go through each match set */
  for(mlist = theMatchList; mlist; mlist = mlist->next){
    for(maux = mlist->mset; maux; maux = maux->next)
      maux->branch->value = maux->ub; 
  }

  pgFreeMatchList(theMatchList);
}
