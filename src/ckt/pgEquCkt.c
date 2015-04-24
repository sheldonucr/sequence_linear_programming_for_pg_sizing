/*
 * $RCSfile: pgEquCkt.c,v $
 * $Revision: 1.2 $
 * $Date: 2000/10/10 01:09:34 $, (c) Copyright 1999, 2000 by X.-D. Sheldon Tan
 */


/************************************************************
Build the equivalent circuit from the original circuits
************************************************************/

#include "pgCircuit.h"
#include "pgEquCkt.h"
#include "pgOptmz.h"

void
pgOptimizeEqnCircuit(Circuit *oldCkt)
{
  
	Circuit *ckt;
	char *name, *str;
	char buf[128];
	double time2, time1 = 1.0 * clock() / 1000000;

	/* create a new circuit */
	theEquCkt = ckt = (Circuit *)malloc(sizeof(Circuit));

	ckt->name = CopyStr(oldCkt->name);
	ckt->theDeviceList = NULL;
	ckt->theNodeArray = NULL;
	ckt->theBranchArray = NULL;
	ckt->theExtConstList = NULL;
	ckt->LPIntConstList = NULL;
	ckt->NLPIntConstList = NULL;

	ckt->nNumDevice = 0;
	ckt->nNumBranch = 0;
	ckt->nNumNode = 0;
	ckt->nMatrixSize = 0;

  
	pgConstructEquCkt(oldCkt, ckt);
	time2 = 1.0 * clock() / 1000000;
	printf("** time for constructing equckt time: %g\n",time2 - time1);

	pgPrintCktStatistic(ckt);

  
	if(!ckt->theNodeArray)
		pgBuildNodeArray(ckt);
  
	/* perform the DC analysis on the original circuit */

	pgComputeStateFromRes(ckt);

	/* read the constraint file */
  
	name = CopyStr(oldCkt->name);
	str = strchr(name,'.');
	if(!str)
    {
		sprintf(buf,"%s.cs", name);
    }
	else
    {
		*str = 0;
		sprintf(buf,"%s.cs", name);
    }
	pgReadConstFile(ckt,buf);
  
	/* perform the optimization */
	pgMainLoop(ckt, optSLP);

	/* back solving for the original circuit */
	pgBackSolveOriginalCkt(oldCkt, ckt);
  
	/* get the new width for each segment wire */
	/* pgCompWidthFromRes(oldCkt);*/
}
/*
** Construct the equivalent ckt. We assume the DC analysis
** has been performed on the original circuit and we know
** the branch currents of each resistant branch.
*/ 

void
pgConstructEquCkt(Circuit *ckt, Circuit *newCkt) /* equivalent circuit nodes */
{
	Nodes *node;
	int i;
	BranchLink *blaux;
	Branches *baux;
	int numNodeRemoved = 0, numNodeRemained = 0;

	assert(ckt);
	assert(newCkt);

	/* build the node array of original circuit if necessary */
	if(!ckt->theNodeArray)
		pgBuildNodeArray(ckt);
  
	/* perform the DC analysis on the original circuit */
	/*pgDCAnalysis(ckt);*/
	pgComputeStateFromRes(ckt);

	/* preparation */
	markAllNodeUnvisited(ckt);

	/* we then go through all the nodes */
	for(i = 0; i <= ckt->nNumNode; i++)
    {
		node = &(ckt->theNodeArray[i]);

		/* skip the node visited */
		if(node->visited)
			continue;

		/* ignore node that needs to be suppressed.  Such a node has two
		   resistant branch and one current.  The current is coming from
		   one branch and go out to another branch.  
		*/
		if(isSuppressedNode(ckt, node->index))
			continue;

		/* node to be kept in the new netlist. Note that we only process
		   resistant branch which has current flowing away from the
		   branch */
		node->visited = 1;
		numNodeRemained++;

		for(blaux = node->blist; blaux; blaux = blaux->next)
		{
			baux = blaux->dev;

			if(baux->type != dRES)
			{
				copyBranch(baux, newCkt);
				continue;
			}

			if(((baux->n1 == node->index) && (baux->current > 0.0f)) ||
			   ((baux->n2 == node->index) && (baux->current < 0.0f)))
			{					       

				if(baux->n1 == node->index)
				{
					if(!isSuppressedNode(ckt,baux->n2))
						copyBranch(baux, newCkt);
					else
						numNodeRemoved += suppressBranches(baux, ckt, newCkt);
				}
				else
				{
					if(!isSuppressedNode(ckt, baux->n1))
						copyBranch(baux, newCkt);
					else
						numNodeRemoved += suppressBranches(baux,ckt, newCkt);
				}
			}
		}
      
    }
	printf("# of nodes suppressed: %d\n", numNodeRemoved);
	printf("# of nodes remains: %d\n", numNodeRemained-1);
	/* newCkt->nNumDevice = ckt->nNumDevice - numNodeRemoved;*/
	newCkt->nMatrixSize = ckt->nMatrixSize;
	newCkt->nNumNode = ckt->nNumNode;
}

