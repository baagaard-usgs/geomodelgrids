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
    def get_top_surface(self, points):
        """Query model for elevation of top surface at points.

        Args:
            points (numpy.array [Nx,Ny])
                Numpy array with coordinates of points in model coordinates.
        """

    @abstractmethod
    def get_topography_bathymetry(self, points):
        """Query model for elevation of topography/bathymetry at points.

        Args:
            points (numpy.array [Nx,Ny])
                Numpy array with coordinates of points in model coordinates.
        """

    @abstractmethod
    def get_values(self, block, top_surface, topo_bathy, batch=None):
        """Query model for values at points.

        Args:
            block (Block)
                Block information.
            top_surface (Surface)
                Elevation of top surface.
            topo_bathy (Surface)
                Elevation of topography or bathymetry used to define depth.
            batch (BatchGenerator3D)
                Current batch of points in block.
        """


# End of file
