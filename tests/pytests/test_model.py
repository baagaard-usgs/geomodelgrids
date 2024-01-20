"""Test _geomodelgrids.Model.
"""

import unittest
import numpy

import geomodelgrids


class TestModel(unittest.TestCase):

    FILENAME = "../data/one-block-topo.h5"

    def setUp(self):
        self.model = geomodelgrids.Model()
        self.model.open(self.FILENAME, self.model.READ)
        self.model.load_metadata()
        self.model.initialize()

    def tearDown(self):
        self.model.close()
        self.model = None

    def test_openclose(self):
        self.assertTrue(self.model)

    def test_accessors(self):
        DATA = {
            "value_names": ["one", "two"],
            "value_units": ["m", "m/s"],
            "data_layout": self.model.VERTEX,
            "dims": [30000, 40000, 5000],
            "origin": [590000, 4150000],
            "y_azimuth": 90.0,
            "crs": "EPSG:26910",
        }

        self.assertEqual(DATA["value_names"], self.model.get_value_names())
        self.assertEqual(DATA["value_units"], self.model.get_value_units())
        self.assertEqual(DATA["data_layout"], self.model.get_data_layout())
        self.assertEqual(DATA["dims"], self.model.get_dims())
        self.assertEqual(DATA["origin"], self.model.get_origin())
        self.assertEqual(DATA["y_azimuth"], self.model.get_y_azimuth())
        self.assertEqual(DATA["crs"], self.model.get_crs())

    def test_contains(self):
        POINTS = numpy.array([
            [37.455, -121.941, 8.0],
            [34.7, -117.8, 1.0e+4],
        ])
        VALUES = numpy.array([
            True,
            False,
        ])

        values = self.model.contains(POINTS)
        diff = numpy.sum(values - VALUES)
        self.assertEqual(diff, 0)

        self.assertRaises(RuntimeError, self.model.contains, numpy.array([0]))

    def test_query_top_elevation(self):
        POINTS = numpy.array([
            [37.455, -121.941],
            [37.479, -121.734],
            [37.381, -121.581],
            [37.283, -121.959],
            [37.262, -121.684],
        ])
        ELEV = numpy.array([
            150.0460941,
            149.77504068,
            150.02306271,
            150.46361282,
            150.52171529,
        ])

        elev = self.model.query_top_elevation(POINTS)
        diff = numpy.sum(numpy.abs(elev - ELEV))
        assert diff < 1.0e-6

        self.assertRaises(RuntimeError, self.model.query_top_elevation, numpy.array([0]))

    def test_query_topobathy_elevation(self):
        POINTS = numpy.array([
            [37.455, -121.941],
            [37.479, -121.734],
            [37.381, -121.581],
            [37.283, -121.959],
            [37.262, -121.684],
        ])
        ELEV = numpy.array([
            150.0460941,
            149.77504068,
            150.02306271,
            150.46361282,
            150.52171529,
        ])

        elev = self.model.query_topobathy_elevation(POINTS)
        diff = numpy.sum(numpy.abs(elev - ELEV))
        assert diff < 1.0e-6

        self.assertRaises(RuntimeError, self.model.query_topobathy_elevation, numpy.array([0]))

    def test_query(self):
        POINTS = numpy.array([
            [37.455, -121.941, 8.0],
            [37.479, -121.734, -5.0e+3],
            [37.381, -121.581, -3.0e+3],
            [37.283, -121.959, -1.5e+3],
            [37.262, -121.684, -4.0e+3],
        ])
        VALUES = numpy.array([
            [5254.1393062,  -858.13826774],
            [31145.02206828, 18536.45902674],
            [32058.97716272,  7545.7134798],
            [16288.95161415,  8357.02682455],
            [36369.15970315, 15326.88773376],
        ])

        values = self.model.query(POINTS)
        diff = numpy.sum(numpy.abs(numpy.array(values) - VALUES))
        assert diff < 1.0e-6

        self.assertRaises(RuntimeError, self.model.query, numpy.array([0]))


def load_tests(loader, tests, pattern):
    TEST_CLASSES = [TestModel]

    suite = unittest.TestSuite()
    for cls in TEST_CLASSES:
        suite.addTests(loader.loadTestsFromTestCase(cls))
    return suite


if __name__ == "__main__":
    unittest.main(verbosity=2)


# End of file
