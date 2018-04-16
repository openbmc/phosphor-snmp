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

LT_PREREQ([2.4.6])
LT_INIT([dlopen disable-static shared])

PKG_CHECK_MODULES([PHOSPHOR_LOGGING], [phosphor-logging],,\
AC_MSG_ERROR(["Requires phosphor-logging package."]))

AC_CHECK_HEADERS([net-snmp/net-snmp-config.h],,\
    AC_MSG_ERROR(["Requires net-snmp headers"]))

# Checks for library functions
LT_INIT # Required for systemd linking

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

