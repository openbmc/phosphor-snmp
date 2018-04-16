AM_CPPFLAGS = -I${top_srcdir} -I${top_builddir}

TESTS = $(check_PROGRAMS)

check_PROGRAMS = test

test_SOURCES = \
	test_error_notification.cpp

generic_cpp_flags = -Igtest $(GTEST_CPPFLAGS) $(AM_CPPFLAGS)

generic_cxx_flags = $(PTHREAD_CFLAGS)

generic_ld_flags =  -lgtest_main -lgtest -lstdc++fs \
					$(OESDK_TESTCASE_FLAGS)

test_CPPFLAGS = ${generic_cpp_flags}
test_CXXFLAGS = ${generic_cxx_flags}
test_LDFLAGS  = ${generic_ld_flags}
