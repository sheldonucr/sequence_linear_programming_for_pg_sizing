/* 
 * $RCSfile: main.c,v $
 * $Revision: 1.5 $
 * $Date: 2003/02/20 02:12:18 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

/* main.cc -
 */

#include "pgShell.h"
#include <time.h>

char *spiceFile;

extern int CmdDo(char*);
extern int RunStats();
extern int Command(FILE *);

int
main
(
	int argc, 
	char *argv[]
)
{
    char cmd[128];

    /* There should be no more than one command line switch, and
     * it contains the name of the .sp file.
     */

    if (argc > 2)
	{
		printf("Too many arguments;  all but the first are ignored.\n");
	}
    if (argc > 1) 
	{
		spiceFile = argv[1];
	}
    else
	{
		spiceFile = NULL;
	}

    printf("\n");
    printf("PGOPT --- Liability-Constrainted P/G Area Optimization\n"); 
    printf("%s, %s\n","$Revision: 1.5 $", "$Date: 2003/02/20 02:12:18 $");
    printf("Copyright (c) 1999 - 2003 by X.-D. Sheldon Tan\n");
    printf("Type 'h' for help.\n\n");

    /* global variable initialization */
    pgInit();

    if (spiceFile != NULL)
	{
		printf(": source %s\n", spiceFile);
		sprintf(cmd, "source %s", spiceFile);
		CmdDo(cmd);
	}
	else 
	{
		printf("No .sp file specified.\n");
	}

	Command(stdin);

	printf("%s PGOPT done.\n", RunStats());
} 
