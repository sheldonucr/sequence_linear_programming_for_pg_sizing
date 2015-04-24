/*
 * $RCSfile: pgIntConst.c,v $                                                                   
 * $Revision: 1.3 $
 * $Date: 2000/10/10 01:01:12 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

/*******************************************************************************
*                                                                              *
* Module name:                              Prefix: pg                         *
*                                           Author: XiangDong Tan              *
* Description:                                                                 *
*    Build the internal constraints optimizing nodal voltages  
*    and branch currents
*******************************************************************************/

#include "math.h"
#include "pgOptmz.h"

#define MIN_CURRENT 1e-15
#define MIN_VOLTAGE 1e-15
double EPSILON = 1.2; /* for avoiding numerical problem at the boundary */ 


/*  
**    This function constructs the constraints for voltage optimization
**    problem, which have linear constraint and linearized objective
**    function.  We have two type constraints: (1) Vi > Vmin (2) if Ii
**    > 0, Ci + Vi2 - Vi1 >= 0 and Vi1 - Vi2 >= 0 if Ii < 0, Vi1 - Vi2
**    - Ci >= 0 and Vi2 - Vi1 >= 0 and Ci = Resistivity*Li*Ii/Wmin, Li
**    is the length of branch i; Wmin is the minimum width constraint.
**    So we need information about external constraints and node
**    voltages and branch currents.
**
**    We get the informaton from theDeviceList for branch current and
**    from theNodeArray for node voltage.  The current direaction for
**    each branch is from n1 to n2 which is specified in the input
**    netlist.  */

