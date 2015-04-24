/*
 * $RCSfile: pgState.c,v $                                                                   
 * $Revision: 1.3 $
 * $Date: 2000/10/10 01:01:12 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

/*******************************************************************************
*                                                                              *
* Module name:                              Prefix: pg                        *
*                                            Author: XiangDong Tan           *
* Description:                                                                 *
*    routines relating the voltage and current 
*    calculation using sparse matrix solver.
*******************************************************************************/

#include "pgOptmz.h"

/* 
**    Compute the node voltages and 
**    branch currents and store them
**    into the node array and branch 
**    list used for constraint generation
**    and intial point in nonlinear 
**    optimization.
*/

int
pgComputeStateFromRes(Circuit *ckt)
{
  FILE *pf;
  char line[1024];
  Branches *baux;
  int i;
 
  assert(ckt);
  assert(ckt->theDeviceList);
  assert(ckt->theNodeArray);
 
  /* build the MNA matrix */
  if( pgBuildMNAEquation(ckt) == -1)
    return -1;
 
  printf("External size of the matrix: %d\n",
	 spGetSize(theMatrix,1));
  printf("Internal size of the matrix: %d\n",
	 spGetSize(theMatrix,0));
  fflush(stdout);

    /* LU decomposition */
  if( spFactor(theMatrix) != spOKAY ){
    error_mesg(INT_ERROR,"Matrix factorization error.");
    return -1;
  }
 
  /* solve it */
  spSolve(theMatrix, theRhs, theSol);
 
  /*
    pgPrintResult(theSol,nMatrixSize);
    */

    /* assigne the node voltage */
  for( i = 1; i <= ckt->nMatrixSize; i++)
    ckt->theNodeArray[i].voltage = theSol[i];

    /* assign the branch current 
    ** The direction of the current 
    ** is from n1 to n2 
    */
  for( baux = ckt->theDeviceList; baux; baux = baux->next ){ 
    if(baux->stat != sNormal)
      continue;
    if(baux->type != dRES)
      continue;
    baux->current = (theSol[baux->n1] - theSol[baux->n2])/
      (baux->value + FTINY);
    baux->vdrop = theSol[baux->n1] - theSol[baux->n2];
    /*
      printf("I(%d, %d) = %g\n",baux->n1, baux->n2, baux->current);
      */
  }

  spDestroy(theMatrix);
  free(theRhs);
  free(theSol);
}

/*
**     Compute resistance from VI states
*/
void
pgComputeResFromState(Circuit *ckt)
{
  double vdrop, value;
  Branches *baux;
 
  assert(ckt);
  assert(ckt->theDeviceList);
  assert(ckt->theNodeArray);
 
  for( baux = ckt->theDeviceList; baux; baux = baux->next ){
    if(baux->stat != sNormal)
      continue;
    if(baux->type != dRES)
      continue;
    baux->vdrop = ckt->theNodeArray[baux->n1].voltage -
      ckt->theNodeArray[baux->n2].voltage;
    value = (baux->vdrop)/(baux->current + FTINY);
 
    /*
      printf("%s: resistance = %g\n",baux->name,value);
      */
    baux->value = value;
  }
}

/*
**     Compute resistance from VI states
*/
void
pgComputeResFromBVolState(Circuit *ckt)
{
  double vdrop, value;
  Branches *baux;
 
  assert(ckt);
  assert(ckt->theDeviceList);
  assert(ckt->theNodeArray);
 
  for( baux = ckt->theDeviceList; baux; baux = baux->next ){
    if(baux->stat != sNormal)
      continue;
    if(baux->type != dRES)
      continue;

    value = (baux->vdrop)/(baux->current + FTINY);
 
    /*
      printf("%s: resistance = %g\n",baux->name,value);
      */
    baux->value = value;
  }
}
 
/*
**    Compute the width from the resistance value
*/

void
pgCompWidthFromRes(Circuit *ckt)
{
  double value;
  Branches *baux;

  for( baux = ckt->theDeviceList; baux; baux = baux->next ){
    if(baux->stat != sNormal)
      continue;
    if(baux->type != dRES)
      continue;
    baux->width = (baux->lay->unitRes*baux->length)/
      (FTINY + baux->value);
    
    /*
    printf("%s: width = %g\t",baux->name,baux->width);
    printf("resist = %g \t", baux->value);
    printf("length = %g \t", baux->length);
    printf("add= %x \n", baux);
    */
  }
}
