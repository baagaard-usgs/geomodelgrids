include $(top_srcdir)/tests/check.am

TESTS = libtest_utils

check_PROGRAMS = libtest_utils

libtest_utils_SOURCES = \
	TestCRSTransformer.cc \
	TestIndexing.cc \
	TestErrorHandler.cc \
	TestCErrorHandler.cc \
	$(top_srcdir)/tests/src/driver_catch2.cc


noinst_HEADERS =


noinst_tmp = \
	error.log


CLEANFILES = $(noinst_tmp)

# End of file
