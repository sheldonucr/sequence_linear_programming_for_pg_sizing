/*
 * $RCSfile: pgOptmz.h,v $
 * $Revision: 1.3 $
 * $Date: 2000/10/10 01:00:04 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */


/*
    Head file for routines related to P/G optimization
    For constaint file format, please check the detailed 
    format description.
*/

#ifndef __PG_OPT_
#define __PG_OPT_

#include "pgCircuit.h"

/* data structure definition */

/************************************************************/
/************************************************************/

typedef enum {
  ctUnknown,
  GT, /* > */
  GE, /* >= */
  LS, /* <= */
  LE, /* < */
  EQ  /* == */
} ConstType;

typedef enum {
  optSLP, /* using seqence of lienar programming */
  optConjugate /* using conjugate gradient method */
} OptMode;

/* the constrain variable has two types */

typedef enum {
  cVOL,     /* voltage */
  cWIDTH     /* width variable */
} ConstTopic;

/* constraint status */
typedef enum {
  csUnknown,
  csSatisfied,
  csViolated
} ConstStat;

  /************************************************************/
  /************************************************************/

#define ExtConst struct _const_rec

ExtConst {
  ConstTopic  topic; /* constrain type -- voltage or width */
  ConstType   type; /* ">=" or "==" etc. */
  ConstStat   stat; /* satisfied or violated ? */
  char     *bname1; /* branch1 name */
  char     *bname2; /* branch2 name */
  int      node1;    /* index for var1 */
  int      node2;    /* no used */
  double    value; /* the second varible maybe a numerical value */
  ExtConst *next;
};

/* routines related to ExtConst  */
extern void pgReadConstFile(Circuit *ckt, char *filename );
extern ExtConst *pgNewConst();
extern void pgDestroyConst(ExtConst *const);
extern void pgAddExtConst( Circuit *ckt, ExtConst *const );

/************************************************************/
/************************************************************/

#define Products struct _products

Products {
  int    sign;    /* sign of the product */
  double    coeff;     /* numerical coefficient */
  Branches *branch; /* != NULL if the variable is a branch current */ 
  Nodes     *node;   /* != NULL if the variable is the node voltage */ 
  Products *next;    
};

/* routines related to Products  */
extern Products *pgNewProductByBranch(int sign, double coeff, Branches *bch);
extern Products *pgNewProductByNode(int sign, double coeff, Nodes *np);
extern void pgFreeProduct(Products*);
extern void pgPrintProduct(FILE *, Products*);
extern void pgPrintBVolProduct(FILE *, Products*);

/************************************************************/
/************************************************************/

/* internal constraints definition.
** we only have two type internal constrains.
** (1)    ax1 + bx2 + cx3 >= 0
** (2)  ax1 + bx2 + cx3 = 0 
*/

#define IntConst struct _intconst

IntConst {
  long        index; /* internal index */
  ConstType   type; /* only EQ pr GE */
  ConstStat   stat; /* satisfied or violated */
  Products    *prod_list; /* product list */
  double        cst; /* the numerical constant */
  IntConst    *next;
};

/* routines related to IntConst  */

extern IntConst *pgNewIntConst( ConstType, long );
extern void pgFreeIntConst(IntConst *);

extern void pgBuildIntConst(Circuit *ckt);
extern void pgBuildNLPConst(Circuit *ckt, ExtConst *);
extern void pgBuildCurLPConst(Circuit *ckt, ExtConst *, OptMode);
extern void pgBuildVolLPConst(Circuit *ckt, ExtConst *);


extern void pgAddProduct(IntConst*, Products*);
extern void pgAddIntConst(IntConst**, IntConst*);
extern void pgAddNumConst(IntConst*, double);
extern double pgComputeIntConstValue(IntConst*);
extern int pgCheckAllIntConst(IntConst*);

extern void pgPrintIntConst(FILE *, IntConst*);
extern void pgPrintCurLPObjFunc(Circuit *ckt, FILE *);
extern void pgPrintVolLPObjFunc(Circuit *ckt, FILE *);
extern void pgPrintLPData(Circuit *ckt, FILE *);
extern void pgPrintAllIntConst(FILE *fp, IntConst *iclist);

/************************************************************/
/************************************************************/

/* function declarations in file: pgCostFunc.c */
extern double pgComputeCostFunct(Circuit *ckt);
extern double pgComputePenaltyCostFunct(Circuit *ckt);
extern void pgPrintLPCostFunct(Circuit *ckt, FILE *);
extern void pgCostFuncGradient(Circuit *ckt, double *grad);

/* function declarations in file: pgConjudate.c */
extern void pgConjudateOptmz(Circuit *ckt, int *iter,
			     double *fmin, double ftol);
extern void pgGetVoltageVector(Circuit *ckt, double *vol);
extern void pgSetVoltageVector(Circuit *ckt, double *vol);
extern void pgLineMin(Circuit *ckt, double *vv, double *xi, int n, double *fmin);
extern void pgMinBracket(Circuit *ckt, double *ax, double *bx, double *cx );
extern double pgParabolicLine(Circuit *ckt, double ax, double bx, double cx,
			      double tol, double *xmin);
extern double pgOneDimCostFunct(Circuit *ckt, double x); 
extern int pgCheckStep(Circuit *ckt, double);

/* function declaration in file: pgGolden.c */
extern void pgDoGoldenLineSearch(Circuit *ckt, double *vol_a, double *dir_a);
extern double pgGolden(Circuit *ckt,
	 double ax,	/* ax and cx define the given bracket */
	 double bx,	/* f(bx) should less than f(ax) and f(bx) */
	 double cx,
	 double (*f)(Circuit *,double),
	 double tol,
	 double *xmin);

/* function declarations in file: pgState.c */
extern int pgComputeStateFromRes(Circuit *);
extern void pgComputeResFromState(Circuit *);
extern void pgComputeResFromBVolState(Circuit *);
extern void pgCompWidthFromRes(Circuit *);

/* function declarations in file: pgMainLoop.c */ 
extern void pgMainLoop(Circuit *ckt, OptMode);
extern void pgInitSolution(Circuit *ckt);
extern void pgNLOptimizer(Circuit *ckt,double *);
extern void pgCurLOptimizer(Circuit *ckt, double *, OptMode);
extern void pgVolLOptimizer(Circuit *ckt, double *);
extern void pgVolLOptimizerLPS(Circuit *ckt,double *);
extern void pgCurLOptimizerLPS(Circuit *ckt,double *, OptMode);
extern void pgCurLOptimizerMPS(Circuit *ckt,double *, OptMode);
extern void pgVolLOptimizerMPS(Circuit *ckt,double *);

/* function declarations in file: pgNewton.c */ 
extern void pgBuildHessianMatrix(Circuit *ckt);
extern void pgNewtonOptmz(Circuit *ckt, int *, double *, double );

/* some global variables declaration */
extern double theResistivity;	/* the resistivity */ 
extern double theEMConst;	/* the eletromigration constraint */
extern double PenalWeight;	/* the postive constant for panelty function */
extern double RestrFactor;	/* restrication factor */ 
extern double EqualPanalFactor; /* penality factor for the equal width constraint */

#endif __PG_OPT_
