
CC		= gcc

CFILES		= spalloc.c spbuild.c spfactor.c \
		  spoutput.c  spsolve.c sputils.c spextra.c

COBJS		= spalloc.o spbuild.o spfactor.o \
		  spoutput.o  spsolve.o sputils.o spextra.o

ANALOG          = /projects/analog/${MACHINE}
LIBRARY		= sparse
TARGET	        = ../$(LIBRARY).a

########################################################################
# Customization - define the following:
#     SPARSELIB:  Directory where the library 'sparse.a' must be installed
########################################################################
# This works fine in Berkeley
#SPARSELIB       = ${ANALOG}/lib
SPARSELIB       = .
########################################################################

CFLAGS = -O -I${OCTTOOLS}/include -I./include

cleaninstall: install clean

install: ${TARGET}
	cp ${TARGET} ${SPARSELIB}

${TARGET}: ${COBJS}
	ar r $@ $?
	ranlib $@

clean:
	rm -f *.o sparse.a

spalloc.o: spalloc.c
spbuild.o: spbuild.c
spfactor.o: spfactor.c
spoutput.o: spoutput.c
#spsmp.o: spsmp.c
spsolve.o: spsolve.c
sputils.o: sputils.c
spextra.o: spextra.c
