#Building Test

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
                     link_with: libsnmp,
                     dependencies: deps),
       )
endforeach
