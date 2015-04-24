/*
 * $RCSfile: pgParseNetlist.c,v $
 * $Revision: 1.2 $
 * $Date: 2000/06/14 17:05:28 $, (c) Copyright 1999 by X.-D. Sheldon, Tan
 */

/*
**    Read the technology file and physical p/g netlist.
*/

#include "pgCommon.h"
#include "pgDoor.h"

char *PGName;
PGTYPE pgType;
double unitLength;
double lengthPrecision;
double unitCurrent;
double currentPrecision;
double unitResistance;
double resistancePrecision;
double theVoltage;
double theVolOffset;
long   MaxExtNodeIndex;

PhyResistor *thePhyResList;
Layer *theLayerTable;
IndpSource *theVolSource;
IndpSource *theCurSource;
long *theConvTable;
long theIntNodeCount;
long theResCount;
long theCurCount;

 
void
pgProcessPhyNetlist(char *fname)
{
    FILE *fp;
    pgParsePhyNetList(fname);

    /* spice netlist */
    sprintf(buf,"%s.sp",fname);
    fp = fopen(buf,"w");
    if(!fp){
        perror(buf);
        return;
    }
    printf("Writing %s ...", buf);
    pgDumpSpiceFile(fp);
    fclose(fp);
    printf(" done.\n");

    /* the constraint file */
    sprintf(buf,"%s.cs",fname);
    fp = fopen(buf,"w");
    if(!fp){
        perror(buf);
        return;
    }
    printf("Writing %s ...", buf);
    pgDumpConstrainFile(fp);
    fclose(fp);
    printf(" done.\n");
}

/*
**    main parse function.
*/
void pgParsePhyNetList(char *inf)
{
    extern FILE *yyin;
 
    if(!(yyin = fopen(inf, "r")))
        {
        perror(inf);
    return;
        }
    
    theResCount = 1;
    MaxExtNodeIndex = 0;
    theIntNodeCount = 2;
    theCurCount = 1;
 
    /*
    sprintf(buf, "Parsing %s ... ", inf);
    print_mesg(buf);
    */
    printf("Parsing %s ... ", inf);
    yyparse();
    /*
    print_mesg("ok.\n");
    */
    printf("ok.\n");
    fclose(yyin);

    if(!MaxExtNodeIndex){
    error_mesg(IO_ERROR,"MaxExtNodeIndex = 0");
    return;
    }

}


void
pgAddPhyParam(char *name, char *cval)
{
    if(!name || !cval)
        return;
    
    if(!strcmp(name,"PGName"))
        PGName = CopyStr(cval);
    else if(!strcmp(name,"PGType")){
        if(!strcmp(ToLower(cval),"power"))
            pgType  = cPOWER;
        else if(!strcmp(ToLower(cval),"ground"))
            pgType = cGROUND;
        else{
            sprintf(buf,"%s: unknown p/g type",cval);
            error_mesg(PARSE_ERROR,buf);
        }
    }
    else if(!strcmp(name,"unitLengthName")){
        if(!strcmp(cval,"micron"))
            unitLength = 1e-6;
        else
            unitLength = 1;
    }
    else if(!strcmp(name,"lengthPrecision"))
        lengthPrecision = TransValue(cval);
    else if(!strcmp(name,"theVoltage"))
        theVoltage = TransValue(cval);
    else if(!strcmp(name,"theVolOffset")) 
        theVolOffset = TransValue(cval);
    else if(!strcmp(name,"unitCurrentName")){
        if(!strcmp(cval,"mA"))
            unitCurrent = 0.001;
        else
            unitCurrent = 1;
    }
    else if(!strcmp(name,"currentPrecision")) 
        currentPrecision = TransValue(cval);
    else if(!strcmp(name,"unitResistanceName")){
        if(!strcmp(cval,"kohm"))
            unitResistance = 1000;
        else
            unitResistance = 1;
    }
    else if(!strcmp(name,"resistancePrecision")) 
        resistancePrecision = TransValue(cval);
    else if(!strcmp(name,"maxExtNodeIndex")) {
        MaxExtNodeIndex = atol(cval);
        pgConvTableInit(MaxExtNodeIndex);
    }
    else{
        sprintf(buf,"%s: unknown p/g parameter",cval);
        error_mesg(PARSE_ERROR,buf);
    }
        
}


