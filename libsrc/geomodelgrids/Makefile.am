SUBDIRS = \
	apps \
	serial \
	utils


lib_LTLIBRARIES = libgeomodelgrids.la

libgeomodelgrids_la_SOURCES = \
	apps/Info.cc \
	serial/ModelInfo.cc \
	serial/Model.cc \
	serial/Topography.cc \
	serial/Block.cc \
	serial/HDF5.cc


if ENABLE_TESTING
libgeomodelgrids_la_SOURCES += \
	utils/TestDriver.cc
endif


AM_CPPFLAGS = -I$(top_srcdir)/libsrc


# End of file
