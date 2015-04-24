/*
 * $RCSfile: pgProduct.c,v $                                                                   
 * $Revision: 1.1 $
 * $Date: 1999/04/30 17:19:46 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

/*******************************************************************************
*                                                                              *
* Module name:                              Prefix: pg                        *
*                                            Author: XiangDong Tan           *
* Description:                                                                 *
*                                                                              *
*      Routines related to products in internal constraint.
*******************************************************************************/
 
#include "pgOptmz.h"

/*
**    Create a new product with branch current as variable
*/
Products *pgNewProductByBranch(int sign, double coeff, Branches *bch)
{
    Products *paux;
    assert(bch);

    paux = (Products *)malloc(sizeof(Products));
    assert(paux);

    paux->sign = sign;
    paux->coeff = coeff;
    paux->branch = bch;
    paux->node = NULL;
    paux->next = NULL;
    return paux;
}

/*
**    Create a new product with node voltage as variable
*/
Products *pgNewProductByNode(int sign, double coeff, Nodes *np)
{
    Products *paux;
    assert(np);

    paux = (Products *)malloc(sizeof(Products));
    assert(paux);

    paux->sign = sign;
    paux->coeff = coeff;
    paux->branch = NULL;
    paux->node = np;
    paux->next = NULL;
    return paux;
}

/*
**    Release the product list in a recursive way.
*/
void pgFreeProduct(Products *pp)
{
    if(!pp)
        return;

    if(pp->next)
        pgFreeProduct(pp->next);
    free(pp);
}

/*
**    Print a product for lp_solve2.0 system
**
**      Note that all the varaibles are required postive
**      in lp_solver, so we need to adjust the signs of
**      corresponing coefficients of these variables.
*/

void
pgPrintProduct( FILE *fp, Products *pp)
{
    char    name[128];
    double  coeff;
    assert(fp);
    assert(pp);
    
    if(pp->branch){
        if(pp->branch->current < 0){
            coeff = -1*pp->sign*pp->coeff; 
            sprintf(name,"IN_%d",pp->branch->index);
        }
        else{
            coeff = pp->sign*pp->coeff;
            sprintf(name,"I_%d",pp->branch->index);
        }

        if(coeff >=0 )
            fprintf(fp, "+%1.10g %s ", coeff, name);
        else
            fprintf(fp, "%1.10g %s ", coeff, name);
    }
    else if(pp->node){
        if(pp->sign*pp->coeff >=0){
            fprintf(fp,"+%1.10g V_%d ",
                pp->sign*pp->coeff, pp->node->index);
        }
        else{
            fprintf(fp,"%1.10g V_%d ",
                pp->sign*pp->coeff, pp->node->index);
        }
    }
}

/*
**    Print a product for lp_solve2.0 system
**
*/

void
pgPrintBVolProduct( FILE *fp, Products *pp)
{
    char    name[128];
    double  coeff;
    assert(fp);
    assert(pp);
    
    if(pp->branch){
        if(pp->branch->current < 0){
            coeff = -1*pp->sign*pp->coeff; 
            sprintf(name,"VN_%d",pp->branch->index);
        }
        else{
            coeff = pp->sign*pp->coeff;
            sprintf(name,"V_%d",pp->branch->index);
        }

        if(coeff >=0 )
            fprintf(fp, "+%1.10g %s ", coeff, name);
        else
            fprintf(fp, "%1.10g %s ", coeff, name);
    }
}
