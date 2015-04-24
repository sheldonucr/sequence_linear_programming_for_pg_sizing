#/*
# * $RCSfile: Makefile,v $
# * $Revision: 1.3 $
# * $Date: 2000/10/10 01:02:50 $, (c) Copyright 1999 by X.-D. Sheldon, Tan
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

OBJS	      = lex.o \
		yacc.o \
		pgWriteSpice.o \
		pgWriteConst.o \
		pgParseNetlist.o
		

PRINT	      = pr

SRCS       = lex.l \
		yacc.y \
		pgWriteSpice.c \
		pgWriteConst.c \
		pgParseNetlist.c


SHELL	      = /bin/sh

PROGRAM	      = door.o
	
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

lex.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
lex.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
lex.o: /usr/include/gnu/stubs-64.h /usr/include/bits/types.h
lex.o: /usr/include/bits/typesizes.h /usr/include/libio.h
lex.o: /usr/include/_G_config.h /usr/include/wchar.h
lex.o: /usr/include/bits/wchar.h /usr/include/gconv.h
lex.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
lex.o: /usr/include/ctype.h /usr/include/endian.h /usr/include/bits/endian.h
lex.o: ../inc/pgDoor.h ../inc/pgCircuit.h ../inc/pgCommon.h
lex.o: ../inc/pgStddefs.h /usr/include/time.h /usr/include/bits/time.h
lex.o: ../inc/pgUnixstd.h /usr/include/string.h /usr/include/stdlib.h
lex.o: /usr/include/sys/types.h /usr/include/sys/select.h
lex.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
lex.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
lex.o: /usr/include/alloca.h /usr/include/unistd.h
lex.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
lex.o: /usr/include/getopt.h /usr/include/errno.h /usr/include/bits/errno.h
lex.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
lex.o: /usr/include/asm-x86_64/errno.h /usr/include/asm-generic/errno.h
lex.o: /usr/include/asm-generic/errno-base.h /usr/include/assert.h
lex.o: ../inc/pgUtility.h ../inc/pgMessage.h ../spinc/spmatrix.h
lex.o: ../spinc/spconfig.h /usr/include/values.h /usr/include/limits.h
lex.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
lex.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
lex.o: ../spinc/sperror.h ../spinc/iferrmsg.h ../spinc/strext.h yacc.h
yacc.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
yacc.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
yacc.o: /usr/include/gnu/stubs-64.h /usr/include/bits/types.h
yacc.o: /usr/include/bits/typesizes.h /usr/include/libio.h
yacc.o: /usr/include/_G_config.h /usr/include/wchar.h
yacc.o: /usr/include/bits/wchar.h /usr/include/gconv.h
yacc.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
yacc.o: ../inc/pgDoor.h ../inc/pgCircuit.h ../inc/pgCommon.h
yacc.o: ../inc/pgStddefs.h /usr/include/time.h /usr/include/bits/time.h
yacc.o: ../inc/pgUnixstd.h /usr/include/string.h /usr/include/ctype.h
yacc.o: /usr/include/endian.h /usr/include/bits/endian.h
yacc.o: /usr/include/stdlib.h /usr/include/sys/types.h
yacc.o: /usr/include/sys/select.h /usr/include/bits/select.h
yacc.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
yacc.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
yacc.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
yacc.o: /usr/include/bits/confname.h /usr/include/getopt.h
yacc.o: /usr/include/errno.h /usr/include/bits/errno.h
yacc.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
yacc.o: /usr/include/asm-x86_64/errno.h /usr/include/asm-generic/errno.h
yacc.o: /usr/include/asm-generic/errno-base.h /usr/include/assert.h
yacc.o: ../inc/pgUtility.h ../inc/pgMessage.h ../spinc/spmatrix.h
yacc.o: ../spinc/spconfig.h /usr/include/values.h /usr/include/limits.h
yacc.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
yacc.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
yacc.o: ../spinc/sperror.h ../spinc/iferrmsg.h ../spinc/strext.h
pgWriteSpice.o: ../inc/pgCommon.h ../inc/pgStddefs.h /usr/include/time.h
pgWriteSpice.o: /usr/include/features.h /usr/include/sys/cdefs.h
pgWriteSpice.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
pgWriteSpice.o: /usr/include/gnu/stubs-64.h /usr/include/bits/time.h
pgWriteSpice.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
pgWriteSpice.o: ../inc/pgUnixstd.h /usr/include/stdio.h /usr/include/libio.h
pgWriteSpice.o: /usr/include/_G_config.h /usr/include/wchar.h
pgWriteSpice.o: /usr/include/bits/wchar.h /usr/include/gconv.h
pgWriteSpice.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
pgWriteSpice.o: /usr/include/string.h /usr/include/ctype.h
pgWriteSpice.o: /usr/include/endian.h /usr/include/bits/endian.h
pgWriteSpice.o: /usr/include/stdlib.h /usr/include/sys/types.h
pgWriteSpice.o: /usr/include/sys/select.h /usr/include/bits/select.h
pgWriteSpice.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
pgWriteSpice.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
pgWriteSpice.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
pgWriteSpice.o: /usr/include/bits/confname.h /usr/include/getopt.h
pgWriteSpice.o: /usr/include/errno.h /usr/include/bits/errno.h
pgWriteSpice.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
pgWriteSpice.o: /usr/include/asm-x86_64/errno.h
pgWriteSpice.o: /usr/include/asm-generic/errno.h
pgWriteSpice.o: /usr/include/asm-generic/errno-base.h /usr/include/assert.h
pgWriteSpice.o: ../inc/pgUtility.h ../inc/pgMessage.h ../inc/pgDoor.h
pgWriteSpice.o: ../inc/pgCircuit.h ../spinc/spmatrix.h ../spinc/spconfig.h
pgWriteSpice.o: /usr/include/values.h /usr/include/limits.h
pgWriteSpice.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
pgWriteSpice.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
pgWriteSpice.o: ../spinc/sperror.h ../spinc/iferrmsg.h ../spinc/strext.h
pgWriteSpice.o: /usr/include/math.h /usr/include/bits/huge_val.h
pgWriteSpice.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
pgWriteConst.o: ../inc/pgCommon.h ../inc/pgStddefs.h /usr/include/time.h
pgWriteConst.o: /usr/include/features.h /usr/include/sys/cdefs.h
pgWriteConst.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
pgWriteConst.o: /usr/include/gnu/stubs-64.h /usr/include/bits/time.h
pgWriteConst.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
pgWriteConst.o: ../inc/pgUnixstd.h /usr/include/stdio.h /usr/include/libio.h
pgWriteConst.o: /usr/include/_G_config.h /usr/include/wchar.h
pgWriteConst.o: /usr/include/bits/wchar.h /usr/include/gconv.h
pgWriteConst.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
pgWriteConst.o: /usr/include/string.h /usr/include/ctype.h
pgWriteConst.o: /usr/include/endian.h /usr/include/bits/endian.h
pgWriteConst.o: /usr/include/stdlib.h /usr/include/sys/types.h
pgWriteConst.o: /usr/include/sys/select.h /usr/include/bits/select.h
pgWriteConst.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
pgWriteConst.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
pgWriteConst.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
pgWriteConst.o: /usr/include/bits/confname.h /usr/include/getopt.h
pgWriteConst.o: /usr/include/errno.h /usr/include/bits/errno.h
pgWriteConst.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
pgWriteConst.o: /usr/include/asm-x86_64/errno.h
pgWriteConst.o: /usr/include/asm-generic/errno.h
pgWriteConst.o: /usr/include/asm-generic/errno-base.h /usr/include/assert.h
pgWriteConst.o: ../inc/pgUtility.h ../inc/pgMessage.h ../inc/pgDoor.h
pgWriteConst.o: ../inc/pgCircuit.h ../spinc/spmatrix.h ../spinc/spconfig.h
pgWriteConst.o: /usr/include/values.h /usr/include/limits.h
pgWriteConst.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
pgWriteConst.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
pgWriteConst.o: ../spinc/sperror.h ../spinc/iferrmsg.h ../spinc/strext.h
pgWriteConst.o: /usr/include/math.h /usr/include/bits/huge_val.h
pgWriteConst.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
pgParseNetlist.o: ../inc/pgCommon.h ../inc/pgStddefs.h /usr/include/time.h
pgParseNetlist.o: /usr/include/features.h /usr/include/sys/cdefs.h
pgParseNetlist.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
pgParseNetlist.o: /usr/include/gnu/stubs-64.h /usr/include/bits/time.h
pgParseNetlist.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
pgParseNetlist.o: ../inc/pgUnixstd.h /usr/include/stdio.h
pgParseNetlist.o: /usr/include/libio.h /usr/include/_G_config.h
pgParseNetlist.o: /usr/include/wchar.h /usr/include/bits/wchar.h
pgParseNetlist.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
pgParseNetlist.o: /usr/include/bits/sys_errlist.h /usr/include/string.h
pgParseNetlist.o: /usr/include/ctype.h /usr/include/endian.h
pgParseNetlist.o: /usr/include/bits/endian.h /usr/include/stdlib.h
pgParseNetlist.o: /usr/include/sys/types.h /usr/include/sys/select.h
pgParseNetlist.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
pgParseNetlist.o: /usr/include/sys/sysmacros.h
pgParseNetlist.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
pgParseNetlist.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
pgParseNetlist.o: /usr/include/bits/confname.h /usr/include/getopt.h
pgParseNetlist.o: /usr/include/errno.h /usr/include/bits/errno.h
pgParseNetlist.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
pgParseNetlist.o: /usr/include/asm-x86_64/errno.h
pgParseNetlist.o: /usr/include/asm-generic/errno.h
pgParseNetlist.o: /usr/include/asm-generic/errno-base.h /usr/include/assert.h
pgParseNetlist.o: ../inc/pgUtility.h ../inc/pgMessage.h ../inc/pgDoor.h
pgParseNetlist.o: ../inc/pgCircuit.h ../spinc/spmatrix.h ../spinc/spconfig.h
pgParseNetlist.o: /usr/include/values.h /usr/include/limits.h
pgParseNetlist.o: /usr/include/bits/posix1_lim.h
pgParseNetlist.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
pgParseNetlist.o: /usr/include/bits/posix2_lim.h ../spinc/sperror.h
pgParseNetlist.o: ../spinc/iferrmsg.h ../spinc/strext.h
