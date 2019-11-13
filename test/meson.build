########################## Get the rpath, when executing in an x86 sdk ########################

if get_option('oe-sdk').enabled()
  # Setup OE SYSROOT
  OECORE_TARGET_SYSROOT = run_command('sh', '-c', 'echo $OECORE_TARGET_SYSROOT').stdout().strip()
  if OECORE_TARGET_SYSROOT == ''
    error('Unable to get $OECORE_TARGET_SYSROOT, check your environment.')
  endif
  message('OE_SYSROOT: ' + OECORE_TARGET_SYSROOT)
  rpath = ':'.join([OECORE_TARGET_SYSROOT + '/lib', OECORE_TARGET_SYSROOT + '/usr/lib'])
  ld_so = run_command('sh', '-c', 'find ' + OECORE_TARGET_SYSROOT + '/lib/ld-*.so | sort -r -n | head -n1').stdout().strip()
  dynamic_linker = ['-Wl,-dynamic-linker,' + ld_so]
else
  dynamic_linker = []
endif


######################################## Building Test ########################################

pthread = dependency('threads')
deps += [gtest, gmock, pthread]

tests_list = [
  'error_notification',
  'snmp_client',
  'snmp_conf_manager',
  'snmp_serialize',
  'snmp_util'
]


######################################## Run the tests ########################################
foreach t : tests_list
  test('test_' + t.underscorify(), executable('phosphor_network_snmpconf-' + t.underscorify(), 'test_' + t + '.cpp',
                     implicit_include_directories: true,
                     objects: exe.extract_all_objects(),
                     include_directories : ['../'],
                     link_with: libsnmp,
                     build_rpath: get_option('oe-sdk').enabled() ? rpath : '',
                     dependencies: deps)
       )
endforeach
