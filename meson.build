project(
    'phosphor-snmp', 'cpp',
    version : '1.0.0',
    meson_version: '>=1.1.1',
    default_options: [
        'warning_level=3',
        'werror=true',
        'cpp_std=c++23',
        'buildtype=debugoptimized',
    ]
)

conf_data = configuration_data()
conf_data.set_quoted('BUSNAME_NETWORK_SNMP', 'xyz.openbmc_project.Network.SNMP')
conf_data.set_quoted('OBJ_NETWORK_SNMP', '/xyz/openbmc_project/network/snmp/manager')
conf_data.set_quoted('SNMP_CONF_PERSIST_PATH', '/var/lib/phosphor-snmp/managers/')
conf_data.set('CLASS_VERSION', 1)

sdbusplus_dep = dependency('sdbusplus')
phosphor_dbus_interfaces_dep = dependency('phosphor-dbus-interfaces')
phosphor_logging_dep = dependency('phosphor-logging')
libsystemd_dep = dependency('libsystemd')
netsnmp_dep = dependency('netsnmp')
libcrypto_dep = dependency('libcrypto')

snmp_headers = include_directories('.')

deps = [
    libsystemd_dep,
    phosphor_dbus_interfaces_dep,
    phosphor_logging_dep,
    sdbusplus_dep,
]

sources = [
    'snmp_main.cpp',
    'snmp_conf_manager.cpp',
    'snmp_client.cpp',
    'snmp_util.cpp',
    'snmp_serialize.cpp',
]

configure_file(output: 'config.h',
    configuration: conf_data
)

executable(
    'phosphor-network-snmpconf',
    sources,
    implicit_include_directories: true,
    dependencies: deps,
    install: true,
    install_dir: get_option('bindir')
)

libsnmp_deps = [
    sdbusplus_dep,
    phosphor_logging_dep,
    phosphor_dbus_interfaces_dep,
    netsnmp_dep,
    libcrypto_dep,
]

libsnmp_sources = files(
    'snmp_notification.cpp',
    'snmp_util.cpp',
)

libsnmp_lib = library(
    'snmp',
    libsnmp_sources,
    include_directories: snmp_headers,
    implicit_include_directories: false,
    dependencies: libsnmp_deps,
    version: meson.project_version(),
    install: true,
)

phosphor_snmp_dep = declare_dependency(
    include_directories: snmp_headers,
    link_with: libsnmp_lib,
)

import('pkgconfig').generate(
    libsnmp_lib,
    name: meson.project_name(),
    version: meson.project_version(),
    requires: sdbusplus_dep,
    description: 'Phosphor snmp utilities',
)

install_headers(
    'snmp.hpp',
    'snmp_notification.hpp',
    subdir: '.',
)

build_tests = get_option('tests')
if not build_tests.disabled()
  subdir('test')
endif
