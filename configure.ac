# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([orientdb-c],[0.9])
AC_CONFIG_SRCDIR([src/o_query_internal.h])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE()

LT_INIT([disable-static])
# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([malloc.h memory.h netdb.h netinet/in.h stdlib.h string.h sys/ioctl.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([gethostbyname memset socket strchr strcspn strdup strerror])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
