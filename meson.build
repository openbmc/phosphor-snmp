project('phosphor-snmp',
        'cpp',
        version: '0.1', meson_version: '>=0.57.0',
        default_options: [
          'default_library=shared',
          'warning_level=3',
          'werror=true',
          'cpp_std=c++20',
          'buildtype=debugoptimized',
        ]
)

conf_data = configuration_data()

conf_data.set_quoted('BUSNAME_NETWORK_SNMP', 'xyz.openbmc_project.Network.SNMP',
                      description : 'The DBus busname to own')

conf_data.set('CLASS_VERSION', 1,
                      description : 'Class version to register with Cereal')

conf_data.set_quoted('OBJ_NETWORK_SNMP', '/xyz/openbmc_project/network/snmp/manager',
                      description : 'The network snmp root DBus object path')

conf_data.set_quoted('SNMP_CONF_PERSIST_PATH', get_option('snmp-conf-persist-path'),
                      description : 'Path of directory having persisted snmp managers')

conf_header = configure_file(output: 'config.h',
    configuration: conf_data)

phosphor_dbus_interfaces = dependency(
  'phosphor-dbus-interfaces',
  fallback: ['phosphor-dbus-interfaces', 'phosphor_dbus_interfaces_dep'],
)

sdbusplus = dependency(
  'sdbusplus',
  fallback: ['sdbusplus', 'sdbusplus_dep'],
)

phosphor_logging = dependency(
  'phosphor-logging',
   fallback: ['phosphor-logging', 'phosphor_logging_dep'],
)

libnetsnmp = meson.get_compiler('cpp').find_library('libnetsnmp')

cpp = meson.get_compiler('cpp')

if not cpp.has_header('net-snmp/net-snmp-config.h')
    warning('Requires net-snmp headers')
endif

src_includes = include_directories('.')

phsophor_network_snmpconf_src = [
    'snmp_main.cpp',
    'snmp_conf_manager.cpp',
    'snmp_client.cpp',
    'snmp_util.cpp',
    'snmp_serialize.cpp',
]

libsnmp_src = [
    'snmp_notification.cpp',
    'snmp_util.cpp',
]

snmp_deps = [
     sdbusplus,
     phosphor_logging,
     phosphor_dbus_interfaces
]

snmp_lib_deps = [
     sdbusplus,
     phosphor_logging,
     phosphor_dbus_interfaces,
     libnetsnmp
]


snmp_lib = static_library(
  'snmp',
  conf_header,
  phsophor_network_snmpconf_src,
  libsnmp_src,
  implicit_include_directories: false,
  include_directories: src_includes,
  dependencies: snmp_deps
)

snmp_dep = declare_dependency(
  dependencies: snmp_deps,
  include_directories: src_includes,
  link_with: snmp_lib
)

libsnmp = library(
    'snmp',
    libsnmp_src,
    dependencies: snmp_lib_deps,
    version: meson.project_version(),
    install: true,
)

exe = executable(
    'phosphor-network-snmpconf',
    phsophor_network_snmpconf_src,
    implicit_include_directories: true,
    dependencies: snmp_deps,
    install: true
)

install_headers('snmp.hpp')
install_headers('snmp_notification.hpp')

import('pkgconfig').generate(
    libraries: libsnmp,
    name: meson.project_name(),
    version: meson.project_version(),
    description: 'Phosphor snmp package'
)

if get_option('tests').enabled()
  subdir('test')
endif