/************************************************************/
/* routine related to the layer table */
/************************************************************/

void 
pgNewLayer(
char *name, 
char *index, /* lader index */
char *ures,  /* resistivity */ 
char *mwid,  /* minimum width */ 
char *mcur   /* current density */
)
{
    Layer *lay;

    lay = (Layer *)malloc(sizeof(Layer));
    assert(lay);

    lay->name = CopyStr(name);
    lay->index = atoi(index);
    /*
    printf("index: %d\n",lay->index);
    */
    lay->unitRes = TransValue(ures);
    lay->minWidth = TransValue(mwid); 
    lay->maxCurDen = TransValue(mcur); 
    lay->next = NULL;

    pgAddLayer(&theLayerTable, lay);
}

void
pgAddLayer(Layer **list, Layer *new)
{
    if(!list || !new)
        return;
    if(!*list)
        *list = new;
    else {
        new->next = *list;
        *list = new;
    }
}

void
pgFreeLayer(Layer *lay)
{    
    if(!lay)
        return;
    if(lay->next)
        pgFreeLayer(lay->next);
    free(lay->name);
    free(lay);
}

Layer *pgFindLayerByIndex(int ind)
{
    Layer *laux;

    for( laux = theLayerTable; laux; laux = laux->next){
        if(laux->index == ind)
            return laux;
    }

    return NULL;
}
    
/************************************************************/
/* routines related to the PhyBox */
/************************************************************/

static PhyResistor *curRes;

pgNewPhyBox(char *lay,char *xmin, char *ymin, char *xmax, char *ymax)
{
    PhyBox *box;
    long    node1, node2;
    box = (PhyBox *)malloc(sizeof(PhyBox));
    assert(box);

    box->xmin = TransValue(xmin);
    box->ymin = TransValue(ymin);
    box->xmax = TransValue(xmax);
    box->ymax = TransValue(ymax);
    box->dx = (double)(box->xmax - box->xmin);
    box->dy = (double)(box->ymax - box->ymin);
    box->layer = atoi(lay);
    box->next = NULL;

    assert(curRes);
    if(pgIfCombinable(curRes->blist,box))
        pgAddBoxIntoPhyRes(curRes, box);
    else{ /* new resistor generated */
        /* first, we create a new node */
        node1 = pgQueryNodeIndex(-1);
        node2 = curRes->n2;
        curRes->n2 = node1;
        pgNewPhyResInt(node1, node2);
        pgAddBoxIntoPhyRes(curRes,box);
    }
}

void
pgAddPhyBox(PhyBox **list, PhyBox *new)
{
    if(!list || !new)
        return;
    if(!*list)
        *list = new;
    else {
        new->next = *list;
        *list = new;
    }
}

int
pgIfCombinable(PhyBox *b1, PhyBox *b2)
{
    if(!b2)
        return 0;
    if(!b1) /* new resistor */
        return 1;
    if(b1->layer != b2->layer)
        return 0;
    if(b1->dx == b2->dx || b1->dy == b2->dy )
        return 1;
    return 0;
}

/************************************************************/
/* routines for internal and external node transformation */
/************************************************************/
void pgConvTableInit(long max_size)
{
    long i;

    if(theConvTable)
        free(theConvTable);
    theConvTable = (long *)malloc((max_size+1)*sizeof(long));

    assert(theConvTable);

    for(i = 0; i <= max_size; i++)
        theConvTable[i] = -1;

    /* since we transfer power network into ground, 0, which
       indicate the power pad will be be ground node(0)
    */
    theConvTable[0] = 0;
}

