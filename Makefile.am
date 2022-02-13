EXTRA_DIST = README.md
ACLOCAL_AMFLAGS = -I m4

pkgconfigdir = $(libdir)/pkgconfig

lib_LTLIBRARIES = src/libicc.la

src_libicc_la_SOURCES = src/icc.c

src_libicc_la_CFLAGS = -g -Wall -Werror -Wextra
src_libicc_la_CPPFLAGS = -g -Wall -Wformat -Wformat-security
src_libicc_la_LDFLAGS = -lm

pkginclude_HEADERS =  src/icc.h

.rc.lo:
	$(AM_V_GEN)$(LIBTOOL) $(AM_V_lt) $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --tag=RC --mode=compile $(RC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) -i "$<" -o "$@"

COMMON_CPPFLAGS = -I$(top_srcdir)/src
COMMON_LIBADD = src/libicc.la

TESTS = check_icc
check_PROGRAMS = check_icc
check_icc_SOURCES = tests/check_icc.c $(top_builddir)/src/icc.h
check_icc_CFLAGS = @CHECK_CFLAGS@
check_icc_LDADD = $(top_builddir)/src/libicc.la @CHECK_LIBS@
