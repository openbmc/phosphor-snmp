# meson build file for phosphor-snmp

project('phosphor-snmp',
        'cpp',
        version: '0.1', meson_version: '>=0.49.0',
        default_options: [
          'default_library=shared',
          'warning_level=3',
          'werror=true',
          'cpp_std=c++17'
        ])

add_project_arguments('-Wno-psabi', language: 'cpp')
add_project_link_arguments('-lstdc++fs', language : 'cpp')

cpp_comp = meson.get_compiler('cpp')


# Compiler flags and warnings

global_c_args = []
cxx_flags = ['-fpic', '-Wall']

foreach arg: cxx_flags
  if cpp_comp.has_multi_arguments(arg)
    global_c_args += arg
  endif
endforeach

add_project_arguments(global_c_args, language: 'cpp')


# Check for headers

if cpp_comp.has_header('systemd/sd-bus.h')
    ## Nothing to do
else
    warning('Could not find systemd/sd-bus.h...systemd development package required')
endif

if cpp_comp.has_header('net-snmp/net-snmp-config.h')
    ## Nothing to do
else
    warning('Requires net-snmp headers')
endif

build_tests = get_option('tests')
gmock = dependency('gmock', disabler: true, required: build_tests)
gtest = dependency('gtest', main: true, disabler: true, required: build_tests)


# Check for libraries

cppfs = meson.get_compiler('cpp').find_library('stdc++fs')
systemd = dependency('libsystemd', version : '>=221')
sdbusplus = dependency('sdbusplus')
sdbuspp = find_program('sdbus++')
phosphor_logging = dependency('phosphor-logging')
phosphor_dbus_interfaces = dependency('phosphor-dbus-interfaces')
netsnmp = meson.get_compiler('cpp').find_library('libnetsnmp')
crypto = dependency('libcrypto')


# Declare dependencies

deps = [
    systemd,
    sdbusplus,
    phosphor_logging,
    phosphor_dbus_interfaces,
    cppfs,
]

# Build configuration data to generate
# congif.h file

conf_data = configuration_data()

conf_data.set_quoted('BUSNAME_NETWORK_SNMP', 'xyz.openbmc_project.Network.SNMP', description : 'The DBus busname to own')
conf_data.set_quoted('OBJ_NETWORK_SNMP', '/xyz/openbmc_project/network/snmp/manager', description : 'The network snmp root DBus object path')
conf_data.set('CLASS_VERSION', '1', description : 'Path of directory having persisted snmp managers')
conf_data.set_quoted('SNMP_CONF_PERSIST_PATH', '/var/lib/phosphor-snmp/managers/', description : 'Class version to register with Cereal')

configure_file(output: 'config.h',
    configuration: conf_data
)


subdir('xyz/openbmc_project/Network/Client/Create')

phsophor_network_snmpconf_sources = [
    'snmp_main.cpp',
    'snmp_conf_manager.cpp',
    'snmp_client.cpp',
    'snmp_util.cpp',
    'snmp_serialize.cpp',
    server_cpp
]

exe = executable(
    'phosphor-network-snmpconf',
    phsophor_network_snmpconf_sources,
    implicit_include_directories: true,
    dependencies: deps,
    install: true,
    install_dir: get_option('bindir'))


# Generating a shared library libsnmp.la

libsnmp = shared_library(
    'snmp',
    'snmp_notification.cpp',
    'snmp_util.cpp',
    dependencies: [
        phosphor_dbus_interfaces,
        phosphor_logging,
        sdbusplus,
        crypto,
        netsnmp
    ],
    install: true
)

pkg = import('pkgconfig')
pkg.generate(name : 'phosphor-snmp',
             description: 'Phosphor snmp Utilities',
             version: meson.project_version(),
             libraries: libsnmp,
             url: 'https://github.com/openbmc/phosphor-snmp',
             filebase: 'phosphor-snmp'
)

# Build Test

if get_option('tests').enabled()
  subdir('test')
endif
