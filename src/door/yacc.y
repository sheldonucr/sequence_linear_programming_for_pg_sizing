/*
 * $RCSfile: yacc.y,v $
 * $Revision: 1.1 $
 * $Date: 1999/04/30 17:07:41 $, (c) Copyright 1999 by X.-D. Sheldon, Tan
 */

%{


#include <stdio.h>
#include <pgDoor.h>


extern	FILE		*yyin;
extern	FILE		*yyout;


%}

%token <str> RESISTOR CURRENT STRING NUMBER EQUAL 
%token <str> LEFT_B RIGHT_B P_SIGN N_SIGN

%union {
    char    str[256];
    };

%start data_in

%%

data_in : delarations 
	  RESISTOR netlist
	  CURRENT currentlist
	;

delarations : assignment
            | delarations assignment
            ;

assignment : STRING EQUAL STRING
	     {
	     pgAddPhyParam($<str>1,$<str>3);
	     }
	   | STRING EQUAL NUMBER
	     {
	     pgAddPhyParam($<str>1,$<str>3);
	     }
	   | STRING STRING NUMBER NUMBER NUMBER NUMBER
	     {
	     pgNewLayer($<str>2, $<str>3, $<str>4, $<str>5, $<str>6);
	     }
	   ;
	
netlist : resistor
	| netlist resistor
	;

resistor : NUMBER NUMBER P_SIGN 
	   {
	   pgNewPhyResistor($<str>1, $<str>2);
	   }
	   boxes
	 | NUMBER NUMBER N_SIGN 
	   {
	   pgNewPhyResistor($<str>1, $<str>2);
	   }
	   boxes
	 ;

boxes : box
      | boxes box
      ;	

box : LEFT_B NUMBER NUMBER NUMBER NUMBER NUMBER RIGHT_B
	{
	pgNewPhyBox($<str>2, $<str>3, $<str>4, $<str>5, $<str>6);
	}
      ;


currentlist : source
	    | currentlist source
	    ;

source : NUMBER NUMBER
	{
	pgNewIndpSource(&theCurSource, $<str>1, "-2", $<str>2);
	}
        ;
%%