void
pgBuildNLPConst(Circuit *ckt, ExtConst *ext_const)
{
  long      index = 1;
  ExtConst  *etaux;
  IntConst  *itaux;
  Nodes       *naux;
  int      sign;
  Products  *paux;
  Branches  *daux, *bch1 = NULL, *bch2=NULL;
  double      cst;

  assert(ext_const);
  assert(ckt->theNodeArray);
  assert(ckt->theDeviceList);

  if(ckt->NLPIntConstList){
    pgFreeIntConst(ckt->NLPIntConstList);
    ckt->NLPIntConstList = NULL;
  }

  /* 
  ** build all the constraints for minimum width
  ** and also reserve current direction 
  */
  for(daux = ckt->theDeviceList; daux; daux = daux->next)
    {

      if(daux->stat != sNormal) 
	continue;

      if(daux->type != dRES) 
	continue;

      /* ignore branch with very small current */
      /*
      if(fabs(daux->current) <= MIN_CURRENT ||
	 fabs(theNodeArray[daux->n1].voltage -
	      theNodeArray[daux->n2].voltage) <= MIN_VOLTAGE ){
	sprintf(buf,"branch %s: current or voltage drop is close to 0, ignored.\n", 
		daux->name);
	print_mesg(buf);
	daux->stat = sAbnormal;
	continue;
      }
      */

      assert(daux->n1 < (ckt->nMatrixSize+1));
      assert(daux->n2 < (ckt->nMatrixSize+1));

      /* first constraint */
      if(daux->current != 0.0 )
	{
	  itaux = pgNewIntConst(GE,index++);
	  pgAddIntConst(&(ckt->NLPIntConstList),itaux);

	  naux = &(ckt->theNodeArray[daux->n1]);
	  paux = pgNewProductByNode(-1, 1/(daux->current),naux);
	  pgAddProduct(itaux,paux);

	  naux = &(ckt->theNodeArray[daux->n2]);
	  paux = pgNewProductByNode(1, 1/(daux->current),naux);
	  pgAddProduct(itaux,paux);

	  cst = (daux->lay->unitRes*daux->length/
		 (daux->lay->minWidth));
	  pgAddNumConst(itaux,cst);

	  /* second constraint */
	  itaux = pgNewIntConst(GE,index++);
	  pgAddIntConst(&(ckt->NLPIntConstList),itaux);

	  /* second constraint */
	  naux = &(ckt->theNodeArray[daux->n1]);
	  paux = pgNewProductByNode(1,1/daux->current, naux);
	  pgAddProduct(itaux,paux);

	  naux = &(ckt->theNodeArray[daux->n2]);
	  paux = pgNewProductByNode(-1,1/daux->current, naux);
	  pgAddProduct(itaux,paux);

	}
      else{
	sprintf(buf,"%s(nonlinear): current=0, ignored.\n", daux->name);
	print_mesg(buf);
      }

    }
        
  /* the internal constrain related to the user defined
     external constraints.
  */
  for( etaux = ext_const; etaux; etaux = etaux->next )
    {
      if(etaux->topic == cVOL)
	{ /* voltage constraint */    

	  /* create a new internal constraints and
	     add it into the constraint list 
	  */ 

	  assert(etaux->node1 <= ckt->nMatrixSize);

	  /* ignore the abnormal case */
	  if(ckt->theNodeArray[etaux->node1].stat != nNormal)
	    continue;

	  naux = &(ckt->theNodeArray[etaux->node1]);

	  if(etaux->type == GT || etaux->type == GE )
	    {
	      itaux = pgNewIntConst(GE,index++);
	      sign = 1;
	    }
	  else if(etaux->type == LE  || etaux->type == LS)
	    {
	      itaux = pgNewIntConst(GE,index++);
	      sign = -1;
	    }
	  else{
	    sprintf(buf, "Invalid voltage constraint: v(%d).", etaux->node1);
	    error_mesg(PARSE_ERROR,buf);
	    continue;
	  }
	  paux = pgNewProductByNode(sign,1/(etaux->value+FTINY),
				    naux);
	  pgAddProduct(itaux,paux);
	  pgAddNumConst(itaux, (-1)*sign);
	  pgAddIntConst(&(ckt->NLPIntConstList),itaux);
	}

      if(etaux->topic == cWIDTH)
	{
	  /*
	   *  Equivalence-width constraint
	   *  (1/(li*Ii))(Vi1-Vi2) - (1/(lj*Ij))*(Vj1-Vj2) = 0
	   */
	  if(etaux->type == EQ) 
	    {
	      naux = &(ckt->theNodeArray[etaux->node1]);

	      /* we only deal with equival width constraint here */
	      if(etaux->type != EQ )
		{
		  sprintf(buf, "Invalid width constraint: v(%d).", etaux->node1);
		  error_mesg(PARSE_ERROR,buf);
		  continue;
		}

	      itaux = pgNewIntConst(EQ,index++);
	      pgAddIntConst(&(ckt->NLPIntConstList),itaux);

	      daux = pgFindBranchByName(ckt,etaux->bname1);
	      assert(daux);
	      cst = (daux->length*daux->current);
            
	      if(daux->n1)
		{
		  naux = &(ckt->theNodeArray[daux->n1]);
		  paux = pgNewProductByNode(1, 1/cst,naux);
		  pgAddProduct(itaux,paux);
		}

	      if(daux->n2)
		{
		  naux = &(ckt->theNodeArray[daux->n2]);
		  paux = pgNewProductByNode(-1, 1/cst,naux);
		  pgAddProduct(itaux,paux);
		}

	      daux = pgFindBranchByName(ckt,etaux->bname2);
	      assert(daux);
	      cst = (daux->length*daux->current);
            
	      if(daux->n1)
		{
		  naux = &(ckt->theNodeArray[daux->n1]);
		  paux = pgNewProductByNode(-1, 1/cst,naux);
		  pgAddProduct(itaux,paux);
		}

	      if(daux->n2)
		{
		  naux = &(ckt->theNodeArray[daux->n2]);
		  paux = pgNewProductByNode(1, 1/cst,naux);
		  pgAddProduct(itaux,paux);
		}
	    }
	}
    }   
}

/*
 *
 *	Building the constraints for solving
 *	for the nodal voltage by sequence of 
 *	linear programming.
 *	Constraints include:
 *	(1) Voltage IR drops:
 *	    Vi >= Vmin or Vi <= Vmax
 *	(2) Electron-migration:
 *          sign(Ii)*(Vi1-Vi2) <= Resistivity*Li*current_density
 *	(3) Minimum width constraint:
 *	    -(Vi1 - Vi2)/I1 + Resistivity*Ii/Wmin >= 0
 *	(4) Linearization companion constraint:
 *	    Vi1-Vi2)/(xi * (Vi1^0-Vi2^0)) -1 >= 0
 *	(5) Equivalence-width constraint"
 *          1/(Li*Ii)(Vi1-Vi2) - 1/(Lj*Ij)*(Vj1-Vj2) = 0
 *	(6) constant widths:
 *	    Vi1 - Vi2 = Vi1^0 - Vi2^0  
 *
 *	Note that constraint (2), (3) and (4) are not independent. 
 *	Only one of them should come into effect.
 *
 * 	We also compute the cost function here.
 */

