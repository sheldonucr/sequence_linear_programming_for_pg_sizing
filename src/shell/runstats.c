/* 
 * $RCSfile: runstats.c,v $
 * $Revision: 1.3 $
 * $Date: 2000/10/10 01:02:00 $, (c) Copyright 1999 by X.-D. Sheldon Tan
 */

/* runstats.c -
 */

#include <sys/types.h>
#include <sys/times.h>
#include <time.h>
#include <limits.h>

/* Library imports: */

/*extern char *sprintf(), *sbrk();*/

extern end;

/* The following variables keep track of the time used as of
 * the last call to one of the procedures in this module.
 */

int lastSysTime = 0;
int lastUserTime = 0;


char *
RunStats()

/*---------------------------------------------------------
 *    This procedure collects information about the
 *    process.
 *
 *    Results:
 *    The return value is a string of the form
 *    "[mins:secs mins:secs size]" where the first time is
 *    the amount of user-space CPU time this process has
 *    used so far, the second time is the amount of system
 *    time used so far, and size is the size of the heap area.
 *
 *    Side Effects:    None.
 *---------------------------------------------------------
 */

{
    struct tms buffer;
    static char string[100];
    int umins, size, smins;
    float usecs, ssecs;

    times(&buffer);
    lastUserTime = buffer.tms_utime + buffer.tms_cutime;
    lastSysTime = buffer.tms_stime + buffer.tms_cstime;
    umins = (buffer.tms_utime + buffer.tms_cutime);
    usecs = umins % (60*CLOCKS_PER_SEC);
    usecs /= CLOCKS_PER_SEC;
    umins /= (60*CLOCKS_PER_SEC);
    smins = (buffer.tms_stime + buffer.tms_cstime);
    ssecs = smins % (60*CLOCKS_PER_SEC);
    ssecs /= CLOCKS_PER_SEC;
    smins /= (60*CLOCKS_PER_SEC);
    size = (((int) sbrk(0) - (int) &end) + 512)/1024;
    sprintf(string, "[%d:%04.3fu %d:%04.3fs %dk]",
    umins, usecs, smins, ssecs, size);
    return string;
}	


char *
RunStatsSince()

/*---------------------------------------------------------
 *    This procedure returns information about what's
 *    happened since the last call.
 *
 *    Results:
 *    Just the same as for RunStats, except that the times
 *    refer to the time used since the last call to this
 *    procedure or RunStats.
 *
 *    Side Effects:    None.
 *---------------------------------------------------------
 */

{
    struct tms buffer;
    static char string[100];
    int umins, size, smins;
    float usecs, ssecs;

    times(&buffer);
    umins = buffer.tms_utime + buffer.tms_cutime - lastUserTime;
    smins = buffer.tms_stime + buffer.tms_cstime- lastSysTime;
    lastUserTime = buffer.tms_utime;
    lastSysTime = buffer.tms_stime;
    usecs = umins % (60*CLOCKS_PER_SEC);
    usecs /= CLOCKS_PER_SEC;
    umins /= (60*CLOCKS_PER_SEC);
    ssecs = smins % (60*CLOCKS_PER_SEC);
    ssecs /= CLOCKS_PER_SEC;
    smins /= (60*CLOCKS_PER_SEC);
    size = (((int) sbrk(0) - (int) &end) + 512)/1024;
    sprintf(string, "[%d:%04.3fu %d:%04.3fs %dk]",
    umins, usecs, smins, ssecs, size);
    return string;
}
