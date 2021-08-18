SUBDIRS = \
	apps \
	serial \
	utils


lib_LTLIBRARIES = libgeomodelgrids.la

libgeomodelgrids_la_SOURCES = \
	apps/Info.cc \
	apps/Query.cc \
	apps/QueryElev.cc \
	apps/Borehole.cc \
	apps/Isosurface.cc \
	serial/Query.cc \
	serial/cquery.cc \
	serial/ModelInfo.cc \
	serial/Model.cc \
	serial/Surface.cc \
	serial/Block.cc \
	serial/HDF5.cc \
	serial/Hyperslab.cc \
	utils/CRSTransformer.cc \
	utils/Indexing.cc \
	utils/ErrorHandler.cc \
	utils/cerrorhandler.cc

pkginclude_HEADERS = \
	geomodelgrids_serial.hh

libgeomodelgrids_la_LIBADD = -lhdf5 -lproj
libgeomodelgrids_la_LDFLAGS = $(HDF5_LDFLAGS) $(PROJ_LDFLAGS)
libgeomodelgrids_la_CPPFLAGS = -I$(top_srcdir)/libsrc $(HDF5_INCLUDES) $(PROJ_INCLUDES)

if ENABLE_GDAL
CPPFLAGS += 
libgeomodelgrids_la_SOURCES += \
	utils/GeoTiff.cc

libgeomodelgrids_la_CPPFLAGS += $(GDAL_INCLUDES) -DWITH_GDAL
libgeomodelgrids_la_LDFLAGS += $(GDAL_LDFLAGS)
libgeomodelgrids_la_LIBADD += -lgdal
endif


if ENABLE_TESTING
libgeomodelgrids_la_SOURCES += \
	utils/TestDriver.cc

libgeomodelgrids_la_CPPFLAGS += $(CPPUNIT_INCLUDES)
libgeomodelgrids_la_LDFLAGS += $(CPPUNIT_LDFLAGS)
libgeomodelgrids_la_LIBADD += -lcppunit
endif




# End of file
