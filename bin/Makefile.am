bin_PROGRAMS = \
	geomodelgrids_info \
	geomodelgrids_query

bin_SCRIPTS = \
	geomodelgrids_rasterize

AM_CPPFLAGS = -I$(top_srcdir)/libsrc

geomodelgrids_info_SOURCES = info.cc
geomodelgrids_info_LDADD = $(top_builddir)/libsrc/geomodelgrids/libgeomodelgrids.la

geomodelgrids_query_SOURCES = query.cc
geomodelgrids_query_LDADD = $(top_builddir)/libsrc/geomodelgrids/libgeomodelgrids.la


# End of file 
