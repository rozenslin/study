#!/bin/sh
# Copyright (C) 2021 rozenslin
#
AC_INIT(server, dev-1.0)

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

AC_PROG_CC

AC_MSG_CHECKING(for Linux)
        if test -f /etc/redhat-release; then
            CPPFLAGS="$CPPFLAGS -DREDHAT -D_GNU_SOURCE"
            AC_MSG_RESULT(is Redhat)
        else
            AC_MSG_RESULT(not Redhat)
        fi

AC_CHECK_FUNCS(accept accept4)

AC_CONFIG_FILES([
  Makefile
])
 
AC_OUTPUT
