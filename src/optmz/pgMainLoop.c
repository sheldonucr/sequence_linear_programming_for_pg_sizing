/*
 * $RCSfile: pgMainLoop.c,v $                                                                   
 * $Revision: 1.3 $
 * $Date: 2000/10/10 01:01:12 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

/*******************************************************************************
 *                                                                              *
 * Module name:                              Prefix: pg                         *
 *                                           Author: XiangDong Tan              *
 * Description:                                                                 *
 *    Main loop of the optimization algorithm
 *******************************************************************************/

#include "pgOptmz.h"
#include <math.h>

#define StopThreshold 0.01	/* 1% */
extern double EPSILON;

/*
**    the main loop of the whole optimization process
*/

#define FTOL 3.0e-8  /* tolerance factor for double precision */
/* #define _PCX_ */

void
pgMainLoop(Circuit *ckt, OptMode mode)
{
	int i;
	double    cost, pcost,ppcost;
	int count = 0, vcount = 0;
	double time1 = 1.0 * clock() / 1000000;
	double time3, time2;
	double c_time;
	double stopThrld;

	/* first we obtain a feasible solution */
	pgInitSolution(ckt);

	if(!ckt->theNodeArray)
		pgBuildNodeArray(ckt);


	if(!ckt->theExtConstList)
    {
		printf("ERROR: No constraints file given, abort!\n");
		return;
    }

	pgComputeStateFromRes(ckt);


	pcost = cost = pgComputeCostFunct(ckt); 

	printf("\nCOST(BEFORE): %lf\n", pcost);

	time2 = 1.0 * clock() / 1000000;
	c_time = time2 - time1;
	printf("** Initialization time: %g\n",c_time);
  
	if(mode == optSLP)
		printf("Using sequence of linear programming.\n");
	else if(mode == optConjugate)
		printf("Using conjugate gradient method.\n");

	stopThrld = (mode == optConjugate)? 0.01*StopThreshold:StopThreshold;
	printf("stopThrld: %g\n", stopThrld);
  
	while(1)
    {
		count++;
		printf("\nBeginning V-I optimization iteration: %d\n", count);
		time3 = 1.0 * clock() / 1000000;
    
		if(getenv("PG_NO_VOLTAGE") == NULL)
		{
	  
			int vcount = 1;
			ppcost = cost;
			while(1) 
			{
	      
				printf("Voltage optimization ... (%d.%d) \n", count, vcount++);
				if(mode == optSLP)
				{
#ifdef _PCX_
					pgVolLOptimizer(ckt, &cost);
#else
					pgVolLOptimizerLPS(ckt, &cost); /* using lp_solve by default */
#endif
				}
				else if(mode == optConjugate)
					pgNLOptimizer(ckt, &cost);
				else
				{
					printf("Invalid optimzation mode\n");
					return;
				}
	      
				printf("** cost after voltage optimization: %g\n",
					   pgComputeCostFunct(ckt));
	      
	      
				printf("** cost %g, ppcost %g\n", cost, ppcost);
	      
				if(mode == optConjugate)
					break;

				if(cost > ppcost || (fabs((cost - ppcost)/cost) <= stopThrld) ||
				   (getenv("PG_ONE_VOLTAGE_PASS") != NULL))
					break;
				else
					ppcost = cost;
			}

		} /* end of voltage optimization */
          
		printf("Current optimization ... \n");
#ifdef _PCX_      
		pgCurLOptimizer(ckt, &cost, mode);
#else
		pgCurLOptimizerLPS(ckt, &cost, mode);
#endif
		printf("cost after current optimization: %g\n",cost);

		pgComputeResFromState(ckt);
      
		EPSILON = EPSILON*0.99;
		if(EPSILON < 1.0)
			EPSILON = 1.0001;
		printf("EPSILON: %g\n", EPSILON);

		if(cost > pcost || fabs((cost - pcost)/cost) < stopThrld)
		{
			if(mode == optConjugate)
			{
				PenalWeight = 0.1*PenalWeight;
				if(PenalWeight < 1e-4)
					break;
				else
				{
					pcost = cost;
				}
			}
			else
				break;
		}
		else
			pcost = cost;

		printf("PanelWeight: %g\n", PenalWeight);
      
		/*
		  RestrFactor = RestrFactor - (1 - RestrFactor)*2;
		  RestrFactor = 0.5;
		*/
		time2 = 1.0 * clock() / 1000000;
		c_time = time2 - time3;
		printf("** CPU time in this iteration: %g\n",c_time);
       
		if(getenv("PG_ONE_PASS") != NULL)
			break;
    }

	/* compute the corresponding resistance vlaue */
	pgComputeResFromState(ckt);

	/* compute the corresponding width value */
	pgCompWidthFromRes(ckt);

	printf("\nCOST(AFTER): %lf\n", pgComputeCostFunct(ckt));
	time2 = 1.0 * clock() / 1000000;
	c_time = time2 - time1;
	printf("** Total CPU time in all the mainloop iterations: %f\n",c_time);

}

