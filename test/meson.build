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
            not get_option('tests').allowed(),
            'Googletest is required if tests are enabled'
        )
  endif
endif

test_sources = [
  '../snmp_conf_manager.cpp',
  '../snmp_client.cpp',
  '../snmp_serialize.cpp',
  '../snmp_util.cpp',
]

tests = [
  'test_error_notification.cpp',
  'test_snmp_conf_manager.cpp',
  'test_snmp_util.cpp',
  'test_snmp_serialize.cpp',
  'test_snmp_client.cpp',
]

foreach t : tests
  test(t, executable(t.underscorify(), t,
                     test_sources,
                     include_directories: ['..'],
                     dependencies: [
                         gtest_dep,
                         gmock_dep,
                         deps
                         ]),
       workdir: meson.current_source_dir())
endforeach
