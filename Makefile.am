ACLOCAL_AMFLAGS = -I m4

SUBDIRS = \
	libsrc \
	bin \
	examples

if ENABLE_PYTHON
SUBDIRS += \
  	modulesrc \
	geomodelgrids
endif

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
	setup.py \
	setup.cfg \
	code.json \
	developer/autopep8.cfg \
	developer/uncrustify.cfg \
	docker/README.md \
	docker/build_binary.py \
	docker/builder.py \
	docker/update_testenv.py \
	docker/debian-stable \
	docker/debian-testing \
	docker/ubuntu-20.04 \
	docker/ubuntu-22.04 \
	docker/ubuntu-23.04 \
	docker/ubuntu-23.10 \
	docker/fedora-38 \
	docker/fedora-39 \
	docker/rockylinux-8 \
	docker/rockylinux-9 \
	docker/geomodelgrids-binaryenv \
	docker/geomodelgrids-testenv \
	docker/geomodelgrids-devenv \
	docker/geomodelgrids-userenv \
	ci-config/run_tests.sh


.PHONY: coverage-libtests coverage-pytests coverage-html clean-coverage

if ENABLE_TEST_COVERAGE
coverage-libtests:
	$(LCOV) --directory libsrc --capture --output-file coverage-libtests.info
	$(LCOV) --remove coverage-libtests.info '/usr/*' '/Library/*' "v1" "*/utils/TestDriver.*" -o coverage-libtests.info
	$(LCOV) --list coverage-libtests.info

coverage-pytests:
	$(PYTHON_COVERAGE) xml -o coverage-pytests.xml tests/pytests/coverage.xml

coverage-html: coverage-libtests coverage-pytests
	genhtml -o coverage-libtests coverage-libtests.info
	$(PYTHON_COVERAGE) html -d coverage-pytests

clean-coverage:
	$(RM) `find . -name "*.gcda"`
	$(RM) `find . -name ".coverage"`

else
coverage-libtests coverage-pytests coverage-html clean-coverage:
	$(warning Test coverage not enabled. Ignoring test coverage targets)
endif


# End of file
