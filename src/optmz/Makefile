#/*
# * $RCSfile: Makefile,v $                                                                   
# * $Revision: 1.3 $
# * $Date: 2000/10/10 01:01:12 $, (c) Copyright 1999 by X.-D. Sheldon Tan
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

OBJS	      = pgConjugate.o \
		pgExtConst.o \
		pgNodeBranch.o \
		pgCostFunc.o \
		pgIntConst.o \
		pgState.o \
		pgMainLoop.o \
		pgNewton.o \
		pginit.o \
		pgGolden.o \
		pgProduct.o
		

PRINT	      = pr

PROGRAM       = pgoptmz.o

SHELL	      = /bin/sh

SRCS	      = pgConjugate.c \
		pgExtConst.c \
		pgNodeBranch.c \
		pgCostFunc.c \
		pgIntConst.c \
		pgState.c \
		pgMainLoop.c \
		pgNewton.c \
		pginit.c \
		pgGolden.c \
		pgProduct.c
	
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

pgConjugate.o: /usr/include/math.h /usr/include/features.h
pgConjugate.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
pgConjugate.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
pgConjugate.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
pgConjugate.o: /usr/include/bits/mathcalls.h ../inc/pgOptmz.h
pgConjugate.o: ../inc/pgCircuit.h ../inc/pgCommon.h ../inc/pgStddefs.h
pgConjugate.o: /usr/include/time.h /usr/include/bits/time.h
pgConjugate.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
pgConjugate.o: ../inc/pgUnixstd.h /usr/include/stdio.h /usr/include/libio.h
pgConjugate.o: /usr/include/_G_config.h /usr/include/wchar.h
pgConjugate.o: /usr/include/bits/wchar.h /usr/include/gconv.h
pgConjugate.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
pgConjugate.o: /usr/include/string.h /usr/include/ctype.h
pgConjugate.o: /usr/include/endian.h /usr/include/bits/endian.h
pgConjugate.o: /usr/include/stdlib.h /usr/include/sys/types.h
pgConjugate.o: /usr/include/sys/select.h /usr/include/bits/select.h
pgConjugate.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
pgConjugate.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
pgConjugate.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
pgConjugate.o: /usr/include/bits/confname.h /usr/include/getopt.h
pgConjugate.o: /usr/include/errno.h /usr/include/bits/errno.h
pgConjugate.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
pgConjugate.o: /usr/include/asm-x86_64/errno.h
pgConjugate.o: /usr/include/asm-generic/errno.h
pgConjugate.o: /usr/include/asm-generic/errno-base.h /usr/include/assert.h
pgConjugate.o: ../inc/pgUtility.h ../inc/pgMessage.h ../spinc/spmatrix.h
pgConjugate.o: ../spinc/spconfig.h /usr/include/values.h
pgConjugate.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
pgConjugate.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
pgConjugate.o: /usr/include/bits/posix2_lim.h ../spinc/sperror.h
pgConjugate.o: ../spinc/iferrmsg.h ../spinc/strext.h
pgExtConst.o: ../inc/pgOptmz.h ../inc/pgCircuit.h ../inc/pgCommon.h
pgExtConst.o: ../inc/pgStddefs.h /usr/include/time.h /usr/include/features.h
pgExtConst.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
pgExtConst.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
pgExtConst.o: /usr/include/bits/time.h /usr/include/bits/types.h
pgExtConst.o: /usr/include/bits/typesizes.h ../inc/pgUnixstd.h
pgExtConst.o: /usr/include/stdio.h /usr/include/libio.h
pgExtConst.o: /usr/include/_G_config.h /usr/include/wchar.h
pgExtConst.o: /usr/include/bits/wchar.h /usr/include/gconv.h
pgExtConst.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
pgExtConst.o: /usr/include/string.h /usr/include/ctype.h
pgExtConst.o: /usr/include/endian.h /usr/include/bits/endian.h
pgExtConst.o: /usr/include/stdlib.h /usr/include/sys/types.h
pgExtConst.o: /usr/include/sys/select.h /usr/include/bits/select.h
pgExtConst.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
pgExtConst.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
pgExtConst.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
pgExtConst.o: /usr/include/bits/confname.h /usr/include/getopt.h
pgExtConst.o: /usr/include/errno.h /usr/include/bits/errno.h
pgExtConst.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
pgExtConst.o: /usr/include/asm-x86_64/errno.h
pgExtConst.o: /usr/include/asm-generic/errno.h
pgExtConst.o: /usr/include/asm-generic/errno-base.h /usr/include/assert.h
pgExtConst.o: ../inc/pgUtility.h ../inc/pgMessage.h ../spinc/spmatrix.h
pgExtConst.o: ../spinc/spconfig.h /usr/include/values.h /usr/include/limits.h
pgExtConst.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
pgExtConst.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
pgExtConst.o: ../spinc/sperror.h ../spinc/iferrmsg.h ../spinc/strext.h
pgNodeBranch.o: ../inc/pgCircuit.h ../inc/pgCommon.h ../inc/pgStddefs.h
pgNodeBranch.o: /usr/include/time.h /usr/include/features.h
pgNodeBranch.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
pgNodeBranch.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
pgNodeBranch.o: /usr/include/bits/time.h /usr/include/bits/types.h
pgNodeBranch.o: /usr/include/bits/typesizes.h ../inc/pgUnixstd.h
pgNodeBranch.o: /usr/include/stdio.h /usr/include/libio.h
pgNodeBranch.o: /usr/include/_G_config.h /usr/include/wchar.h
pgNodeBranch.o: /usr/include/bits/wchar.h /usr/include/gconv.h
pgNodeBranch.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
pgNodeBranch.o: /usr/include/string.h /usr/include/ctype.h
pgNodeBranch.o: /usr/include/endian.h /usr/include/bits/endian.h
pgNodeBranch.o: /usr/include/stdlib.h /usr/include/sys/types.h
pgNodeBranch.o: /usr/include/sys/select.h /usr/include/bits/select.h
pgNodeBranch.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
pgNodeBranch.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
pgNodeBranch.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
pgNodeBranch.o: /usr/include/bits/confname.h /usr/include/getopt.h
pgNodeBranch.o: /usr/include/errno.h /usr/include/bits/errno.h
pgNodeBranch.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
pgNodeBranch.o: /usr/include/asm-x86_64/errno.h
pgNodeBranch.o: /usr/include/asm-generic/errno.h
pgNodeBranch.o: /usr/include/asm-generic/errno-base.h /usr/include/assert.h
pgNodeBranch.o: ../inc/pgUtility.h ../inc/pgMessage.h ../spinc/spmatrix.h
pgNodeBranch.o: ../spinc/spconfig.h /usr/include/values.h
pgNodeBranch.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
pgNodeBranch.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
pgNodeBranch.o: /usr/include/bits/posix2_lim.h ../spinc/sperror.h
pgNodeBranch.o: ../spinc/iferrmsg.h ../spinc/strext.h
pgCostFunc.o: ../inc/pgOptmz.h ../inc/pgCircuit.h ../inc/pgCommon.h
pgCostFunc.o: ../inc/pgStddefs.h /usr/include/time.h /usr/include/features.h
pgCostFunc.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
pgCostFunc.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
pgCostFunc.o: /usr/include/bits/time.h /usr/include/bits/types.h
pgCostFunc.o: /usr/include/bits/typesizes.h ../inc/pgUnixstd.h
pgCostFunc.o: /usr/include/stdio.h /usr/include/libio.h
pgCostFunc.o: /usr/include/_G_config.h /usr/include/wchar.h
pgCostFunc.o: /usr/include/bits/wchar.h /usr/include/gconv.h
pgCostFunc.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
pgCostFunc.o: /usr/include/string.h /usr/include/ctype.h
pgCostFunc.o: /usr/include/endian.h /usr/include/bits/endian.h
pgCostFunc.o: /usr/include/stdlib.h /usr/include/sys/types.h
pgCostFunc.o: /usr/include/sys/select.h /usr/include/bits/select.h
pgCostFunc.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
pgCostFunc.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
pgCostFunc.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
pgCostFunc.o: /usr/include/bits/confname.h /usr/include/getopt.h
pgCostFunc.o: /usr/include/errno.h /usr/include/bits/errno.h
pgCostFunc.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
pgCostFunc.o: /usr/include/asm-x86_64/errno.h
pgCostFunc.o: /usr/include/asm-generic/errno.h
pgCostFunc.o: /usr/include/asm-generic/errno-base.h /usr/include/assert.h
pgCostFunc.o: ../inc/pgUtility.h ../inc/pgMessage.h ../spinc/spmatrix.h
pgCostFunc.o: ../spinc/spconfig.h /usr/include/values.h /usr/include/limits.h
pgCostFunc.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
pgCostFunc.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
pgCostFunc.o: ../spinc/sperror.h ../spinc/iferrmsg.h ../spinc/strext.h
pgIntConst.o: /usr/include/math.h /usr/include/features.h
pgIntConst.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
pgIntConst.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
pgIntConst.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
pgIntConst.o: /usr/include/bits/mathcalls.h ../inc/pgOptmz.h
pgIntConst.o: ../inc/pgCircuit.h ../inc/pgCommon.h ../inc/pgStddefs.h
pgIntConst.o: /usr/include/time.h /usr/include/bits/time.h
pgIntConst.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
pgIntConst.o: ../inc/pgUnixstd.h /usr/include/stdio.h /usr/include/libio.h
pgIntConst.o: /usr/include/_G_config.h /usr/include/wchar.h
pgIntConst.o: /usr/include/bits/wchar.h /usr/include/gconv.h
pgIntConst.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
pgIntConst.o: /usr/include/string.h /usr/include/ctype.h
pgIntConst.o: /usr/include/endian.h /usr/include/bits/endian.h
pgIntConst.o: /usr/include/stdlib.h /usr/include/sys/types.h
pgIntConst.o: /usr/include/sys/select.h /usr/include/bits/select.h
pgIntConst.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
pgIntConst.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
pgIntConst.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
pgIntConst.o: /usr/include/bits/confname.h /usr/include/getopt.h
pgIntConst.o: /usr/include/errno.h /usr/include/bits/errno.h
pgIntConst.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
pgIntConst.o: /usr/include/asm-x86_64/errno.h
pgIntConst.o: /usr/include/asm-generic/errno.h
pgIntConst.o: /usr/include/asm-generic/errno-base.h /usr/include/assert.h
pgIntConst.o: ../inc/pgUtility.h ../inc/pgMessage.h ../spinc/spmatrix.h
pgIntConst.o: ../spinc/spconfig.h /usr/include/values.h /usr/include/limits.h
pgIntConst.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
pgIntConst.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
pgIntConst.o: ../spinc/sperror.h ../spinc/iferrmsg.h ../spinc/strext.h
pgState.o: ../inc/pgOptmz.h ../inc/pgCircuit.h ../inc/pgCommon.h
pgState.o: ../inc/pgStddefs.h /usr/include/time.h /usr/include/features.h
pgState.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
pgState.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
pgState.o: /usr/include/bits/time.h /usr/include/bits/types.h
pgState.o: /usr/include/bits/typesizes.h ../inc/pgUnixstd.h
pgState.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
pgState.o: /usr/include/wchar.h /usr/include/bits/wchar.h
pgState.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
pgState.o: /usr/include/bits/sys_errlist.h /usr/include/string.h
pgState.o: /usr/include/ctype.h /usr/include/endian.h
pgState.o: /usr/include/bits/endian.h /usr/include/stdlib.h
pgState.o: /usr/include/sys/types.h /usr/include/sys/select.h
pgState.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
pgState.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
pgState.o: /usr/include/alloca.h /usr/include/unistd.h
pgState.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
pgState.o: /usr/include/getopt.h /usr/include/errno.h
pgState.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
pgState.o: /usr/include/asm/errno.h /usr/include/asm-x86_64/errno.h
pgState.o: /usr/include/asm-generic/errno.h
pgState.o: /usr/include/asm-generic/errno-base.h /usr/include/assert.h
pgState.o: ../inc/pgUtility.h ../inc/pgMessage.h ../spinc/spmatrix.h
pgState.o: ../spinc/spconfig.h /usr/include/values.h /usr/include/limits.h
pgState.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
pgState.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
pgState.o: ../spinc/sperror.h ../spinc/iferrmsg.h ../spinc/strext.h
pgMainLoop.o: ../inc/pgOptmz.h ../inc/pgCircuit.h ../inc/pgCommon.h
pgMainLoop.o: ../inc/pgStddefs.h /usr/include/time.h /usr/include/features.h
pgMainLoop.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
pgMainLoop.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
pgMainLoop.o: /usr/include/bits/time.h /usr/include/bits/types.h
pgMainLoop.o: /usr/include/bits/typesizes.h ../inc/pgUnixstd.h
pgMainLoop.o: /usr/include/stdio.h /usr/include/libio.h
pgMainLoop.o: /usr/include/_G_config.h /usr/include/wchar.h
pgMainLoop.o: /usr/include/bits/wchar.h /usr/include/gconv.h
pgMainLoop.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
pgMainLoop.o: /usr/include/string.h /usr/include/ctype.h
pgMainLoop.o: /usr/include/endian.h /usr/include/bits/endian.h
pgMainLoop.o: /usr/include/stdlib.h /usr/include/sys/types.h
pgMainLoop.o: /usr/include/sys/select.h /usr/include/bits/select.h
pgMainLoop.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
pgMainLoop.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
pgMainLoop.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
pgMainLoop.o: /usr/include/bits/confname.h /usr/include/getopt.h
pgMainLoop.o: /usr/include/errno.h /usr/include/bits/errno.h
pgMainLoop.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
pgMainLoop.o: /usr/include/asm-x86_64/errno.h
pgMainLoop.o: /usr/include/asm-generic/errno.h
pgMainLoop.o: /usr/include/asm-generic/errno-base.h /usr/include/assert.h
pgMainLoop.o: ../inc/pgUtility.h ../inc/pgMessage.h ../spinc/spmatrix.h
pgMainLoop.o: ../spinc/spconfig.h /usr/include/values.h /usr/include/limits.h
pgMainLoop.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
pgMainLoop.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
pgMainLoop.o: ../spinc/sperror.h ../spinc/iferrmsg.h ../spinc/strext.h
pgMainLoop.o: /usr/include/math.h /usr/include/bits/huge_val.h
pgMainLoop.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
pgNewton.o: /usr/include/math.h /usr/include/features.h
pgNewton.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
pgNewton.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
pgNewton.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
pgNewton.o: /usr/include/bits/mathcalls.h ../inc/pgOptmz.h ../inc/pgCircuit.h
pgNewton.o: ../inc/pgCommon.h ../inc/pgStddefs.h /usr/include/time.h
pgNewton.o: /usr/include/bits/time.h /usr/include/bits/types.h
pgNewton.o: /usr/include/bits/typesizes.h ../inc/pgUnixstd.h
pgNewton.o: /usr/include/stdio.h /usr/include/libio.h
pgNewton.o: /usr/include/_G_config.h /usr/include/wchar.h
pgNewton.o: /usr/include/bits/wchar.h /usr/include/gconv.h
pgNewton.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
pgNewton.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
pgNewton.o: /usr/include/bits/endian.h /usr/include/stdlib.h
pgNewton.o: /usr/include/sys/types.h /usr/include/sys/select.h
pgNewton.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
pgNewton.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
pgNewton.o: /usr/include/alloca.h /usr/include/unistd.h
pgNewton.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
pgNewton.o: /usr/include/getopt.h /usr/include/errno.h
pgNewton.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
pgNewton.o: /usr/include/asm/errno.h /usr/include/asm-x86_64/errno.h
pgNewton.o: /usr/include/asm-generic/errno.h
pgNewton.o: /usr/include/asm-generic/errno-base.h /usr/include/assert.h
pgNewton.o: ../inc/pgUtility.h ../inc/pgMessage.h ../spinc/spmatrix.h
pgNewton.o: ../spinc/spconfig.h /usr/include/values.h /usr/include/limits.h
pgNewton.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
pgNewton.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
pgNewton.o: ../spinc/sperror.h ../spinc/iferrmsg.h ../spinc/strext.h
pginit.o: ../inc/pgOptmz.h ../inc/pgCircuit.h ../inc/pgCommon.h
pginit.o: ../inc/pgStddefs.h /usr/include/time.h /usr/include/features.h
pginit.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
pginit.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
pginit.o: /usr/include/bits/time.h /usr/include/bits/types.h
pginit.o: /usr/include/bits/typesizes.h ../inc/pgUnixstd.h
pginit.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
pginit.o: /usr/include/wchar.h /usr/include/bits/wchar.h /usr/include/gconv.h
pginit.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
pginit.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
pginit.o: /usr/include/bits/endian.h /usr/include/stdlib.h
pginit.o: /usr/include/sys/types.h /usr/include/sys/select.h
pginit.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
pginit.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
pginit.o: /usr/include/alloca.h /usr/include/unistd.h
pginit.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
pginit.o: /usr/include/getopt.h /usr/include/errno.h
pginit.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
pginit.o: /usr/include/asm/errno.h /usr/include/asm-x86_64/errno.h
pginit.o: /usr/include/asm-generic/errno.h
pginit.o: /usr/include/asm-generic/errno-base.h /usr/include/assert.h
pginit.o: ../inc/pgUtility.h ../inc/pgMessage.h ../spinc/spmatrix.h
pginit.o: ../spinc/spconfig.h /usr/include/values.h /usr/include/limits.h
pginit.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
pginit.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
pginit.o: ../spinc/sperror.h ../spinc/iferrmsg.h ../spinc/strext.h
pgGolden.o: ../inc/pgCircuit.h ../inc/pgCommon.h ../inc/pgStddefs.h
pgGolden.o: /usr/include/time.h /usr/include/features.h
pgGolden.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
pgGolden.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
pgGolden.o: /usr/include/bits/time.h /usr/include/bits/types.h
pgGolden.o: /usr/include/bits/typesizes.h ../inc/pgUnixstd.h
pgGolden.o: /usr/include/stdio.h /usr/include/libio.h
pgGolden.o: /usr/include/_G_config.h /usr/include/wchar.h
pgGolden.o: /usr/include/bits/wchar.h /usr/include/gconv.h
pgGolden.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
pgGolden.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
pgGolden.o: /usr/include/bits/endian.h /usr/include/stdlib.h
pgGolden.o: /usr/include/sys/types.h /usr/include/sys/select.h
pgGolden.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
pgGolden.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
pgGolden.o: /usr/include/alloca.h /usr/include/unistd.h
pgGolden.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
pgGolden.o: /usr/include/getopt.h /usr/include/errno.h
pgGolden.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
pgGolden.o: /usr/include/asm/errno.h /usr/include/asm-x86_64/errno.h
pgGolden.o: /usr/include/asm-generic/errno.h
pgGolden.o: /usr/include/asm-generic/errno-base.h /usr/include/assert.h
pgGolden.o: ../inc/pgUtility.h ../inc/pgMessage.h ../spinc/spmatrix.h
pgGolden.o: ../spinc/spconfig.h /usr/include/values.h /usr/include/limits.h
pgGolden.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
pgGolden.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
pgGolden.o: ../spinc/sperror.h ../spinc/iferrmsg.h ../spinc/strext.h
pgGolden.o: ../inc/pgOptmz.h /usr/include/math.h /usr/include/bits/huge_val.h
pgGolden.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
pgProduct.o: ../inc/pgOptmz.h ../inc/pgCircuit.h ../inc/pgCommon.h
pgProduct.o: ../inc/pgStddefs.h /usr/include/time.h /usr/include/features.h
pgProduct.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
pgProduct.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
pgProduct.o: /usr/include/bits/time.h /usr/include/bits/types.h
pgProduct.o: /usr/include/bits/typesizes.h ../inc/pgUnixstd.h
pgProduct.o: /usr/include/stdio.h /usr/include/libio.h
pgProduct.o: /usr/include/_G_config.h /usr/include/wchar.h
pgProduct.o: /usr/include/bits/wchar.h /usr/include/gconv.h
pgProduct.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
pgProduct.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
pgProduct.o: /usr/include/bits/endian.h /usr/include/stdlib.h
pgProduct.o: /usr/include/sys/types.h /usr/include/sys/select.h
pgProduct.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
pgProduct.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
pgProduct.o: /usr/include/alloca.h /usr/include/unistd.h
pgProduct.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
pgProduct.o: /usr/include/getopt.h /usr/include/errno.h
pgProduct.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
pgProduct.o: /usr/include/asm/errno.h /usr/include/asm-x86_64/errno.h
pgProduct.o: /usr/include/asm-generic/errno.h
pgProduct.o: /usr/include/asm-generic/errno-base.h /usr/include/assert.h
pgProduct.o: ../inc/pgUtility.h ../inc/pgMessage.h ../spinc/spmatrix.h
pgProduct.o: ../spinc/spconfig.h /usr/include/values.h /usr/include/limits.h
pgProduct.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
pgProduct.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
pgProduct.o: ../spinc/sperror.h ../spinc/iferrmsg.h ../spinc/strext.h
