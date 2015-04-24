#/*
# * $RCSfile: Makefile,v $
# * $Revision: 1.3 $
# * $Date: 2000/10/10 00:58:55 $, (c) Copyright 1999 by X.-D. Sheldon Tan
# */

CFLAGS	      = -g -I../inc -I../spinc 

CC	      = gcc

DEST	      = .

HDRS	      =

INSTALL	      = install

LD	      = ld

LDFLAGS	      = -r

LIBS	      = 

MAKEFILE      = Makefile

OBJS	      = pgCircuit.o \
		pgMessage.o \
		pgSens.o \
		pgSysCall.o \
		pgMPS.o \
		pgEquCkt.o \
		pgUtility.o

PRINT	      = pr

PROGRAM       = pgckt.o

SHELL	      = /bin/sh

SRCS	      = pgCircuit.c \
		pgMessage.c \
		pgSens.c \
		pgSysCall.c \
		pgMPS.c \
		pgEquCkt.c \
		pgUtility.c 

all:		$(PROGRAM)

$(PROGRAM):     $(OBJS)  
		@echo "Linking $(PROGRAM) ..."
		@$(LD) $(LDFLAGS) $(OBJS) -o $(PROGRAM) $(LIBS) 
		@echo "done"
		@echo ""

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

pgCircuit.o: ../inc/pgCircuit.h ../inc/pgCommon.h ../inc/pgStddefs.h
pgCircuit.o: /usr/include/time.h /usr/include/features.h
pgCircuit.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
pgCircuit.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
pgCircuit.o: /usr/include/bits/time.h /usr/include/bits/types.h
pgCircuit.o: /usr/include/bits/typesizes.h ../inc/pgUnixstd.h
pgCircuit.o: /usr/include/stdio.h /usr/include/libio.h
pgCircuit.o: /usr/include/_G_config.h /usr/include/wchar.h
pgCircuit.o: /usr/include/bits/wchar.h /usr/include/gconv.h
pgCircuit.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
pgCircuit.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
pgCircuit.o: /usr/include/bits/endian.h /usr/include/stdlib.h
pgCircuit.o: /usr/include/sys/types.h /usr/include/sys/select.h
pgCircuit.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
pgCircuit.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
pgCircuit.o: /usr/include/alloca.h /usr/include/unistd.h
pgCircuit.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
pgCircuit.o: /usr/include/getopt.h /usr/include/errno.h
pgCircuit.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
pgCircuit.o: /usr/include/asm/errno.h /usr/include/asm-x86_64/errno.h
pgCircuit.o: /usr/include/asm-generic/errno.h
pgCircuit.o: /usr/include/asm-generic/errno-base.h /usr/include/assert.h
pgCircuit.o: ../inc/pgUtility.h ../inc/pgMessage.h ../spinc/spmatrix.h
pgCircuit.o: ../spinc/spconfig.h /usr/include/values.h /usr/include/limits.h
pgCircuit.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
pgCircuit.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
pgCircuit.o: ../spinc/sperror.h ../spinc/iferrmsg.h ../spinc/strext.h
pgMessage.o: ../inc/pgUnixstd.h /usr/include/stdio.h /usr/include/features.h
pgMessage.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
pgMessage.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
pgMessage.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
pgMessage.o: /usr/include/libio.h /usr/include/_G_config.h
pgMessage.o: /usr/include/wchar.h /usr/include/bits/wchar.h
pgMessage.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
pgMessage.o: /usr/include/bits/sys_errlist.h /usr/include/string.h
pgMessage.o: /usr/include/ctype.h /usr/include/endian.h
pgMessage.o: /usr/include/bits/endian.h /usr/include/stdlib.h
pgMessage.o: /usr/include/sys/types.h /usr/include/time.h
pgMessage.o: /usr/include/bits/time.h /usr/include/sys/select.h
pgMessage.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
pgMessage.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
pgMessage.o: /usr/include/alloca.h /usr/include/unistd.h
pgMessage.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
pgMessage.o: /usr/include/getopt.h /usr/include/errno.h
pgMessage.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
pgMessage.o: /usr/include/asm/errno.h /usr/include/asm-x86_64/errno.h
pgMessage.o: /usr/include/asm-generic/errno.h
pgMessage.o: /usr/include/asm-generic/errno-base.h /usr/include/assert.h
pgMessage.o: ../inc/pgMessage.h
pgSens.o: ../inc/pgCircuit.h ../inc/pgCommon.h ../inc/pgStddefs.h
pgSens.o: /usr/include/time.h /usr/include/features.h
pgSens.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
pgSens.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
pgSens.o: /usr/include/bits/time.h /usr/include/bits/types.h
pgSens.o: /usr/include/bits/typesizes.h ../inc/pgUnixstd.h
pgSens.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
pgSens.o: /usr/include/wchar.h /usr/include/bits/wchar.h /usr/include/gconv.h
pgSens.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
pgSens.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
pgSens.o: /usr/include/bits/endian.h /usr/include/stdlib.h
pgSens.o: /usr/include/sys/types.h /usr/include/sys/select.h
pgSens.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
pgSens.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
pgSens.o: /usr/include/alloca.h /usr/include/unistd.h
pgSens.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
pgSens.o: /usr/include/getopt.h /usr/include/errno.h
pgSens.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
pgSens.o: /usr/include/asm/errno.h /usr/include/asm-x86_64/errno.h
pgSens.o: /usr/include/asm-generic/errno.h
pgSens.o: /usr/include/asm-generic/errno-base.h /usr/include/assert.h
pgSens.o: ../inc/pgUtility.h ../inc/pgMessage.h ../spinc/spmatrix.h
pgSens.o: ../spinc/spconfig.h /usr/include/values.h /usr/include/limits.h
pgSens.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
pgSens.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
pgSens.o: ../spinc/sperror.h ../spinc/iferrmsg.h ../spinc/strext.h
pgSens.o: ../inc/pgSens.h
pgSysCall.o: ../inc/pgCommon.h ../inc/pgStddefs.h /usr/include/time.h
pgSysCall.o: /usr/include/features.h /usr/include/sys/cdefs.h
pgSysCall.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
pgSysCall.o: /usr/include/gnu/stubs-64.h /usr/include/bits/time.h
pgSysCall.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
pgSysCall.o: ../inc/pgUnixstd.h /usr/include/stdio.h /usr/include/libio.h
pgSysCall.o: /usr/include/_G_config.h /usr/include/wchar.h
pgSysCall.o: /usr/include/bits/wchar.h /usr/include/gconv.h
pgSysCall.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
pgSysCall.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
pgSysCall.o: /usr/include/bits/endian.h /usr/include/stdlib.h
pgSysCall.o: /usr/include/sys/types.h /usr/include/sys/select.h
pgSysCall.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
pgSysCall.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
pgSysCall.o: /usr/include/alloca.h /usr/include/unistd.h
pgSysCall.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
pgSysCall.o: /usr/include/getopt.h /usr/include/errno.h
pgSysCall.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
pgSysCall.o: /usr/include/asm/errno.h /usr/include/asm-x86_64/errno.h
pgSysCall.o: /usr/include/asm-generic/errno.h
pgSysCall.o: /usr/include/asm-generic/errno-base.h /usr/include/assert.h
pgSysCall.o: ../inc/pgUtility.h ../inc/pgMessage.h /usr/include/signal.h
pgSysCall.o: /usr/include/bits/signum.h /usr/include/bits/siginfo.h
pgSysCall.o: /usr/include/bits/sigaction.h /usr/include/bits/sigcontext.h
pgSysCall.o: /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h
pgSysCall.o: /usr/include/sys/wait.h /usr/include/sys/resource.h
pgSysCall.o: /usr/include/bits/resource.h /usr/include/bits/waitflags.h
pgSysCall.o: /usr/include/bits/waitstatus.h
pgMPS.o: ../inc/pgMPS.h ../inc/pgCircuit.h ../inc/pgCommon.h
pgMPS.o: ../inc/pgStddefs.h /usr/include/time.h /usr/include/features.h
pgMPS.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
pgMPS.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
pgMPS.o: /usr/include/bits/time.h /usr/include/bits/types.h
pgMPS.o: /usr/include/bits/typesizes.h ../inc/pgUnixstd.h
pgMPS.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
pgMPS.o: /usr/include/wchar.h /usr/include/bits/wchar.h /usr/include/gconv.h
pgMPS.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
pgMPS.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
pgMPS.o: /usr/include/bits/endian.h /usr/include/stdlib.h
pgMPS.o: /usr/include/sys/types.h /usr/include/sys/select.h
pgMPS.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
pgMPS.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
pgMPS.o: /usr/include/alloca.h /usr/include/unistd.h
pgMPS.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
pgMPS.o: /usr/include/getopt.h /usr/include/errno.h /usr/include/bits/errno.h
pgMPS.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
pgMPS.o: /usr/include/asm-x86_64/errno.h /usr/include/asm-generic/errno.h
pgMPS.o: /usr/include/asm-generic/errno-base.h /usr/include/assert.h
pgMPS.o: ../inc/pgUtility.h ../inc/pgMessage.h ../spinc/spmatrix.h
pgMPS.o: ../spinc/spconfig.h /usr/include/values.h /usr/include/limits.h
pgMPS.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
pgMPS.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
pgMPS.o: ../spinc/sperror.h ../spinc/iferrmsg.h ../spinc/strext.h
pgMPS.o: ../inc/pgOptmz.h
pgEquCkt.o: ../inc/pgCircuit.h ../inc/pgCommon.h ../inc/pgStddefs.h
pgEquCkt.o: /usr/include/time.h /usr/include/features.h
pgEquCkt.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
pgEquCkt.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
pgEquCkt.o: /usr/include/bits/time.h /usr/include/bits/types.h
pgEquCkt.o: /usr/include/bits/typesizes.h ../inc/pgUnixstd.h
pgEquCkt.o: /usr/include/stdio.h /usr/include/libio.h
pgEquCkt.o: /usr/include/_G_config.h /usr/include/wchar.h
pgEquCkt.o: /usr/include/bits/wchar.h /usr/include/gconv.h
pgEquCkt.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
pgEquCkt.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
pgEquCkt.o: /usr/include/bits/endian.h /usr/include/stdlib.h
pgEquCkt.o: /usr/include/sys/types.h /usr/include/sys/select.h
pgEquCkt.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
pgEquCkt.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
pgEquCkt.o: /usr/include/alloca.h /usr/include/unistd.h
pgEquCkt.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
pgEquCkt.o: /usr/include/getopt.h /usr/include/errno.h
pgEquCkt.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
pgEquCkt.o: /usr/include/asm/errno.h /usr/include/asm-x86_64/errno.h
pgEquCkt.o: /usr/include/asm-generic/errno.h
pgEquCkt.o: /usr/include/asm-generic/errno-base.h /usr/include/assert.h
pgEquCkt.o: ../inc/pgUtility.h ../inc/pgMessage.h ../spinc/spmatrix.h
pgEquCkt.o: ../spinc/spconfig.h /usr/include/values.h /usr/include/limits.h
pgEquCkt.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
pgEquCkt.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
pgEquCkt.o: ../spinc/sperror.h ../spinc/iferrmsg.h ../spinc/strext.h
pgEquCkt.o: ../inc/pgEquCkt.h ../inc/pgOptmz.h
pgUtility.o: ../inc/pgCommon.h ../inc/pgStddefs.h /usr/include/time.h
pgUtility.o: /usr/include/features.h /usr/include/sys/cdefs.h
pgUtility.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
pgUtility.o: /usr/include/gnu/stubs-64.h /usr/include/bits/time.h
pgUtility.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
pgUtility.o: ../inc/pgUnixstd.h /usr/include/stdio.h /usr/include/libio.h
pgUtility.o: /usr/include/_G_config.h /usr/include/wchar.h
pgUtility.o: /usr/include/bits/wchar.h /usr/include/gconv.h
pgUtility.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
pgUtility.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
pgUtility.o: /usr/include/bits/endian.h /usr/include/stdlib.h
pgUtility.o: /usr/include/sys/types.h /usr/include/sys/select.h
pgUtility.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
pgUtility.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
pgUtility.o: /usr/include/alloca.h /usr/include/unistd.h
pgUtility.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
pgUtility.o: /usr/include/getopt.h /usr/include/errno.h
pgUtility.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
pgUtility.o: /usr/include/asm/errno.h /usr/include/asm-x86_64/errno.h
pgUtility.o: /usr/include/asm-generic/errno.h
pgUtility.o: /usr/include/asm-generic/errno-base.h /usr/include/assert.h
pgUtility.o: ../inc/pgUtility.h ../inc/pgMessage.h /usr/include/math.h
pgUtility.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
pgUtility.o: /usr/include/bits/mathcalls.h
