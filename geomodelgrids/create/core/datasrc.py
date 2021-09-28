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

    def get_metadata(self):
        """Get any additional metadata provided by data source.

        @returns Dict
        """
        return {}

    @abstractmethod
    def get_top_surface(self, points):
        """Query model for elevation of top surface at points.

        Args:
            points (numpy.array [Nx,Ny])
                Numpy array with coordinates of points in model coordinates.
        Returns:
            Numpy array with elevation at points.
        """

    @abstractmethod
    def get_topography_bathymetry(self, points):
        """Query model for elevation of topography/bathymetry at points.

        Args:
            points (numpy.array [Nx,Ny])
                Numpy array with coordinates of points in model coordinates.
        Returns:
            Numpy array with elevation at points.
        """

    @abstractmethod
    def get_values(self, block, top_surface, topo_bathy, batch=None):
        """Query model for values at points.

        Args:
            block (Block)
                Block information.
            top_surface (Surface)
                Top surface.
            topo_bathy (Surface)
                Topography or bathymetry surface used to define depth.
            batch (BatchGenerator3D)
                Current batch of points in block.
        """


# End of file
