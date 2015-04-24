
/*
 * $RCSfile: pgConjugate.c,v $
 * $Revision: 1.3 $
 * $Date: 2000/10/10 01:01:12 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

/*******************************************************************************
*                                                                              *
* Module name:                              Prefix: pg                         *
*                                           Author: XiangDong Tan              *
* Description:                                                                 *
*    Implementation of Conjudate gradient method. 
*    and 1-dimentional line search algorithms.
*******************************************************************************/

#include <math.h>
#include "pgOptmz.h"

/*
**    ITMAX is the maximum allowed number of iteration,
**    while EPS is a small number to rectify the special
**    case of converging to exactly zero function value.
**    DPTOL is the convergence tolerance for double 
**    procision(DP).
*/

#define ITMAX 100000
/*
#define ITMAX MAX(200,nMatrixSize)
#define ITMAX 100000
*/
#define EPS 1.0e-10
#define DPTOL 3.0e-8

double pxx;


/*
**      Obtain the voltage vector.
**      The vector space has been allocated with
**      size of nMatrixSize+1.
*/
 
void pgGetVoltageVector(Circuit *ckt, double *vol)
{
  int i;
  assert(vol);
  assert(ckt->theNodeArray);
 
  for(i=0; i <= ckt->nMatrixSize; i++)
    vol[i] = ckt->theNodeArray[i].voltage;
}

/*
**      Set back the voltage vector.
**      The vector space has been allocated with
**      size of nMatrixSize+1.
*/
 
void pgSetVoltageVector(Circuit *ckt, double *vol)
{
  int i;
  assert(vol);
  assert(ckt->theNodeArray);
 
  for(i=0; i <= ckt->nMatrixSize; i++)
    ckt->theNodeArray[i].voltage = vol[i];
}

/*
**    Perform the Flecher-Reeves-Polak-Ribiere 
**    conjudate gradient minimization.
**    iter: number of iterations were performed.
**    fmin: minimum value of the cost function.
**    ftol: convergence tolerance on the function value.
*/

void pgConjudateOptmz(Circuit *ckt , int *iter, double *fmin, double ftol)
{
	int i, its;
	double gg, gam, fvv, dgg;
	double *g, *h, *xi,*vv, prev;

	/* initialization */
	vv = (double *)malloc((ckt->nMatrixSize+1)*sizeof(double));
	h = (double *)malloc((ckt->nMatrixSize+1)*sizeof(double));
	g = (double *)malloc((ckt->nMatrixSize+1)*sizeof(double));
	xi = (double *)malloc((ckt->nMatrixSize+1)*sizeof(double));

 loop:
	fvv = prev = pgComputePenaltyCostFunct(ckt);
	/* printf("fvv %g \n", fvv); */
	pgGetVoltageVector(ckt, vv);
	pgCostFuncGradient(ckt, xi);

	for(i = 1; i <= ckt->nMatrixSize; i++)
    {
		g[i] = -xi[i];
		xi[i] = h[i] = g[i];
		/*
		  printf("xi[%d]: %g\n",i,xi[i]);
		*/
    }

	/* initial step size */
	pxx = 0.1;

	for( its = 1; its <= ITMAX; its++)
    {  /* loop over iterations */
		*iter = its;
		pgLineMin(ckt, vv, xi, ckt->nMatrixSize,fmin); 
		/*
		  if(2.0*fabs(*fmin-fvv) <= ftol*(fabs(*fmin) + fabs(fvv) + EPS)) {
		  printf("fmin: %g diff: %g\n",*fmin, *fmin-fvv);
		  pgSetVoltageVector(vv);
		  free(vv); free(xi); free(h); free(g);
		  return;
		  }
		*/
		/* printf("fmin: %g \n", *fmin); */
		if(2.0*fabs(*fmin-fvv) <= ftol*(fabs(*fmin) + fabs(fvv) + EPS))
		{
			break;
			/*
			  if(its == 1)
			  break;
			  else
			  {
			  pgSetVoltageVector(vv);
			  goto loop;
			  }
			*/
		}

		fvv = *fmin;
		pgSetVoltageVector(ckt, vv);
		/* printf("\nCOST: %lf\n", pgComputeCostFunct());*/
		/* printf("PENALTY COST: %lf\n", fvv); */
		/*
		  pgPrintNodeVoltage(ckt);
		*/
		pgCostFuncGradient(ckt,xi);
		dgg = gg = 0.0;
		for( i = 1; i <= ckt->nMatrixSize; i++)
		{
			gg +=g[i]*g[i];

			/* Fletcher-Reeves method */
			/* dgg += xi[i]*xi[i]; */

			/* Polak-Ribiere */
			dgg += (xi[i] + g[i])*xi[i];
		}
		if( gg == 0.0 ) 
		{
			pgSetVoltageVector(ckt, vv);
			free(vv); free(xi); free(h); free(g);
			printf("Optimium point is found.!!!\n");
			return;
		}
		gam = dgg/gg;
		for( i=1; i <= ckt->nMatrixSize; i++ )
		{
			g[i] = -xi[i];
			xi[i] = h[i] = g[i] + gam*h[i];
		}
    }
	/*
	  if(1)
	  {
      printf("fmin: %g, fvv: %g \n", *fmin, fvv);
	  }
	*/

	pgSetVoltageVector(ckt, vv);
	printf("iterations: %d reached\n ",its );
	if(2.0*fabs(*fmin-prev) <= ftol*(fabs(*fmin) + fabs(prev) + EPS) ||
	   *fmin > prev )
    {
		free(vv); free(xi); free(h); free(g);
		return;
    }
	else 
    {
		printf("\nCOST: %lf\n", pgComputeCostFunct(ckt));
		printf("PENALTY COST: %lf\n", fvv); 
		printf("min: %g, prev: %g\n", *fmin, prev);
		sprintf(buf,"Reinitialize the conjudage direction");
		print_warn(buf);
		prev = *fmin;
		goto loop;
    }
}

