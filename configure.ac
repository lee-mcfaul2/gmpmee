# Copyright 2008 Douglas Wikström

# This file is part of GMP Modular Exponentiation Extension (GMPMEE).

# GMPMEE is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# GMPMEE is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
# License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with GMPMEE.  If not, see
# <http://www.gnu.org/licenses/>.

#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([gmpmee], [m4_esyscmd([m4 .version.m4])], [dog@csc.kth.se])
AC_LANG_PUSH([C])
AM_INIT_AUTOMAKE([-Wall -Werror])
AC_CONFIG_SRCDIR([redc_init_set.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# We are building a library.
AC_PROG_LIBTOOL

AC_CONFIG_MACRO_DIR([m4])

# Checks for libraries.
AC_CHECK_LIB(gmp, __gmpz_init, ,
       [AC_MSG_ERROR(["GNU MP library not found, see http://gmplib.org/"])])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])
AC_CHECK_HEADERS([gmp.h], ,
       [AC_MSG_ERROR(["GNU MP header not found, see http://gmplib.org/"])])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC

# Conditional compilation of redc-representation of moduli.
AC_ARG_ENABLE([redc],
     [  --enable-redc    Use experimental redc-representation of moduli.],
     [case "${enableval}" in
       yes) redc=true ;

            # We can not check gmp-impl.h in the usual way, since it
            # requires that gmp.h has been included before it and
            # defined some macros.
            echo -n "checking gmp-impl.h presence... "
            AC_COMPILE_IFELSE([AC_LANG_PROGRAM([
#include "gmp.h"
#include "gmp-impl.h"
])
],
		              [echo "yes"],
                              [echo "no"; AC_MSG_ERROR(["The header file gmp-impl.h is not part of a standard GMP installation. It only appears in its source. Please add GMP's source directory to your include path."])]);;

       no)  redc=false ;;
       *) AC_MSG_ERROR([bad value ${enableval} for --enable-redc]) ;;
     esac],[redc=false])
     AM_CONDITIONAL([REDC], [test x$redc = xtrue])

AM_CONDITIONAL([BITS64], [test x`getconf LONG_BIT` = x64])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
