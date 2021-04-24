bin_PROGRAMS = \
	geomodelgrids_info \
	geomodelgrids_query \
	geomodelgrids_queryelev \
	geomodelgrids_borehole \
	geomodelgrids_isosurface

if ENABLE_PYTHON
bin_SCRIPTS = \
	geomodelgrids_create_model
endif

AM_CPPFLAGS = -I$(top_srcdir)/libsrc

geomodelgrids_info_SOURCES = info.cc
geomodelgrids_info_LDADD = $(top_builddir)/libsrc/geomodelgrids/libgeomodelgrids.la

geomodelgrids_query_SOURCES = query.cc
geomodelgrids_query_LDADD = $(top_builddir)/libsrc/geomodelgrids/libgeomodelgrids.la

geomodelgrids_queryelev_SOURCES = queryelev.cc
geomodelgrids_queryelev_LDADD = $(top_builddir)/libsrc/geomodelgrids/libgeomodelgrids.la

geomodelgrids_borehole_SOURCES = borehole.cc
geomodelgrids_borehole_LDADD = $(top_builddir)/libsrc/geomodelgrids/libgeomodelgrids.la

geomodelgrids_isosurface_SOURCES = isosurface.cc
geomodelgrids_isosurface_LDADD = $(top_builddir)/libsrc/geomodelgrids/libgeomodelgrids.la


# End of file
