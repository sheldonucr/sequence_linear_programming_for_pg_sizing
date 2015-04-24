/*
 * $RCSfile: pgMPS.c,v $
 * $Revision: 1.3 $
 * $Date: 2000/10/10 00:57:40 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

/************************************************************
 
 Routines reading in and writing out the lp problem instances
 file for MPS format for linear programming solver.
 
************************************************************/
   
#include "pgMPS.h"

void
pgMPSBuildBchVarList(Circuit *ckt)
{
    Branches *baux;
    long    count = 1;
    int    i;
 
   if(theVarArray)
     pgFreeVarList(ckt);
 
   theVarArray = (VariableList *)malloc((ckt->nNumDevice + 2)*
					sizeof(VariableList));
   assert(theVarArray);
 
   for(baux = ckt->theDeviceList; baux; baux = baux->next){
        if(baux->stat != sNormal || baux->type != dRES){
            theVarArray[baux->index].bch = NULL;
            theVarArray[baux->index].head = NULL;
            continue;
        }
        theVarArray[baux->index].index = 0.0;
        theVarArray[baux->index].bch = baux;
        theVarArray[baux->index].head = NULL;
   }

   /* build the new indices which are consecutive */
   for(i = 1; i <= ckt->nNumDevice; i++){
        if(theVarArray[i].bch)
            theVarArray[i].index = count++;
    }
}

void
pgFreeVarList(Circuit *ckt)
{
    long i;

    if(!theVarArray)
        return;

    for(i = 1; i <= ckt->nNumDevice; i++){
        if(theVarArray[i].head)
            pgFreeColEntry(theVarArray[i].head);
        }
    theVarArray = NULL;
}

/************************************************************/
/************************************************************/

ColumnEntry *pgNewColEntry(long _row, double _coeff)
{
    ColumnEntry *new;

    new = (ColumnEntry *)malloc(sizeof(ColumnEntry));
    assert(new);

    new->row = _row;
    new->coeff = _coeff;
    new->next = NULL;

    return new;
}

void
pgAddColEntry(ColumnEntry **head, ColumnEntry *new)
{
    ColumnEntry *ceaux;
    assert(head);
    assert(new);

    new->next = *head;
    *head = new;
}

void 
pgFreeColEntry(ColumnEntry *cen)
{
    if(!cen)
        return;
    if(cen->next)
        pgFreeColEntry(cen->next);
    free(cen);
}
    
#define CS_INDEX(in) (num_ic - in + 2)

void
pgMPSBuildCurVarColumn(Circuit *ckt, IntConst *ilist)
{
    IntConst *iaux;
    Products *paux;
    ColumnEntry *caux;
    Branches  *daux;
    double    cst,vdrop;
    long    num_ic;
    assert(ilist);

    num_ic = ilist->index;

    /* we first go through the objective function */
    for(daux = ckt->theDeviceList; daux; daux = daux->next){
        if(daux->stat != sNormal)
            continue;

        if(daux->type != dRES)
            continue;
        
        vdrop = ckt->theNodeArray[daux->n1].voltage -
            ckt->theNodeArray[daux->n2].voltage;

        cst = daux->length*daux->length*
            daux->lay->unitRes/(vdrop + FTINY);

        caux = pgNewColEntry(1, cst);
        pgAddColEntry(&theVarArray[daux->index].head,caux);
    }
        

    /* then we go through the constraint list*/
    for(iaux = ilist; iaux; iaux = iaux->next){
        for(paux = iaux->prod_list; paux; paux = paux->next){
            assert(paux->branch);
            caux = 
            pgNewColEntry(CS_INDEX(iaux->index),
            paux->sign*paux->coeff);
            pgAddColEntry(&theVarArray[paux->branch->index].head,
                caux);
        }
    }
}

/************************************************************/
/************************************************************/

/*
**    print the ROW information 
*/
void
pgMPSPrintRows(FILE *fp, IntConst *ilist)
{
    IntConst *iaux;
    int     num_ic;

    assert(theVarArray);

    assert(fp);
    assert(ilist);

    /* first two rows are fixed */
    fprintf(fp,"ROWS\n");
    fprintf(fp,"  N %-5d\n", 1);

    num_ic = ilist->index;
    for(iaux = ilist; iaux ; iaux = iaux->next){
        if(iaux->type == EQ)
            fprintf(fp,"%-4s%-8d\n","  E", CS_INDEX(iaux->index));
        else if(iaux->type == GE || iaux->type == GT)
            fprintf(fp,"%-4s%-8d\n","  G", CS_INDEX(iaux->index));
        else{
            error_mesg(INT_ERROR,"Invalid internal constrain type");
        }
    }
}

/*
**    print the COLUMNS information
*/
void
pgMPSPrintColumns(Circuit *ckt, FILE *fp)
{
    long i;
    long count = 1,lvar;
    Branches *baux;
    double    coeff;
    ColumnEntry *caux;
    assert(theVarArray);

    fprintf(fp,"COLUMNS\n");

    for(i = 1; i <= ckt->nNumDevice; i++){
       if(theVarArray[i].bch){
        baux = theVarArray[i].bch;
        theVarArray[i].index = count++;
        for(caux = theVarArray[i].head; caux; caux = caux->next){
            if(baux->current > 0.0)
                coeff = caux->coeff;
            else
                coeff = -caux->coeff;
            fprintf(fp,"    X%-7d  %-8d  %-g\n",
                theVarArray[i].index, caux->row, coeff);
        }
        }
    }
}

/*
**    print the Right hand side information
*/
void
pgMPSPrintRHS(FILE *fp, IntConst *ilist)
{
    IntConst *iaux;
    int    num_ic;

    assert(fp);
    assert(ilist);

    /* first row is the objective */
    fprintf(fp,"RHS\n");

    num_ic = ilist->index;
    for(iaux = ilist; iaux ; iaux = iaux->next){
        fprintf(fp,"    %-8s  %-8d  %g\n","RHS",
            CS_INDEX(iaux->index), -iaux->cst);
    }
}

/*
**    Read back the result from lp sover 
*/
void
pgMPSReadCurResult(char *file)
{
  FILE *fp;
  char *pstr, *pstr1;
  char line[1024], backbuf[1024], cval[128];
  int    vindex;
  Branches *baux;
  double value;
  int    offset = 0;
  int    pos = 1;

  if(!file){
    error_mesg(IO_ERROR,"No result file name given.\n");
    return;
  }
 
  assert(theVarArray);

  if((fp = fopen(file,"r")) == NULL){
    sprintf(buf, "Can not open result file: %s.\n",file);
    error_mesg(IO_ERROR,buf);
    return;
  }

  while((pstr = fgets(line,1023,fp)) != NULL){
        
    strcpy(backbuf,line);
    sscanf(pstr,"%d %s",&vindex,cval);
    value = TransValue(cval);

    /* find the real index */
    while(!theVarArray[vindex+offset].bch) offset++;
        
    if(vindex+offset != theVarArray[vindex+offset].index){
      sprintf(buf,"Invalid offset: %d.",offset);
      error_mesg(INT_ERROR,buf);
      continue;
    }

    if(theVarArray[vindex].bch->current > 0)
      theVarArray[vindex+offset].bch->current = value;
    else
      theVarArray[vindex+offset].bch->current = -value;
            
  }
}
