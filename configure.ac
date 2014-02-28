#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([arkanoid], [0.1], [erik.sohns@web.de], [arkanoid], [http://www.example.org])
AC_PREREQ(2.63)
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_AUX_DIR([aux_config])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# define meta-package
META_PACKAGE=arkanoid
AC_DEFINE([META_PACKAGE], ["arkanoid"], [meta-package])

# define version information
AC_SUBST([ARKANOID_SO_VERSION], [0:0:0])
AC_SUBST([ARKANOID_API_VERSION], [0.1])

# get system information
AC_CANONICAL_TARGET

AC_LANG([C++])

# checks for programs
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
# AC_PROG_RANLIB
AC_PROG_LEX
AC_PROG_YACC

# init other tools
AM_INIT_AUTOMAKE
# build time sanity check...
AM_SANITY_CHECK
LT_INIT

# prefix config.h
AX_PREFIX_CONFIG_H([arkanoid_config.h], [arkanoid], [config.h])

# reset the cpp compiler flags
CXXFLAGS=-std=gnu++11
# --> enable debug support ?
AC_MSG_CHECKING(whether to enable debugging)
debug_default="no"
AC_ARG_ENABLE(debug,
              [ --enable-debug=[no/yes] turn on debugging [default=$debug_default]],
              ,
              enable_debug=$debug_default)
if test "x$enable_debug" = "xyes"; then
  CXXFLAGS="${CXXFLAGS} -g -O0 -Wall -D_DEBUG"
  AC_MSG_RESULT(yes)
else
  CXXFLAGS="${CXXFLAGS} -O3 -march=native -Wall -Werror -Wno-unused-variable -Wno-sign-compare"
  AC_MSG_RESULT(no)
fi

# --> enable valgrind support ?
AC_MSG_CHECKING(whether to enable valgrind support)
valgrind_support_default="no"
AC_ARG_ENABLE(valgrind,
              [ --enable-valgrind=[no/yes] turn on valgrind support [default=$valgrind_support_default]],
              ,
              enable_valgrind=$valgrind_support_default)
if test "x$enable_valgrind" = "xyes"; then
  CXXFLAGS="${CXXFLAGS} -DARKANOID_ENABLE_VALGRIND_SUPPORT"
  AC_MSG_RESULT(yes)
else
  AC_MSG_RESULT(no)
fi

# export these variable (so Makefile substitutions can be made
AC_SUBST(META_PACKAGE)
AC_SUBST(CXXFLAGS)

# checks for libraries
PKG_CHECK_MODULES(pkg_libs, sdl SDL_ttf SDL_image)
AC_SEARCH_LIBS([Mix_Init], [SDL_mixer])

# checks for other libraries (non-pkg-config)
# FIXME: Replace `main' with a function in `-lACE':
#AC_CHECK_LIB([ACE], [main])
# FIXME: Replace `main' with a function in `-lpthread':
#AC_CHECK_LIB([pthread], [main])
# FIXME: Replace `main' with a function in `-lxerces-c':
#AC_CHECK_LIB([xerces-c], [main])

# checks for header files
AC_HEADER_STDBOOL

# checks for typedefs, structures, and compiler characteristics.

# checks for library functions
AC_CHECK_FUNCS([memset])

AC_CONFIG_FILES([Makefile
                 arkanoid-${ARKANOID_API_VERSION}.pc:arkanoid.pc.in
                 Arkanoid/Makefile])

AC_OUTPUT

