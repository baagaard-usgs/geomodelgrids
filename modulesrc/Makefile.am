
AM_CPPFLAGS = \
	-I$(top_srcdir)/libsrc \
	$(PYBIND11_CPPFLAGS)


pkgpython_LTLIBRARIES = _geomodelgrids.la

_geomodelgrids_la_SOURCES = \
	ModelInfo_wrap.cc \
	Model_wrap.cc \
	Query_wrap.cc \
	ErrorHandler_wrap.cc \
	geomodelgrids.cc

_geomodelgrids_la_LDFLAGS = -module -avoid-version \
	$(AM_LDFLAGS) $(PYTHON_LA_LDFLAGS)

_geomodelgrids_la_LIBADD = \
	$(top_builddir)/libsrc/geomodelgrids/libgeomodelgrids.la \
	$(PYTHON_BLDLIBRARY) $(PYTHON_LIBS) $(PYTHON_SYSLIBS)


# End of file
