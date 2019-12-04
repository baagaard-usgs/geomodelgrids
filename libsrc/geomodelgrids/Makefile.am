SUBDIRS = \
	apps \
	serial \
	utils


lib_LTLIBRARIES = libgeomodelgrids.la

libgeomodelgrids_la_SOURCES = \
	apps/Info.cc \
	apps/Query.cc \
	serial/Query.cc \
	serial/ModelInfo.cc \
	serial/Model.cc \
	serial/Topography.cc \
	serial/Block.cc \
	serial/HDF5.cc \
	serial/Hyperslab.cc \
	utils/Projection.cc

libgeomodelgrids_la_LIBADD = -lhdf5


if ENABLE_TESTING
libgeomodelgrids_la_SOURCES += \
	utils/TestDriver.cc

libgeomodelgrids_la_LIBADD += -lcppunit
endif


AM_CPPFLAGS = -I$(top_srcdir)/libsrc


# End of file
