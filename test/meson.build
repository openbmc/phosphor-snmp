gtest_dep = dependency('gtest', main: true, disabler: true, required: false)
gmock_dep = dependency('gmock', disabler: true, required: false)
if not gtest_dep.found() or not gmock_dep.found()
    gtest_proj = import('cmake').subproject('googletest', required: false)
    if gtest_proj.found()
        gtest_dep = declare_dependency(
            dependencies: [
                dependency('threads'),
                gtest_proj.dependency('gtest'),
                gtest_proj.dependency('gtest_main'),
            ]
        )
        gmock_dep = gtest_proj.dependency('gmock')
    else
        assert(
            not get_option('tests').enabled(),
            'Googletest is required if tests are enabled'
        )
    endif
endif

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
      implicit_include_directories: true,
      dependencies: [
            gtest_dep,
            gmock_dep,
            snmp_dep,
        ],
 ))
endforeach
