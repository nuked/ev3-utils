## -*- makefile -*-
##
## Process this file with automake to produce Makefile.in
##
## $Source:$
##
## $Id:$
##

VPATH = @srcdir@
INCLUDES=-I.
DEFS=@DEFS@ -Wall
CFLAGS=@CFLAGS@

## Cannot use ansi2knr because it is under GPL
## AUTOMAKE_OPTIONS=ansi2knr
## config.guess and config.sub have special exceptions
AUTOMAKE_OPTIONS=foreign

bin_PROGRAMS = pwmcmd

pwmcmd_SOURCES = pwmcmd.c support.c support.h

