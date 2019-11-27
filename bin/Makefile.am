bin_PROGRAMS = \
	geogrids_info \
	geogrids_query

bin_SCRIPTS = \
	geogrids_rasterize

AM_CPPFLAGS = -I$(top_srcdir)/libsrc

geogrids_info_SOURCES = geogrids_info.cc
geogrids_info_LDADD = $(top_builddir)/libsrc/geomodelgrids/libgeomodelgrids.la

geogrids_query_SOURCES = geogrids_query.cc
geogrids_query_LDADD = $(top_builddir)/libsrc/geomodelgrids/libgeomodelgrids.la


# End of file 
