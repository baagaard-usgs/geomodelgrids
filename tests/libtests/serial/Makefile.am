subpackage = serial
include $(top_srcdir)/subpackage.am
include $(top_srcdir)/check.am

TESTS = test_serial

check_PROGRAMS = test_serial

test_serial_SOURCES = \
	TestModelInfo.cc \
	TestHDF5.cc \
	TestHyperslab.cc \
	TestSurface.cc \
	TestSurface_Cases.cc \
	TestBlock.cc \
	TestBlock_Cases.cc \
	TestModel.cc \
	TestQuery.cc \
	TestCQuery.cc \
	../../data/ModelPoints.cc \
	test_driver.cc

noinst_HEADERS = \
	TestSurface.hh \
	TestBlock.hh

test_serial_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-I$(top_srcdir)/tests/data

test_serial_LDFLAGS = $(AM_LDFLAGS)
test_serial_LDADD = \
	-lcppunit -ldl \
	$(top_builddir)/libsrc/geomodelgrids/libgeomodelgrids.la \
	-lhdf5 \
	-lproj


# End of file