int
isSuppressedNode(Circuit *ckt, int idx)
{
	BranchLink *blaux;
	Branches* baux;
	int res_ct = 0, cur_ct = 0;
	Nodes *node = &(ckt->theNodeArray[idx]);

	/* 
	   baux1 is the branch that current flow into the node 
	   baux2 is the branch that currnet flow away from the node 
	*/
	Branches *baux1 = NULL, *baux2 = NULL;

	assert(ckt);
	assert(node);

	/* must have three branches */
	if(node->num_branch != 3)
		return 0;

	/* must have two resistant branchs and one current branch */
  
	for(blaux = node->blist; blaux; blaux = blaux->next)
    {
		baux = blaux->dev;
		if(baux->type == dRES)
		{
			res_ct++;
			if(((baux->n1 == node->index) && (baux->current > 0.0f)) ||
			   ((baux->n2 == node->index) && (baux->current < 0.0f)) )
				baux2 = baux;

			else if (((baux->n1 == node->index) && (baux->current < 0.0f)) ||
					 ((baux->n2 == node->index) && (baux->current > 0.0f)) )   
				baux1 = baux;
		}
		else if(baux->type == dCUR)
			cur_ct++;
		else
			return 0;
    }
  
	if((res_ct != 2) || cur_ct != 1)
		return 0;

	if(!baux1 || !baux2)
		return 0;
  
	return 1;
}

void
markAllNodeUnvisited(Circuit *ckt)
{
	int i;
	Nodes *nodeArray = ckt->theNodeArray;
 
	assert(ckt);
	if(!nodeArray)
		return;

	for(i=0; i <= ckt->nNumNode; i++)
	{
		nodeArray[i].visited = 0;
	}
}

void
copyBranch(Branches *bch, Circuit *ckt)
{
	Branches* daux;
	BranchLink *bchlist = NULL, *blaux;

	assert(bch);
	assert(ckt);
  
	daux = (Branches *)malloc(sizeof(Branches));
	daux->name = CopyStr(bch->name);
	daux->n1 = bch->n1;
	daux->n2 = bch->n2;
	daux->type = bch->type;
	daux->sign = bch->sign;
	daux->current = bch->current;
	daux->value = bch->value;
	daux->stat = bch->stat;
	daux->length = bch->length;
	daux->width = bch->width;
	daux->vdrop = bch->vdrop;
	daux->layer = bch->layer;
	daux->lay = bch->lay;
	daux->pI = bch->pI;
	daux->nI = bch->nI;
	daux->wmin = bch->wmin;
	daux->vn = bch->vn;
	daux->next = NULL;

	blaux = pgNewBranchLink(bch);
	pgAddBranchLink(&bchlist,blaux);
	daux->origBranches  = bchlist;
	daux->curN1 = NULL;
	daux->curN2 = NULL;
	daux->oldRs = 0.0f;

	if(bch->type == dRES)
		daux->index = ++ckt->nNumDevice;

	pgAddDevice(ckt,daux);
}

