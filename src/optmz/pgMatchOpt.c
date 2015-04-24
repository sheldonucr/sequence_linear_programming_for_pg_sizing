/*
 * $RCSfile: pgMatchOpt.c,v $                                                                   
 * $Revision: 1.1 $
 * $Date: 1999/04/30 17:19:46 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

/*******************************************************************************
*                                                                              *
* Module name:                              Prefix: pg                        *
*                                            Author: XiangDong Tan           *
* Description:                                                                 *
*    routines for P/G optimization under match constraints.
*******************************************************************************/

#include "pgOptmz.h"
#include "pgMatchOpt.h"
#include "pgSens.h"

#define TOL 0.01
#define FLT_MAX    1e300

void pgMatchSetOpt()
{
    assert(theDeviceList);

    /* first build the matchset list */
    pgBuildMatchList(theExtConstList);

    /* print out for checking */
    pgPrintMatchList(stdout,theMatchList);

    /* perform the optimization */
    pgOptmzAllMatch();

    /* compute the width value of each branch */
    pgCompWidthFromRes();
}

/*
**    Optimize all the matchsets
**    We assume the existance of 
**    NLPIntConstList -- non linear 
**    internal constraints.
*/

extern void pgOptmzAllMatch()
{
    MatchList *mlist;

    assert(NLPIntConstList);

    if(!theMatchList)
        return;

    for(mlist = theMatchList; mlist; mlist = mlist->next){
        pgCompMatchSetBound(mlist->mset);
        pgOptmzMatchSet(mlist->mset,TOL);    
    }
        
}

/*
**    Perform the MatchSet optimization
*/
void pgOptmzMatchSet(MatchSet *mset, double tol)
{
    double ub, lb, cval;
    MatchSet *maux;

    if(!mset)
        return;
    
    ub = mset->ub;
    lb = mset->lb;
    if(ub == lb){
        print_mesg("Upper bound and lower bound are same.\n"); 
        print_mesg("No optimization is needed.\n");
        return;
    }
        
    cval = (mset->ub + mset->lb)/2;

    while( (ub - lb) > tol*(cval) ){
        cval = (ub + lb)/2;
        printf("lb: %f, c: %f, ub:%f\n",lb, cval, ub);
        pgUpdateMatchValue(mset, cval);
        pgDCAnalysis();
        pgBuildNLPConst(theExtConstList);

        if(!pgCheckAllIntConst(NLPIntConstList))
            ub = cval;    
        else
            lb = cval;
    }
    /*
    pgPrintBranchState();
    */
    
    /*
    for(maux = mset; maux; maux = maux->next){
        step = pgMatchSetMaxStep(maux);        
        
        while(step > MIN_STEP && step < mset->ub ){
            pgUpdateMatchValue(mset, step);
            break;
        }
    }
    */
}

/*
**    Build the MatchSet list 
*/
void
pgBuildMatchList(ExtConst *elist)
{
    ExtConst  *etaux;
    MatchList *mlist, *mlist1, *mlist2;
    MatchSet  *maux; 
    Branches  *bch1, *bch2;

    assert(elist);

    if(theMatchList){
        pgFreeMatchList(theMatchList);
        theMatchList = NULL;
    }

    for( etaux = elist; etaux; etaux = etaux->next ){
        if(etaux->topic == cWIDTH && etaux->type == EQ){

            assert(etaux->bname1);
            bch1 = bch2 = NULL;
            bch1 = pgFindBranchByName(etaux->bname1);
            if(!bch1){
                 sprintf(buf,"Can not find branch: %s",
                 etaux->bname1);
                 error_mesg(PARSE_ERROR,buf);
                 continue;
            }

            assert(etaux->bname2);
            bch2 = pgFindBranchByName(etaux->bname2);
            if(!bch2){
            	sprintf(buf,"Can not find branch: %s",
             	etaux->bname2);
             	error_mesg(PARSE_ERROR,buf);
                continue;
             }

            mlist1 = mlist2 = NULL;
            mlist1 = pgFindMatchList(bch1);
            mlist2 = pgFindMatchList(bch2);
            
            if(!mlist1 && !mlist2){
                mlist = pgNewMatchList();
                pgAddMatchList(&theMatchList,mlist);
            }

            if(!mlist1 && mlist2 ){
                maux = pgNewMatch(bch1);
                pgAddMatch(&mlist2->mset, maux);
            }
            else if(mlist1 && !mlist2){
                maux = pgNewMatch(bch2);
                pgAddMatch(&mlist1->mset, maux);
            }
            else{
                maux = pgNewMatch(bch1);
                pgAddMatch(&mlist->mset, maux);
                maux = pgNewMatch(bch2);
                pgAddMatch(&mlist->mset, maux);
            }
                
        }
    }
}