int pgQueryNodeIndex(long extnode)
{
    assert(theConvTable);

    /* -1 means a new internal node */
    if(extnode == -1)
        return theIntNodeCount++;

    /* -2 means the ground node (indexed 0 in SPICE) */
    if(extnode == -2)
        return 0;

    if(extnode > MaxExtNodeIndex){
        sprintf(buf,"%ld > MaxExtNodeIndex(%ld).",
        extnode,MaxExtNodeIndex);
        error_mesg(FAT_ERROR,buf);
        exit(-1);
    }
    if(theConvTable[extnode] != -1)
        return theConvTable[extnode];
    else
        theConvTable[extnode] = theIntNodeCount++;
    return theConvTable[extnode];
}
    

/************************************************************/
/* routines related to the Indenpendent sources */
/************************************************************/

void
pgNewIndpSource(IndpSource **list, char *pn, char *nn, char *cval)
{
    long ext1, ext2;
    IndpSource *iaux;

    ext1 = atol(pn);
    ext2 = atol(nn);
    
    iaux = (IndpSource *)malloc(sizeof(IndpSource));
    assert(iaux);

    iaux->value = TransValue(cval);

    /* check current with 0 value */
    if(iaux->value == 0.0){
        free(iaux);
        return;
    }

    iaux->pn = pgQueryNodeIndex(ext1);
    /*
    printf("ext: %ld, conved: %ld. \n",ext1, iaux->pn);
    */
    iaux->nn = pgQueryNodeIndex(ext2);
        
    iaux->next = NULL;
    iaux->index = theCurCount++;
    pgAddIndpSource(list,iaux);
}

void
pgAddIndpSource(IndpSource **list, IndpSource *new)
{
    if(!list || !new)
        return;
    if(!*list)
        *list = new;
    else {
        new->next = *list;
        *list = new;
    }
}

/************************************************************/
/* routines for PhyResisitor */
/************************************************************/

/* the this function, we also change
   value of the curRes.
*/
void
pgNewPhyResistor(char *en1, char *en2)
{
    PhyResistor *paux;
    long        node;
    long        n1, n2;

    paux = (PhyResistor *)malloc(sizeof(PhyResistor));
    assert(paux);

    n1 = atol(en1);
    n2 = atol(en2);
    paux->index = theResCount++;
    paux->n1 = pgQueryNodeIndex(n1);
    paux->n2 = pgQueryNodeIndex(n2);
    paux->blist = NULL;
    paux->dir = rX;
    paux->next = NULL;

    curRes = paux;
    pgAddPhyResistor(&thePhyResList, paux);

}

/*
**    the argument is converted nodes
*/
void
pgNewPhyResInt(long n1, long n2)
{
    PhyResistor *paux;
    long        node;

    paux = (PhyResistor *)malloc(sizeof(PhyResistor));
    assert(paux);

    paux->index = theResCount++;
    paux->n1 = n1;
    paux->n2 = n2;
    paux->blist = NULL;
    paux->dir = rX;
    paux->next = NULL;

    curRes = paux;
    pgAddPhyResistor(&thePhyResList, paux);
}


void 
pgAddPhyResistor(PhyResistor **list, PhyResistor *new)
{
    if(!list || !new)
        return;
    if(!*list)
        *list = new;
    else {
        new->next = *list;
        *list = new;
    }
}

void 
pgFreePhyResistor(PhyResistor *reslist)
{
    if(!reslist)
        return;
    if(reslist->next)
        pgFreePhyResistor(reslist->next);
    free(reslist);
}

/*
**    We assume the the box has the same layer
**    as the boxes in the resistor and is also
**    combinable with these boxes.
*/
void
pgAddBoxIntoPhyRes(PhyResistor *res, PhyBox *box)
{
    long width;
    long length;
    PhyBox *prev;

    assert(res);
    if(!box)
        return;
    prev = res->blist;

    res->layer = box->layer;
    if(!prev){

        res->layer = box->layer;

        if(box->dx > box->dy)
            res->dir = rY;
        else
            res->dir = rX;
        res->dx = box->dx;
        res->dy = box->dy;
    }
    else {
        if(box->dx == prev->dx)
            res->dy += box->dy;
        else 
            res->dx += box->dx;

        if(box->dx > box->dy)
            res->dir = rY;
        else
            res->dir = rX;
    }
        
    pgAddPhyBox(&(res->blist),box);
}
