ACLOCAL_AMFLAGS = -I m4

SUBDIRS = \
	libsrc \
	geomodelgrids \
	bin

if ENABLE_TESTING
	SUBDIRS += \
		tests
endif

DIST_SUBDIRS = $(SUBDIRS) \
	docs \
	models

EXTRA_DIST = \
	README.md \
	CHANGES.md \
	CODE_OF_CONDUCT.md \
	CONTRIBUTING.md \
	DISCLAIMER.md \
	LICENSE.md \
	MANIFEST \
	setup.py \
	code.json


# End of file
