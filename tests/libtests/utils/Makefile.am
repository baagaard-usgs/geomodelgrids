subpackage = utils
include $(top_srcdir)/subpackage.am
include $(top_srcdir)/check.am

TESTS = test_utils

check_PROGRAMS = test_utils

test_utils_SOURCES = \
	TestProjection.cc \
	test_driver.cc

noinst_HEADERS =

test_utils_LDFLAGS = $(AM_LDFLAGS)
test_utils_LDADD = \
	-lcppunit -ldl \
	$(top_builddir)/libsrc/geomodelgrids/libgeomodelgrids.la \
	-lhdf5


# End of file
