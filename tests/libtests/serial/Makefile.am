include $(top_srcdir)/tests/check.am

TESTS = libtest_serial

check_PROGRAMS = libtest_serial

libtest_serial_SOURCES = \
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
	$(top_srcdir)/tests/data/ModelPoints.cc \
	$(top_srcdir)/tests/src/driver_catch2.cc

noinst_HEADERS = \
	TestSurface.hh \
	TestBlock.hh

# End of file
