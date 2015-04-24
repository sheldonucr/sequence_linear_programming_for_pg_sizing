#/*
# * $RCSfile: Makefile,v $
# * $Revision: 1.1 $
# * $Date: 2002/05/05 23:27:17 $, (c) Copyright 2002 by X.-D. Sheldon Tan
# */

CFLAGS	      = -g 

CC	      = gcc

DEST	      = .

HDRS	      =

INSTALL	      = install

LD	      = ld

LDFLAGS	      = -r

LIBS	      = 

MAKEFILE      = Makefile

PRINT	      = pr

SHELL	      = /bin/sh

all:		gen_gd gen_pr gen_gd_sim gen_pr_sim gen_pr_sim_rc

gen_gd:		gen_gd.c
		$(CC) $(CFLAGS) -o gen_gd gen_gd.c 	

gen_pr:		gen_pr.c
		$(CC) $(CFLAGS) -o gen_pr gen_pr.c 	
		
gen_gd_sim:		gen_gd_sim.c
		$(CC) $(CFLAGS) -o gen_gd_sim gen_gd_sim.c 	

gen_pr_sim:		gen_pr_sim.c
		$(CC) $(CFLAGS) -o gen_pr_sim gen_pr_sim.c 	

gen_pr_sim_rc:		gen_pr_sim_rc.c
		$(CC) $(CFLAGS) -o gen_pr_sim_rc gen_pr_sim_rc.c 	
clean: 
	rm gen_gd gen_pr gen_gd_sim gen_pr_sim gen_pr_sim_rc