void
pgBuildVolLPConst(Circuit *ckt, ExtConst *ext_const)
{
  long      i,index = 1;
  ExtConst  *etaux;
  IntConst  *itaux;
  Nodes     *naux;
  int       sign;
  Products  *paux;
  Branches  *daux, *bch1 = NULL, *bch2=NULL;
  double    cst;
  double    vdrop;
  char      pgiBname[128];

  assert(ext_const);
  assert(ckt->theNodeArray);
  assert(ckt->theDeviceList);

  if(ckt->NLPIntConstList)
    {
      pgFreeIntConst(ckt->NLPIntConstList);
      ckt->NLPIntConstList = NULL;
    }

  for(i=0; i<=ckt->nMatrixSize; i++)
    {
      ckt->theNodeArray[i].coeff = 0.0;
    }

  /*
   * We then begin to build the constraints
   */
  for(daux = ckt->theDeviceList; daux; daux = daux->next)
    {

      if(daux->stat != sNormal) 
	continue;

      if(daux->type != dRES  && daux->type != dVOL) 
	continue;

      /* ignore branch with very small current */
      /*
	if(fabs(daux->current) <= MIN_CURRENT ||
	fabs(ckt->theNodeArray[daux->n1].voltage -
	ckt->theNodeArray[daux->n2].voltage) <= MIN_VOLTAGE )
	{
	sprintf(buf,"branch %s: current or voltage drop is close to 0, ignored.\n", 
	daux->name);
	print_mesg(buf);
	daux->stat = sAbnormal;
	continue;
	}
      */
      /* 
       * For power network, the power pin should be 
       * restricted to the power supply voltage.
       */
      if(daux->type == dVOL)
	{
	  itaux = pgNewIntConst(EQ,index++);
	  pgAddIntConst(&(ckt->NLPIntConstList),itaux);

	  if(daux->n1)
	    naux = &(ckt->theNodeArray[daux->n1]);
	  else
	    naux = &(ckt->theNodeArray[daux->n2]);

	  paux = pgNewProductByNode(1, 1.0,naux);
	  pgAddProduct(itaux,paux);
	  pgAddNumConst(itaux,-daux->value);
	  continue;
	}

      /* ignore branch with very small current */
      assert(daux->n1 < (ckt->nMatrixSize+1));
      assert(daux->n2 < (ckt->nMatrixSize+1));

      daux->vdrop = ckt->theNodeArray[daux->n1].voltage -
	            ckt->theNodeArray[daux->n2].voltage;
      if(fabs(daux->current) <= MIN_CURRENT || fabs(daux->vdrop) <= MIN_VOLTAGE )
	{
	  sprintf(buf,"branch %s: current or voltage close to 0, ignored.\n", 
		  daux->name);
	  print_mesg(buf);
	  daux->stat = sAbnormal;
	  continue;
	}

      /* Compute the cost or objective function: calculate the
       * coefficient of each node voltage in the objective function.  */

      /* daux->vdrop = ckt->theNodeArray[daux->n1].voltage -
	 ckt->theNodeArray[daux->n2].voltage; */

      cst = -(daux->current*daux->length*daux->length*
	      daux->lay->unitRes)/(daux->vdrop*daux->vdrop +FTINY);
        
      ckt->theNodeArray[daux->n1].coeff += cst;
      ckt->theNodeArray[daux->n2].coeff += -cst;

      /* 
       * minimum width constraint:
       *  -(Vi1 - Vi2)/I1 + Resistivity*Ii/Wmin >= 0 
       */

      if(daux->current != 0.0 )
	{
	  itaux = pgNewIntConst(GE,index++);
	  pgAddIntConst(&(ckt->NLPIntConstList),itaux);

	  if(daux->n1)
	    {
	      naux = &(ckt->theNodeArray[daux->n1]);
	      paux = pgNewProductByNode(-1, 1/(daux->current),naux);
	      pgAddProduct(itaux,paux);
	    }

	  if(daux->n2)
	    {
	      naux = &(ckt->theNodeArray[daux->n2]);
	      paux = pgNewProductByNode(1, 1/(daux->current),naux);
	      pgAddProduct(itaux,paux);
	    }

	  cst = (daux->lay->unitRes*daux->length/
		 daux->lay->minWidth);
	  pgAddNumConst(itaux,cst);

	  /* 
	   * Linearizaton companion constraint: 
	   * (Vi1-Vi2)/(xi * (Vi1^0-Vi2^0)) -1 >= 0.
	   */

	  itaux = pgNewIntConst(GE,index++);
	  pgAddIntConst(&(ckt->NLPIntConstList),itaux);

	  if(daux->n1)
	    {
	      naux = &(ckt->theNodeArray[daux->n1]);
	      paux = pgNewProductByNode(1,1/((daux->vdrop+FTINY)*RestrFactor), naux);
	      pgAddProduct(itaux,paux);
	    }

	  if(daux->n2)
	    {
	      naux = &(ckt->theNodeArray[daux->n2]);
	      paux = pgNewProductByNode(-1,1/((daux->vdrop+FTINY)*RestrFactor), naux);
	      pgAddProduct(itaux,paux);
	    }
	  pgAddNumConst(itaux,-1.0);
	}
      else
	{
	  sprintf(buf,"%s(nonlinear): current=0, ignored.\n", daux->name);
	  print_mesg(buf);
	}

    }

  for( etaux = ext_const; etaux; etaux = etaux->next )
    {
      assert(etaux->node1 <= ckt->nMatrixSize);

      /*
       *    Voltage IR drop constraint: 
       *    Vi >= Vmin or Vi <= Vmax
       */
      if(etaux->topic == cVOL)
	{ 

	  /* skip ground node which has 0 index */
	  if(etaux->node1 == 0)
	    continue;

	  /* ignore the abnormal case */
	  if(ckt->theNodeArray[etaux->node1].stat != nNormal)
	    continue;

	  if(ckt->theNodeArray[etaux->node1].coeff == 0.0)
	    continue;

	  naux = &(ckt->theNodeArray[etaux->node1]);

	  if(etaux->type == GT || etaux->type == GE )
	    {
	      itaux = pgNewIntConst(GE,index++);
	      sign = 1;
	    }
	  else if(etaux->type == LE  || etaux->type == LS)
	    {
	      itaux = pgNewIntConst(GE,index++);
	      sign = -1;
	    }
	  else
	    {
	      sprintf(buf, "Invalid voltage constraint: v(%d).", etaux->node1);
	      error_mesg(PARSE_ERROR,buf);
	      continue;
	    }
	  paux = pgNewProductByNode(sign,1/(etaux->value+FTINY), naux);
	  pgAddProduct(itaux,paux);

	  pgAddNumConst(itaux, (-1)*sign);
	  pgAddIntConst(&(ckt->NLPIntConstList),itaux);
	}

      /*
       *  Equivalence-width constraint
       *  1/(li*Ii)(Vi1-Vi2) - 1/(lj*Ij)*(Vj1-Vj2) = 0
       */
      else if( etaux->topic == cWIDTH )
	{

	  naux = &(ckt->theNodeArray[etaux->node1]);

	  /* we only deal with equivalence width constraint here */
	  if(etaux->type != EQ )
	    {
	      sprintf(buf, "Invalid width constraint: v(%d).", etaux->node1);
	      error_mesg(PARSE_ERROR,buf);
	      continue;
	    }

	  itaux = pgNewIntConst(EQ,index++);
	  pgAddIntConst(&(ckt->NLPIntConstList),itaux);

	  daux = pgFindBranchByName(ckt, etaux->bname1);
	  assert(daux);
	  cst = (daux->length*daux->current);
            
	  if(daux->n1)
	    {
	      naux = &(ckt->theNodeArray[daux->n1]);
	      paux = pgNewProductByNode(1, 1/cst,naux);
	      pgAddProduct(itaux,paux);
	    }

	  if(daux->n2)
	    {
	      naux = &(ckt->theNodeArray[daux->n2]);
	      paux = pgNewProductByNode(-1, 1/cst,naux);
	      pgAddProduct(itaux,paux);
	    }

	  daux = pgFindBranchByName(ckt,etaux->bname2);
	  assert(daux);
	  cst = (daux->length*daux->current);
            
	  if(daux->n1)
	    {
	      naux = &(ckt->theNodeArray[daux->n1]);
	      paux = pgNewProductByNode(-1, 1/cst,naux);
	      pgAddProduct(itaux,paux);
	    }

	  if(daux->n2)
	    {
	      naux = &(ckt->theNodeArray[daux->n2]);
	      paux = pgNewProductByNode(1, 1/cst,naux);
	      pgAddProduct(itaux,paux);
	    }
	}
    }
  /* node 0 is bounded to zero */
  /*
    itaux = pgNewIntConst(LE,index++);
    pgAddIntConst(&(ckt->NLPIntConstList),itaux);
    paux = pgNewProductByNode(1, 1, &(ckt->theNodeArray[0]));
    pgAddProduct(itaux,paux);
    pgAddNumConst(itaux, 0);
  */
    
}