/*
**     Find a initial feasible solution.
**     A hierustic is used to guarattee the 
**     initial solution is feasible. 
*/

void
pgInitSolution(Circuit *ckt)
{;}

/*
**    Nonlinear optimizer
*/

void
pgNLOptimizer(Circuit *ckt, double *cost)
{
	double fmin = 0;
	int    iter = 0;

	pgBuildNLPConst(ckt, ckt->theExtConstList);
	printf("PANELTY COST(BEFORE): %g\n",pgComputePenaltyCostFunct(ckt));
	/*
	  pgPrintAllIntConst(stdout,ckt->NLPIntConstList);
	*/

	pgConjudateOptmz(ckt, &iter, &fmin, FTOL);
	/*
	  pgNewtonOptmz(ckt, &iter, &fmin, FTOL);
	*/
	*cost = pgComputeCostFunct(ckt);
	printf("PANELTY COST(AFTER): %g\n",fmin);
 
}

/* 
**    Linear optimizer What is accutually done is to call a standalong
**    linear programming solver with the constraints generated for the
**    solver according to the solver format. Now we use PCx 1.1 
*/

void
pgCurLOptimizer(Circuit *ckt, double *cost, OptMode mode)
{
	double fmin = 0, *grad;
	int    iter = 0;
	char    comm[256], *tmp_name;
	char    cmd[256], in[256], out[256], mps[256];
	FILE     *fp;

	pgBuildCurLPConst(ckt, ckt->theExtConstList, mode);
	/*
	  pgPrintAllIntConst(stdout,LPIntConstList);
	*/
	tmp_name = tempnam (".", "_pgI_");
	/* tmp_name = mkstemp("_pgI_"); */
	/* tmp_name = tmpnam("_pgI_"); */
	sprintf(in,"%s.in",tmp_name);
	sprintf(mps,"%s.mps",tmp_name);
	sprintf(out,"%s.out",tmp_name);
	fp = fopen(in,"w");
	if(!fp)
    {
		sprintf(buf,"Can not open %s to write!",in);
		error_mesg(IO_ERROR,buf);
    }
	/* output the lp data */
	pgPrintLPData(ckt, fp);
	fclose(fp);

	/* invoke a system call */
	sprintf(comm, "lp2mps < %s > %s",in, mps);
	pgSystem(comm);

	/* execute the linear programming and read back the result */
	sprintf(comm, "PCx  %s ", mps);
  
	if(pgSystem(comm) == 0)
		pgReadCurLPResult(ckt, out);
	else
		error_mesg(IO_ERROR,"Incorrect results, ignored!");

	*cost = pgComputeCostFunct(ckt);

	/* remove two temporary files */
	unlink(in); 
	unlink(mps);
	unlink(out);
	free(tmp_name);
}

/* Linear optimizer by node voltage using PCx 1.1.  */