/* suppress the branchs and put the new generated branchs into the
** given circuit. It turns the number of nodes suppressed.  */
int
suppressBranches(Branches *bch, Circuit *ckt, Circuit *newCkt)
{
	BranchLink *blaux, *blaux1, *bchlist = NULL;
	Nodes *nextNode;
	Branches *baux, *rbaux, *curBch1, *curBch2, *oldLinkBch, *linkBch = bch;
	char buf[128];
	int n1 = -1, n2 = -1; /* the n1 and n2 nodes  in the new branch */
	double eqnResist, eqnLength, current1, current2;
	int numNodeRemoved = 0;

	assert(bch);
	assert(ckt);

	/* the search direction follows that of the branch's current */
	while(linkBch)
    {
		oldLinkBch = linkBch;
		nextNode = NULL;
		if(linkBch->current > 0.0f)
		{
			if(isSuppressedNode(ckt, linkBch->n2))
			{  
				blaux = pgNewBranchLink(linkBch);
				pgAddBranchLink(&bchlist,blaux); 
				nextNode = &(ckt->theNodeArray[linkBch->n2]);
			}
			if(n1 == -1)
				n1 = linkBch->n1;
			n2 = linkBch->n2;
		}
		else if(linkBch->current < 0.0f)
		{
			if(isSuppressedNode(ckt, linkBch->n1))
			{
				blaux = pgNewBranchLink(linkBch);
				pgAddBranchLink(&bchlist,blaux); 
				nextNode = &(ckt->theNodeArray[linkBch->n1]);
	      
			}
			if(n1 == -1)
				n1 = linkBch->n2;
			n2 = linkBch->n1;
		}
		else /* == 0 */
			return;

		if(!nextNode)
		{
			blaux = pgNewBranchLink(linkBch);
			pgAddBranchLink(&bchlist,blaux);
			break;
		}

		numNodeRemoved++;
		assert(!nextNode->visited);

		nextNode->visited = 1;

		for(blaux = nextNode->blist; blaux; blaux = blaux->next)
		{
			if(blaux->dev->type == dCUR)
			{
				blaux1 = pgNewBranchLink(blaux->dev);
				pgAddBranchLink(&bchlist,blaux1);
			}
			else if((blaux->dev->type == dRES) && (blaux->dev != oldLinkBch))
			{
				linkBch = blaux->dev;
			}
		}
    }

	/* create three new branches */
 
	/* first the resistant branch */

	computeEqnPiCircuit(bchlist, &eqnResist,
						&eqnLength, &current1,
						&current2);
 
	rbaux = (Branches *)malloc(sizeof(Branches));
	assert(strlen(bch->name) < 170);
	sprintf(buf, "%s", bch->name);
	/* printf("%s \t", buf); */
	rbaux->name = CopyStr(buf);

	rbaux->n1 = n1;
	rbaux->n2 = n2;
	rbaux->type = dRES;
	rbaux->value = eqnResist;
	rbaux->stat = sNormal;
	rbaux->sign = 1;
	rbaux->current = 0.0;
	rbaux->vdrop = 0.0;
	rbaux->pI = 0.0;
	rbaux->nI = 0.0;
	rbaux->wmin = 0.0;
	rbaux->vn = 0;
	rbaux->layer = bch->layer;
	rbaux->width = bch->width;
	rbaux->length = eqnLength;
	rbaux->index = ++newCkt->nNumDevice;
	rbaux->next = NULL;
	  
	rbaux->origBranches = bchlist;
  
	rbaux->curN1 = NULL;
	rbaux->curN2 = NULL;
	rbaux->oldRs = eqnResist;

	pgAddDevice(newCkt, rbaux);

	/* the first current branches */
  
	curBch1 = curBch2 = NULL;

	if(current1 != 0.0f)
    {
		curBch1 = (Branches *)malloc(sizeof(Branches));     
		assert(curBch1 != NULL);

		sprintf(buf, "i_%s_%d", bch->name,n1);
		/* printf("%s: %g \t", buf, current1); */
		curBch1->name = CopyStr(buf);

		curBch1->n1 = n1;
		curBch1->n2 = 0;
		curBch1->type = dCUR;
		curBch1->sign = 1;
		curBch1->current = 0.0;
		curBch1->value = current1;
		curBch1->stat = sNormal;
		curBch1->pI = 0.0;
		curBch1->nI = 0.0;
		curBch1->wmin = 0.0;
		curBch1->vn = 0;
		curBch1->next = NULL;
	  
		curBch1->origBranches = NULL;
		curBch1->curN1 = NULL;
		curBch1->curN2 = NULL;
		curBch1->oldRs = 0.0f;

		pgAddDevice(newCkt, curBch1);
    }

	/* second the current branch */
	if(current2 != 0.0f)
    {
		curBch2 = (Branches *)malloc(sizeof(Branches));     
		assert(curBch2 != NULL);

		sprintf(buf, "i_%s_%d", bch->name,n2);
		/* printf("%s: %g \n", buf, current2); */
		curBch2->name = CopyStr(buf);

		curBch2->n1 = n2;
		curBch2->n2 = 0;
		curBch2->type = dCUR;
		curBch2->sign = 1;
		curBch2->current = 0.0;
		curBch2->value = current2;
		curBch2->stat = sNormal;
		curBch2->pI = 0.0;
		curBch2->nI = 0.0;
		curBch2->wmin = 0.0;
		curBch2->vn = 0;
		curBch2->next = NULL;
	  
		curBch2->origBranches = NULL;
		curBch2->curN1 = NULL;
		curBch2->curN2 = NULL;
		curBch2->oldRs = 0.0f;

		pgAddDevice(newCkt, curBch2);
    }

	/* make the link in the resistant branch */
	rbaux->curN1 = curBch1;
	rbaux->curN2 = curBch2;

	return numNodeRemoved;
}

