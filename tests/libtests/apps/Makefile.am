subpackage = apps
include $(top_srcdir)/subpackage.am
include $(top_srcdir)/check.am

TESTS = test_apps

check_PROGRAMS = test_apps

test_apps_SOURCES = \
	TestInfo.cc \
	TestQuery.cc \
	TestQueryElev.cc \
	TestBorehole.cc \
	../../data/ModelPoints.cc \
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

if ENABLE_GDAL
test_apps_SOURCES += TestIsosurface.cc
test_apps_LDADD += -lgdal

test_apps_CPPFLAGS += $(GDAL_INCLUDES) -DWITH_GDAL
test_apps_LDFLAGS += $(GDAL_LDFLAGS)
endif


noinst_tmp = \
		error.log \
		one-block-flat.in \
		one-block-flat.out \
		two-blocks-topo.in \
		two-blocks-topo.out \
		three-blocks-topo.in \
		three-blocks-topo.out \
		two-models.in \
		two-models.out


CLEANFILES = $(noinst_tmp)


# End of file
