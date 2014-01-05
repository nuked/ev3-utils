dnl -*- shell-script -*-
dnl
dnl Process this file with autoconf to produce a configure script.
dnl

AC_REVISION($Revision: 1.1 $)
AC_INIT(support.c)
AM_CONFIG_HEADER(config.h)
AC_PREREQ(2.12)

AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([ev3-utils], [0.0.1])

dnl Checks for programs.
AC_PROG_CC
AM_PROG_CC_STDC
AC_PROG_MAKE_SET

AM_C_PROTOTYPES

dnl Checks for libraries.

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(unistd.h sys/socket.h netdb.h netinet/in.h arpa/inet.h sys/wait.h stdlib.h)

dnl Checks for typedefs, structures, and compiler characteristics.

dnl Checks for library functions.

AC_OUTPUT([Makefile])

