"""Georeferenced data source providing gridded data.
"""

from abc import ABC, abstractmethod


class DataSrc(ABC):
    """Georeferenced data source providing gridded data.
    """

    def __init__(self):
        """Constructor.
        """
        return

    def initialize(self):
        """Initialize data source.
        """
        return

    @abstractmethod
    def get_topography(self, points):
        """Query EarthVision model for elevation of ground surface at points.

        Args:
            points (numpy.array [Nx,Ny])
                Numpy array with coordinates of points in model coordinates.
        """

    @abstractmethod
    def get_values(self, block, topography, batch=None):
        """Query EarthVision model for values at points.

        Args:
            block (Block)
                Block information.
            batch (BatchGenerator3D)
                Current batch of points in block.
        """


# End of file
