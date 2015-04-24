/*
 * $RCSfile: pgCircuit.c,v $
 * $Revision: 1.3 $
 * $Date: 2000/10/10 00:57:40 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

/************************************************************

Parse a spice file and build the corresponding MNA matrix

************************************************************/

#include "pgCircuit.h"

extern  double pgComputeCostFunct();

/* 
**    This function read the spice netlist and 
**    build the corresponding MNA matrix and solve it.
*/

/* some global variable definitions */
char *theMatrix;
double *theRhs;
double *theSol;
double *orgSol;

/*********************************/
/*********************************/

/*
**    read the spice netlist and build
**    internal date structure to store
**    the netlist information.
*/

Circuit *
pgParseCircuit( char *pfilename )
{
	char   *pstr;
	FILE   *fp;
	char   line[1024], name[128];
	int    vcount,error;
	char   cValue[128], cval1[128], cval2[128], cval3[128];
	double dValue;
	int    n1, n2, aux;
	Branches  *daux;
  
	Circuit *ckt;

	if(!pfilename)
    {
		error_mesg(IO_ERROR,"no file name given.");
		return NULL;
    }

	if((fp = fopen(pfilename,"r")) == NULL)
    {
		sprintf(buf, "Can not open file: %s.\n",pfilename);
		error_mesg(IO_ERROR,buf);
		return NULL;
    }

	/* create a new circuit */
	ckt = (Circuit *)malloc(sizeof(Circuit));

	ckt->name = CopyStr(pfilename);
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

	/* we assume the first line contains the size of the matrix
	   to be solved and the nodes are consecutively numbered.
	   So we can safely allocate the matrix and rhs vector. 
	*/

	pstr = fgets(line,1023,fp);
	if(!pstr)
		return NULL;
  
	/* ignore the # of node */
	while(*pstr == ' ') pstr++;
	sscanf(pstr, "%*s %d", &(ckt->nNumNode));
	vcount = ckt->nNumNode + 1;

	pstr = fgets(line,1023,fp);
	if(!pstr)
		return NULL;
	while(*pstr == ' ') pstr++;
	sscanf(pstr, "%*s %d", &ckt->nMatrixSize);

	if(ckt->nNumNode == 0)
    {
		error_mesg(FAT_ERROR,"#node == 0");
		return NULL;
    }

	while((pstr = fgets(line,1023,fp)) != NULL)
    {

		pstr = ToUpper(pstr);

#if defined(DEBUG)
		/*printf("%s",line);*/
#endif

		while(*pstr == ' ') pstr++;

		switch(*pstr)
		{

		case '.': /* spice control line -- ignore */
		case '*': /* comment line -- ignored */
		case '\n': /* black line -- ignored */
		case '\0': /* end of line -- ignored */
			break;
            
		case 'R': /* resistor */
		case 'r': 
			sscanf(pstr,"%s %d %d %s %s %s %s ",
				   name, &n1, &n2, cValue,cval1,cval2,cval3);
			daux = (Branches *)malloc(sizeof(Branches));     
			assert(daux != NULL);
			daux->name = CopyStr(name);
			daux->n1 = n1;
			daux->n2 = n2;
			daux->type = dRES;
			dValue = TransValue(cValue);
			daux->value = dValue;
			daux->stat = sNormal;
			daux->sign = 1;
			daux->current = 0.0;
			daux->vdrop = 0.0;
			daux->pI = 0.0;
			daux->nI = 0.0;
			daux->wmin = 0.0;
			daux->vn = 0;
			daux->layer = atoi(cval3);
			daux->width = TransValue(cval1);
			daux->length = TransValue(cval2);
			daux->index = ++ckt->nNumDevice;
			daux->next = NULL;
	  
			daux->origBranches = NULL;
			daux->curN1 = NULL;
			daux->curN2 = NULL;
			daux->oldRs = 0.0f;

	  
			if(( dValue == 0.0) ||
			   ( n1 == n2) )
			{
				free(daux);
				sprintf(buf,"abnormal branch: %s",line);
				error_mesg(FAT_ERROR, buf);
				break;
			}

			pgAddDevice(ckt, daux);
			/*pgAddDeviceNCheck(ckt, daux);*/

			break;

		case 'I': /* independent current source */
		case 'i': 
			sscanf(pstr,"%s %d %d %s",name, &n1, &n2, cValue);
			dValue = TransValue(cValue);

			daux = (Branches *)malloc(sizeof(Branches));     
			assert(daux != NULL);
			daux->name = CopyStr(name);
			daux->n1 = n1;
			daux->n2 = n2;
			daux->type = dCUR;
			daux->sign = 1;
			daux->current = 0.0;
			daux->value = dValue;
			daux->stat = sNormal;
			daux->pI = 0.0;
			daux->nI = 0.0;
			daux->wmin = 0.0;
			daux->vn = 0;
			daux->next = NULL;
	  
			daux->origBranches = NULL;
			daux->curN1 = NULL;
			daux->curN2 = NULL;
			daux->oldRs = 0.0f;


			if(( dValue == 0.0) || ( n1 == n2) )
			{
				free(daux);
				sprintf(buf,"abnormal branch: %s",line);
				error_mesg(FAT_ERROR, buf);
				break;
			}

			pgAddDevice(ckt, daux);
			/*pgAddDeviceNCheck(ckt, daux);*/

			break;

		case 'V': /* independent voltage source */
		case 'v': 
			sscanf(pstr,"%s %d %d %s",name, &n1, 
				   &n2, cValue);
			dValue = TransValue(cValue);

			daux = (Branches *)malloc(sizeof(Branches));     
			assert(daux);
			daux->name = CopyStr(name);
			daux->n1 = n1;
			daux->n2 = n2;
			daux->type = dVOL;
			daux->sign = 1;
			daux->current = 0.0;
			daux->value = dValue;
			daux->stat = sNormal;
			daux->pI = 0.0;
			daux->nI = 0.0;
			daux->wmin = 0.0;
			daux->vn = vcount++;
			daux->next = NULL;

			daux->origBranches = NULL;
			daux->curN1 = NULL;
			daux->curN2 = NULL;
			daux->oldRs = 0.0f;

			if(( dValue == 0.0) &&
			   ( n1 == n1) )
			{
				free(daux);
				sprintf(buf,"abnormal branch: %s",line);
				error_mesg(FAT_ERROR, buf);
				break;
			}

			pgAddDevice(ckt, daux);
			/*pgAddDeviceNCheck(thCkt, daux);*/
	
			break;

		default: 
			sprintf(buf, "Unknown device type: %s.",pstr);
			error_mesg(INT_ERROR,buf);
			break; 
		}
    }
	if((vcount-1) != ckt->nMatrixSize)
    {
		error_mesg(PARSE_ERROR, "matrix size is inconsistant with #node.");
		exit(-1);
    }

	pgPrintCktStatistic(ckt);
	fclose(fp);
	return ckt;
}
 

