# export these headers
nobase_include_HEADERS = snmp.hpp \
		snmp_notification.hpp

libsnmpdir = ${libdir}

libsnmp_LTLIBRARIES = libsnmp.la
libsnmp_la_SOURCES = \
		snmp_notification.cpp

libsnmp_la_LDFLAGS = \
		$(SDBUSPLUS_LIBS) \
		$(PHOSPHOR_DBUS_INTERFACES_LIBS) \
		$(PHOSPHOR_LOGGING_LIBS) \
		-lnetsnmp \
		-lcrypto \
		-version-info 0:0:0 -shared

pkgconfiglibdir = ${libdir}/pkgconfig
pkgconfiglib_DATA = phosphor-snmp.pc

@CODE_COVERAGE_RULES@

check_PROGRAMS =
XFAIL_TESTS =

include test/Makefile.am.include

TESTS = $(check_PROGRAMS)

clean-local: code-coverage-clean
dist-clean-local: code-coverage-dist-clean

