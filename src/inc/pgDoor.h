/*
 * $RCSfile: pgDoor.h,v $
 * $Revision: 1.1 $
 * $Date: 1999/04/30 17:43:27 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

/* 
**    For parsing the raw PG network data from Milkway
*/

#include "pgCircuit.h"

#define PhyBox struct _phy_box

PhyBox {
    long    xmin, ymin;
    long    xmax, ymax;
    double    dx;
    double    dy;
    int    layer;
    PhyBox *next;
};

extern pgNewPhyBox(char*, char*, char*, char*, char*);
extern void pgFreePhyBox(PhyBox *);
extern void pgAddPhyBox(PhyBox **, PhyBox *);
extern int  pgIfCombinable(PhyBox *, PhyBox *);

/************************************************************/
/************************************************************/

/* the direction along which, the pysical resistor can be changed */
typedef enum {
    rX,    
    rY
} ResDirection;

#define PhyResistor struct _phy_resistor
PhyResistor {
    long        index;    /* the resistor count */
    long        n1;    /* converted node1 */
    long        n2;    /* converted node2 */
    double         dx;    /* delta in x direction */
    double         dy;     /* delta in y direction */
    int        layer;    /* layer index */
    PhyBox        *blist; /* physical parameters of the resistors */
    ResDirection    dir;    
    PhyResistor    *next;
};

extern void pgNewPhyResistor(char *, char *);
extern void pgNewPhyResInt(long n1, long n2);
extern void pgAddBoxIntoPhyRes(PhyResistor *, PhyBox *);
extern void pgFreePhyResistor(PhyResistor *); 
extern void pgAddPhyResistor(PhyResistor **, PhyResistor *);

extern PhyResistor *thePhyResList;
extern long theResCount;

/************************************************************/
/************************************************************/
#define IndpSource struct _indp_source
/*
**    for current source, positive node is the node from 
**    which the current is flowing into the current source.
**    for the voltage source, postive node is the potenitally 
**    positiv node.
*/
IndpSource { 
    long index;
    long pn; 
    long nn; 
    double    value;
    IndpSource *next;
};

extern void pgNewIndpSource(IndpSource **, char *pn, char *nn, char *cval);
extern void pgAddIndpSource(IndpSource **, IndpSource *);

extern IndpSource *theVolSource;
extern IndpSource *theCurSource;

/************************************************************/
/************************************************************/

/* beauxe we need the consecutive internal index, we
   have to generate a new set of node indices. 
   The address of theConvTable is the external node index
   and its content is the converted new index
*/

extern long *theConvTable;
extern long theIntNodeCount;

extern void pgConvTableInit(long);
extern int pgQueryNodeIndex(long);

/************************************************************/
/************************************************************/
extern void pgParsePhyNetList(char *);
extern void pgAddPhyParam(char *, char *);
extern void pgDumpSpiceFile(FILE *);
extern void pgDumpConstrainFile(FILE *);
extern void pgDumpCrossReference(FILE *);

extern void pgProcessPhyNetlist(char *fname);

/* global variable definition */

typedef enum {
    cGENERAL,
    cPOWER,
    cGROUND
} PGTYPE;

extern char *PGName;
extern PGTYPE pgType;     /* power or ground */
extern double unitLength;    /* unit of length */
extern double lengthPrecision;    /* length precision */
extern double unitCurrent;    /* unit of current */
extern double currentPrecision; /* current precision */ 
extern double unitResistance;    /* unit of resistance */
extern double resistancePrecision; /* resistance precision */

extern double theVoltage;    /* the voltage for power ground */
extern double theVolOffset;    /* the maxinum voltage divation allowed */ 
extern long   MaxExtNodeIndex;  /* maximum external node index */
extern long theCurCount;    /* the current count */
