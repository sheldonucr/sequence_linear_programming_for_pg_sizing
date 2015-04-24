/*
 * $RCSfile: commands.c,v $
 * $Revision: 1.3 $
 * $Date: 2000/10/10 01:02:00 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

/* commands.cc -
 *
 * This file contains routines to read in the pgopt script and
 * process the commands it contains.  The script is read from
 * standard input.  Each line contains a command name followed
 * by additional parameters, if necessary.
 */

#include <stdio.h>
#include <ctype.h>
#include "pgShell.h"
#include "pgCircuit.h"
#include "pgOptmz.h"
#include "pgEquCkt.h"

extern double RestrFactor;

/* Imports from other Gopsim modules: */

extern char *RunStats();
extern char *RunStatsSince();
extern int Lookup(char[], char *([]));

/* interface function */
extern int Help(char*);
extern int QuitCmd(char*);
extern int Load(char*);
extern int Source(char*);
extern int DCAnalysis(char *);
extern int DCSensAnalysis(char *);
extern int ConstCmd(char *);
extern int BuildIntConstCmd(char *);
extern int OptimizeCmd(char *);
extern int LPOutCmd(char *);
extern int OptimizeEquCmd(char *);
extern int StateCmd(char *);
extern int ReadLPResultCmd(char *);
extern int MatchSetCmd(char *);
extern int EMoptmzCmd(char *);
extern int ProcessPhyNLCmd(char *);
extern int SetCmd(char *);
extern int DumpSizedCktCmd(char *);

/* application related functions */
extern void pgDCSensAnalysis();
extern int pgProcessPhyNetlist(char *);

/* The following two arrays hold the names of the commands, and
 * the routine to be invoked to process each command.
 */

char *(cmds[]) = {
	"help",
	"dc analysis ",
	"sensitivity analysis ",
	"load [file]",
	"source [file]",
	"const [file]",
	"opt {conjugate(default), slp}",
	"equ {conjugate(default), slp}",
	"lp_solve [file]",
	"output_netlist [file]",
	"stat",
	"process [physical_netlist] ",
	"resf [restricaton_factor] ",
	"quit",
	NULL};

/* Compiler barfs if command procedures aren't declared here. */

int (*(rtns[]))(char*) = {
	Help,  
	DCAnalysis,
	DCSensAnalysis,
	Load,
	Source,
	ConstCmd,
	OptimizeCmd,
	OptimizeEquCmd,
	LPOutCmd,
	DumpSizedCktCmd,
	StateCmd,
	ProcessPhyNLCmd,
	SetCmd,
	QuitCmd,
	NULL};

int
CmdDo(char* line)
{
	int  index, nargs;
	char cmd[100], residue[200];

	nargs = sscanf(line, " %99s %[^]", cmd, residue);
	if (nargs <= 0) return;
	if (cmd[0] == '!') return;
	if (cmd[0] == '*') return;
	if (cmd[0] == '#') return;
	if (nargs == 1) residue[0] = 0;
	index = Lookup(cmd, cmds);
	if (index == -1)
		printf("Ambiguous command: %s\n", cmd);
	else if (index == -2)
		printf("Unknown command: %s\n", cmd);
	else
    {
		(void) fflush(stdout);
		if (rtns[index] != NULL) (*(rtns[index]))(residue);
		printf("%s\n", RunStatsSince());
    }
}

int
Command(FILE* f)

	/*---------------------------------------------------------
 *    Command just reads commands from the script, and
 *    calls the corresponding routines.
 *
 *    Results:    None.
 *
 *    Side Effects:
 *    Whatever the commands do.  This procedure continues
 *    until it hits end-of-file.
 *---------------------------------------------------------
 */

{
	char line[800], *p;
	int tty, length;

	tty = isatty(fileno(f));
	while (TRUE)
    {
		if (tty)
		{
			fputs(": ", stdout);
			(void) fflush(stdout);
		}
		p = line;
		*p = 0;
		length = 799;
		while (TRUE)
		{
			if (fgets(p, length, f) == NULL) return;

			/* Get rid of the stupid newline character.  If we didn't
			 * get a whole line, keep reading.
			 */

			for ( ; (*p != 0) && (*p != '\n'); p++);
			if (*p == '\n')
			{
				*p = 0;
				if (p == line) break;
				if (*(p-1) != '\\') break;
				else p--;
			}
			length = 799 - strlen(line);
			if (length <= 0) break;
		}
		if (line[0] == 0) continue;
		if (!tty) printf(": %s\n", line);
		CmdDo(line);
    }
}

int
Help(char *name)

	/*---------------------------------------------------------
 *    This command routine just prints out the legal commands.
 *
 *    Results:    None.
 *
 *    Side Effects:
 *    Valid command names are printed one per line on standard
 *    output.
 *---------------------------------------------------------
 */

{
	char **p = cmds;
	printf("Valid commands are:\n");
	while (*p != NULL)
    {
		printf("  %s\n", *p);
		p++;
    }
}

