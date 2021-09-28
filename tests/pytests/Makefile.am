include $(top_srcdir)/check.am

TESTS = test_geomodelgrids.py

dist_check_SCRIPTS = test_geomodelgrids.py

dist_noinst_PYTHON = \
	test_driver.py \
	test_units.py \
	test_batch.py \
	test_createapp.py


dist_noinst_DATA = \
	test_createapp.cfg \
	test_createapp_batch.cfg \
	test_createapp_varz.cfg \
	test_createapp_varxyz.cfg \
	test_updatemetadata_varxyz.cfg

noinst_TMP = \
	test-model-1.0.0.h5 \
	test-model-1.0.0-batch.h5 \
	test-model-varz-1.0.0.h5 \
	test-model-varxyz-1.0.0.h5 \
	test_createapp.log \
	coverage.xml


# 'export' the input files by performing a mock install
export_datadir = $(top_builddir)/tests/pytests/
export-data: $(noinst_DATA)
	if [ "X$(top_srcdir)" != "X$(top_builddir)" ]; then $(install_sh) -d $(export_datadir); for f in $(dist_noinst_DATA); do $(install_sh_DATA) $(srcdir)/$$f $(export_datadir); done; fi

clean-data:
	if [ "X$(top_srcdir)" != "X$(top_builddir)" ]; then for f in $(dist_noinst_DATA) $(noinst_TMP); do $(RM) $(RM_FLAGS) $(export_datadir)/$$f; done; fi

BUILT_SOURCES = export-data
clean-local: clean-data


# End of file