/*
**    This function constructs the constraints for
**    optimizing branch currents.
**    We have three type constraints:
**
**    (1) KVL law: 
**        sum di*Ii  = 0
**    (2) Minimu width constraints
**        1/(Vi2 - Vi1)*Ii - Wmin/(Resistivity*Li) >= 0
**        Li is the length of branch i;
**        Wmin is the minimum width constraint.
**    (3) Equivalence-width constraint
**	  li/(Vi1-Vi2)*I1 - lj/(Vj1-Vj2)*Ij = 0
*/

void
pgBuildCurLPConst(Circuit *ckt, ExtConst *ext_const, OptMode mode)
{
  long      index = 1;
  ExtConst  *etaux;
  IntConst  *itaux;
  Nodes     *naux;
  int       i,sign;
  Products  *paux;
  Branches  *daux, *bch1 = NULL, *bch2=NULL;
  BranchLink *blaux;
  double    coeff, cst;
  double    vdrop;

  assert(ext_const);
  assert(ckt->theNodeArray);
  assert(ckt->theDeviceList);

  if(ckt->LPIntConstList)
    {
      pgFreeIntConst(ckt->LPIntConstList);
      ckt->LPIntConstList = NULL;
    }

  /* 
   * constraints due to KCL law
   */
  for( i = 1; i <= ckt->nNumNode; i++)
    {

      /* ignore the dangling node */
      if(ckt->theNodeArray[i].stat != nNormal ||
	 ckt->theNodeArray[i].isVDD )
	continue;
      /*
	if(ckt->theNodeArray[i].stat != nNormal)
	continue;
      */

      /* each node has a constraint */
      itaux = pgNewIntConst(EQ,index++);
      pgAddIntConst(&(ckt->LPIntConstList),itaux);

      for(blaux = ckt->theNodeArray[i].blist; blaux; blaux = blaux->next)
	{

	  /* the sign is positive if the current direction
	     of the branch is toward node i.
	  */
	  if(blaux->dev->type == dCUR && blaux->dev->n1 == i)
	    {
	      cst = -1*blaux->dev->value;  
	      pgAddNumConst(itaux,cst);
	    }
	  else if(blaux->dev->type == dCUR && blaux->dev->n2 == i)
	    {
	      cst = blaux->dev->value;  
	      pgAddNumConst(itaux,cst);
	    }

	  /* the sign is positive if the current direction
	     of the branch is toward node i.
	  */    
	  else if(blaux->dev->type == dRES && blaux->dev->n1 == i)
	    {
	      paux = pgNewProductByBranch(-1, 1.0,blaux->dev);
	      pgAddProduct(itaux,paux);
	    }
	  else if(blaux->dev->type == dRES && blaux->dev->n2 == i)
	    {
	      paux = pgNewProductByBranch(1, 1.0,blaux->dev);
	      pgAddProduct(itaux,paux);
	    }
	  else
	    {
	      sprintf(buf, "Irrelevant branch: %s for node %d.", 
		      blaux->dev->name, i);
	      error_mesg(INT_ERROR,buf);
	      continue;
	    }
	}
    }

  /* 
   *    minimum width constraints
   *    1/(Vi2 - Vi1)*Ii - Wmin/(Resistivity*Li) >= 0
   */

  for(daux = ckt->theDeviceList; daux; daux = daux->next)
    {

      if(daux->stat != sNormal) 
	continue;

      if(daux->type != dRES) 
	continue;

      /* ignore branch with very small current */
      /*
	if(fabs(daux->current) <= MIN_CURRENT ||
	fabs(ckt->theNodeArray[daux->n1].voltage -
	ckt->theNodeArray[daux->n2].voltage) <= MIN_VOLTAGE )
	{
	sprintf(buf,"branch %s: current or voltage drop is close to 0, ignored.\n", 
	daux->name);
	print_mesg(buf);
	daux->stat = sAbnormal;
	continue;
	}
      */

      /* make sure current is not zero */
      if(daux->current != 0.0)
	{

	  itaux = pgNewIntConst( GE,index++ );
	  pgAddIntConst(&(ckt->LPIntConstList),itaux);
            
	  /* we creat a product terms and a constant */
	  /* the product */
	  assert(daux->n1 < (ckt->nMatrixSize+1));
	  assert(daux->n2 < (ckt->nMatrixSize+1));

	  vdrop = ckt->theNodeArray[daux->n1].voltage -
	          ckt->theNodeArray[daux->n2].voltage;
	  if(vdrop == 0)
	    {
	      printf("n%d (%g) -> n%d (%g)", 
		     daux->n1,  ckt->theNodeArray[daux->n1].voltage,
		     daux->n2,  ckt->theNodeArray[daux->n2].voltage);
	      
	      printf("current %g", daux->current);
	      assert(vdrop != 0.0);
	    }
	    
	  coeff = 1/(vdrop+FTINY);

	  paux = pgNewProductByBranch(1, coeff, daux);
	  pgAddProduct(itaux,paux);

	  /* the constant */
	  if(mode == optConjugate)
	    cst = 0 - (daux->lay->minWidth*EPSILON)/(daux->length*
						     daux->lay->unitRes);
	  else
	    cst = 0 - (daux->lay->minWidth)/(daux->length*
						     daux->lay->unitRes);

	  pgAddNumConst(itaux,cst);

	}
      else
	{
	  sprintf(buf,"%s(Linear): current=0, ignored.\n", daux->name);
	  print_mesg(buf);
	}
        
    }

  /*
   *  Equivalence-width constraint
   *  (li*Ii)/(Vi1-Vi2) - (lj*Ij)/(Vj1-Vj2) = 0
   */
  for( etaux = ext_const; etaux; etaux = etaux->next )
    {
      if( etaux->topic == cWIDTH )
	{

	  if(etaux->type != EQ )
	    {
	      sprintf(buf,"Invalid width constraint: v(%d).", etaux->node1);
	      error_mesg(PARSE_ERROR,buf);
	      continue;
	    }

	  itaux = pgNewIntConst(EQ, index++);
	  pgAddIntConst(&(ckt->LPIntConstList),itaux);

	  daux = pgFindBranchByName(ckt,etaux->bname1);
	  assert(daux);
	  vdrop = ckt->theNodeArray[daux->n1].voltage -
	          ckt->theNodeArray[daux->n2].voltage;
	  assert(vdrop != 0.0);

	  coeff = daux->length/(vdrop+FTINY);
	  paux = pgNewProductByBranch(1, coeff, daux);
	  pgAddProduct(itaux, paux);

	  daux = pgFindBranchByName(ckt, etaux->bname2);
	  assert(daux);
	  vdrop = ckt->theNodeArray[daux->n1].voltage -
	          ckt->theNodeArray[daux->n2].voltage;
	  assert(vdrop != 0.0);

	  coeff = daux->length/(vdrop + FTINY);
	  paux = pgNewProductByBranch(-1, coeff, daux);
	  pgAddProduct(itaux, paux);
	}
    }
}

