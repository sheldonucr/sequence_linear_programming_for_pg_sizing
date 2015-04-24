/*
 * $RCSfile: pgNewton.c,v $                                                                   
 * $Revision: 1.2 $
 * $Date: 2000/10/10 01:01:12 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

/*******************************************************************************
*                                                                              *
* Module name:                              Prefix: pg                        *
*                                            Author: XiangDong Tan           *
* Description:                                                                 *
*    Use the newton's method for the Nonlinear optimization.
*******************************************************************************/

#include <math.h>
#include "pgOptmz.h"

#define ITMAX  MAX(200,ckt->nMatrixSize)
#define EPS 1.0e-10
char *theHessian; /* the Hessian matrix */

static int ncom;
static double *pcom, *xicom;


void
pgNewtonOptmz(Circuit *ckt, int *iter, double *fmin, double ftol)
/*
**      iter: number of iterations were performed.
**      fmin: minimum value of the cost function.
**      ftol: convergence tolerance on the function value.
*/
{
    double *grad,*vv;
    double fvv;
    int    i,its;
    int    error;

    assert(ckt);
    assert(ckt->theNodeArray);
    assert(ckt->theDeviceList);
    assert(ckt->NLPIntConstList);
    
    theHessian = spCreate(1,0,&error);
    spClear(theHessian);

    theRhs = (double *)malloc((ckt->nMatrixSize + 1) * sizeof(double));
    theSol = (double *)malloc((ckt->nMatrixSize + 1) * sizeof(double));
    grad = (double *)malloc((ckt->nMatrixSize + 1) * sizeof(double));
    vv = (double *)malloc((ckt->nMatrixSize+1)*sizeof(double));

    /* initialization */
    memset(theSol, 0, (ckt->nMatrixSize + 1)*sizeof(double));

    fvv = pgComputePenaltyCostFunct(ckt);
    pgGetVoltageVector(ckt,vv);

    for( its = 1; its <= ITMAX; its++){ 
        *iter = its;
        spClear(theHessian);

        /* initialization */
        memset(theSol, 0, (ckt->nMatrixSize + 1)*sizeof(double));
    
        pgBuildHessianMatrix(ckt);    
        pgCostFuncGradient(ckt,grad);
    
        for(i = 1; i <= ckt->nMatrixSize; i++){
        theRhs[i] = -grad[i];    
        }
    
        /* LU decomposition */
        if( spFactor(theHessian) != spOKAY ){
        error_mesg(INT_ERROR,"Matrix factorization error.");
        return;
        }

        /* the solution is the possible moving direction */
        spSolve(theHessian, theRhs, theSol);

        /* 1-dimentional line search */
        
        pgLineMin(ckt,vv, theSol, ckt->nMatrixSize,fmin);
        if(2.0*fabs(*fmin-fvv) <= ftol*(fabs(*fmin) + fabs(fvv) + EPS)) {
        printf("fmin: %g diff: %g\n",*fmin, *fmin-fvv);
        pgSetVoltageVector(ckt,vv);
        break;
        }
        fvv = *fmin;
        pgSetVoltageVector(ckt,vv);
          printf("\nCOST: %lf\n", pgComputeCostFunct(ckt));
        printf("PENALTY COST: %lf\n", fvv);
    }
    free(theRhs);
    free(theSol);
    free(grad);
    free(vv);
    spDestroy(theHessian);
}

/* 
**    we build the Hessian matrix from
**    the objective function which is
**    used for the computation of the
**    direction.
*/
void
pgBuildHessianMatrix(Circuit *ckt)
{
    Branches *blist;
    double  num, den;
    spREAL    *spElep;
    IntConst *iclist;
    Products  *paux1, *paux2;
    double  value;
    int i, error;

    assert(theHessian);

    /* we go throuht all the resistor branches */
    for(blist = ckt->theDeviceList; blist; blist = blist->next){
        /* skip abmormal branches */
        if(blist->stat != sNormal)
            continue;
        /* we skip the non-resistive branches */
        if(blist->type != dRES)
            continue;
        
        num = blist->lay->unitRes*blist->length*blist->length*
                 blist->current;
        
        den = ckt->theNodeArray[blist->n1].voltage -
            ckt->theNodeArray[blist->n2].voltage;
        den = den*den*den;
        if(den == 0.0){
            /*
                        sprintf(buf,
                        "zero voltage drop between v(%d) and v(%d).",
                        blist->n1, blist->n2);
                        error_mesg(INT_ERROR,buf);
                        */
            continue;    
        }
        value = 2*num/den;

        /* first the diagonal elements */
        spElep = spGetElement(theHessian,blist->n1,blist->n1);
        assert(spElep);
        spADD_REAL_ELEMENT(spElep,value);

        spElep = spGetElement(theHessian,blist->n2,blist->n2);
        assert(spElep);
        spADD_REAL_ELEMENT(spElep,value);

        /* non-diagonal elements */
        spElep = spGetElement(theHessian,blist->n1,blist->n2);
        assert(spElep);
        spADD_REAL_ELEMENT(spElep,(-1)*value);

        spElep = spGetElement(theHessian,blist->n2,blist->n1);
        assert(spElep);
        spADD_REAL_ELEMENT(spElep,(-1)*value);
    }

    /* we go through all the corresponding internal constraints */
    for(iclist = ckt->NLPIntConstList; iclist; iclist = iclist->next){
        den = pgComputeIntConstValue(iclist);
        den = den*den*den;
        if(den == 0.0){
                        /*
                        pgPrintIntConst(stdout,iclist);
                        print_mesg(" becomes zero, ignore!\n");
             
                 */
                        continue;
                }
    
        /* first the diagonal elements */
        paux1 = iclist->prod_list;
        num = PenalWeight*paux1->coeff*paux1->coeff;
        value = 2*num/den;
        spElep = spGetElement(theHessian,paux1->node->index,                                paux1->node->index);
        assert(spElep);
        spADD_REAL_ELEMENT(spElep,value);

        /* then possible non-diagnoal elements */
        if(paux1->next)
            paux2 = paux1->next;
        else
            continue;
        num = PenalWeight*paux1->sign*paux1->coeff*
                  paux2->sign*paux2->coeff;
        value = 2*num/den;
        spElep = spGetElement(theHessian,paux1->node->index,
                         paux2->node->index);
        assert(spElep);
        spADD_REAL_ELEMENT(spElep,value);

        spElep = spGetElement(theHessian,paux2->node->index,
                         paux1->node->index);
        assert(spElep);
        spADD_REAL_ELEMENT(spElep,value);
    }
}
