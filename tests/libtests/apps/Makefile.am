include $(top_srcdir)/tests/check.am

TESTS = libtest_apps

check_PROGRAMS = libtest_apps

libtest_apps_SOURCES = \
	TestInfo.cc \
	TestQuery.cc \
	TestQueryElev.cc \
	TestBorehole.cc \
	$(top_srcdir)/tests/data/ModelPoints.cc \
	$(top_srcdir)/tests/src/driver_catch2.cc


noinst_HEADERS =


if ENABLE_GDAL
libtest_apps_SOURCES += TestIsosurface.cc

AM_CPPFLAGS += $(GDAL_INCLUDES) -DWITH_GDAL
LDFLAGS += $(GDAL_LDFLAGS)
LDADD += -lgdal
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