void
pgVolLOptimizer(Circuit *ckt, double *cost)
{
	double fmin = 0, *grad;
	int    iter = 0;
	char    comm[256], *tmp_name;
	char    cmd[256], in[256], out[256], mps[256];
	FILE     *fp;

	pgBuildVolLPConst(ckt, ckt->theExtConstList);

	/*
	  if(!pgCheckAllIntConst(ckt->NLPIntConstList))
	  {
	  printf("This is no infeasible solution.\n");
	  return;
	  }
	*/

	tmp_name = tempnam(".","_pgV_");
	/* tmp_name = mkstemp("_pgV_"); */	
	sprintf(in,"%s.in",tmp_name);
	sprintf(mps,"%s.mps",tmp_name);
	sprintf(out,"%s.out",tmp_name);
	fp = fopen(in,"w");
	if(!fp)
    {
		sprintf(buf,"Can not open %s to write!",in);
		error_mesg(IO_ERROR,buf);
		return;
    }


	/* output the lp data */
	pgPrintVolLPObjFunc(ckt, fp);
	pgPrintAllIntConst(fp,ckt->NLPIntConstList);

	fclose(fp);

	/* invoke a system call */
	sprintf(comm, "lp2mps < %s > %s",in, mps);
	pgSystem(comm);

	/* execute the linear programming and read back the result */
	sprintf(comm, "PCx  %s ", mps);
	if(pgSystem(comm) == 0)
		pgReadVolLPResultAndLineSearch(ckt, out);
	else
		error_mesg(IO_ERROR,"Incorrect results, ignored!");

	*cost = pgComputeCostFunct(ckt);

	/* remove two temporary files */
	unlink(in);
	unlink(mps);
	unlink(out);
	free(tmp_name);
}

/*
 * Linear optimizer by solving currents
 * with lp_solve format.
 */
void
pgCurLOptimizerLPS(Circuit *ckt, double *cost, OptMode mode)
{
	double fmin = 0, *grad;
	int    iter = 0;
	char    comm[256], *tmp_name;
	char    cmd[256], in[256], out[256];
	FILE     *fp;

	pgBuildCurLPConst(ckt, ckt->theExtConstList, mode);
	/*
	  pgPrintAllIntConst(stdout,LPIntConstList);
	*/
	tmp_name = tempnam(".","_pgV_");
	/* tmp_name = mkstemp("_pgI_"); */	
	sprintf(in,"%s.in",tmp_name);
	sprintf(out,"%s.out",tmp_name);
	fp = fopen(in,"w");
	if(!fp)
    {
		sprintf(buf,"Can not open %s to write!",in);
		error_mesg(IO_ERROR,buf);
    }
	/* output the lp data */
	pgPrintLPData(ckt, fp);
	fclose(fp);

	/* invoke a system call */
	sprintf(comm, "lp_solve < %s > %s",in, out);
	pgSystem(comm);
	/* read back the result */
	pgReadCurLPResult(ckt, out);
	*cost = pgComputeCostFunct(ckt);

	/* remove two temporary files */
	unlink(in); 
	unlink(out);
	free(tmp_name);
}

/*
**    Linear optimizer by
**    solving node voltage 
**    with lp_sovler format.
*/

void
pgVolLOptimizerLPS(Circuit *ckt, double *cost)
{
	double fmin = 0, *grad;
	int    iter = 0;
	char    comm[256], *tmp_name;
	char    cmd[256], in[256], out[256];
	FILE     *fp;

	pgBuildVolLPConst(ckt, ckt->theExtConstList);

	tmp_name = tempnam (".","_pg_");
	/* tmp_name = mkstemp ("_pg_"); */
	sprintf(in,"%s.in",tmp_name);
	sprintf(out,"%s.out",tmp_name);
	fp = fopen(in,"w");
	if(!fp)
    {
		sprintf(buf,"Can not open %s to write!",in);
		error_mesg(IO_ERROR,buf);
		return;
    }
	/* output the lp data */

	pgPrintVolLPObjFunc(ckt,fp);
	pgPrintAllIntConst(fp,ckt->NLPIntConstList);

	fclose(fp);

	/* invoke a system call */
	sprintf(comm, "lp_solve < %s > %s",in, out);
	pgSystem(comm);
  
	/* read back the result */
	pgReadVolLPResultAndLineSearch(ckt, out);
	*cost = pgComputeCostFunct(ckt);

	/* remove two temporary files */
	unlink(in);
	unlink(out);
	free(tmp_name);
}

