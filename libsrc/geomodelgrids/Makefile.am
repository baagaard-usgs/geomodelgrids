SUBDIRS = \
	apps \
	serial \
	utils


lib_LTLIBRARIES = libgeomodelgrids.la

libgeomodelgrids_la_SOURCES = \
	apps/Info.cc \
	apps/Query.cc \
	serial/Query.cc \
	serial/cquery.cc \
	serial/ModelInfo.cc \
	serial/Model.cc \
	serial/Topography.cc \
	serial/Block.cc \
	serial/HDF5.cc \
	serial/Hyperslab.cc \
	utils/CRSTransformer.cc \
	utils/ErrorHandler.cc

pkginclude_HEADERS = \
	geomodelgrids_serial.hh

libgeomodelgrids_la_LIBADD = -lhdf5 -lproj
libgeomodelgrids_la_LDFLAGS = $(HDF5_LDFLAGS) $(PROJ_LDFLAGS)
libgeomodelgrids_la_CPPFLAGS = -I$(top_srcdir)/libsrc $(HDF5_INCLUDES) $(PROJ_INCLUDES)

if ENABLE_TESTING
libgeomodelgrids_la_SOURCES += \
	utils/TestDriver.cc

libgeomodelgrids_la_CPPFLAGS += $(CPPUNIT_INCLUDES)
libgeomodelgrids_la_LDFLAGS += $(CPPUNIT_LDFLAGS)
libgeomodelgrids_la_LIBADD += -lcppunit
endif




# End of file
