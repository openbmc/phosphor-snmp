#Building Test
 
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

gtest = dependency('gtest', main: true, disabler: true, required: true)
gmock = dependency('gmock', disabler: true, required: true)

deps += [gtest, gmock]

tests = [
  'test_error_notification',
  'test_snmp_conf_manager',
  'test_snmp_util',
  'test_snmp_serialize',
  'test_snmp_client',
]

foreach t : tests
  test(t, executable(t.underscorify(), t + '.cpp',
                     implicit_include_directories: true,
                     objects: exe.extract_all_objects(),
                     include_directories: '..',
                     link_args: [dynamic_linker, '-lgmock'],
                     link_with: libsnmp,
                     build_rpath: get_option('oe-sdk').enabled() ? rpath : '',
                     dependencies: deps),
       workdir: meson.current_source_dir())
endforeach
