"""Test _geomodelgrids.Query.
"""

import os
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
            # one-block-topo
            150.0460941024,
            149.7750406823,
            150.0230627088,
            150.4636128215,
            150.521715288,
            # three-blocks-topo
            150.0656563649,
            152.6548214496,
            149.7113371897,
            151.6903942121,
            149.9736582888,
            150.9868649589,
        ])

        elev = self.query.query_top_elevation(POINTS)
        diff = numpy.sum(numpy.abs(elev - ELEV)/ELEV)
        self.assertLess(diff, 2.0e-6)

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
            # one-block-topo
            150.0460941024,
            149.7750406823,
            150.0230627088,
            150.4636128215,
            150.521715288,
            # three-block-topo
            148.1558111735,
            146.7969400116,
            148.9909748132,
            148.6913232065,
            149.6395557378,
            150.3334343519,
        ])

        elev = self.query.query_topobathy_elevation(POINTS)
        diff = numpy.sum(numpy.abs(elev - ELEV)/ELEV)
        self.assertLess(diff, 2.0e-6)

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
            # one-block-topo
            [-858.138272226666,   5254.139302454365],
            [18536.459026739467,  31145.022068279795],
            [7545.7134769230406,  32058.977160321396],
            [8357.0268263148791,  16288.95161562109],
            [15326.887733376869,  36369.15970282998],
            # three-block-topo
            [-19613.353832578789,  49858.836992768927],
            [197905.09549258184, 242203.32475264152],
            [686.50704726498418,  45995.075588561485],
            [63391.023139392652, 101918.04931034689],
            [113532.86043323125, 118500.90059607077],
            [453.6481620888689,  21296.582163089526],
        ])
        if os.environ.get("PROJ_VERSION") == "6":
            # Coarse resolution datum files are less accurate
            VALUES = numpy.array([
                # one-block-topo
                [  -858.13826774,   5254.1393062 ],
                [ 18536.45902674,  31145.02206828],
                [  7545.7134798 ,  32058.97716272],
                [  8357.02682455,  16288.95161415],
                [ 15326.88773376,  36369.15970315],
                # three-block-topo
                [-19613.62959036,  49859.0364433 ],
                [197904.82060923, 242203.5252072 ],
                [   686.23173858,  45995.27292008],
                [ 63390.74886799, 101918.24668   ],
                [113532.58587332, 118501.09562028],
                [   453.37460241,  21296.77644269]
            ])
            
        values, err = self.query.query(POINTS)
        diff = numpy.sum(numpy.abs(numpy.array(values) - VALUES)/VALUES)
        self.assertLess(diff, 1.0e-6)
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
            # one-block-topo
            [-1481.6944995026847,   4734.509113057683],
            [18536.459026739467,  31145.022068279795],
            [7266.1440037784841,  31826.002599367599],
            [7866.2803258882177,  15879.996198598872],
            [15186.722105058896,  36252.355012565007],
            # three-blocks-topo
            [-20234.157806227118,  49341.500348061985],
            [197905.09549258184, 242203.32475264152],
            [400.10039355118113,  45756.40337713332],
            [63391.023139392652, 101918.04931034689],
            [113532.86043323125, 118500.90059607077],
            [148.96746671345025,  21042.68158361001]])
        if os.environ.get("PROJ_VERSION") == "6":
            # Coarse resolution datum files are less accurate
            VALUES = numpy.array([
                # one-block-topo
                [-1.48166395e+03,  4.73444292e+03],
                [ 1.85364806e+04,  3.11449941e+04],
                [ 7.26615711e+03,  3.18259874e+04],
                [ 7.86632505e+03,  1.58799299e+04],
                [ 1.51867543e+04,  3.62523092e+04],
                # three-blocks-topo
                [-2.02344333e+04,  4.93417000e+04],
                [ 1.97904821e+05,  2.42203525e+05],
                [ 3.99825162e+02,  4.57566008e+04],
                [ 6.33907489e+04,  1.01918247e+05],
                [ 1.13532586e+05,  1.18501096e+05],
                [ 1.48693968e+02,  2.10428759e+04],
            ])

        self.query.set_squash_min_elev(-4.999e+3)
        values, err = self.query.query(POINTS)
        diff = numpy.sum(numpy.abs(numpy.array(values) - VALUES)/VALUES)
        self.assertLess(diff, 1.0e-6)
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
        diff = numpy.sum(numpy.abs(numpy.array(values) - VALUES)/VALUES)
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