/*
**    For adjusting the nodal voltages
**
**    Print the objective function for linear programming solver ---
**    lp_solver
**
**    In order to make the objective linear in terms of nodal
**    voltages, we add up reciprocals of branch area and use the sum
**    the objective function.  */
void
pgPrintVolLPObjFunc(Circuit *ckt, FILE *fp)
{

  double    cst, vdrop;
  char     name[128];
  Branches *daux;
  int i,count = 0;

  assert(ckt->theDeviceList);

  /*
    for(i=0; i<=ckt->nMatrixSize; i++)
    {
    ckt->theNodeArray[i].coeff = 0.0;
    }

    for(daux = ckt->theDeviceList; daux; daux = daux->next)
    {

    if(daux->stat != sNormal) 
    continue;

    if(daux->type != dRES) 
    continue;

    assert(daux->n1 < (ckt->nMatrixSize+1));
    assert(daux->n2 < (ckt->nMatrixSize+1));
    vdrop = ckt->theNodeArray[daux->n1].voltage -
    ckt->theNodeArray[daux->n2].voltage;
    cst = -(daux->current*daux->length*daux->length*
    daux->lay->unitRes)/(daux->vdrop*daux->vdrop);
        
    ckt->theNodeArray[daux->n1].coeff += cst;
    ckt->theNodeArray[daux->n2].coeff += -cst;
    }
  */

  fprintf(fp,"MIN: ");
  for(i=1; i<=ckt->nMatrixSize; i++)
    {

      if(ckt->theNodeArray[i].stat != nNormal)
	continue;
      if(ckt->theNodeArray[i].coeff == 0.0)
	continue;

      cst = ckt->theNodeArray[i].coeff;
      if(cst >= 0)
	fprintf(fp,"+%1.10g V_%d ", cst, i);
      else
	fprintf(fp,"%1.10g V_%d ", cst, i);
      if(count++%3 == 0)
	fprintf(fp,"\n");
    }
  fprintf(fp,";\n");
}

