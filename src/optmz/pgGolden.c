
/*
 * $RCSfile: pgGolden.c,v $
 * $Revision: 1.2 $
 * $Date: 2000/10/10 01:01:12 $,  (c) Copyright 1999 by X.-D. Sheldon Tan
 */

/*******************************************************************************
*                                                                              *
* Module name:                              Prefix: pg                         *
*                                           Author: XiangDong Tan              *
* Description:                                                                 *
* 	Perform one-dimension golden section search
*******************************************************************************/

#include "pgCircuit.h"
#include "pgOptmz.h"

#include <math.h>
#include <float.h>

#define R 0.6180339		/* the gloden ratio */
#define C (1.0 - R)
#define SHFT2(a, b, c) (a) = (b); (b) = (c)
#define SHFT3(a, b, c, d) (a) = (b); (b) = (c); (c) = (d)

static double pgOneDimensionCost(Circuit *ckt, double alpha);

double
pgGolden(Circuit *ckt,
	 double ax,	/* ax and cx define the given bracket */
	 double bx,	/* f(bx) should less than f(ax) and f(bx) */
	 double cx,
	 double (*f)(Circuit *, double),
	 double tol,
	 double *xmin)
{
  double f1, f2, x0, x1, x2, x3;
  
  x0 = ax;
  x3 = cx;

  if(fabs(cx-bx) > fabs(bx-ax))	/* make x0 and x1 the smaller segment */
     {
       x1 = bx;
       x2 = bx + C*(cx-bx);
     }
  else
    { 
      x2 = bx;
      x1 = bx - C*(bx- ax);
    }
  
  /*printf("x0 = %g, x1 = %g, x2 = %g, x3 = %g \n", x0, x1, x2, x3);*/
  
  f1 = (*f)(ckt,x1);		/* initial function evaluation */
  f2 = (*f)(ckt,x2);

  while(fabs(x3-x0) > tol*(fabs(x1) + fabs(x2)))
    {
      /* printf("f1 = %g, f2 = %g \n", f1, f2); */
      if(f2 < f1)		/* Out possible outcome */
	{
	  SHFT3(x0, x1, x2, R*x1 + C*x3); /* its housekeeping */
	  SHFT2(f1, f2, (*f)(ckt,x2));
	 }
      else			/* The other outcome */
	{
	  SHFT3(x3, x2, x1, R*x2 + C*x0);
	  SHFT2(f2, f1, (*f)(ckt,x1));
	}

      if(tol*(fabs(x1) + fabs(x2)) < FLT_MIN)
	break;
      /* printf("x0 = %g, x1 = %g, x2 = %g, x3 = %g \n", x0, x1, x2, x3); */
    }
  
  if(f1 < f2)			/* we are done. Output the best of the two */
    {				/* current value */
      *xmin = x1;
      return f1;
    }
  else 
    {
      *xmin = x2;
      return f2;
    }
}

/*
** It is a wrapper cost funciton. Alphin is belong [0,1]
 */
static double *dir;
static double *vol;

static double
pgOneDimensionCost(Circuit *ckt, double alpha)
{
  int i;
  double cost;
  
  assert(alpha >= 0);
  assert(alpha <= 1);
  
  for(i=0; i <= ckt->nMatrixSize; i++)
    {
      ckt->theNodeArray[i].voltage = vol[i] + alpha*dir[i]; 
    }
  cost = pgComputeCostFunct(ckt);
  /* printf("cost: %g \t", cost); */
  return cost;
}

/*
** Initialization
*/
static void
pgInitGoldenLineSearch(Circuit *ckt, double *vol_a, double *dir_a)
{
  int i;

  if(!vol)
    {
      vol = (double *)malloc((ckt->nMatrixSize+1)*sizeof(double));
      dir = (double *)malloc((ckt->nMatrixSize+1)*sizeof(double));
    }

  for(i=0; i <= ckt->nMatrixSize; i++)
    {
      vol[i] = vol_a[i];
      dir[i] = dir_a[i];
    }
}

/* 
   perform the gloden section search to find the minimum value in a
   given bracket.  DPTOL is the convergence tolerance for double
   procision(DP).
*/
#define DPTOL 3.0e-8
void
pgDoGoldenLineSearch(Circuit *ckt, double *vol_a, double *dir_a)
{
  double ax = 0, bx = R, cx = 1, xmin, cost;
  
  pgInitGoldenLineSearch(ckt, vol_a, dir_a);
  
  pgGolden(ckt, ax, bx, cx, pgOneDimensionCost, DPTOL, &xmin);
  
  printf("xmin: %g\n", xmin);
  
  cost = pgOneDimensionCost(ckt,xmin);
}

