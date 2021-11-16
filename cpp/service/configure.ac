#!/bin/sh
# Copyright (C) 2021 rozenslin
#
AC_INIT(server, dev-1.0)
AC_CANONICAL_SYSTEM
AC_PROG_CC

AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([dist-bzip2])
AH_TOP([
/* rozen-service: config.h */
#ifndef _STUDY_CONFIG_H_
#define _STUDY_CONFIG_H_

])
AH_BOTTOM([
#endif  /* _STUDY_CONFIG_H_ */
/* rozen-service: config.h -- end */
])


case "$target_os" in
darwin*)
    TARGET_OS=darwin
    ;;
linux*)
    TARGET_OS=linux
    AC_MSG_CHECKING(for Linux)
    if test -f /etc/redhat-release; then
        CPPFLAGS="$CPPFLAGS -DREDHAT -D_GNU_SOURCE"
        AC_MSG_RESULT(is Redhat)
        VENDOR=redhat
    fi
    if test -f /etc/debian_version; then
        CPPFLAGS="$CPPFLAGS -DDEBIAN -D_GNU_SOURCE"
        AC_MSG_RESULT(is Debian)
        VENDOR=debian
    fi
    ;;
*)
    ;;
esac

AC_CHECK_FUNCS(accept accept4)

AC_SUBST(TARGET_OS)
AC_SUBST(VENDOR)

AM_CONDITIONAL(REDHAT, test "$VENDOR" = redhat)
AM_CONDITIONAL(DEBIAN, test "$VENDOR" = debian)
AM_CONDITIONAL(DARWIN, test "$TARGET_OS" = darwin)

AC_CONFIG_FILES([
  Makefile
])
 
AC_OUTPUT
