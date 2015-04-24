/*
 * $RCSfile: pgCostFunc.c,v $
 * $Revision: 1.3 $
 * $Date: 2000/10/10 01:01:12 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

/*******************************************************************************
*                                                                              *
* Module name:                              Prefix: pg                         *
*                                           Author: XiangDong Tan              *
* Description:                                                                 *
*    Compute the objective functions for the linear
*    non-linear optimization problem.
*******************************************************************************/

#include "pgOptmz.h"
#define SMALLNUM (1e-8)

/* 
**    Compute the cost of the objective function 
**    without penalty factor.
*/

double pgComputeCostFunct(Circuit *ckt)
{
	Branches *blist;
	double    bvol;
	double    cost = 0.0;

	assert(ckt);
	assert(ckt->theNodeArray);
	assert(ckt->theDeviceList);

	/* we go throuht all the resistor branches */
	for(blist = ckt->theDeviceList; blist; blist = blist->next)
    {
		/* skip abmormal branches */
		if(blist->stat != sNormal)
			continue;
		/* we skip the non-resistive branches */
		if(blist->type != dRES)
			continue;

		bvol = ckt->theNodeArray[blist->n1].voltage - 
			ckt->theNodeArray[blist->n2].voltage;
		/*
		  bvol = blist->vdrop;
		*/
		if(bvol*blist->current < 0)
		{
			sprintf(buf,"current in branch: %s is inconsistant with its voltage",blist->name);
			error_mesg(INT_ERROR,buf);
			continue;
        }
      
		if(bvol == 0.0)
		{
			/*
			  sprintf(buf,"volatage drop in branch: %s is 0.0",blist->name);
			  error_mesg(INT_ERROR,buf);
			*/
			continue;
        }
      
		cost += (blist->current/(bvol + FTINY))*blist->length
			*blist->length*blist->lay->unitRes;
    }
	return cost;
}


/*    Compute the cost of the objective function with the penalty
**    function coming from the NLP constraints.  The penalty function
**    is based on the interior-penalty function due to Fiacco and
**    McCormick's work, which makes the combined function a convex
**    function.  
*/

double pgComputePenaltyCostFunct(Circuit *ckt)
{
	double    cost = 0.0, pcost;
	IntConst *icaux;

	assert(ckt);
	assert(ckt->NLPIntConstList);

	/* first obtain the real cost */
	cost = pgComputeCostFunct(ckt);

	/* then, we compute the cost associated with panelty factor*/
	for(icaux = ckt->NLPIntConstList; icaux; icaux = icaux->next)
    {
		pcost = pgComputeIntConstValue(icaux);
		/*
		  pgPrintIntConst(stdout,icaux);
		  printf("VALUE: %g cost: %g\n",pcost, cost);
		*/
		if(pcost < 1e-8 && (icaux->type == GE || icaux->type == GT))
		{
	  	  
			printf("Value:  %g; ", pcost);
			pgPrintIntConst(stdout, icaux);
	  
		}
      
		if(icaux->type == GT || icaux->type == GE)
			cost += PenalWeight/(pcost + FTINY);
		else if(icaux->type == EQ)
		{
			double addCost = EqualPanalFactor*pcost*pcost*pcost*pcost;
			/*
			  printf("the add cost from equal width: %g\n", addCost);
			  printf("EqualPenalFactor: %g \n ", EqualPanalFactor);
			*/
			cost += addCost;
		}
      
    }

	return cost;
}

/*
**    Compute the gradient of the combined
**    cost function with respect to voltage.
**    Since all the voltage variable x , y are 
**    in the form c/(a*x + b*y), so the 
**    derivative of it will be -c*a/(a*x + b*y)^^2.
**    The vector space has been allocated with 
**    size of nMatrixSize+1.
*/

void pgCostFuncGradient(Circuit *ckt, double *grad)
{
	int i;
	double    num, den, value;
	Branches *blist;
	IntConst *iclist;
	Products  *paux;

	assert(ckt);
	assert(grad);
	assert(ckt->theNodeArray);
	assert(ckt->theDeviceList);
	assert(ckt->NLPIntConstList);

	/* initialize the the gradient vector */
	for(i=0; i <= ckt->nMatrixSize; i++)
		grad[i] = 0;
    
	/* then we go through all the branches */
	for(blist = ckt->theDeviceList; blist; blist = blist->next)
    {
		if(blist->stat != sNormal)
			continue;
		if(blist->type != dRES)
			continue;
		num = blist->lay->unitRes*blist->length*blist->length*
			blist->current;
		den = ckt->theNodeArray[blist->n1].voltage - 
			ckt->theNodeArray[blist->n2].voltage;
		den = den*den;
		if(den == 0.0)
		{
	  
			/*
			  sprintf(buf,
			  "zero voltage drop between v(%d) and v(%d).", 
			  blist->n1, blist->n2);
			  error_mesg(INT_ERROR,buf);
			*/
			/* continue; */
        }
		grad[blist->n1] += -num/(den + FTINY);
		grad[blist->n2] += num/(den + FTINY);
    }

	/* third, we go through all the corresponding 
	** internal constraints. 
	*/
	for(iclist = ckt->NLPIntConstList; iclist; iclist = iclist->next)
    {
		den = value = pgComputeIntConstValue(iclist);
		den = den*den;
		if(den < 1e-10)
		{
			/*
			  printf("ConstValue:  %g; ", sqrt(den));
			  pgPrintIntConst(stdout, iclist);
			*/
			/* continue; */
        }
		if(iclist->type == GE || iclist->type == GT)
		{
			for(paux = iclist->prod_list; paux; paux = paux->next)
			{
				num = PenalWeight*paux->sign*paux->coeff;
				grad[paux->node->index] -= num/(den + FTINY); 
			}
		}
		else if(iclist->type = EQ)
		{
			for(paux = iclist->prod_list; paux; paux = paux->next)
			{
				num = 4*EqualPanalFactor*value*value*value*paux->sign*paux->coeff;
				grad[paux->node->index] = num;
			}
		}
    }
}
