noinst_HEADERS = \
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
