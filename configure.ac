AC_PREREQ([2.69])
AC_INIT([phosphor-snmp], [1.0], [https://github.com/openbmc/openbmc/phosphor-snmp.git])
AC_LANG([C++])
AM_INIT_AUTOMAKE([subdir-objects -Wall -Werror foreign])
AM_SILENT_RULES([yes])

# Compiler flags
CXXFLAGS="$CXXFLAGS -fpic -Wall -Werror"

# Check for programs
AC_PROG_CXX
AM_PROG_AR
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P

LT_PREREQ([2.4.6])
LT_INIT([dlopen disable-static shared])

# Checks for libraries.
PKG_CHECK_MODULES([SYSTEMD], [libsystemd >= 221])

PKG_CHECK_MODULES([SDBUSPLUS], [sdbusplus],,\
AC_MSG_ERROR(["Requires sdbusplus package."]))


AC_PATH_PROG([SDBUSPLUSPLUS], [sdbus++])

PKG_CHECK_MODULES([PHOSPHOR_LOGGING], [phosphor-logging],,\
AC_MSG_ERROR(["Requires phosphor-logging package."]))

# Checks for header files.
AC_CHECK_HEADER(systemd/sd-bus.h, ,\
[AC_MSG_ERROR([Could not find systemd/sd-bus.h...systemd development package required])])

AX_PKG_CHECK_MODULES([PHOSPHOR_DBUS_INTERFACES], [], [phosphor-dbus-interfaces],\
[], [AC_MSG_ERROR(["phosphor-dbus-interfaces required and not found."])])

AC_CHECK_HEADERS([net-snmp/net-snmp-config.h],,\
    AC_MSG_ERROR(["Requires net-snmp headers"]))

AC_ARG_VAR(BUSNAME_NETWORK_SNMP, [The Dbus busname to own])
AS_IF([test "x$BUSNAME_NETWORK_SNMP" == "x"], [BUSNAME_NETWORK_SNMP="xyz.openbmc_project.Network.SNMP"])
AC_DEFINE_UNQUOTED([BUSNAME_NETWORK_SNMP], ["$BUSNAME_NETWORK_SNMP"], [The DBus busname to own])

AC_ARG_VAR(OBJ_NETWORK_SNMP, [The network snmp root DBus object path])
AS_IF([test "x$OBJ_NETWORK_SNMP" == "x"], [OBJ_NETWORK_SNMP="/xyz/openbmc_project/network/snmp/manager"])
AC_DEFINE_UNQUOTED([OBJ_NETWORK_SNMP], ["$OBJ_NETWORK_SNMP"], [The network snmp root DBus object path])


# Checks for library functions
LT_INIT # Required for systemd linking

# Create configured output
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT