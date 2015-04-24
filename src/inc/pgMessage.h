/*
 * $RCSfile: pgMessage.h,v $
 * $Revision: 1.1 $
 * $Date: 1999/04/30 17:43:27 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */


#ifndef MESSAGE_H
#define MESSAGE_H

#define FAT_ERROR    0    /* fatal error */
#define INT_ERROR    1    /* internal error*/
#define IO_ERROR    2    /* I/O file error */
#define MEM_ERROR     3    /* memory error */
#define PARSE_ERROR     4    /* parse error */


/* function interface delaratioins */
extern void error_mesg(int code, char *mesg); 
extern void print_mesg(char *mesg); 
extern void print_warn(char *mesg); 

extern char buf[]; /* used for buffering output message */

#define _DEBUG

#endif MESSAGE_H