/*
**    1-dimentional line minimazation algorithm.
**    The algorithm assume the exisitance of 
**    1-dimentional cost function. 
**    It use the Parabolic inerpolation method
**    to locate the minimum with the double precision
**
**    vv: initial vector and obtained value for minimum point 
**    xi: search direaction
**    n:  dimention
**    fmin: minimum valut obtaint.
**    
*/

static int ncom;
static double *pcom, *xicom;

void pgLineMin(Circuit *ckt, double *p, double *xi, int n, double *fmin)
{
    
  int i;
  double    xx, xmin, fx, fb, fa, bx, ax;
  ncom = n;

  /* initalize the global variable */
  ncom = n;
  pcom = (double *)malloc((n+1)*sizeof(double));
  xicom = (double *)malloc((n+1)*sizeof(double));

  /* define the static vaibles */
  for( i = 1; i <= n; i++)
    {
      pcom[i] = p[i];
      xicom[i] = xi[i];
    }

  /* initial guess for the brackets */
  ax = 0.0;
  xx = pxx;
  *fmin = pgOneDimCostFunct(ckt, 0.0);
  if(!pgCheckStep(ckt, xx))
    {
      xx = xx/10;
      /*
	printf("In line search: try step = %g\n", xx);
      */
      while(!pgCheckStep(ckt,xx))
	{
	  /*
	    printf("In line search: try step = %g\n", xx);
	  */
	  xx = xx/10;
	  if(xx < 1e-50)
	    {
	      free(xicom);
	      free(pcom);
	      return;
	    }
	}
    }
  else
    {
      xx = xx*10;
      /*
	printf("In line search: try step = %g\n", xx);
      */
      while(pgCheckStep(ckt,xx))
	{
	  /*
	    printf("In line search: try step = %g\n", xx);
	  */
	  xx = xx*10;
	  if(xx > 0.1)
	    {
	      break;
	      /*
	      free(xicom);
	      free(pcom);
	      return;
	      */
	    }
	}
      xx = xx/10;
    }
  pxx = xx;
  /* printf("final step: %g\n", xx); */
  /*pgPrintNodeVoltage(ckt);*/

  /*
    pgMinBracket(ckt, &ax, &xx, &bx);
    *fmin = pgParabolicLine(ckt, ax, xx, bx, DPTOL, &xmin);
    */

  *fmin = pgParabolicLine(ckt, ax, 0.5*(xx-ax) + ax, xx, DPTOL, &xmin); 
  /**fmin = pgGolden(ckt, ax, 0.5*(xx-ax) + ax, xx, pgOneDimCostFunct , DPTOL, &xmin); */
  /* construct the results to return */ 
  for( i = 1; i <= n; i++)
    {
      xi[i] *= xmin;
      p[i] += xi[i];
    }
  free(xicom);
  free(pcom);
}

