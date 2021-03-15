"""Test create.utils.units.
"""

import unittest
import os

import numpy

from geomodelgrids.create.utils import units


class TestUnits(unittest.TestCase):

    def test_lengthscale(self):
        """Test units.length_scale().
        """
        names = ["m", "meter", "meters"]
        for name in names:
            msg = f"Mismatch in length scale for '{name}'."
            self.assertEqual(1.0, units.length_scale(name), msg=msg)

        names = ["km", "kilometer", "kilometers"]
        for name in names:
            msg = f"Mismatch in length scale for '{name}'."
            self.assertEqual(1.0e+3, units.length_scale(name), msg=msg)

        names = ["ft", "feet", "foot"]
        for name in names:
            msg = f"Mismatch in length scale for '{name}'."
            self.assertEqual(0.3048, units.length_scale(name), msg=msg)


def test_classes():
    return [TestUnits]


if __name__ == "__main__":
    suite = unittest.TestSuite()
    for cls in test_classes():
        suite.addTest(unittest.makeSuite(cls))
    unittest.TextTestRunner(verbosity=2).run(suite)


# End of file
