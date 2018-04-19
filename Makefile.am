nobase_nodist_include_HEADERS = \
		xyz/openbmc_project/Network/Client/Create/server.hpp

sbin_PROGRAMS = phosphor-network-snmpconf

noinst_HEADERS = \
		snmp_client.hpp \
		snmp_manager.hpp

phosphor_network_snmpconf_SOURCES = \
		snmp_main.cpp \
		snmp_manager.cpp \
		snmp_client.cpp \
		xyz/openbmc_project/Network/Client/Create/server.cpp

CLEANFILES = \
		xyz/openbmc_project/Network/Client/Create/server.cpp \
		xyz/openbmc_project/Network/Client/Create/server.hpp

BUILT_SOURCES = \
        xyz/openbmc_project/Network/Client/Create/server.cpp \
        xyz/openbmc_project/Network/Client/Create/server.hpp

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

xyz/openbmc_project/Network/Client/Create/server.cpp: xyz/openbmc_project/Network/Client/Create.interface.yaml xyz/openbmc_project/Network/Client/Create/server.hpp
	@mkdir -p `dirname $@`
	$(SDBUSPLUSPLUS) -r $(srcdir) interface server-cpp xyz.openbmc_project.Network.Client.Create > $@

xyz/openbmc_project/Network/Client/Create/server.hpp: xyz/openbmc_project/Network/Client/Create.interface.yaml
	@mkdir -p `dirname $@`
	$(SDBUSPLUSPLUS) -r $(srcdir) interface server-header xyz.openbmc_project.Network.Client.Create > $@

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