int
pgDCAnalysis(Circuit *ckt)
{
	FILE *pf;
	char line[1024];
 
	assert(ckt->theDeviceList);
 
	/* build the MNA matrix */
	if( pgBuildMNAEquation(ckt) == -1)
		return -1;
 
	/* print the matrix in asiic form */
	/*
	  spPrint(theMatrix, 0, 1, 1);
	*/
	printf("External size of the matrix: %d\n",spGetSize(theMatrix,1));
	printf("Internal size of the matrix: %d\n",spGetSize(theMatrix,0));
	fflush(stdout);
 
	/* LU decomposition */
	if( spFactor(theMatrix) != spOKAY )
    {
		error_mesg(INT_ERROR,"Matrix factorization error.");
		return -1;
    }
 
	/* solve it */
	spSolve(theMatrix, theRhs, theSol);
 
	/* store the outcome */
	if(!ckt->theNodeArray)
		pgBuildNodeArray(ckt);
	pgComputeStateFromRes(ckt);

	pgPrintResult(theSol, ckt->nMatrixSize);
	
	// Following statements will cause crash in linux 7/9/02 Sheldon
    //spDestroy(theMatrix);
	//free(theRhs);
	//free(theSol);
}

/* Build MNA matrix from a internal netlist and 
** allocate the memory for global theMatrix,
** theRhs and theSol.
** Upon error, -1 is return.
**
*/

