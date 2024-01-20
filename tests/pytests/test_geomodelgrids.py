#!/usr/bin/env python3
"""Script to run geomodelgrids Python test suite.

Run `coverage report` to generate a report (included).
Run `coverage html -d DIR` to generate an HTML report in directory `DIR`.
"""


import unittest
import sys


def create_coverage():
    """Create test coverage, return None if not available."""
    _cov = None
    try:
        import coverage
        src_dirs = [
            "geomodelgrids",
            "geomodelgrids.create.apps",
            "geomodelgrids.create.core",
            "geomodelgrids.create.io",
            "geomodelgrids.create.testing",
            "geomodelgrids.create.utils",
        ]
        _cov = coverage.Coverage(source=src_dirs)
        _cov.start()
    except ImportError:
        pass
    return _cov


def close_coverage(_cov):
    """End test coverage measurement and generate coverage report."""
    if _cov:
        _cov.stop()
        _cov.save()
        _cov.report()
        _cov.xml_report(outfile="coverage.xml")


def load_tests(loader, tests, pattern):
    """Load tests for unittest module."""
    import test_units
    import test_batch
    import test_createapp
    import test_query
    import test_model
    import test_modelinfo
    import test_errorhandler

    _suite = unittest.TestSuite()
    for mod in [
        test_units,
        test_batch,
        test_createapp,
        test_query,
        test_model,
        test_modelinfo,
        test_errorhandler,
    ]:
        _suite.addTests(loader.loadTestsFromModule(mod))
    return _suite


# ----------------------------------------------------------------------
if __name__ == '__main__':
    cov = create_coverage()

    suite = load_tests(unittest.defaultTestLoader, tests=None, pattern=None)
    success = unittest.TextTestRunner(verbosity=2).run(suite).wasSuccessful()
    close_coverage(cov)
    if not success:
        sys.exit(1)


# End of file
