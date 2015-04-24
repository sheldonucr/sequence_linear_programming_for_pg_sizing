/*
 * $RCSfile: pgSens.c,v $
 * $Revision: 1.2 $
 * $Date: 2000/10/10 00:57:40 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */


/************************************************************

Parse a spice file and build the corresponding MNA matrix

************************************************************/

#include "pgCircuit.h"
#include "pgSens.h"

/* 
**    This function read the spice netlist and 
**    build the corresponding MNA matrix and solve it.
*/

/* some global variable definitions */

unsigned short theSize;

/*********************************/

/*********************************/

/* 
**    Adjoint method is used to calculate the 
**    incremental sensitivity.
*/


void
pgDCSensAnalysis(Circuit *ckt)
{
    int i;

    assert(ckt->theDeviceList);

    /* free previously used memory */
    if(theVolSenList)
        pgFreeVolSensList(theVolSenList);
    theVolSenList = NULL;

    /* build the MNA matrix */
    if( pgBuildMNAEquation(ckt) == -1)
        return;

    /* print the matrix in asiic form */
    /*
    spPrint(theMatrix, 0, 1, 1);
    */

    /* LU decomposition */
    if( spFactor(theMatrix) != spOKAY ){
        error_mesg(INT_ERROR,"Matrix factorization error.");
        return;
    }

    /* first we solve for original soluation  */
    spSolve(theMatrix, theRhs, theSol);

    orgSol = (double *)malloc((ckt->nMatrixSize + 1) * sizeof(double));
    for( i = 1; i <= ckt->nMatrixSize; i++)
        orgSol[i] = theSol[i];

    /* then we solve for adjoint circuit */

    for( i = 1; i <= ckt->nMatrixSize; i++ ){
        pgComputeNodeSens(ckt, i);
        pgPrintNodeSens(i);
    }

    /* release all the memory */
    /*pgFreeVolSensList(theVolSenList);*/
    spDestroy(theMatrix);
    free(theRhs);
    free(theSol);
    free(orgSol);
}


/* 
**     Calculate the given node voltage sensitivity w.r.t to each
**    circuit devices.
*/

void 
pgComputeNodeSens(Circuit *ckt, int node)
{

    int i,j;
    double *adjSol;
    double **sens_matrix;

    memset(theRhs, 0, (ckt->nMatrixSize + 1)*sizeof(double));
    memset(theSol, 0, (ckt->nMatrixSize + 1)*sizeof(double));

    theRhs[node] = 1;

    spSolveTransposed(theMatrix, theRhs, theSol);

    adjSol = (double *)malloc((ckt->nMatrixSize + 1) * sizeof(double));
    for( i = 1; i <= ckt->nMatrixSize; i++)
        adjSol[i] = theSol[i];

    /* we create the senstivtity matrix */
    sens_matrix = (double **)malloc((ckt->nMatrixSize+1)*sizeof(double *));
    for(i=1; i <= ckt->nMatrixSize; i++){
        sens_matrix[i] = (double *)malloc((ckt->nMatrixSize+1)*sizeof(double));
        for(j=1; j <= ckt->nMatrixSize; j++)
            sens_matrix[i][j] = -(adjSol[i]*orgSol[j]);
    }

    /* computer all sensitivity of devices for the given node */
    pgComputeDeviceSens(ckt, node, sens_matrix);

    /* free the memory */
    for( i = 0; i <= ckt->nMatrixSize; i++)
        free(sens_matrix[i]);
    free(sens_matrix);
    free(adjSol);
}

/* 
**    Calculate the sensitivity w.r.t each circuit device.
**     the build-in derivate chain rule for 1/r is used.
**    i.e (1/r)' = -1/r^2.
*/
void
pgComputeDeviceSens(Circuit *ckt, int node, double **sens_matrix)
{
    Branches *baux;
    DEV_SENS *pdev_sens;
    VOL_SENS *pvol_sens;

    assert(sens_matrix);    

    pvol_sens = (VOL_SENS *)malloc(sizeof(VOL_SENS));
    assert(pvol_sens);
    pvol_sens->dslist = NULL;
    pvol_sens->next = NULL;
    pvol_sens->node = node;
    pgAddVolSens(pvol_sens);
    
    for(baux = ckt->theDeviceList; baux; baux = baux->next){

        /* ignore no resistor device */
        if(baux->stat != sNormal)
            continue;
        if(baux->type != dRES)
            continue;

        pdev_sens = (DEV_SENS *)malloc(sizeof(DEV_SENS));        
        assert(pdev_sens);
        pdev_sens->pdev = baux;
        pdev_sens->sens = 0.0;
        pdev_sens->next = NULL;
        pgAddDevSens(pvol_sens,pdev_sens);

        if(baux->n1 != 0)
            pdev_sens->sens -= +(1/(baux->value*baux->value)*
            sens_matrix[baux->n1][baux->n1]);
        if(baux->n2 != 0)
            pdev_sens->sens -= (1/(baux->value*baux->value))*
            sens_matrix[baux->n2][baux->n2];
        if((baux->n1 !=0) && (baux->n2 != 0)){
            pdev_sens->sens += (1/(baux->value*baux->value))*
            sens_matrix[baux->n1][baux->n2];
            pdev_sens->sens += (1/(baux->value*baux->value))*
            sens_matrix[baux->n2][baux->n1];
        }
    }
}
        

/*
**    free the voltage-sensitivty list
*/

void pgFreeVolSensList(VOL_SENS *vslist)
{
    if(!vslist)
        return;
    if(vslist->next)
        pgFreeVolSensList(vslist->next);
    
    if(vslist->dslist)
        pgFreeDevSensList(vslist->dslist);
    free(vslist);
}

/*
**    free the device-sensivity list
*/
void pgFreeDevSensList(DEV_SENS *dslist)
{
    if(!dslist)
        return;
    if(dslist->next)
        pgFreeDevSensList(dslist->next);
    free(dslist);
}

/*
**    Add the vol-sensitivity into 
**    a glabol list.
**    Addition is performed at the 
**    beginning of the list.
*/

void
pgAddVolSens(VOL_SENS *pvol_sens)
{
    if(!theVolSenList)
        theVolSenList = pvol_sens;
    else{
        pvol_sens->next = theVolSenList;
        theVolSenList = pvol_sens;
    }
}
            
/*
**    Add the device-sensitivity into 
**    a  list given.
**    Addition is performed at the beginning of
**    the list.
*/

void
pgAddDevSens(VOL_SENS *pvol_sens, DEV_SENS *pdev_sens)
{
    VOL_SENS *vaux;
    DEV_SENS *daux;

    assert(pvol_sens);

    if(!pvol_sens->dslist)
        pvol_sens->dslist = pdev_sens;
    else{
        pdev_sens->next = pvol_sens->dslist;
        pvol_sens->dslist = pdev_sens;
    }
}

/*
** Print out the node sensitivity wrt all
** the circuit device.
*/

void
pgPrintNodeSens( int node )
{
    VOL_SENS *vaux;
    DEV_SENS *daux;

    assert(theVolSenList);

    for(vaux = theVolSenList; vaux; vaux = vaux->next){
        if(vaux->node == node)
            break;
    }
    if(!vaux){
        sprintf(buf,"No sensistivity info for node: %d.\n",node);
        error_mesg(INT_ERROR,buf);
        return;
    }

    printf("Sensititvity of v[%d] w.r.t \n",node);
    for(daux = vaux->dslist; daux; daux = daux->next){
        printf("%s = %e\n",daux->pdev->name, daux->sens);
    }
    printf("\n");
}