void
computeEqnPiCircuit(BranchLink *bchlist,
					double *eqnResist,
					double *eqnLength,
					double *current1,
					double *current2)
{
	BranchLink *blaux;
	Branches *daux;
	double res_n1 = 0.0, res_n2 = 0.0;
	double resist = 0.0, length = 0.0, cur1 = 0.0, cur2 = 0.0;

	if(!bchlist)
		return;

	/* first pass to get total resistant and length */
	for(blaux = bchlist; blaux; blaux = blaux->next)
    {
		if(blaux->dev->type == dRES)
		{
			resist += blaux->dev->value;
			length += blaux->dev->length;
		}
    }
     
	/* second pass to get equivalent current */
	assert(resist != 0.0);

	res_n2 = resist;
	res_n1 = 0;

	/* the first branch in bchlist is touching n2 due to
	   the way to add the branchlink */
	for(blaux = bchlist; blaux; blaux = blaux->next)
    {
		if(blaux->dev->type == dRES)
		{
			res_n2 -= blaux->dev->value;
			res_n1 += blaux->dev->value;
		}

		if(blaux->dev->type == dCUR)
		{
			if(blaux->dev->n1)
			{
				cur2 += res_n2/resist * blaux->dev->value;
				cur1 += res_n1/resist * blaux->dev->value;
			}
			else
			{
				cur2 -= res_n2/resist * blaux->dev->value;
				cur1 -= res_n1/resist * blaux->dev->value;
			}
		}
    }

	*eqnResist = resist;
	*eqnLength = length;

	*current1 = cur1;
	*current2 = cur2;
}


/* back solve for all the branch widthes for the original circuit from
  the equivalent circuit.  */
pgBackSolveOriginalCkt(Circuit *oldCkt, Circuit *ckt)
{
	Branches *baux, *baux1;
	BranchLink *blist;
	double vs, ii, vnext, vi, ie;
	int ankorNode;
  

	assert(oldCkt);
	assert(ckt);

	for(baux = ckt->theDeviceList; baux; baux = baux->next)
    {
		/* check if it is an equivalent serise */
      
		if(baux->origBranches &&  baux->curN1 && baux->curN2)
		{
	 
			/* first copy back the voltage at the both ends of
			   the equivalent ckt */
			oldCkt->theNodeArray[baux->n2].voltage = 
				ckt->theNodeArray[baux->n2].voltage;
			oldCkt->theNodeArray[baux->n1].voltage = 
				ckt->theNodeArray[baux->n1].voltage;
	  
			/* remember, the branches begins from n2 in current super
			   branch */
			ankorNode = baux->n2;
			blist = baux->origBranches;
			vs = ckt->theNodeArray[baux->n2].voltage - 
				ckt->theNodeArray[baux->n1].voltage;
	 
	  
			while(blist)
			{
	      
				vi = oldCkt->theNodeArray[ankorNode].voltage;
	      
				baux1 = blist->next->dev;
				assert(baux1->type == dCUR);

				ie = baux->curN2->value;

				baux->curN2->value -= (baux1->n1)?baux1->value:-baux1->value;

				assert(blist->dev->type == dRES);
				vnext = vi - vs * blist->dev->value / baux->oldRs -
					ie * blist->dev->value*baux->value/baux->oldRs;

				if( blist->dev->n1 == ankorNode)
				{
		  
					oldCkt->theNodeArray[blist->dev->n2].voltage = vnext;

					ankorNode = blist->dev->n2;
				}
				else
				{
					assert(blist->dev->n2 == ankorNode);
		  
					oldCkt->theNodeArray[blist->dev->n1].voltage = vnext;
		  
					ankorNode = blist->dev->n1;
				}

				/* generate the V-I state in the original branch */
				blist->dev->value = blist->dev->value*baux->value/baux->oldRs;
				blist->dev->current = 
					(oldCkt->theNodeArray[blist->dev->n1].voltage -
					 oldCkt->theNodeArray[blist->dev->n2].voltage)/
					blist->dev->value;
				blist->dev->width = baux->width;
	      
				blist = blist->next;
				blist = blist->next;

				if(!blist->next)
				{
					blist->dev->value =
						blist->dev->value*baux->value/baux->oldRs;
					blist->dev->current = 
						(oldCkt->theNodeArray[blist->dev->n1].voltage -
						 oldCkt->theNodeArray[blist->dev->n2].voltage)/
						blist->dev->value;
					blist->dev->width = baux->width;

					break;
				}
			}
		}

		else if(baux->type == dRES)	      	/* copied branch */
		{
			oldCkt->theNodeArray[baux->n2].voltage = 
				ckt->theNodeArray[baux->n2].voltage;
			oldCkt->theNodeArray[baux->n1].voltage = 
				ckt->theNodeArray[baux->n1].voltage;
			/* current */
			baux->origBranches->dev->current = baux->current;
			/* resistant value */
			baux->origBranches->dev->value = baux->value;
	  
			/* width */
			baux->origBranches->dev->width = baux->width;
		}   
    }
}
