subpackage = apps
include $(top_srcdir)/subpackage.am
include $(top_srcdir)/check.am

TESTS = test_apps

check_PROGRAMS = test_apps

test_apps_SOURCES = \
	TestInfo.cc \
	TestQuery.cc \
	$(top_srcdir)/tests/data/ModelPoints.cc \
	test_driver.cc

noinst_HEADERS =

test_apps_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-I$(top_srcdir)/tests/data

test_apps_LDFLAGS = $(AM_LDFLAGS)
test_apps_LDADD = \
	-lcppunit -ldl \
	$(top_builddir)/libsrc/geomodelgrids/libgeomodelgrids.la \
	-lhdf5 \
	-lproj



# End of file