/* 
**    Find the branches if it is in already in the 
**    matchset list.
*/
MatchList *pgFindMatchList(Branches *bch)
{
    MatchList *mlist;
    MatchSet  *mset;

    if(!bch)    
        return NULL;
    
    if(!theMatchList)
        return NULL;
    
    for(mlist = theMatchList; mlist; mlist = mlist->next){
        for(mset = mlist->mset; mset; mset = mset->next){
            if(!strcmp(mset->branch->name, bch->name))
                return mlist;
        }
    }
    return NULL;
}

/*
**    Print out all the match for each matchset
*/
void pgPrintMatchList(FILE *fp, MatchList *mlist)
{

    MatchList *maux;
    MatchSet  *mset;

    assert(fp);
    if(!mlist)
        return;

    for(maux = mlist; maux; maux = maux->next){
        fprintf(fp,"match set: \n");
        for(mset = maux->mset; mset; mset = mset->next)
            fprintf(fp,"branch: %s\n",mset->branch->name);
        fprintf(fp,"\n");
    }
}

/*
**    Create a new MatchList
*/
MatchList *pgNewMatchList()
{
    MatchList *mlist;

    mlist = (MatchList *)malloc(sizeof(MatchList));
    assert(mlist);
    mlist->next = NULL;
    mlist->mset = NULL;
    mlist->num = 0;
    return mlist;
}
    
/*
**     Add a MatchSet into a MatchSet List
**    The addition is performed at the beginning
**    of the linked list.
*/

void
pgAddMatchList(MatchList **list, MatchList *new)
{

    assert(list);
    assert(new);

    if(!*list){
        *list = new;
        return;
    }

    new->next = *list;
    *list = new;
}

/*
**    Free the MatchList
*/
void pgFreeMatchList(MatchList *mlist)
{
    if(!mlist)
        return;
    if(mlist->next)
        pgFreeMatchList(mlist->next);
    
    if(mlist->mset)
        pgFreeMatchSet(mlist->mset);
    free(mlist);
}


/************************************************************/
/************************************************************/
/*
** Create a new match
*/
MatchSet *pgNewMatch(Branches *bch)
{
    MatchSet *mset;

    if(!bch)
        return NULL;
    
    mset = (MatchSet *)malloc(sizeof(MatchSet));
    assert(mset);

    mset->branch = bch;
    mset->next = NULL;
    mset->ub = 0;
    mset->lb = 0;
    return mset;
}

/*
**    Free a linked  MatchSet
*/

void pgFreeMatchSet(MatchSet *mset)
{
    if(!mset)
        return;
    if(mset->next)
        pgFreeMatchSet(mset->next);
    
    free(mset);
}

/*
**    Add a match into a MatchSet
**    The addition is performed at the end
**    of the linked list.
*/

void pgAddMatch(MatchSet **list, MatchSet *new)
{
    assert(list);
    assert(new);

    if(!*list){
        *list = new;
        return;
    }

    new->next = *list;
    *list = new;
}

/*
**    Find the maximum and minimum resistance value
**    as the lower bound and upper bound.
*/

void pgCompMatchSetBound(MatchSet *list)
{
    MatchSet *mset;
    double     max, min;

    assert(list);

    max = 0;
    min = FLT_MAX;

    for(mset = list; mset; mset = mset->next){
        if(mset->branch->value < min)
            min = mset->branch->value;
        if(mset->branch->value > max)
            max = mset->branch->value;
    }
    list->ub = max;
    list->lb = min;
    printf("ub: %lf, lb: %lf \n", max, min);
}

