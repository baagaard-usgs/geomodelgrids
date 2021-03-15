#!/usr/bin/env python3
"""Script to run geomodelgrids Python test suite.

Run `coverage report` to generate a report (included).
Run `coverage html -d DIR` to generate an HTML report in directory `DIR`.
"""


import unittest
import sys


# sys.path.append("./pyre")


class TestApp(object):
    """Application to run tests.
    """
    cov = None
    try:
        import coverage
        src_dirs = [
            "geomodelgrids.create.apps",
            "geomodelgrids.create.core",
            "geomodelgrids.create.io",
            "geomodelgrids.create.testing",
            "geomodelgrids.create.utils",
        ]
        cov = coverage.Coverage(source=src_dirs)
    except ImportError:
        pass

    def main(self):
        """
        Run the application.
        """
        if self.cov:
            self.cov.start()

        success = unittest.TextTestRunner(verbosity=2).run(self._suite()).wasSuccessful()
        if not success:
            sys.exit(1)

        if self.cov:
            self.cov.stop()
            self.cov.save()
            self.cov.report()
            self.cov.xml_report(outfile="coverage.xml")

    def _suite(self):
        """Setup the test suite.
        """
        import test_units
        import test_batch
        import test_createapp

        test_cases = []
        for mod in [
            test_units,
            test_batch,
            test_createapp,
        ]:
            test_cases += mod.test_classes()

        suite = unittest.TestSuite()
        for test_case in test_cases:
            suite.addTest(unittest.makeSuite(test_case))

        return suite


# ----------------------------------------------------------------------
if __name__ == '__main__':
    TestApp().main()


# End of file
