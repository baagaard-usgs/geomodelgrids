"""Test _geomodelgrids.Model.
"""

import unittest

import numpy

import geomodelgrids


class TestErrorHandler(unittest.TestCase):

    def test_enums(self):
        handler = geomodelgrids.ErrorHandler()
        self.assertEqual(0, handler.OK)
        self.assertEqual(1, handler.WARNING)
        self.assertEqual(2, handler.ERROR)

    def test_status(self):
        FILENAMES = ("../data/one-block-flat.h5",)
        VALUES = ("two", "one")
        CRS = "EPSG:4326"
        POINTS = numpy.array([[80.0, 0.0, 10.0]])

        query = geomodelgrids.Query()
        self.assertRaises(RuntimeError, query.query, POINTS)
        errorHandler = query.get_error_handler()
        self.assertEqual(errorHandler.ERROR, errorHandler.get_status())
        errorHandler.reset_status()
        self.assertEqual(errorHandler.OK, errorHandler.get_status())


def load_tests(loader, tests, pattern):
    TEST_CLASSES = [TestErrorHandler]

    suite = unittest.TestSuite()
    for cls in TEST_CLASSES:
        suite.addTests(loader.loadTestsFromTestCase(cls))
    return suite


if __name__ == "__main__":
    unittest.main(verbosity=2)


# End of file