int 
pgBuildMNAEquation(Circuit *ckt)
{
	char     *matrix;
	char     *pstr;
	FILE     *fp;
	char     line[1024];
	int	   error;
	char     cValue[128];
	double   dValue;
	int      n1, n2;
	struct     spTemplate sTemplate;
	Branches *daux;

	assert(ckt->theDeviceList);

	theMatrix = spCreate(1, 0, &error);
	spClear(theMatrix);

	theRhs = (double *)malloc((ckt->nMatrixSize + 1) * sizeof(double));
	theSol = (double *)malloc((ckt->nMatrixSize + 1) * sizeof(double));

	/* initialization */
	memset(theRhs, 0, (ckt->nMatrixSize + 1)*sizeof(double));
	memset(theSol, 0, (ckt->nMatrixSize + 1)*sizeof(double));

	for( daux= ckt->theDeviceList; daux; daux = daux->next )
    {
		if(daux->stat == sAbnormal)
			continue;

		switch(daux->type)
		{

		case dRES: /* resistor */
			error =
				spGetAdmittance(theMatrix, 
								daux->n1, daux->n2, &sTemplate);
			if(error != spOKAY)
				return -1;
			dValue = 1/daux->value;
			/*
			  printf("admittance: %lf.\n",dValue);
			*/
			spADD_REAL_QUAD(sTemplate, dValue);
			break;

		case dCUR: /* independent current source */
			/*
			  printf("current source: %g.\n",daux->value);
			*/
			/* the current direction is from 
			   n1 to n2 */
			if(daux->n1 != 0)
				theRhs[daux->n1] -= daux->value;
			if(daux->n2 !=0)
				theRhs[daux->n2] += daux->value;
			break;

		case dVOL: /* independent voltage source */
			/*
			  printf("voltage source: %g.\n",daux->value);
			*/
			/* n1: positive, n2: negative */
			if(daux->vn != 0)
				theRhs[daux->vn] = daux->value;
			error =
				spGetOnes(theMatrix, daux->n1, daux->n2, 
						  daux->vn, &sTemplate);
			if(error != spOKAY)
				return -1;

			/*
			  spADD_REAL_QUAD(sTemplate, 1);
			*/

			break;

		default: 
			sprintf(buf, "Unknow device type: %s.", pstr);
			error_mesg(INT_ERROR,buf);
			break; 
		}
    }
	return 0;
}


/*
** Print out the result in a realable way.
*/

void
pgPrintResult(double *psol, int size)
{
	int i = 1;
	
	for( i = 1; i<=size; i++)
	{
		printf("v[%d] = %g.\n",i, psol[i]);
	
	}
	printf("**** end of report ****\n\n");
    
}

/*
**    Add a device into the global device list(theDeviceList)
**    and also check the ocurrance of the same name.
*/

void
pgAddDeviceNCheck(Circuit *ckt, Branches *pdev)
{
	Branches *daux, *pdaux;
	if(!ckt->theDeviceList)
		ckt->theDeviceList    = pdev;
	else{
		daux = pdaux = ckt->theDeviceList;
		while(daux)
		{
			if(!strcmp(daux->name,pdev->name))
			{    
				sprintf(buf,"Duplicated name: %s ignored.\n",
						pdev->name);
				print_warn(buf);
				return;
			}
			pdaux = daux;
			daux = daux->next;
		}
		pdaux->next = pdev;
	}
}

/*
**    Add a device without duplicate name checking
*/

void
pgAddDevice(Circuit *ckt, Branches *pdev)
{
	Branches *daux, *pdaux;
	if(!ckt->theDeviceList)
		ckt->theDeviceList    = pdev;
	else{
		pdev->next = ckt->theDeviceList;
		ckt->theDeviceList = pdev;
	}
	ckt->nNumBranch++;
}


/*
**    Free the theDeviceList
*/

void
pgFreeDeviceList(Branches *pdev)
{
	Branches *daux, *ndaux;

	if(!pdev)
		return;
	daux = pdev;
	while(daux)
    {
		ndaux = daux->next;
		free(daux->name);
		free(daux);
		daux = ndaux;
    }

}

/* 
**    return the branch for the give name.
**    We assume that all the charaters in
**     the branch name and given name are 
**    in the upper case.
*/

Branches *pgFindBranchByName(Circuit *ckt, char *bname )
{
	Branches *baux;

	assert(ckt->theDeviceList);

	for(baux = ckt->theDeviceList; baux; baux = baux->next)
    {
		if(!strcmp(baux->name, bname))
			return baux;
    }
	return NULL;

}

/*
**    Print the node voltages 
*/
void
pgPrintNodeVoltage(Circuit *ckt)
{
	int i = 1;
	double max = -10000, min = 10000;
	double vol = 0;

	assert(ckt->theNodeArray);
	printf("**** current nodal voltage ****\n\n");
	for( i = 1; i<=ckt->nMatrixSize; i++)
	{
		printf("v[%d] = %g.\n",i, ckt->theNodeArray[i].voltage);
		vol = ckt->theNodeArray[i].voltage;
		if((vol < min) && (vol > 0))
		{
			min = vol;
		}
		if((vol > max) && (vol > 0))
		{
			max = vol;
		}
		
	}

	printf("max voltage = %g, min voltage = %g, vol diff = %g \n",
		   max, min, fabs(max-min));
	printf("***\n\n");
}