/*
**    For adjusting branch currents 
**
**    Print the objective function for linear programming solver ---
**    lp_solver

**    Note that all the varaibles are required postive in lp_sqolver,
**    so we need to adjust the signs of corresponing coefficients of
**    these variables.
** */

void
pgPrintCurLPObjFunc(Circuit *ckt, FILE *fp)
{

  double    cst, vdrop;
  char     name[128];
  Branches *daux;
  int count = 0;

  assert(ckt->theDeviceList);

  fprintf(fp,"MIN: ");
  for(daux = ckt->theDeviceList; daux; daux = daux->next)
    {

      if(daux->stat != sNormal) 
	continue;

      if(daux->type != dRES) 
	continue;

      /* ignore branch with very small current */
      if(fabs(daux->current) <= MIN_CURRENT ||
	 fabs(ckt->theNodeArray[daux->n1].voltage -
	      ckt->theNodeArray[daux->n2].voltage) <= MIN_VOLTAGE )
	{
	  sprintf(buf,"branch %s: current or voltage drop is close to 0, ignored.\n", 
		  daux->name);
	  print_mesg(buf);
	  daux->stat = sAbnormal;
	  continue;
	}

      count++;
      assert(daux->n1 < (ckt->nMatrixSize+1));
      assert(daux->n2 < (ckt->nMatrixSize+1));
      vdrop = ckt->theNodeArray[daux->n1].voltage -
	      ckt->theNodeArray[daux->n2].voltage;
      cst = daux->length*daux->length*daux->lay->unitRes/(vdrop + FTINY);
        
      /* variable must be positive */
      if(daux->current < 0)
	{
	  cst = -cst;
	  sprintf(name,"IN_%d",daux->index);
	}
      else
	{
	  sprintf(name,"I_%d",daux->index);
	}

      if(cst >= 0)
	fprintf(fp,"+%1.10g %s ", cst, name);
      else
	fprintf(fp,"%1.10g %s ", cst, name);
      if(count%3 == 0)
	fprintf(fp,"\n");
    }
  fprintf(fp,";\n");
}