/*
**    The 1-dimentional cost function.
**    it must accompany pgLineMin() 
**    or pgLineMinD().
*/


double pgOneDimCostFunct(Circuit *ckt, double x)
{
  int i;
  double f, *xt;

  xt = (double *)malloc((ncom+1)*sizeof(double));

  for(i=1; i<=ncom; i++) 
    xt[i] = pcom[i] + x*xicom[i];

  pgSetVoltageVector(ckt, xt);
  f = pgComputePenaltyCostFunct(ckt);
  free(xt);
  return f;
}


/*
** check the such step is feasible or not
*/

int pgCheckStep(Circuit *ckt, double x)
{
  int i, result;
  double *xt;
  double cmax = 0.0, cval;

  xt = (double *)malloc((ncom+1)*sizeof(double));

  for(i=1; i<=ncom; i++)
    { 
      xt[i] = pcom[i] + x*xicom[i];
      /*
	if(fabs(x*xicom[i]) > cmax)
	{
	cmax = fabs(x*xicom[i]);
	cval = pcom[i];
	}
      */
    }
  /*
    printf("max change: %g, orign: %g \n",cmax, cval);
  */

  pgSetVoltageVector(ckt, xt);
  result = pgCheckAllIntConst(ckt->NLPIntConstList);
  free(xt);
  return result;
}

/*    
**    GLIMIT is the maximum maginifcation allowed for a 
**    parabolic-fit step.
*/

#define GOLD 1.618034
#define GLIMIT 100.0
#define TINY 10e-20
#define SHFT(a, b, c, d) (a) = (b); (b) = (c); (c) = (d);
#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))

/*
**    Routine for initially bracketing a mimimum
*/

extern void pgMinBracket(Circuit *ckt, double *ax, double *bx, double *cx )
{
  double fa, fb, fc;
  double ulim, u, r, q, fu, dum;

  fa = pgOneDimCostFunct(ckt, *ax);
  fb = pgOneDimCostFunct(ckt, *bx);

  /* switch roles of a and b so that we can
     go downhill in the direction from a to b
  */
  if(fb > fa)
    { 
      SHFT(dum, *ax, *bx, dum);
      SHFT(dum, fa, fb, dum);
    }

  /* first guess for c */
  *cx = (*bx) + GOLD*(*bx - *ax);
  fc  = pgOneDimCostFunct(ckt, *cx);

  /* keep running until we bracket */
  while( fb > fc )
    {
      /* compute u by parabolic extrpolation 
	 from a, b, c */
      r = (*bx - *ax)*(fb - fc);
      q = (*bx - *cx)*(fb - fa);
      u = *bx - ((*bx - *cx)*q - (*bx - *ax)*r)/
	(2.0*SIGN(MAX(fabs(q-r),TINY),q-r));
      ulim = (*bx) + GLIMIT*(*cx-*bx);

      /* Parabolic u is between b and c: try it */    
      if ((*bx - u)*(u - *cx) > 0.0)
	{ 

	  fu = pgOneDimCostFunct(ckt, u); 

	  /* got a minimum between b and c */
	  if( fu < fc)
	    {
	      *ax == *bx;
	      *bx = u;
	      fa = fb;
	      fb = fu;
	      return;
	      /* Got a minimum between a and u */
	    } 
	  else if (fu > fb)
	    { 
	      *cx = u;
	      fc = fu;
	      return;
	    }

	  /* Parabolic fit was no use. 
	  ** Use the default magification
	  */
	  u = (*cx) + GOLD*(*cx - *bx);
	  fu = pgOneDimCostFunct(ckt, u);

	  /* Parabolic fit is between c and its allowd value */
	} 
      else if ((*cx - u)*(u-ulim) > 0.0) 
	{
	  fu = pgOneDimCostFunct(ckt, u);
	  if( fu < fc)
	    {
	      SHFT(*bx,*cx, u, *cx+GOLD*(*cx - *bx))
	      SHFT(fb, fc, fu, pgOneDimCostFunct(ckt, u))
	    }
	  /* Limit parabolic u to maximum allowed value */
	} 
      else if((u-ulim)/(ulim-*cx) >= 0.0)
	{
	  u = ulim;
	  fu = pgOneDimCostFunct(ckt, u);
	  /* reject parabolic u, use default magnification */    
	} 
      else
	{
	  u = *cx + GOLD*(*cx - *bx);
	  fu = pgOneDimCostFunct(ckt, u);
	}
      SHFT(*ax, *bx, *cx, fu)
	SHFT(fa, fb, fc, fu)
	}
}

