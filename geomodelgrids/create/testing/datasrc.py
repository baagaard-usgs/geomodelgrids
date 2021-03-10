"""Fake data source used for testing purposes.

We use analytical functions for the spatial variations of values.
"""

import numpy

from geomodelgrids.create.core.datasrc import DataSrc
from geomodelgrids.create.utils import units


class AnalyticDataSrc(DataSrc):
    """Model with values from analytical functions.
    """

    def __init__(self, config=None):
        """Constructor.

        Args:
            config (dict)
                Configuration parameters.
        """
        super().__init__()

    def get_topography(self, points):
        """Get values topography using analytical function.

        Args:
            points (numpy.array [Nx,Ny])
                Numpy array with coordinates of points in model coordinates.
        Returns:
            Numpy array [Nx,Ny] of elevation of ground surface at points.
        """
        lx = 25.0e+3
        ly = 30.0e+3
        amplitude = 20.0
        twopi = 2.0*numpy.pi
        elev = amplitude * numpy.sin(twopi*points[:, :, 0]/lx)*numpy.cos(twopi*points[:, :, 1]/ly)
        return elev

    def get_values(self, block, topography, batch=None):
        """Get block values using analytical function.

        Args:
            block (Block)
                Block information.
            topography ()
            batch (BatchGenerator3D)
                Current batch of points in block.
        """
        NVALUES = 2

        points = block.generate_points(self, batch)
        npts = points.shape
        values = numpy.zeros((npts[0], npts[1], npts[2], NVALUES), dtype=numpy.float32)
        values[:, :, :, 0] = self._get_values_one(points)
        values[:, :, :, 1] = self._get_values_two(points)
        return values

    @staticmethod
    def _get_values_one(points):
        """
        """
        lx = 40.0e+3
        ly = 20.0e+3
        lz = 50.0e+3
        amplitude = 400.0
        twopi = 2.0 * numpy.pi
        return amplitude * numpy.cos(twopi*points[:, 0]/lx) \
            * numpy.sin(twopi*points[:, 1]/ly) \
            * numpy.sin(twopi*points[:, 2]/lz)

    @staticmethod
    def _get_values_two(points):
        """
        """
        lx = 30.0e+3
        ly = 50.0e+3
        lz = 80.0e+3
        amplitude = 200.0
        twopi = 2.0 * numpy.pi
        return 150.0 + amplitude * numpy.cos(twopi*points[:, 0]/lx) \
            * numpy.sin(twopi*points[:, 1]/ly) \
            * numpy.sin(twopi*points[:, 2]/lz)

# End of file