/*
**    Print branch currents
*/
void
pgPrintBranchState(Circuit *ckt)
{
	Branches *baux;

	assert(ckt->theDeviceList);

	for( baux = ckt->theDeviceList; baux; baux = baux->next )
    {
		if(baux->type != dRES)
			continue;
		printf("\n branch: %s :\n",baux->name);
		printf("\t [%d -> %d] current = %g\n",
			   baux->n1, baux->n2, baux->current);
		printf("\t voltage drop = %g\n",baux->vdrop);
		printf("\t resistance = %g\n",baux->value);
		printf("\t width = %g\n",baux->width);
    }
}


/*
**
*/
void
pgPrintState(Circuit *ckt)
{
    
	extern double pgComputeCostFunct();

	printf("\n #### node voltage value:\n");
	pgPrintNodeVoltage(ckt);
	//printf("\n #### branch statistic :\n");
	//pgComputeResFromState(ckt);
	//pgPrintBranchState(ckt);
	printf("\n #### cost value: %g\n", pgComputeCostFunct(ckt));
}


/*
**    Read back the result from lp_solve
**    for branch current.
*/
void
pgReadCurLPResult(Circuit *ckt, char *file)
{
	FILE *fp;
	char *pstr, *pstr1;
	char line[1024], name[128],  backbuf[1024], cval[128];
	Branches *baux;
	double value;
	int    pos = 1;

	if(!file)
    {
		error_mesg(IO_ERROR,"No result file name given.\n");
		return;
    }
 
	if((fp = fopen(file,"r")) == NULL)
    {
		sprintf(buf, "Can not open result file: %s.\n",file);
		error_mesg(IO_ERROR,buf);
		return;
    }

	pstr = fgets(line,1023,fp); /* skip the first line */

	while((pstr = fgets(line,1023,fp)) != NULL)
    {       
		pos = 1;
		strcpy(backbuf,line);
		sscanf(pstr,"%s %s",name,cval);
		value = TransValue(cval);
		pstr1 = strchr(name,'_');
		if(!pstr1)
		{
			sprintf(buf, "Line: %s", backbuf);        
			error_mesg(PARSE_ERROR, buf);
			return;
		}
		*pstr1 = 0;
		if(!strcmp(name,"IN"))
			pos = 0;
		baux = pgFindBranchByIndex(ckt, atoi(++pstr1));
		if(!baux)
		{
			sprintf(buf,"Cannot find branch: %s.", pstr1);
			error_mesg(PARSE_ERROR,buf);
			continue;
		}
		if(pos)
			baux->current = value;
		else
			baux->current = -value;

		/* printf("value: %lf.\n",baux->current); */
      
    }
  
	fclose(fp);
}


/* 
**    Read back the result from lp_solve for nodal voltages. In case the
**    result does not ** improve the cost function, we do the binary
**    line search to find the improved result. 
*/
void
pgReadVolLPResultAndLineSearch(Circuit *ckt, char *file)
{
	FILE *fp;
	char *pstr, *pstr1;
	char line[1024], name[128],  backbuf[1024], cval[128];
	Branches *baux;
	double value, pcost, cost;
	double *vol, *dir;
	int    i, pos = 1, index, count;

	/* restore the current voltages before read the new one */
	vol = (double *)malloc((ckt->nMatrixSize+1)*sizeof(double));
	dir = (double *)malloc((ckt->nMatrixSize+1)*sizeof(double));
  
	for(i=0; i <= ckt->nMatrixSize; i++)
		vol[i] = ckt->theNodeArray[i].voltage;

	/* current cost */
	pcost = pgComputeCostFunct(ckt);

	/* then read the new result */
	if(!file)
    {
		error_mesg(IO_ERROR, "No result file name given.\n");
		return;
    }
 
	if((fp = fopen(file,"r")) == NULL)
    {
		sprintf(buf, "Can not open result file: %s.\n",file);
		error_mesg(IO_ERROR,buf);
		return;
    }

	pstr = fgets(line,1023,fp); /* skip the first line */

	while((pstr = fgets(line,1023,fp)) != NULL)
    {    
		pos = 1;
		strcpy(backbuf,line);
		sscanf(pstr,"%s %s",name,cval);
		value = TransValue(cval);
		pstr1 = strchr(name,'_');
		if(!pstr1)
		{
			sprintf(buf, "Line: %s", backbuf);        
			error_mesg(PARSE_ERROR, buf);
			return;
		}
		*pstr1 = 0;
		index = atoi(++pstr1);

		ckt->theNodeArray[index].voltage = value;
		/* printf("vol[%d] value: %lf.\n",index,value); */
      
    }
	fclose(fp);
	/* get the new cost */
	cost = pgComputeCostFunct(ckt);
	printf("cost after lp: %g\n", cost);

	for(i=0; i <= ckt->nMatrixSize; i++)
		dir[i] = ckt->theNodeArray[i].voltage - vol[i];

	if(cost > pcost)
    {
		pgDoGoldenLineSearch(ckt, vol, dir);
    }
  
 
	/* free the space */
	free(vol);
	free(dir);

	/* check if the new set of voltages result in better result */
}


