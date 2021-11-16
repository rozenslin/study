SRCS = \
	   server.c

bin_PROGRAMS = server
server_SOURCES = $(SRCS) config.h
AM_CPPFLAGS = -I./
if REDHAT
AM_CPPFLAGS += -DREDHAT -D_GNU_SOURCE
endif

if DEBIAN
AM_CPPFLAGS += -DDEBIAN -D_GNU_SOURCE
endif

if DARWIN
AM_CPPFLAGS +=
endif
