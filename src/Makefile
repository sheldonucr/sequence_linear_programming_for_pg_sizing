#/*
# * $RCSfile: Makefile,v $
# * $Revision: 1.2 $
# * $Date: 2000/06/14 17:09:26 $, (c) Copyright 1999 by X.-D. Sheldon Tan
# */

.SUFFIX: .o .c .h

GETOPTION =
DELTAOPTION =
SCCSOPTION = 
RELEASEOPTION =

SHELL = /bin/sh

CFLAGS = -I$(INCFLAG) -p

INCFLAG = ../inc

DEBUG =
ULIB =   ../lib/sparse.a

SLIB =  -L$(OPENWINHOME)/lib\
	-lm
#	-lsunmath

RUNABLE = ../bin/pgopt
RUNABLE2 = clean
RUNABLE3 = depend

SUBDIR = ckt \
	 optmz \
	 door \
	 shell

OBJ = \
	 ckt/pgckt.o \
	 optmz/pgoptmz.o \
	 door/door.o \
	 shell/pgshell.o 

$(RUNABLE): ./.lastlog
	@echo " Waiting for build up object files "
	@for i in $(SUBDIR); \
	do \
	( \
	echo "#####"; \
	echo "    The Current Directory Is $$i :"; \
	cd $$i; \
	make; \
	) done
	@echo "#####"
	@echo -n "	    Wait for $(RUNABLE) generation ... "
	@gcc -o $(RUNABLE) $(OBJ) $(ULIB) $(SLIB) 
#	@cc -O -o $(RUNABLE) $(OBJ) $(ULIB) $(SLIB) 
#	@cc -o $(RUNABLE) $(OBJ) $(ULIB) $(SLIB) 
	@chmod 755 $(RUNABLE)
	@sleep 1
	@touch ./.lastlog
	@echo "OK !"

$(RUNABLE2):
	@for i in $(SUBDIR); \
	do \
	( \
	echo "#####"; \
	echo "    The Current Directory Is $$i :"; \
	cd $$i; \
	make clean; \
	) done
	@echo "#####"
	@echo "OK !"

$(RUNABLE3):
	@for i in $(SUBDIR); \
	do \
	( \
	echo "#####"; \
	echo "    The Current Directory Is $$i :"; \
	cd $$i; \
	make depend; \
	) done
	@echo "#####"
	@echo "OK !"

tags 	: ./.lastlog
	@rm tags
	@tag
	@echo "OK !"

touch:; @touch ./.lastlog
