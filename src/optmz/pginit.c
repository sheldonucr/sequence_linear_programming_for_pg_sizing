/*
 * $RCSfile: pginit.c,v $                                                                   
 * $Revision: 1.3 $
 * $Date: 2000/10/10 01:01:12 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

/* 
**    global variables for pgopt system
*/

#include "pgOptmz.h"

Circuit *theCkt, *theEquCkt;

double PenalWeight;
double EqualPanalFactor;
double theEMConst;
double theResistivity;
double RestrFactor;



/*
**    assign the initial value to
**    some global variables
*/

void
pgInit()
{
  theCkt = NULL;
  theEquCkt = NULL;

  RestrFactor = 0.85;
  PenalWeight = 1;
  EqualPanalFactor = 100; 
  theEMConst = 0;
  theResistivity = 0;

}
