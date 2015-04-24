/*
 * $RCSfile: pgMessage.c,v $
 * $Revision: 1.1 $
 * $Date: 1999/04/30 17:12:50 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */


#include "pgUnixstd.h"
#include "pgMessage.h"

char buf[1024];

void
error_mesg(int code, char *mesg)
{
    if(code == FAT_ERROR)
        printf("FATAL ERROR: %s\n", mesg);
    else if(code == INT_ERROR)
        printf("INTERNAL ERROR: %s\n", mesg);
    else if(code == IO_ERROR)
        printf("I/O ERROR: %s\n", mesg);
    else if(code == MEM_ERROR)
        printf("MEMORY ERROR: %s\n", mesg);
    else if(code == PARSE_ERROR)
        printf("PARSE ERROR: %s\n", mesg);
    else
        printf("ERROR: %s\n", mesg);
    printf("");
    fflush(stdout);
}

void
print_mesg(char *mesg)
{
    printf("%s",mesg);
    fflush(stdout);
}

void
print_warn(char *mesg)
{
    printf("WARNING: %s.\n",mesg);
    fflush(stdout);
}