int
QuitCmd(char*cmd)

	/*---------------------------------------------------------
 *    This routine just returns from pgopt to the shell.
 *
 *    Results:    None -- it never returns.
 *
 *    Side Effects:    pgopt exits.
 *---------------------------------------------------------
 */

{
	printf("%s pgopt done. Have a nice day!\n", RunStats());
	exit(0);
}


int
Load(char* name)

	/*---------------------------------------------------------
 *    This command routine merely reads other commands
 *    from a given file.
 *
 *    Results:    None.
 *
 *    Side Effects:
 *    Whatever the commands in the file do.
 *---------------------------------------------------------
 */

{
	FILE *f;
	char string[100];

	if (name[0] == 0)
    {
		printf("No command file name given.\n");
		return;
    }
	sscanf(name, "%s", string);
	f = fopen(string, "r");
	if (f == NULL)
    {
		printf("Couldn't find file %s.\n", string);
		return;
    }
	Command(f);
	fclose(f);
}


int
Source(char* name)
{
	FILE *f;
	char string[128];
	char line[128], *str;

	if (name[0] == 0)
    {
		error_mesg(IO_ERROR,"No spice file name given.\n");
		return;
    }
	theCkt = pgParseCircuit(name);
	if(theCkt)
    {
		str = strchr(name,'.');
		if(!str)
		{
			sprintf(string,"%s.cs", name);
		}
		else
		{
			*str = 0;
			sprintf(string,"%s.cs", name);
		}
		pgReadConstFile(theCkt,string);
    }
}

int
DCAnalysis( char *str )
{
	if(!theCkt)
    {
		error_mesg(IO_ERROR,"No input spice file loaded.\n");
		return;
    }
	pgDCAnalysis(theCkt);
}

int
DCSensAnalysis( char *str )
{
	if(!theCkt)
    {
		error_mesg(IO_ERROR,"No input spice file loaded.\n");
		return;
    }
	pgDCSensAnalysis(theCkt);
}

int
ConstCmd(char*filename)
{
	if(!theCkt)
    {
		error_mesg(IO_ERROR,"No input spice file loaded.\n");
		return;
    }
	pgReadConstFile(theCkt,filename );
}


int
OptimizeCmd(char *str)
{
	if(!theCkt)
    {
		error_mesg(IO_ERROR,"No input spice file loaded.\n");
		return;
    }
 
	if( !strcmp(str,"slp") || !strcmp(str,"SLP"))
	{
		pgMainLoop(theCkt, optSLP);
	}
	else if(!strcmp(str,"conjugate") || !strcmp(str,"CONJUGATE"))
	{
		pgMainLoop(theCkt, optConjugate);
	}
	else
	{
		pgMainLoop(theCkt, optConjugate);
	}
}

int
OptimizeEquCmd(char *str)
{
	if(!theCkt)
    {
		error_mesg(IO_ERROR,"No input spice file loaded.\n");
		return;
    }
	pgOptimizeEqnCircuit(theCkt);
  
	/*
	  if( !strcmp(str,"slp") || !strcmp(str,"SLP"))
	  pgMainLoop(theCkt, optSLP);
	  else if(!strcmp(str,"conjugate") || !strcmp(str,"CONJUGATE"))
	  pgMainLoop(theCkt, optConjugate);
	  else
	  pgMainLoop(theCkt, optConjugate);
	*/
}

int
LPOutCmd(char*filename)
{
	FILE *fp;
	if(!theCkt)
    {
		error_mesg(IO_ERROR,"No input spice file loaded.\n");
		return;
    }
	if (filename[0] == 0) 
    {
		pgPrintLPData(theCkt, stdout);
		return;
    }
	else
    {
		fp = fopen(filename,"w");
		if(!fp)
		{
			sprintf(buf,"Can not open %s to write!");
			error_mesg(IO_ERROR,buf);
		}
		pgPrintLPData(theCkt, fp);
    }
	fclose(fp);
}

int
DumpSizedCktCmd(char*filename)
{
	FILE *fp;
	if(!theCkt)
    {
		error_mesg(IO_ERROR,"No input spice file loaded.\n");
		return;
    }
	if (filename[0] == 0) 
    {
		error_mesg(IO_ERROR,"No file name is given.\n");
		return;
    }
	else
    {
		pgDumpSizedNetlist(filename);
    }
}

int
StateCmd(char *str)
{
	if(!theCkt)
	{
		error_mesg(IO_ERROR,"No input spice file loaded.\n");
		return;
	}
	if( theEquCkt && (!strcmp(str,"equ") || !strcmp(str,"EQU")))
	{
		pgPrintState(theEquCkt);
	}
	else
	{
		pgPrintState(theCkt);
	}
}


int
ProcessPhyNLCmd(char *fname)
{
	pgProcessPhyNetlist( fname );
}

int
SetCmd(char *str)
{
	RestrFactor = atof(str);
	printf("Restricaton factor: %g",RestrFactor);
}