/* 
**    Create a new internal constraint with given type.
*/
IntConst *pgNewIntConst( ConstType type, long index)
{
  IntConst *icaux;    

  icaux = (IntConst *)malloc(sizeof(IntConst));
  assert(icaux);
  icaux->type = type;
  icaux->cst = 0.0;
  icaux->stat = csUnknown;
  icaux->prod_list = NULL;
  icaux->index = index;
  icaux->next = NULL;
  return icaux;
}

/* 
**    Release all the internal constraint in a recursive way.
*/
void pgFreeIntConst( IntConst *int_const)
{
  assert(int_const);

  if(int_const->next)
    pgFreeIntConst(int_const->next);
  if(int_const->prod_list)
    pgFreeProduct(int_const->prod_list);
  free(int_const);
  return;
}

/*
**    Add a new product into the given internal constraint list
**    The addition is performed at the beginning of the list.
*/
void pgAddProduct( IntConst *int_const, Products *newp)
{
  assert(int_const);
  assert(newp);

  if(!int_const->prod_list) /* insert at the beginning */
    int_const->prod_list = newp;
  else
    {
      newp->next = int_const->prod_list;
      int_const->prod_list = newp;
      /*
	while(paux->next) paux = paux->next;
	paux->next = newp;
      */
    }
}

/*
**    Add a new constraint into the given internal constraint list.
**    The addition is performed at the beginning of the list.
*/
void pgAddIntConst(IntConst **ic_listp, IntConst *newic)
{
  IntConst *iaux;
  assert(ic_listp);
  assert(newic);

  if(!(*ic_listp))
    *ic_listp = newic;
  else
    {
      newic->next = *ic_listp;
      *ic_listp = newic;
    }
}

/*
**    Add a numerical constant into a internal constraint.
*/
void pgAddNumConst(IntConst *ic_list, double cst)
{
  assert(ic_list);
  ic_list->cst = cst;
}


/* 
**    Compute the value of the given internal constraint. 
*/

