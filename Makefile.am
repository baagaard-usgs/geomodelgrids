ACLOCAL_AMFLAGS = -I m4

SUBDIRS = \
	libsrc \
	geomodelgrids \
	bin

if ENABLE_TESTING
  SUBDIRS += tests
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
	code.json \
	developer/autopep8.cfg \
	developer/uncrustify.cfg \
	docker/debian-stable-gcc8 \
	docker/geomodelgrids-testenv


coverage-libsrc:
	lcov --directory libsrc --capture --output-file coverage.info
	lcov --remove coverage.info '/usr/*' '/Library/*' "v1" "*/utils/TestDriver.*" -o coverage.info
	lcov --list coverage.info

coverage-html: coverage-libsrc
	genhtml -o coverage coverage.info

coverage-clean:
	$(RM) `find . -name "*.gcda"`

# End of file
