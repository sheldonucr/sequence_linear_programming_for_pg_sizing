/*
 * $RCSfile: pgNodeBranch.c,v $                                                                   
 * $Revision: 1.3 $
 * $Date: 2000/10/10 01:01:12 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

/*******************************************************************************
*                                                                              *
* Module name:                              Prefix: pg                        *
*                                            Author: XiangDong Tan           *
* Description:                                                                 *
*    Nodes and BranchLink manipulation functions 
*******************************************************************************/

#include "pgCircuit.h"


BranchLink *
pgNewBranchLink(Branches *branch)
{
    BranchLink *baux;
    baux = (BranchLink *)malloc(sizeof(BranchLink));
    assert(baux);
    baux->dev = branch;
    baux->next = NULL; 
    return baux;
}

/* addition is performed at the beginning of the list */
void 
pgAddBranchLink(BranchLink **blist, BranchLink *new)
{
    assert(blist);

    if(!(*blist))
      {
        *blist = new;
	new->next = NULL;
      }
    else{
        new->next = *blist;
        *blist = new;
    }
}

/*
**    We assume the the branch list 
**    has been built.
*/

void
pgBuildNodeArray(Circuit *ckt)
{
    int i;
    Branches *daux;
    BranchLink *dbrch;
    Nodes *nodeArray;

    assert(ckt);

    if(ckt->theNodeArray)
        pgFreeNodeArray(ckt);
    
    /* initialization */
    ckt->theNodeArray = nodeArray =
    (Nodes *)malloc(sizeof(Nodes)*(ckt->nMatrixSize+1));

    for(i=0; i<=ckt->nMatrixSize; i++){
        nodeArray[i].index = i;
	nodeArray[i].visited = 0;
        nodeArray[i].blist = NULL;
        nodeArray[i].stat = nNormal;
        nodeArray[i].isVDD = 0;
        nodeArray[i].num_branch = 0;
        nodeArray[i].voltage = 0.0;
        nodeArray[i].coeff = 0.0;
    }
        
    for(daux = ckt->theDeviceList; daux; daux = daux->next){

        /* mark the node which connects the power supply */
        if(daux->type == dVOL){
	    if(daux->n1) /* one of them must be ground node */
	    	nodeArray[daux->n1].isVDD = 1;
	    else
	    	nodeArray[daux->n2].isVDD = 1;
            /* continue; */
	}

        if(daux->n1){
            dbrch = pgNewBranchLink(daux);
            pgAddBranchLink(&nodeArray[daux->n1].blist,dbrch); 
            nodeArray[daux->n1].num_branch++;
        }
        if(daux->n2){
            dbrch = pgNewBranchLink(daux);
            pgAddBranchLink(&nodeArray[daux->n2].blist,dbrch); 
            nodeArray[daux->n2].num_branch++;
        }
    }

    /* delete all the dangling node and related branches */
    pgCheckDanglingNode(ckt);

    /* build the quick refence of branches */
    pgBuildBranchQuickIndex(ckt);
}

/*
**    free the node array
*/
void
pgFreeNodeArray(Circuit *ckt)
{
    int i;

    assert(ckt);

    if(!ckt->theNodeArray)
        return;
    
    for(i = 0; i<= ckt->nMatrixSize;  i++){
        if(ckt->theNodeArray[i].blist)
            pgFreeBranchLink(ckt->theNodeArray[i].blist);
    }
    free(ckt->theNodeArray);
}

/*
**
*/
void
pgFreeBranchLink(BranchLink *bl)
{
    if(!bl)
        return;
    if(bl->next)
        pgFreeBranchLink(bl->next);
    free(bl);
}

/*
**    check the dangling nodes (only connect one branch)
**    we also mark the corresponding branch as "abnormal"
**    We assume that node array and its linked branch list
**    have been built.
*/
void
pgCheckDanglingNode(Circuit *ckt)
{
    long i;
    Branches *baux;
    int ncount = 0, bcount = 0;
    
    Nodes *nodeArray = ckt->theNodeArray;
    
    assert(ckt);

    for(i=0; i <= ckt->nNumNode; i++)
      {
	if(nodeArray[i].num_branch <= 1)
	  {
            nodeArray[i].stat = nDangling;
	    ncount++;
            /* printf("dangling node: %ld eliminated\n",i); */
            if(nodeArray[i].blist)
	      {
                baux = nodeArray[i].blist->dev;
                baux->stat = sAbnormal;
                printf("dangling branch: %s eliminated\n",
		       baux->name);
		bcount++;
	      }
	  }
      }
    printf("** # dangling nodes deleted: %d\n", ncount);
    printf("** # dangling branches deletes: %d\n", bcount);
}
