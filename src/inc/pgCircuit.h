/*
 * $RCSfile: pgCircuit.h,v $
 * $Revision: 1.3 $
 * $Date: 2000/10/10 01:00:04 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

/*
**    Parsing and building of MNA matrix from SPICE netlist
*/

#ifndef __CIRCUIT_
#define __CIRCUIT_

#include "pgCommon.h"
#include "spmatrix.h"

/* data structure definitions */

/* device type definition */
 
/* now we only suppert three device types */
typedef enum {
  dRES,    /* resistor */
  dCUR,    /* independent current */
  dVOL     /* independent voltage */
} DeviceType;

typedef enum {
  sNormal, /* normal device */
  sAbnormal /* abnormal device */
} DevStat;

/************************************************************/
/************************************************************/

#define Layer struct _layer
 
Layer {
  char    *name;
  int     index;
  double  unitRes;        /* unit (sheet) resistance */
  double  minWidth;       /* minimum width required */
  double  maxCurDen;      /* maximum current density */
  Layer   *next;
};
 
extern void pgNewLayer(char *, char *, char *, char *, char *);
extern void pgFreeLayer(Layer *);
extern void pgAddLayer(Layer **, Layer *);
extern Layer *pgFindLayerByIndex(int);
 
extern Layer *theLayerTable;

/************************************************************/
/************************************************************/

#define Branches struct _branches
#define BranchLink struct _branchlist
#define Nodes struct _nodes
#define Circuit struct _circuit

#define ExtConst struct _const_rec
#define IntConst struct _intconst


Circuit {

  char *name;

  Branches   *theDeviceList; /* device list */
  Nodes      *theNodeArray;		/* node array */
  BranchLink *theBranchArray;

  ExtConst  *theExtConstList; /* the external constaint list */

  /* constraint list for linear programming */
  IntConst  *LPIntConstList; 
  
  /* constraint lisr for non linear programming */
  IntConst  *NLPIntConstList;

  int nNumDevice; /* number of passitive devices */
  int nNumBranch; /* number of branches (active & passitive) */
  int nNumNode;   /* number of nodes in the circuits */
  int nMatrixSize; /* MNA matrix size */
};

extern Circuit *theCkt, *theEquCkt;
void pgPrintCktStatistic(Circuit *);
void pgPrintState(Circuit *ckt);

/*
**    Note that the current direction is 
**    from n1 to n2.
*/

Branches {
  char       *name;		/* name of the device */
  int        n1;		/* postive node */
  int        n2;		/* negative node */
  int        index;		/* the index of the branch */
  DeviceType type;		/* device type */
  double     value;		/* corresponding numerical value */
  DevStat    stat;		/* normal or abnormal */
  double     length;		/* length of the resistor */
  double     width;		/* width of the resistor */
  double     current;		/* current in this branches */ 
  double     vdrop;		/* voltage drop */
  int        sign;		/* sign of the branch */    
  int        vn;		/* node index in MNA for voltage source */ 
  int        layer;		/* physical layer of the branch */
  Layer      *lay;		/* more info about layer */
  Branches   *next;
  
  /* electromigration constrain */
  double        pI;		/* positive current */
  double        nI;		/* negative current */
  double        wmin;		/* minimum width requirement from EM */

  /* information about condensed branch */
  BranchLink    *origBranches;	 /* list of original resistant branches */
  Branches      *curN1;		/* the equivalent current at n1 */
  Branches      *curN2;		/* the equivalent current at n2 */
  double        oldRs;		/* new value of resistant value */
};

/* 
   function declaration related to Branches
   in file ckt/pgCircuit.c 
*/

extern int pgDCAnalysis(Circuit *);
extern Circuit * pgParseCircuit( char* );
extern int pgBuildMNAEquation(Circuit *);
extern void pgPrintResult(double*, int );
extern void pgAddDevice(Circuit *, Branches *);
extern void pgAddDeviceNCheck(Circuit *ckt, Branches *pdev);
extern void pgFreeDeviceList(Branches*);
extern Branches *pgFindBranchByName(Circuit *, char *);
extern Branches *pgFindBranchByIndex(Circuit *, int);
extern void pgFindLayerInfor(Circuit *);

/************************************************************/
/************************************************************/

/*
**    BranchLink definition. It is used as
**    linked list for representing the branches
**    that a node is connects.
*/


BranchLink {
  Branches     *dev;    /* the corresponding device */
  BranchLink     *next;
};

extern BranchLink *pgNewBranch( Branches *);
extern void pgAddBranch(BranchLink **list, BranchLink *new);
extern void pgFreeBranchLink( BranchLink *);

extern void pgBuildBranchQuickIndex(Circuit *);
extern BranchLink * pgNewBranchLink(Branches *branch);
extern void pgAddBranchLink(BranchLink **blist, BranchLink *new);

/************************************************************/
/************************************************************/



typedef enum {
  nNormal, /* normal node */
  nDangling /* dangling node */
} NodeStat;

Nodes {
  int        index;     /* node index */
  int        visited;   /* if visited? */
  BranchLink *blist;    /* branches the node connencts */
  NodeStat   stat;      /* state */
  int        isVDD;     /* is the node connected to power supply? */
  int        num_branch;  /* number of branches on the node */
  double     voltage;     /* node voltage */
  double     coeff;       /* voltage coefficient in objective func*/ 
};

/* function declarations */
extern void pgBuildNodeArray(Circuit *);
extern void pgFreeNodeArray(Circuit *);
extern void pgPrintNodeVoltage(Circuit *);
extern void pgPrintBranchState(Circuit *);
extern void pgReadCurLPResult(Circuit *ckt, char *);
extern void pgReadVolLPResultAndLineSearch(Circuit *, char *);
extern void pgReadBVolLPResult(Circuit *, char *);
extern void pgCheckDanglingNode(Circuit *);

/************************************************************/
/************************************************************/

/* some global variables declaration */
extern char *theMatrix; /* the matrix equation */
extern double *theRhs; /* the right hand side vector */
extern double *theSol; /* the soluation vector */
extern double *orgSol; /* keep the dc solution */

#endif __CIRCUIT_



