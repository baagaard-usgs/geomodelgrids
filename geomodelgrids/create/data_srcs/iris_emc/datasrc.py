"""IRIS EMC data sources.
"""

import os
import re
import sys
from importlib import import_module

import numpy

from geomodelgrids.create.core.datasrc import DataSrc
from geomodelgrids.create.utils import units
from geomodelgrids.create.utils.config import string_to_list
from geomodelgrids.create.core import NODATA_VALUE


class EMCNetCDF(DataSrc):
    """IRIS EMC model as NetCDF file.
    """

    def __init__(self, config):
        """Constructor.

        Args:
            config (dict)
                Configuration parameters.
        """
        try:
            import netCDF4
        except ImportError as err:
            raise ImportError("Python netCDF4 module is required to read IRIS EMC NetCDF files.")
        super().__init__()
        self.config = config

    def initialize(self):
        """Initialize model.
        """
        import netCDF4
        filename = os.path.expanduser(self.config["iris_emc"]["filename"])
        self.emc = netCDF4.Dataset(filename, "r")
        self._read_metadata()

    def _read_metadata(self):
        return

    def get_top_surface(self, points):
        return None

    def get_topography_bathymetry(self, points):
        return None

    def get_values(self, block, top_surface, topo_bathy, batch=None):
        """Get values at points.

        Args:
            block (Block)
                Block information.
            top_surface (Surface)
                Elevation of top surface of model.
            topo_bathy (Surface)
                Elevation of topography or bathymetry used to define depth.
            batch (BatchGenerator3D)
                Current batch of points in block.
        """
        values = values.reshape((points.shape[0], points.shape[1], points.shape[2], -1))
        return values

# End of file