/*
**    Set the resistance value to the given for 
**     the branches in a match set.
*/
void pgSetResistValue(MatchSet *list, double value)
{
    MatchSet *mset;

    assert(list);

    if(value > list->ub){
        sprintf(buf,"value; %f is bigger than the upper bound.",value);
        error_mesg(INT_ERROR,buf);
    }
    if(value < list->lb){
        sprintf(buf,"value; %f is maller than the lower bound.",value);
        error_mesg(INT_ERROR,buf);
    }

    for(mset = list; mset; mset = mset->next)
        mset->branch->value = value;

}

/************************************************************/
/************************************************************/



/*
**    Compute the maximum feasible step for
**     given match set and return the step
**    found.
**    We use sensitivity obtain by
**    adjont method to predict the possible
**    step.
*/


double
pgMatchSetMaxStep(MatchSet *mset)
{
    int i;
    double    min_step = FLT_MAX;
    double    vbudget;    /* voltage drop budegt allowed */
    double  step, sens; 
    ExtConst *etaux;

    assert(mset);
    assert(theDeviceList);
    assert(theNodeArray);

    /*we unused memory */
    if(theVolSenList)
        pgFreeVolSensList(theVolSenList);

    theVolSenList = NULL;

    /* build the MNA matrix 
    ** global variable theMatrix, theRhs, theSol are alloced here 
    */
    if( pgBuildMNAEquation(theDeviceList) == -1)
        return;

    /* print the matrix in asiic form */
    spPrint(theMatrix, 0, 1, 1);

    /* LU decomposition */
    if( spFactor(theMatrix) != spOKAY ){
        error_mesg(INT_ERROR,"Matrix factorization error.");
        return;
    }

    /* first we solve for original soluation  */
    spSolve(theMatrix, theRhs, theSol);

    orgSol = (double *)malloc((nMatrixSize + 1) * sizeof(double));

    for( i = 1; i <= nMatrixSize; i++)
        orgSol[i] = theSol[i];

    /* we go throuth all the nodes constrained and
       solve for adjoint circuit 
    */
    /* first we need to build the internal constraint list */

    for( etaux = theExtConstList; etaux; etaux = etaux->next ){
        if( etaux->topic == cVOL ){
        if(etaux->type == GE || etaux->type == GT){
            vbudget =  theNodeArray[etaux->node1].voltage -
                  etaux->value;
            if(vbudget < 0){
                sprintf(buf, 
                    "Voltage in node %d is violated!.");
                error_mesg(IO_ERROR, buf);
                continue;
            }
        }
        if(etaux->type == LE || etaux->type == LS){
            vbudget =  etaux->value - 
                theNodeArray[etaux->node1].voltage;
            if(vbudget < 0){
                sprintf(buf, 
                    "Voltage in node %d is violated!.");
                error_mesg(IO_ERROR, buf);
                continue;
            }
        }
        pgComputeNodeSens(etaux->node1);
        sens = pgMatchSetSens(mset, etaux->node1);

        /* such caculation is only valid for 
           ground network!
        */
        step = vbudget/(sens + FTINY);

        if(step < min_step)
            min_step = step;
        }
    }

    /* release all the memory */
    spDestroy(theMatrix);
    free(theRhs);
    free(theSol);
    free(orgSol);

    return min_step;
}

/*
**    Compute the sensitivity of a node voltage wrt 
**    given MatchSet variable.
*/

double
pgMatchSetSens( MatchSet *mset, int node)
{
    VOL_SENS *vaux;
    DEV_SENS *daux;
    MatchSet *maux;
    double sens = 0;
    
    assert(theVolSenList);

    if(!mset)
        return 0;

    for(vaux = theVolSenList; vaux; vaux = vaux->next){
                if(vaux->node == node)
                        break;
        }

    for(daux = vaux->dslist; daux; daux = daux->next){
        for(maux = mset; maux; maux = maux->next){
            if(!strcmp(mset->branch->name,daux->pdev->name))
                sens += daux->sens;
        }
    }
    return sens;
}

/*
**    Update the all the value of branches
**    in match set.
*/
void pgUpdateMatchValue( MatchSet *mset, double value )
{
    MatchSet *maux;
    for(maux = mset; maux ; maux = maux->next){
        maux->branch->value = value;
    }
}
