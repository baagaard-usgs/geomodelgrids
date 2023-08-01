subpackage = utils
include $(top_srcdir)/subpackage.am

subpkginclude_HEADERS = \
	CRSTransformer.hh \
	Indexing.hh \
	ErrorHandler.hh \
	cerrorhandler.h \
	constants.hh \
	utilsfwd.hh

noinst_HEADERS =


if ENABLE_GDAL
subpkginclude_HEADERS += \
	GeoTiff.hh
endif


# End of file
