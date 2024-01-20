"""Test _geomodelgrids.Query.
"""

import unittest
import numpy

import geomodelgrids


class TestQuery(unittest.TestCase):

    FILENAMES = (
        "../data/one-block-topo.h5",
        "../data/three-blocks-topo.h5",
    )
    VALUES = ("two", "one")
    CRS = "EPSG:4326"

    def setUp(self):
        self.query = geomodelgrids.Query()
        self.query.initialize(self.FILENAMES, self.VALUES, self.CRS)

    def tearDown(self):
        self.query.finalize()
        self.query = None

    def test_initialize(self):
        self.assertTrue(self.query)

    def test_accessors(self):
        handler = self.query.get_error_handler()

    def test_query_top_elevation(self):
        POINTS = numpy.array([
            # one-block-topo
            [37.455, -121.941],
            [37.479, -121.734],
            [37.381, -121.581],
            [37.283, -121.959],
            [37.262, -121.684],
            # three-blocks-topo
            [35.3, -118.2],
            [35.5, -117.9],
            [35.0, -118.1],
            [35.1, -117.7],
            [34.7, -117.9],
            [34.7, -117.5],
        ])
        ELEV = numpy.array([
            150.0460941,
            149.77504068,
            150.02306271,
            150.46361282,
            150.52171529,
            150.06565636,
            152.65482145,
            149.71133719,
            151.69039421,
            149.97365829,
            150.98686496,
        ])

        elev = self.query.query_top_elevation(POINTS)
        diff = numpy.sum(numpy.abs(elev - ELEV))
        assert diff < 1.0e-6

        self.assertRaises(RuntimeError, self.query.query_top_elevation, numpy.array([1.0]))

    def test_query_topobathy_elevation(self):
        POINTS = numpy.array([
            # one-block-topo
            [37.455, -121.941],
            [37.479, -121.734],
            [37.381, -121.581],
            [37.283, -121.959],
            [37.262, -121.684],
            # three-blocks-topo
            [35.3, -118.2],
            [35.5, -117.9],
            [35.0, -118.1],
            [35.1, -117.7],
            [34.7, -117.9],
            [34.7, -117.5],
        ])
        ELEV = numpy.array([
            150.0460941,
            149.77504068,
            150.02306271,
            150.46361282,
            150.52171529,
            148.15581117,
            146.79694001,
            148.99097481,
            148.69132321,
            149.63955574,
            150.33343435,
        ])

        elev = self.query.query_topobathy_elevation(POINTS)
        diff = numpy.sum(numpy.abs(elev - ELEV))
        assert diff < 1.0e-6

        self.assertRaises(RuntimeError, self.query.query_topobathy_elevation, numpy.array([1.0]))

    def test_query(self):
        POINTS = numpy.array([
            # one-block-topo
            [37.455, -121.941, 8.0],
            [37.479, -121.734, -5.0e+3],
            [37.381, -121.581, -3.0e+3],
            [37.283, -121.959, -1.5e+3],
            [37.262, -121.684, -4.0e+3],
            # three-blocks-topo
            [35.3, -118.2, 10.0],
            [35.5, -117.9, -45.0e+3],
            [35.0, -118.1, -3.0e+3],
            [35.1, -117.7, -15.0e+3],
            [34.7, -117.9, -25.0e+3],
            [34.7, -117.5, 43.0],
        ])
        VALUES = numpy.array([
            [-858.13826774,   5254.1393062],
            [18536.45902674,  31145.02206828],
            [7545.7134798,  32058.97716272],
            [8357.02682455,  16288.95161415],
            [15326.88773376,  36369.15970315],
            [-19613.35382726,  49858.8369985],
            [197905.09882116, 242203.32167217],
            [686.5072936,  45995.07540723],
            [63391.02424635, 101918.04829633],
            [113532.86227974, 118500.89891462],
            [453.64814667,  21296.58215576],
        ])

        values, err = self.query.query(POINTS)
        diff = numpy.sum(numpy.abs(numpy.array(values) - VALUES))
        assert diff < 1.0e-6
        assert numpy.sum(err) == 0

    def test_query_squashed(self):
        POINTS = numpy.array([
            # one-block-squashed
            [37.455, -121.941, -8.0],
            [37.479, -121.734, -5.0e+3],
            [37.381, -121.581, -3.0e+3],
            [37.283, -121.959, -1.5e+3],
            [37.262, -121.684, -4.0e+3],
            # three-blocks-squashed
            [35.3, -118.2, -10.0],
            [35.5, -117.9, -45.0e+3],
            [35.0, -118.1, -3.0e+3],
            [35.1, -117.7, -15.0e+3],
            [34.7, -117.9, -25.0e+3],
            [34.7, -117.5, -43.0],
        ])
        VALUES = numpy.array([
            [-783.5758283,   5316.2746724],
            [18536.45902674,  31145.02206828],
            [7545.7134798,  32058.97716272],
            [8357.02682455,  16288.95161415],
            [15326.88773376,  36369.15970315],
            [-19517.6729018,  49938.57110046],
            [197905.09882116, 242203.32167217],
            [686.5072936,  45995.07540723],
            [63391.02424635, 101918.04829633],
            [113532.86227974, 118500.89891462],
            [865.0677318,  21639.43179899]])

        values, err = self.query.query(POINTS)
        diff = numpy.sum(numpy.abs(numpy.array(values) - VALUES))
        assert diff < 1.0e-6
        assert numpy.sum(err) == 0

    def test_query_outsidedomain(self):
        POINTS = numpy.array([
            [37.455, -121.941, +5.0e+6],
            [-37.479, +121.734, -5.0e+3],
        ])
        VALUES = numpy.array([
            [geomodelgrids.Query.NODATA_VALUE, geomodelgrids.Query.NODATA_VALUE],
            [geomodelgrids.Query.NODATA_VALUE, geomodelgrids.Query.NODATA_VALUE],
        ])

        values, err = self.query.query(POINTS)
        diff = numpy.sum(numpy.abs(numpy.array(values) - VALUES))
        assert diff < 1.0e-6
        assert numpy.sum(err) == 2

    def test_query_badquery(self):
        POINTS = numpy.array([
            [37.455, -121.941, +5.0e+6],
            [-37.479, +121.734, -5.0e+3],
        ])
        query = geomodelgrids.Query()
        self.assertRaises(TypeError, query.query)
        self.assertRaises(RuntimeError, query.query, POINTS)
        self.assertRaises(RuntimeError, query.query, numpy.array([1.0]))


def load_tests(loader, tests, pattern):
    TEST_CLASSES = [TestQuery]

    suite = unittest.TestSuite()
    for cls in TEST_CLASSES:
        suite.addTests(loader.loadTestsFromTestCase(cls))
    return suite


if __name__ == "__main__":
    unittest.main(verbosity=2)


# End of file
