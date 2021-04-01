install-exec-local:
		cd $(top_srcdir) && \
			$(PYTHON) setup.py egg_info && \
			PYTHONUSERBASE=$(DESTDIR)$(prefix) $(PYTHON) setup.py install --record record.txt --single-version-externally-managed --user && \
			cd $(abs_builddir)

# Pip install is too slow, because it makes a copy of the entire source tree, including .git.
# See https://github.com/pypa/pip/issues/2195
# PYTHONUSERBASE=$(DESTDIR)$(prefix) $(PYTHON) -m pip install --user $(top_srcdir)

# End of file
