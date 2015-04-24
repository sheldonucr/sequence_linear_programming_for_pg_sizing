/*
 * $RCSfile: pgSysCall.c,v $
 * $Revision: 1.2 $
 * $Date: 2000/06/14 17:03:52 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */


/* sys_rout.cc */

#include "pgCommon.h"
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#define PCx  "/home/xtan/PCx/PCx"

int
pgExeclPCx( char *file )
{
    int childid;
    int p;

    if(!file)
        return -1;

    if( !(childid = fork()) ){ /* in child */
    	execl(PCx,"PCx", file, NULL);
    }
    else if( childid == -1){
        perror("fork");
        return -1;
    }

    wait(&p);
    return p;
}

int
pgSystem( char *comm )
{
    int childid;
    int p;

    if(!comm)
        return -1;

    if( !(childid = fork()) ){ /* in child */
        p = system(comm);
	exit(p);
    }
    else if( childid == -1){
        perror("fork");
        return -1;
    }

    wait(&p);
    return p;
}
