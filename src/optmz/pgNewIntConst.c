/*
 * $RCSfile: pgNewIntConst.c,v $                                                                   
 * $Revision: 1.1 $
 * $Date: 1999/04/30 17:19:46 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

/*******************************************************************************
*                                                                              *
* Module name:                              Prefix: pg                        *
*                                            Author: XiangDong Tan           *
* Description:                                                                 *
*    Build the internal constraints for non-linear (NLP) 
*    and linear (LP) optimization subproblems.
*******************************************************************************/

#include "math.h"
#include "pgOptmz.h"

#define MIN_CURRENT 1e-10

void
pgBuildBVolLPConst(ExtConst *ext_const)
{
    long      index = 1;
    ExtConst  *etaux;
    IntConst  *itaux;
    Nodes       *naux;
    int      i,sign;
    Products  *paux;
    Branches  *daux, *bch1 = NULL, *bch2=NULL;
    BranchLink *blaux;
    double      coeff, cst;

    assert(ext_const);
    assert(theNodeArray);
    assert(theDeviceList);

    if(NLPIntConstList){
        pgFreeIntConst(NLPIntConstList);
        NLPIntConstList = NULL;
    }

    /* second, we build the minimun width constraints */

    for(daux = theDeviceList; daux; daux = daux->next){

        if(daux->stat != sNormal) 
            continue;

        if(daux->type != dRES) 
            continue;

        /* ignore branch with very small current */
        if(fabs(daux->current) <= MIN_CURRENT){
            sprintf(buf,"branch %s: close to 0, ignored.\n", 
            daux->name);
            print_mesg(buf);
            daux->stat = sAbnormal;
            continue;
        }

        /* check the current direction */
        if(daux->current != 0.0){

            /* first constraint */
            itaux = pgNewIntConst( GE,index++ );
            pgAddIntConst(&NLPIntConstList,itaux);

            paux = pgNewProductByBranch(1, 
                Sign(daux->current), daux);
            pgAddProduct(itaux,paux);

            /* the constant */
            cst = -1/0.01;
            pgAddNumConst(itaux,cst);

            /* second constraint */
            itaux = pgNewIntConst( GE,index++ );
            pgAddIntConst(&NLPIntConstList,itaux);
            
            /* we creat a product terms and a constant */
            /* the product */
            assert(daux->n1 < (nMatrixSize+1));
            assert(daux->n2 < (nMatrixSize+1));

            coeff = daux->lay->unitRes*daux->length*
                daux->current;

            paux = pgNewProductByBranch(1, coeff, daux);
            pgAddProduct(itaux,paux);

            /* the constant */
            cst = -daux->lay->minWidth;
            pgAddNumConst(itaux,cst);
        }
        else{
            sprintf(buf,"%s(Linear): current=0, ignored.\n",
            daux->name);
            print_mesg(buf);
        }
        
        }
}

/*
*/

void
pgPrintBVolLPObjFunc(FILE *fp)
{

    double    cst, vdrop;
    char     name[128];
    Branches *daux;
    int count = 0;

    assert(theDeviceList);

    fprintf(fp,"MIN: ");
    for(daux = theDeviceList; daux; daux = daux->next){


        if(daux->stat != sNormal) 
            continue;

        if(daux->type != dRES) 
            continue;

        cst = daux->current*daux->length*daux->length*
              daux->lay->unitRes;

        if(cst < 0)
            fprintf(fp,"+%1.10g VN_%d ", -cst, daux->index);
        else
            fprintf(fp,"+%1.10g V_%d ", cst, daux->index);

        if(count++%3 == 0)
            fprintf(fp,"\n");
    }
    fprintf(fp,";\n");
}

/*
**    Print the internal constraint in a 
**    form acceptable to lp_solve2.0 system.
*/

void
pgPrintBVolIntConst(FILE *fp, IntConst *icp)
{
    Products *paux;

    assert(fp);
    assert(icp);

    for(paux = icp->prod_list; paux; paux = paux->next)
        pgPrintBVolProduct(fp,paux);
    if(icp->cst > 0)
        fprintf(fp," +%1.10g ",icp->cst);
    else if(icp->cst < 0)
        fprintf(fp," %1.10g ",icp->cst);
    if(icp->type == EQ)
        fprintf(fp," = 0;\n");
    else if(icp->type == GT)
        fprintf(fp," > 0;\n");
    else if(icp->type == GE)
        fprintf(fp," >= 0;\n");
    else if(icp->type == LS)
        fprintf(fp," < 0;\n");
    else if(icp->type == LE)
        fprintf(fp," <= 0;\n");
    else 
        fprintf(fp," ? 0;\n");
}

/*
**    Print the internal constraint list
*/

void
pgPrintAllBVolIntConst(FILE *fp, IntConst *iclist)
{
    Products *paux;
    IntConst *icaux;

    assert(fp);
    assert(iclist);

    fprintf(fp,"/* ### Internal Constraint List --- */\n");
    for(icaux = iclist; icaux; icaux = icaux->next)
        pgPrintBVolIntConst(fp, icaux);
        
    fprintf(fp,"/* ### End of List --- */\n\n");
}