/*
**    Find more layer information for each branch 
**    It assumes the layer table has been built.
*/

void
pgFindLayerInfor(Circuit *ckt)
{
	Branches *baux;
	Layer *lay;

	assert(theLayerTable);
	assert(ckt->theDeviceList);

	for(baux = ckt->theDeviceList; baux; baux = baux->next)
    {
		if(baux->type != dRES)
			continue;
		baux->lay = pgFindLayerByIndex(baux->layer);
		if(!baux->lay)
		{
			sprintf(buf,"can not find layer: %d definition.\n",
					baux->layer);
			error_mesg(FAT_ERROR,buf);
			exit(-1);
		}
    }
}


/*
**    Build the branch index array
**    we assume that the branch are indexed
**    consecutively beginning with 1
*/

void
pgBuildBranchQuickIndex(Circuit *ckt)
{
	Branches *baux;

	if(ckt->theBranchArray)
		return; /* we only need to build once */
    
	ckt->theBranchArray = (BranchLink *)malloc((ckt->nNumDevice + 1)*
											   sizeof(BranchLink));
	assert(ckt->theBranchArray);

	for(baux = ckt->theDeviceList; baux; baux = baux->next)
    {
		if(baux->stat != sNormal || baux->type != dRES)
			continue;
		ckt->theBranchArray[baux->index].dev = baux;
    }
}

/*
**    Find the branch by its index
*/
Branches *
pgFindBranchByIndex(Circuit *ckt, int index)
{
	assert(ckt->theBranchArray);
	return ckt->theBranchArray[index].dev;
}


void
pgPrintCktStatistic(Circuit *ckt)
{
	printf("\n--------------------\n");
	printf("#branches: %d.\n",ckt->nNumBranch);
	printf("#resistors: %d.\n",ckt->nNumDevice);
	printf("#nodes: %d.\n",ckt->nNumNode);
	printf("matrix size: %d.\n",ckt->nMatrixSize);
	printf("--------------------\n");
}

/*
**    Output the sized SPICE-like netlist
*/

int
pgDumpSizedNetlist(char *outfile)
{

	FILE   *fp;
	double value;
	Branches *baux;

	if(!outfile)
    {
		error_mesg(IO_ERROR,"no file name given.");
		return;
    }

	if((fp = fopen(outfile,"w")) == NULL)
    {
		sprintf(buf, "Can not open file: %s.\n",outfile);
		error_mesg(IO_ERROR,buf);
		return;
    }

	if(!theCkt || !theCkt->theDeviceList)
	{
		sprintf(buf, "No circuit is loaded.");
		error_mesg(IO_ERROR,buf);
		return;
	}

	fprintf(fp, "* Wire sized spice-like netlist file generated by PGOPT.\n");
	fprintf(fp, "* Copyright 1999-2002, X.D. Sheldon Tan. \n\n");
	fprintf(fp, "* num_node: %d\n", theCkt->nNumNode);
	fprintf(fp, "* matrix_size: %d\n", theCkt->nMatrixSize);

	fprintf(fp, "* --- Format ---\n");
	fprintf(fp, "* Rxxx, n1, n2, value, width, length, lay_index\n");
	fprintf(fp, "* Ixxx(Vxxx), n1, n2, value\n\n");

	for( baux = theCkt->theDeviceList; baux; baux = baux->next )
	{
		switch (baux->type)
		{
		case dRES:
			fprintf(fp,"%s %d %d %g %g %g %d\n",
					baux->name, baux->n1, baux->n2, baux->value,
					baux->width, baux->length, baux->layer);
			break;
			
		case dVOL:
		case dCUR:
            fprintf(fp,"%s %d %d %g\n",
					baux->name, baux->n1, baux->n2, baux->value);
			break;

		default:
			break;
		}    
   
	}

	fclose(fp);
}
