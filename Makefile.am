# export these headers
nobase_include_HEADERS = snmp.hpp \
		snmp_notification.hpp

bin_PROGRAMS = phosphor-network-snmpconf

noinst_HEADERS = \
		snmp_client.hpp \
		snmp_conf_manager.hpp \
		snmp_util.hpp \
		snmp_serialize.hpp

phosphor_network_snmpconf_SOURCES = \
		snmp_main.cpp \
		snmp_conf_manager.cpp \
		snmp_client.cpp \
		snmp_util.cpp \
		snmp_serialize.cpp

phosphor_network_snmpconf_LDFLAGS = \
		$(SYSTEMD_LIBS) \
		$(SDBUSPLUS_LIBS) \
		$(PHOSPHOR_DBUS_INTERFACES_LIBS) \
		$(PHOSPHOR_LOGGING_LIBS) \
		-lstdc++fs

phosphor_network_snmpconf_CXXFLAGS = \
		$(SYSTEMD_CFLAGS) \
		$(SDBUSPLUS_CFLAGS) \
		$(PHOSPHOR_DBUS_INTERFACES_CFLAGS) \
		$(PHOSPHOR_LOGGING_CFLAGS)

libsnmpdir = ${libdir}

libsnmp_LTLIBRARIES = libsnmp.la
libsnmp_la_SOURCES = \
		snmp_notification.cpp \
		snmp_util.cpp

libsnmp_la_LDFLAGS = \
		$(SDBUSPLUS_LIBS) \
		$(PHOSPHOR_DBUS_INTERFACES_LIBS) \
		$(PHOSPHOR_LOGGING_LIBS) \
		-lnetsnmp \
		-lcrypto \
		-version-info 0:0:0 -shared

pkgconfiglibdir = ${libdir}/pkgconfig
pkgconfiglib_DATA = phosphor-snmp.pc

if AUTOCONF_CODE_COVERAGE_2019_01_06
include $(top_srcdir)/aminclude_static.am
clean-local: code-coverage-clean
distclean-local: code-coverage-dist-clean
else
@CODE_COVERAGE_RULES@
endif

check_PROGRAMS =
XFAIL_TESTS =

include test/Makefile.am.include

TESTS = $(check_PROGRAMS)
