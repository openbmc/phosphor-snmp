AM_CPPFLAGS = -I${top_srcdir} -I${top_builddir}

TESTS = $(check_PROGRAMS)

check_PROGRAMS = test

test_SOURCES = \
	test_error_notification.cpp \
	test_snmp_conf_manager.cpp \
	test_snmp_util.cpp

generic_cpp_flags = -Igtest $(GTEST_CPPFLAGS) $(AM_CPPFLAGS)

generic_cxx_flags = $(PTHREAD_CFLAGS) \
		$(SYSTEMD_CFLAGS) \
		$(SDBUSPLUS_CFLAGS) \
		$(PHOSPHOR_LOGGING_CFLAGS) \
		$(PHOSPHOR_DBUS_INTERFACES_CFLAGS)

generic_ld_flags =  -lgtest_main -lgtest -lstdc++fs \
		$(OESDK_TESTCASE_FLAGS) \
		$(SYSTEMD_LIBS) \
		$(SDBUSPLUS_LIBS) \
		$(PHOSPHOR_LOGGING_LIBS) \
		$(PHOSPHOR_DBUS_INTERFACES_LIBS)


test_CPPFLAGS = ${generic_cpp_flags}
test_CXXFLAGS = ${generic_cxx_flags}
test_LDFLAGS  = ${generic_ld_flags}

test_LDADD = $(top_builddir)/phosphor_network_snmpconf-snmp_conf_manager.o \
		$(top_builddir)/phosphor_network_snmpconf-snmp_client.o \
		$(top_builddir)/phosphor_network_snmpconf-snmp_serialize.o \
		$(top_builddir)/phosphor_network_snmpconf-snmp_util.o \
		$(top_builddir)/xyz/openbmc_project/Network/Client/Create/phosphor_network_snmpconf-server.o
