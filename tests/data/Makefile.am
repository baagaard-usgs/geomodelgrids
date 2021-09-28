EXTRA_DIST = \
	generate.py \
	ModelPoints.hh \
	ModelPoints.cc


dist_noinst_DATA = \
	one-block-flat.h5 \
	one-block-flat-varz.h5 \
	one-block-topo.h5 \
	one-block-topo-varxy.h5 \
	one-block-topo-varxy-bad-surf-coords.h5 \
	one-block-topo-varxy-bad-block-coords.h5 \
	three-blocks-flat.h5 \
	three-blocks-topo.h5 \
	three-blocks-topo-varxyz.h5 \
	one-block-topo-bad-topo.h5 \
	three-blocks-topo-bad-blocks.h5 \
	three-blocks-topo-missing-metadata.h5 \
	three-blocks-topo-inconsistent-units.h5


# 'export' the input files by performing a mock install
export_datadir = $(abs_top_builddir)/tests/data
export-data: $(dist_noinst_DATA)
	if [ "X$(abs_top_srcdir)" != "X$(abs_top_builddir)" ]; then for f in $(dist_noinst_DATA); do $(install_sh_DATA) $(srcdir)/$$f $(export_datadir); done; fi

clean-data:
	if [ "X$(abs_top_srcdir)" != "X$(abs_top_builddir)" ]; then for f in $(dist_noinst_DATA); do $(RM) $(RM_FLAGS) $(export_datadir)/$$f; done; fi

BUILT_SOURCES = export-data
clean-local: clean-data

# End of file
