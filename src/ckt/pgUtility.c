
/*
 * $RCSfile: pgUtility.c,v $
 * $Revision: 1.1 $
 * $Date: 1999/04/30 17:12:50 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */


#include "pgCommon.h"
#include "pgUtility.h"
#include <math.h>
#include <time.h>
#include <sys/types.h>

void InitRandom();

/* some convenient functions definition */

/*----------------------------------------------------
    FuncName: CopyStr(char *str)
    Spec: alloc space and copy the content
 -----------------------------------------------------
    Inputs: char *str
    Outputs: the address of alloced space

    Global Variblas used: none
----------------------------------------------------*/

#include <string.h>
#include <ctype.h>

char *
CopyStr(char *str)
{
    int length = strlen(str);
    char *aux = (char *)malloc((length+1)*sizeof(char));

    if(!str)
        return;
    aux = strncpy(aux, str,length);
    aux[length] = '\0';
    return aux;
}

/*----------------------------------------------------
    FuncName:ToUpper
    Spec: convert all the charaters into capital case
 -----------------------------------------------------
    Inputs: char *str
    Outputs: the converted string

    Global Variblas used: none
----------------------------------------------------*/
char *
ToUpper(char *str)
{
    int length = strlen(str);
    int i;

    if(!str)
        return NULL;
    for(i = 0; i<length; i++)
        str[i] = toupper(str[i]);
    return str;
}    
    
        
/*----------------------------------------------------
    FuncName: ToLower
    Spec: convert all the charaters into lower case
 -----------------------------------------------------
    Inputs: char *str
    Outputs: the converted string

    Global Variblas used: none
----------------------------------------------------*/

char *
ToLower(char *str)
{
    int length = strlen(str);
    int i;

    if(!str)
        return NULL;
    for(i = 0; i<length; i++)
        str[i] = tolower(str[i]);
    return str;
}

/*----------------------------------------------------
    FuncName: TransValue
    Spec:
    function that gets the floating point value from a string
    that represents the value. If value has a value like 3547K
    this function returns the value 3547 by putting 1000 in the
    mulfac variable.It decodes the K,M,G,U,...
 -----------------------------------------------------
    Inputs: char *str with SPICE stardard unit suffixes
    Outputs: double type value

    Global Variblas used: none
----------------------------------------------------*/

/* MEMO unit used in SPICE
T=1e12 G=1e9 MEG=1e6 K=1e1 MIL=25.4e-6 M=1e-3 U=1e-6
N=1e-9 P=1e-12 F=1e-15
*/

double
TransValue(char *_value)
{
        char ch;
        int i = 0;
        double val;
        double var1;
        double mulfac = 1.0;
    char *value;

    if(!_value){
        error_mesg(INT_ERROR,"NULL pointer is passed for TransValue");
        return 0.0;
    }

    value = CopyStr(_value);

        ch = value[0];
        while ( ch >= '0' && ch <= '9'|| 
            ch == '.' || ch == '+' || ch == '-') {
            ch = value[++i];
        }
 
        /* decode the letter symbols */
        if (ch != ' ') {
        switch (ch) {
 
        case 'T' :
        case 't' :
            mulfac = 1e12;
            break;
 
        case 'G' :
        case 'g' :
            mulfac = 1e9;
            break;
 
        case 'K' :
        case 'k' :
            mulfac = 1e3;
            break;
 
        case 'U' :
        case 'u' :
            mulfac = 1e-6;
            break;
 
        case 'N' :
        case 'n' :
            mulfac = 1e-9;
            break;
 
        case 'P' :
        case 'p' :
            mulfac = 1e-12;
        break;
 
        case 'F' :
        case 'f' :
            mulfac = 1e-15;
            break;
 
        case 'M' :
        case 'm' :
            switch (value[i+1]){
            case  'E' :
            case 'e' :
                mulfac = 1e6;
                break;
            case  'I' :
            case 'i' :
                mulfac = 25.4e-6;
                break;
            default :
                mulfac = 1e-3;
                break;
            }
            break;
        case 'E' :
        case 'e' :
            var1 = strtod(&value[i+1],NULL);
            mulfac = pow(10,var1);
            break;
        }
    }
    value[i] = 0;
    sscanf( value, "%lg", &val);
    /*
    printf(" output(base)-> %g\n",val);
    printf(" output(-> %g\n",val*mulfac);
    */
    free(value);
    return (val * mulfac);
}
 
/*----------------------------------------------------
    FuncName: 
    Spec: routines related to random generator
 -----------------------------------------------------
    Inputs:
    Outputs:

    Global Variblas used:
----------------------------------------------------*/
void
InitRandom()
{
   unsigned seed;
   int i,n;
   time_t tloc;
   time_t t;
 
   t = time(&tloc);
   seed = t%1000;
   srand(seed);
}

static rand_initialized = 0;
double 
RandVar(double lbound, double ubound)
{
    double value;

    if(!rand_initialized){
        InitRandom();
        rand_initialized = 1;
    }

    value = exp(log(lbound) + (float) (rand()%1000)/1000.0 *
            (log(ubound) - log(lbound)));

    return value;
}

