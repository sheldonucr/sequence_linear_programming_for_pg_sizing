/*
 * $RCSfile: lex.l,v $
 * $Revision: 1.1 $
 * $Date: 1999/04/30 17:07:41 $, (c) Copyright 1999 by X.-D. Sheldon, Tan
 */

%{ 

#include <stdio.h>
#include <ctype.h>
#include "pgDoor.h"
#include "yacc.h"

#define T_RETURN(TOKEN)\
    {\
    strcpy(yylval.str, yytext);\
    return (TOKEN);\
    }

#define S_RETURN(TOKEN)\
    {\
    strcpy(yylval.str, yytext + 1); /* skip a char " */\
    return (TOKEN);\
    }

#define DEBUG
#define DEBUG_ { \
	printf("%s ",yytext); \
	}

int lineno = 1;
%}

number		[+-]?[0-9]+[\.]?[0-9]*([e]?[+-]?[0-9]+)?	
string		[A-Za-z][A-Za-z0-9_\-.+]*
comnt   	\/\*.*\*\/

%%

{comnt}		{
		DEBUG;
	}

"["	{
		DEBUG;
		T_RETURN(LEFT_B);
	}
"]"	{
		DEBUG;
		T_RETURN(RIGHT_B)
	}
"+"	{
		DEBUG;
		T_RETURN(P_SIGN);
	}
"-"	{
		DEBUG;
		T_RETURN(N_SIGN);
	}
"="	{
		DEBUG;
		T_RETURN(EQUAL);
	}

"RESISTOR"  	{
		DEBUG;
		T_RETURN(RESISTOR);
	}

"CURRENT"  	{
		DEBUG;
		T_RETURN(CURRENT);
	}

{string}  	{
		DEBUG;
		T_RETURN(STRING);
	}

{number} 	{ 
		DEBUG;
	   	T_RETURN(NUMBER);
	 }

\n	{
	DEBUG;
	lineno++;
	}

[ \t,";] {}

%%

yywrap()
{
    return 1;
}

yyerror(s)
    char *s;
{
    printf("\nline no. = %d\n<<- %s ->> %s\n", lineno, yytext, s);
	exit(1);
}
