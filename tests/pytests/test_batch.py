"""Test create.utils.batch.
"""

import unittest
import os

import numpy

from geomodelgrids.create.utils import batch


class TestBatchGenerator2D(unittest.TestCase):

    def test_nomax(self):
        NUM_X = 30
        NUM_Y = 40
        BATCHES = [
            [(0, 30), (0, 40)],
        ]
        genbatch = batch.BatchGenerator2D(NUM_X, NUM_Y)
        self._check_batches(BATCHES, genbatch)

    def test_limitxy(self):
        NUM_X = 15
        NUM_Y = 23
        BATCHES = [
            [(0, 10), (0, 10)],
            [(0, 10), (10, 20)],
            [(0, 10), (20, 23)],
            [(10, 15), (0, 10)],
            [(10, 15), (10, 20)],
            [(10, 15), (20, 23)],
        ]
        genbatch = batch.BatchGenerator2D(NUM_X, NUM_Y, max_nvalues=10**2)
        self._check_batches(BATCHES, genbatch)

    def test_limitx(self):
        NUM_X = 28
        NUM_Y = 15
        BATCHES = [
            [(0, 15), (0, 15)],
            [(15, 28), (0, 15)],
        ]
        genbatch = batch.BatchGenerator2D(NUM_X, NUM_Y, max_nvalues=15**2)
        self._check_batches(BATCHES, genbatch)

    def test_limity(self):
        NUM_X = 15
        NUM_Y = 23
        BATCHES = [
            [(0, 15), (0, 16)],
            [(0, 15), (16, 23)],
        ]
        genbatch = batch.BatchGenerator2D(NUM_X, NUM_Y, max_nvalues=15*16)
        self._check_batches(BATCHES, genbatch)

    def _check_batches(self, batchesE, genbatch):
        count = 0
        for bE, b in zip(batchesE, genbatch):
            self.assertEqual(f"Batch2D [{bE[0][0]}:{bE[0][1]}, {bE[1][0]}:{bE[1][1]}]", str(b))
            self.assertEqual(bE[0], b.x_range)
            self.assertEqual(bE[1], b.y_range)
            count += 1
        self.assertEqual(len(batchesE), count)


class TestBatchGenerator3D(unittest.TestCase):

    def test_nomax(self):
        NUM_X = 30
        NUM_Y = 40
        NUM_Z = 44
        BATCHES = [
            [(0, 30), (0, 40), (0, 44)],
        ]

        genbatch = batch.BatchGenerator3D(NUM_X, NUM_Y, NUM_Z)
        self._check_batches(BATCHES, genbatch)

    def test_limitxyz(self):
        NUM_X = 15
        NUM_Y = 23
        NUM_Z = 18
        BATCHES = [
            [(0, 10), (0, 10), (0, 10)],
            [(0, 10), (0, 10), (10, 18)],
            [(0, 10), (10, 20), (0, 10)],
            [(0, 10), (10, 20), (10, 18)],
            [(0, 10), (20, 23), (0, 10)],
            [(0, 10), (20, 23), (10, 18)],
            [(10, 15), (0, 10), (0, 10)],
            [(10, 15), (0, 10), (10, 18)],
            [(10, 15), (10, 20), (0, 10)],
            [(10, 15), (10, 20), (10, 18)],
            [(10, 15), (20, 23), (0, 10)],
            [(10, 15), (20, 23), (10, 18)],
        ]
        genbatch = batch.BatchGenerator3D(NUM_X, NUM_Y, NUM_Z, max_nvalues=10**3)
        self._check_batches(BATCHES, genbatch)

    def test_limitx(self):
        NUM_X = 23
        NUM_Y = 12
        NUM_Z = 13
        BATCHES = [
            [(0, 14), (0, 12), (0, 13)],
            [(14, 23), (0, 12), (0, 13)],
        ]
        genbatch = batch.BatchGenerator3D(NUM_X, NUM_Y, NUM_Z, max_nvalues=13**3)
        self._check_batches(BATCHES, genbatch)

    def test_limity(self):
        NUM_X = 14
        NUM_Y = 29
        NUM_Z = 13
        BATCHES = [
            [(0, 14), (0, 18), (0, 13)],
            [(0, 14), (18, 29), (0, 13)],
        ]
        genbatch = batch.BatchGenerator3D(NUM_X, NUM_Y, NUM_Z, max_nvalues=15**3)
        self._check_batches(BATCHES, genbatch)

    def test_limitz(self):
        NUM_X = 13
        NUM_Y = 14
        NUM_Z = 35
        BATCHES = [
            [(0, 13), (0, 14), (0, 18)],
            [(0, 13), (0, 14), (18, 35)],
        ]
        genbatch = batch.BatchGenerator3D(NUM_X, NUM_Y, NUM_Z, max_nvalues=15**3)
        self._check_batches(BATCHES, genbatch)

    def test_limitxy(self):
        NUM_X = 23
        NUM_Y = 28
        NUM_Z = 13
        BATCHES = [
            [(0, 16), (0, 16), (0, 13)],
            [(0, 16), (16, 28), (0, 13)],
            [(16, 23), (0, 16), (0, 13)],
            [(16, 23), (16, 28), (0, 13)],
        ]
        genbatch = batch.BatchGenerator3D(NUM_X, NUM_Y, NUM_Z, max_nvalues=15**3)
        self._check_batches(BATCHES, genbatch)

    def test_limityz(self):
        NUM_X = 12
        NUM_Y = 28
        NUM_Z = 30
        BATCHES = [
            [(0, 12), (0, 15), (0, 15)],
            [(0, 12), (0, 15), (15, 30)],
            [(0, 12), (15, 28), (0, 15)],
            [(0, 12), (15, 28), (15, 30)],
        ]
        genbatch = batch.BatchGenerator3D(NUM_X, NUM_Y, NUM_Z, max_nvalues=14**3)
        self._check_batches(BATCHES, genbatch)

    def test_limitxz(self):
        NUM_X = 23
        NUM_Y = 13
        NUM_Z = 28
        BATCHES = [
            [(0, 16), (0, 13), (0, 16)],
            [(0, 16), (0, 13), (16, 28)],
            [(16, 23), (0, 13), (0, 16)],
            [(16, 23), (0, 13), (16, 28)],
        ]
        genbatch = batch.BatchGenerator3D(NUM_X, NUM_Y, NUM_Z, max_nvalues=15**3)
        self._check_batches(BATCHES, genbatch)

    def _check_batches(self, batchesE, genbatch):
        count = 0
        for bE, b in zip(batchesE, genbatch):
            self.assertEqual(f"Batch3D [{bE[0][0]}:{bE[0][1]}, {bE[1][0]}:{bE[1][1]}, {bE[2][0]}:{bE[2][1]}]", str(b))
            self.assertEqual(bE[0], b.x_range)
            self.assertEqual(bE[1], b.y_range)
            self.assertEqual(bE[2], b.z_range)
            count += 1
        self.assertEqual(len(batchesE), count)


def test_classes():
    return [TestBatchGenerator2D, TestBatchGenerator3D]


if __name__ == "__main__":
    suite = unittest.TestSuite()
    for cls in test_classes():
        suite.addTest(unittest.makeSuite(cls))
    unittest.TextTestRunner(verbosity=2).run(suite)


# End of file
