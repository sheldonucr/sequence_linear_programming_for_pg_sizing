#/* 
# * $RCSfile: Makefile,v $
# * $Revision: 1.3 $
# * $Date: 2000/10/10 01:02:00 $, (c) Copyright 1999 by X.-D. Sheldon Tan
# */

CFLAGS	      = -g -I../inc  -I../spinc

DEST	      = .

CC	      = gcc

HDRS	      =

INSTALL	      = install

LD	      = ld

LDFLAGS	      = -r

LIBS	      =

MAKEFILE      = Makefile

OBJS	      = lookup.o \
		nextfield.o \
		commands.o \
		main.o \
		runstats.o 

PRINT	      = pr

PROGRAM       = pgshell.o

SHELL	      = /bin/sh

SRCS	      = lookup.c \
		nextfield.c \
		commands.c \
		main.c \
		runstats.c 

all:		$(PROGRAM)

$(PROGRAM):     $(OBJS) 
		@echo "Linking $(PROGRAM) ..."
		@$(LD) $(LDFLAGS) $(OBJS) -o $(PROGRAM)
		@echo "done"

clean:;		@rm -f $(OBJS) core

clobber:;	@rm -f $(OBJS) $(PROGRAM) core tags

depend:;	@makedepend $(CFLAGS) -- $(SRCS)

echo:;		@echo $(HDRS) $(SRCS)

index:;		@ctags -wx $(HDRS) $(SRCS)

install:	$(PROGRAM)
		@echo Installing $(PROGRAM) in $(DEST)
		@-strip $(PROGRAM)
		@if [ $(DEST) != . ]; then \
		(rm -f $(DEST)/$(PROGRAM); $(INSTALL) -f $(DEST) $(PROGRAM)); fi

print:;		@$(PRINT) $(HDRS) $(SRCS)

tags:           $(HDRS) $(SRCS); @ctags $(HDRS) $(SRCS)

update:		$(DEST)/$(PROGRAM)

###
# DO NOT DELETE THIS LINE -- make depend depends on it.

lookup.o: /usr/include/stdio.h /usr/include/features.h
lookup.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
lookup.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
lookup.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
lookup.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
lookup.o: /usr/include/bits/wchar.h /usr/include/gconv.h
lookup.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
lookup.o: /usr/include/ctype.h /usr/include/endian.h
lookup.o: /usr/include/bits/endian.h
nextfield.o: /usr/include/stdio.h /usr/include/features.h
nextfield.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
nextfield.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
nextfield.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
nextfield.o: /usr/include/libio.h /usr/include/_G_config.h
nextfield.o: /usr/include/wchar.h /usr/include/bits/wchar.h
nextfield.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
nextfield.o: /usr/include/bits/sys_errlist.h /usr/include/ctype.h
nextfield.o: /usr/include/endian.h /usr/include/bits/endian.h
commands.o: /usr/include/stdio.h /usr/include/features.h
commands.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
commands.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
commands.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
commands.o: /usr/include/libio.h /usr/include/_G_config.h
commands.o: /usr/include/wchar.h /usr/include/bits/wchar.h
commands.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
commands.o: /usr/include/bits/sys_errlist.h /usr/include/ctype.h
commands.o: /usr/include/endian.h /usr/include/bits/endian.h ../inc/pgShell.h
commands.o: ../inc/pgCircuit.h ../inc/pgCommon.h ../inc/pgStddefs.h
commands.o: /usr/include/time.h /usr/include/bits/time.h ../inc/pgUnixstd.h
commands.o: /usr/include/string.h /usr/include/stdlib.h
commands.o: /usr/include/sys/types.h /usr/include/sys/select.h
commands.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
commands.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
commands.o: /usr/include/alloca.h /usr/include/unistd.h
commands.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
commands.o: /usr/include/getopt.h /usr/include/errno.h
commands.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
commands.o: /usr/include/asm/errno.h /usr/include/asm-x86_64/errno.h
commands.o: /usr/include/asm-generic/errno.h
commands.o: /usr/include/asm-generic/errno-base.h /usr/include/assert.h
commands.o: ../inc/pgUtility.h ../inc/pgMessage.h ../spinc/spmatrix.h
commands.o: ../spinc/spconfig.h /usr/include/values.h /usr/include/limits.h
commands.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
commands.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
commands.o: ../spinc/sperror.h ../spinc/iferrmsg.h ../spinc/strext.h
commands.o: ../inc/pgOptmz.h ../inc/pgEquCkt.h
main.o: ../inc/pgShell.h ../inc/pgCircuit.h ../inc/pgCommon.h
main.o: ../inc/pgStddefs.h /usr/include/time.h /usr/include/features.h
main.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
main.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
main.o: /usr/include/bits/time.h /usr/include/bits/types.h
main.o: /usr/include/bits/typesizes.h ../inc/pgUnixstd.h /usr/include/stdio.h
main.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
main.o: /usr/include/bits/wchar.h /usr/include/gconv.h
main.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
main.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
main.o: /usr/include/bits/endian.h /usr/include/stdlib.h
main.o: /usr/include/sys/types.h /usr/include/sys/select.h
main.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
main.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
main.o: /usr/include/alloca.h /usr/include/unistd.h
main.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
main.o: /usr/include/getopt.h /usr/include/errno.h /usr/include/bits/errno.h
main.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
main.o: /usr/include/asm-x86_64/errno.h /usr/include/asm-generic/errno.h
main.o: /usr/include/asm-generic/errno-base.h /usr/include/assert.h
main.o: ../inc/pgUtility.h ../inc/pgMessage.h ../spinc/spmatrix.h
main.o: ../spinc/spconfig.h /usr/include/values.h /usr/include/limits.h
main.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
main.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
main.o: ../spinc/sperror.h ../spinc/iferrmsg.h ../spinc/strext.h
runstats.o: /usr/include/sys/types.h /usr/include/features.h
runstats.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
runstats.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
runstats.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
runstats.o: /usr/include/time.h /usr/include/bits/time.h
runstats.o: /usr/include/endian.h /usr/include/bits/endian.h
runstats.o: /usr/include/sys/select.h /usr/include/bits/select.h
runstats.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
runstats.o: /usr/include/bits/pthreadtypes.h /usr/include/sys/times.h
runstats.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
runstats.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
runstats.o: /usr/include/bits/posix2_lim.h