/*
**   Linear optimizer by solving currents
**   with MPS format.
*/

void
pgCurLOptimizerMPS(Circuit *ckt, double *cost, OptMode mode)
{
	double  fmin = 0,  *grad;
	int     iter = 0;
	char    comm[256], *tmp_name;
	char    cmd[256],  in[256], out[256];
	FILE     *fp;

	pgBuildCurLPConst(ckt, ckt->theExtConstList, mode);
	/*
	  pgPrintAllIntConst(stdout,LPIntConstList);
	*/
	tmp_name = tempnam(".","_pg_");
	/* tmp_name = mkstemp ("_pg_"); */
	sprintf(in,"%s.mps",tmp_name);
	sprintf(out,"%s.out",tmp_name);
	fp = fopen(in,"w");
	if(!fp)
    {
		sprintf(buf,"Can not open %s to write!",in);
		error_mesg(IO_ERROR,buf);
    }
	/* output the lp data */
	/*
	  pgMPSBuildCurVarList();
	  pgMPSBuildCurVarColumn(LPIntConstList);
	*/

	fprintf(fp,"NAME PGOPT GENERATED MPS FILE (MIN)\n");
	pgMPSPrintRows(fp, ckt->LPIntConstList);
	pgMPSPrintColumns(ckt, fp);
	pgMPSPrintRHS(fp, ckt->LPIntConstList);
	fprintf(fp,"ENDATA\n");

	fclose(fp);

	/* invoke a system call */
	sprintf(comm, "PCx %s",in);
	pgSystem(comm);

	/* read back the result */
	pgReadCurLPResult(ckt, out);
	*cost = pgComputeCostFunct(ckt);

	/* remove two temporary files */
	unlink(in); unlink(out);
	free(tmp_name);
}


/*
**   Voltage optimization using MPS format 
**    
*/

void
pgVolLOptimizerMPS(Circuit *ckt, double *cost)
{
	double fmin = 0, *grad;
	int    iter = 0;
	char    comm[256], *tmp_name;
	char    cmd[256], in[256], out[256];
	FILE     *fp;

	pgBuildVolLPConst(ckt, ckt->theExtConstList);
	/*
	  pgPrintAllIntConst(stdout,ckt->NLPIntConstList);
	*/
	tmp_name = tempnam(".","_pg_"); 
	/* tmp_name = mkstemp ("_pg_"); */ 
	sprintf(in,"%s.mps",tmp_name);
	sprintf(out,"%s.out",tmp_name);
	fp = fopen(in,"w");
	if(!fp)
    {
		sprintf(buf,"Can not open %s to write!",in);
		error_mesg(IO_ERROR,buf);
    }
	/* output the lp data */
	/*
	  pgMPSBuildVolVarList();
	  pgMPSBuildVolVarColumn(ckt->NLPIntConstList);
	*/

	fprintf(fp,"NAME PGOPT GENERATED MPS FILE (MIN)\n");
	pgMPSPrintRows(fp, ckt->NLPIntConstList);
	pgMPSPrintColumns(fp);
	pgMPSPrintRHS(fp, ckt->NLPIntConstList);
	fprintf(fp,"ENDATA\n");

	fclose(fp);

	/* invoke a system call */
	sprintf(comm, "PCx %s",in);
	pgSystem(comm);

	/* read back the result */
	pgReadVolLPResultAndLineSearch(ckt,out);
	*cost = pgComputeCostFunct(ckt);

	/* remove two temporary files */
	unlink(in); unlink(out);
	free(tmp_name);
}