/*
**    ITMAX_LINE: maximum allowed number of iterations;
**    CGOLD: is the golden ratios
**    ZEPS:    is small number that protests against trying to achieve
**    fraction accuracy for a minimum that happens to be exactly zero
*/

#define ITMAX_LINE 100
#define CGOLD 0.3819660
#define ZEPS 1.0e-10

/*
**    Parabolic interpolation and Brant's Method
**    in one dimension line search.
*/
double pgParabolicLine(Circuit *ckt, double ax, double bx, double cx,
		       double tol, double *xmin)
{
  int iter;
  double a, b, d, etemp, fu, fv, fw, fx, p, q, r; 
  double tol1, tol2, u, v, w, x, xm;
  double e = 0.0;

  /* a and b must be in ascending order */
  a = (ax < cx ? ax : cx);
  b = (ax > cx ? ax : cx);
  x = w = v = bx;
  fw = fv = fx = pgOneDimCostFunct(ckt, x);

  /* main program loop */
  for(iter = 1; iter <= ITMAX_LINE; iter++)
    {
      xm = 0.5*(a + b);
      tol2 = 2.0*(tol1 = tol*fabs(x) + ZEPS);

      /* test for done here */
      if ( fabs(x - xm) <= (tol2 - 0.5*(b-a)))
	{
	  *xmin = x;
	  return fx;
	}
      if( fabs(e) > tol1)
	{
	  r = (x - w)*(fx - fv);
	  q = (x - v)*(fx - fw);
	  p = (x -v)*q - (x-w)*r;
	  q = 2.0*(q-r);
	  if( q > 0.0) p = -p;
	  q = fabs(q);
	  etemp = e;
	  e = d;

	  /* determine the acceptability of the 
	     parabolic fit.
	  */
	  if( fabs(p) >= fabs(0.5*q*etemp) || p < q*(a-x)  ||
	      p >= q*(b -x))
	    d = CGOLD*(e=(x >= xm ? a -x : b -x));
	  else
	    {
	      d = p/q;
	      u = x + d;
	      if(u-a < tol2 || b-u < tol2)
		d = SIGN(tol1, xm-x);
	  }
	} else 
	  d = CGOLD*(e = (x >= xm ? a - x: b-x));
      u = (fabs(d) >= tol1 ? x + d: x + SIGN(tol1,d));
      fu = pgOneDimCostFunct(ckt, u);

      /* now decide what to do with our function
	 evaluation.
      */
      if( fu <= fx )
	{
	  if( u >= x ) a = x; else b = x;
	  SHFT(v,w,x,u);
	  SHFT(fv,fw,fx,fu);
	} 
      else 
	{
	  if( u < x ) a = u; else b = u;
	  if ( fu < fw || w == x)
	    {
	      v = w;
	      w = u;
	      fv = fw;
	      fw = fu;
	    } 
	  else if (fu < fv || v == x || v == w ) 
	    {
	      v = u;
	      fv = fu;
	    }
	}
    }
  error_mesg(INT_ERROR,"Too many iteration in pgParabolicLine()");
  *xmin = x;
  return fx;
}