double pgComputeIntConstValue(IntConst * icp)
{
  Products *paux;
  double     dsum = 0, diff;

  if(!icp)
    return 0;
    
  for(paux = icp->prod_list; paux; paux = paux->next)
    {
      if(paux->branch)/* current variable */
	dsum += paux->sign*paux->coeff*paux->branch->current;
      else if(paux->node) /* voltage variable */
	dsum += paux->sign*paux->coeff*paux->node->voltage;
      else
	{
	  error_mesg(FAT_ERROR,"No variable in product."); 
	  continue;
	}
    }

  dsum += icp->cst;

  if(icp->type == GE && dsum >= 0.0)
    icp->stat = csSatisfied;
  else if((icp->type == GE) && (dsum < 0) && 
	  (fabs(dsum/icp->cst) > 1e-6) )
    {
      icp->stat = csViolated;
      pgPrintIntConst(stdout,icp);
      sprintf(buf," is violated (%g) vol: %g.\n",dsum,
	      icp->prod_list->node->voltage);
      print_mesg(buf);
    }
  else if(icp->type == EQ)
      icp->stat = csSatisfied;    
  else
    {
      /*
	pgPrintIntConst(stdout,icp);
	print_mesg(" is not supported.\n");
      */
      return 0;
    }
  return dsum;
}

/*
**    Check violation of internal constraints
*/
int
pgCheckAllIntConst(IntConst *iclist)
{

  Products *paux;
  double     dsum = 0;
  IntConst *icp;

  if(!iclist)
    return 1;
    
  for(icp = iclist; icp; icp = icp->next)
    {
      dsum = 0.0;
      for(paux = icp->prod_list; paux; paux = paux->next)
	dsum += paux->sign*paux->coeff*paux->node->voltage;

	dsum += icp->cst;

      if(dsum < 0 && fabs(dsum/icp->cst) > 1e-6 && (icp->type == GE))
	{
	  /*
	    pgPrintIntConst(stdout,icp);
	    printf("dsum: %g\n",dsum);
	    printf("v1: %g\n",icp->prod_list->node->voltage);
	    if(icp->prod_list->next)
	    printf("v2: %g\n",icp->prod_list->next->node->voltage);
	  */
	  return 0;
	}
    }
  return 1;
}


/*
**    Print the internal constraint in a 
**    form accetable to lp_solve2.0 system.
*/

void
pgPrintIntConst(FILE *fp, IntConst *icp)
{
  Products *paux;

  assert(fp);
  assert(icp);

  for(paux = icp->prod_list; paux; paux = paux->next)
    pgPrintProduct(fp,paux);
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
pgPrintAllIntConst(FILE *fp, IntConst *iclist)
{
  Products *paux;
  IntConst *icaux;

  assert(fp);
  assert(iclist);

  fprintf(fp,"/* ### Internal Constraint List --- */\n");
  for(icaux = iclist; icaux; icaux = icaux->next)
    pgPrintIntConst(fp, icaux);
        
  fprintf(fp,"/* ### End of List --- */\n\n");
}

/*
**    Print objective function and linear constraints
**    for linear programming solver.
*/
void
pgPrintLPData(Circuit *ckt, FILE *fp)
{
  if(!fp)
    return;
  assert(ckt->LPIntConstList);
  pgPrintCurLPObjFunc(ckt, fp);
  pgPrintAllIntConst(fp,ckt->LPIntConstList);
}

/*
**    Build the internal constraints for both
**    linear and non linear subproblems.
*/
void
pgBuildIntConst(Circuit *ckt)
{

  if(!ckt->theExtConstList)
    {
      error_mesg(IO_ERROR,"No constraint file loaded.");
      return;
    }

  /*
  ** First, we build the node array and corresponding
  ** linked list to store the topology of the circuit.
  */
  if(!ckt->theNodeArray)
    pgBuildNodeArray(ckt);

  /* 
  ** then compute the voltage and current for 
  ** each node and branch respectively 
  */
  pgComputeStateFromRes(ckt);

  /* 
  ** then build the internal constraint
  ** for the nonlinear optimization problem.
  */

  pgBuildNLPConst(ckt, ckt->theExtConstList);
  /*
    pgPrintAllIntConst(stdout,ckt->NLPIntConstList);
  */


  /* 
  ** Then build the internal constraint
  ** for the linear programming problem.
  */
  pgBuildCurLPConst(ckt, ckt->theExtConstList, optConjugate);
  /*
    pgPrintAllIntConst(stdout,ckt->LPIntConstList);
  */
}
