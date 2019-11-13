############################## meson build file for phosphor-snmp #############################

project('phosphor-snmp',
        'cpp',
        version: '0.1', meson_version: '>=0.54.2',
        default_options: [
          'default_library=shared',
          'warning_level=3',
          'werror=true',
          'cpp_std=c++17',
          'buildtype=debugoptimized',
          'b_lto=true'
        ])


################################## Validate the c++ Standard ##################################

if get_option('cpp_std') != 'c++17'
    error('This project requires cpp std to be in c++17 mode')
endif


############################# Get compiler and default build type #############################

cpp_comp = meson.get_compiler('cpp')
build = get_option('buildtype')
summary('Build Type', build, section : 'Build Info')


#################################### Set the compiler flags ###################################

add_project_arguments(
  cpp_comp.get_supported_arguments([
        '-Wno-pedantic',
        '-fpic',
        ]),
        language : 'cpp')


###################################### Check for headers ######################################

if not cpp_comp.has_header('systemd/sd-bus.h')
    warning('Could not find systemd/sd-bus.h...systemd development package required')
endif

if not cpp_comp.has_header('net-snmp/net-snmp-config.h')
    warning('Requires net-snmp headers')
endif

gmock = dependency('gmock', required: get_option('tests'))
gtest = dependency('gtest', main: true, disabler: true, required: get_option('tests'))


################################ Check for dependency modules #################################

systemd = dependency('libsystemd', version : '>=221')
sdbusplus = dependency('sdbusplus')
sdbuspp = find_program('sdbus++')
phosphor_logging = dependency('phosphor-logging')
phosphor_dbus_interfaces = dependency('phosphor-dbus-interfaces')
netsnmp = meson.get_compiler('cpp').find_library('libnetsnmp')
crypto = dependency('libcrypto')
sdbusgen_prog = find_program('sdbus++-gendir')


##################################### Declare dependencies ####################################

deps = [
    systemd,
    sdbusplus,
    phosphor_logging,
    phosphor_dbus_interfaces,
]


##################### Build configuration data to generate 'congif.h' file ####################

conf_data = configuration_data()

conf_data.set_quoted('BUSNAME_NETWORK_SNMP', 'xyz.openbmc_project.Network.SNMP',
                      description : 'The DBus busname to own')
conf_data.set('CLASS_VERSION', 1,
                      description : 'Class version to register with Cereal')
conf_data.set_quoted('OBJ_NETWORK_SNMP', '/xyz/openbmc_project/network/snmp/manager',
                      description : 'The network snmp root DBus object path')
conf_data.set_quoted('SNMP_CONF_PERSIST_PATH', get_option('snmp-conf-persist-path'),
                      description : 'Path of directory having persisted snmp managers')

configure_file(output: 'config.h',
    configuration: conf_data
)


################################# Generate the custom targets #################################

custom_gen_files = files(
    run_command(
        sdbusgen_prog,
        '--tool', sdbuspp,
        '--output', meson.current_build_dir(),
        'xyz',
        check: true
    ).stdout().strip().split('\n')
)


############################ Gather sources for the target binaries ############################

phsophor_network_snmpconf_sources = [
    custom_gen_files,
    'snmp_main.cpp',
    'snmp_conf_manager.cpp',
    'snmp_client.cpp',
    'snmp_util.cpp',
    'snmp_serialize.cpp',
]


################################## Build the target binaries ##################################

exe = executable(
    'phosphor-network-snmpconf',
    phsophor_network_snmpconf_sources,
    implicit_include_directories: true,
    dependencies: deps,
    install: true)


############################ Generating shared library libsnmp.la #############################

libsnmp_src = files(
    'snmp_notification.cpp',
    'snmp_util.cpp',
)

libsnmp = library(
    'snmp',
    libsnmp_src,
    dependencies: [
        phosphor_dbus_interfaces,
        phosphor_logging,
        sdbusplus,
        crypto,
        netsnmp
    ],
    version: meson.project_version(),
    install: true,
)


########################################## Build Test ##########################################

if get_option('tests').enabled()
  subdir('test')
endif

#################################### Generate pkgconfig file ###################################

install_subdir(
    'include',
    install_dir: get_option('includedir'),
    strip_directory: false,
)

import('pkgconfig').generate(
    libraries: libsnmp,
    name: meson.project_name(),
    version: meson.project_version(),
    requires: [systemd, sdbusplus],
    description: 'Phosphor snmp Utilities'
)

