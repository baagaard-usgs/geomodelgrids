ACLOCAL_AMFLAGS = -I m4

SUBDIRS = \
	libsrc \
	geomodelgrids \
	bin \
	examples

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


.PHONY: coverage-libtests coverage-html clean-coverage

if ENABLE_TEST_COVERAGE
coverage-libtests:
	$(LCOV) --directory libsrc --capture --output-file coverage-libtests.info
	$(LCOV) --remove coverage-libtests.info '/usr/*' '/Library/*' "v1" "*/utils/TestDriver.*" -o coverage-libtests.info
	$(LCOV) --list coverage-libtests.info

coverage-html: coverage-libtests
	genhtml -o coverage-libtests coverage-libtests.info

clean-coverage:
	$(RM) `find . -name "*.gcda"`
	$(RM) `find . -name ".coverage"`

else
coverage-libtests coverage-pytests coverage-html clean-coverage:
	$(warning Test coverage not enabled. Ignoring test coverage targets)
endif


# End of file
