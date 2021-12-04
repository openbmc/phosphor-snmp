gtest = dependency('gtest', main: true, disabler: true, required: false)
gmock = dependency('gmock', disabler: true, required: false)
if not gtest.found() or not gmock.found()
  gtest_opts = import('cmake').subproject_options()
  gtest_opts.add_cmake_defines({'CMAKE_CXX_FLAGS': '-Wno-pedantic'})
  gtest_proj = import('cmake').subproject(
    'googletest',
    options: gtest_opts,
    required: false)
  if gtest_proj.found()
    gtest = declare_dependency(
      dependencies: [
        dependency('threads'),
        gtest_proj.dependency('gtest'),
        gtest_proj.dependency('gtest_main'),
      ])
    gmock = gtest_proj.dependency('gmock')
  else
    assert(not get_option('tests').enabled(), 'Googletest is required')
  endif
endif



test_headers = include_directories('.')

test_deps = [
  snmp_dep,
  gtest,
  gmock,
]

test_dep = declare_dependency(
  dependencies: test_deps,
  include_directories: test_headers)

tests_list = [
  'error_notification',
  'snmp_client',
  'snmp_conf_manager',
  'snmp_serialize',
  'snmp_util'
]


######################################## Run the tests ########################################
foreach t : tests_list
  test(
    t,
    executable(
      t.underscorify(),
      'test_' + t + '.cpp',
      implicit_include_directories: false,
      dependencies: test_dep))
endforeach
