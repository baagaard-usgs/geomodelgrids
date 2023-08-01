include $(top_srcdir)/tests/check.am

TESTS = libtest_utils

check_PROGRAMS = libtest_utils

libtest_utils_SOURCES = \
	TestCErrorHandler.cc \
	$(top_srcdir)/tests/src/driver_catch2.cc

#	TestCRSTransformer.cc \
#	TestIndexing.cc \
#	TestErrorHandler.cc \
#

noinst_HEADERS =


noinst_tmp = \
	error.log


CLEANFILES = $(noinst_tmp)

# End of file
