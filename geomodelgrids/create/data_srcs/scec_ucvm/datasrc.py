"""scec_ucvm data sources.
"""

import os
import re
import sys
from importlib import import_module

import numpy
import pyproj

from geomodelgrids.create.core.datasrc import DataSrc
from geomodelgrids.create.utils import units
from geomodelgrids.create.utils.config import string_to_list
from geomodelgrids.create.data_srcs.scec_ucvm import api
from geomodelgrids.create.core import NODATA_VALUE


class UCVMDataSrc(DataSrc):
    """Model constructed from querying models using the SCEC UCVM framework.
    """

    def __init__(self, config):
        """Constructor.

        Args:
            config (dict)
                Configuration parameters.
        """
        self.config = config
        self.api = None
        super().__init__()

    def initialize(self):
        """Initialize model.
        """
        ucvm_conf = self.config["scec_ucvm"]["conf"]
        ucvm_env = self.config["scec_ucvm.environment"]
        self.api = api.UCVMAPI(ucvm_conf, ucvm_env)
        self.config["auxiliary"] = {}
        self.cs_transformer = pyproj.Transformer.from_crs(self.config["coordsys"]["crs"], "EPSG:4326")

    def get_top_surface(self, points):
        """Query UCVM model for elevation of top surface at points.

        ucvm_query uses longitude (WGS84), latitude (WGS84), depth (m) coordinates.

        Args:
            points (numpy.array [Nx,Ny])
                Numpy array with coordinates of points in model coordinates.
        Returns:
            Numpy array [Nx,Ny] of elevation of top surface at points.
        """

        hscale = units.length_scale(self.config["scec_ucvm"]["xy_units"])

        surf_shape = points.shape[0:2]
        points = points.reshape((-1, points.shape[2])) / hscale
        points_geo = self.cs_transformer.transform(points[:,0], points[:,1])

        depth = numpy.zeros(points.shape[0])
        # Coordinates are longitude, latitude, depth
        points_geo = numpy.vstack((points_geo[1], points_geo[0], depth)).transpose()

        elev = self.api.ucvm_surface(points_geo).reshape(surf_shape)
        elev *= units.length_scale(self.config["scec_ucvm"]["elev_units"])
        return elev.reshape((elev.shape[0], elev.shape[1], 1))

    def get_topography_bathymetry(self, points):
        """Query scec_ucvm model for elevation of topoggraphy or bathymetry at points.

        Args:
            points (numpy.array [Nx,Ny])
                Numpy array with coordinates of points in model coordinates.
        Returns:
            Numpy array [Nx,Ny] of elevation of topography or bathymetry at points.
        """
        return

    def get_values(self, block, top_surface, topo_bathy, batch=None):
        """Query UCVM model for values at points.

        ucvm_query uses longitude (WGS84), latitude (WGS84), depth (m) coordinates.

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
        DTYPE = {
            "names": ("Vp", "Vs", "density"),
            "formats": ("f4", "f4", "f4")
        }
        vscale = 1.0
        dscale = 1.0
        converters = {
            0: lambda s: vscale*float(s),
            1: lambda s: vscale*float(s),
            2: lambda s: dscale*float(s),
        }

        points = block.generate_points(top_surface, batch)
        points_shape = points.shape
        points_tmp = points.reshape((-1, points.shape[3])).copy()
        hscale = units.length_scale(self.config["scec_ucvm"]["xy_units"])
        vscale = units.length_scale(self.config["scec_ucvm"]["elev_units"])
        points_tmp[:, 0:2] /= hscale
        points_tmp[:, 2] /= vscale
        points_geo = self.cs_transformer.transform(points_tmp[:,0], points_tmp[:,1], points_tmp[:,2])
        del points_tmp

        top_surf_elev = block.get_surface(top_surface, batch)
        depth = numpy.zeros(points.shape[:-1])
        for iz in range(depth.shape[-1]):
            depth[:, :, iz] = top_surf_elev - points[:, :, iz, 2]
        depth[:, :, 0] -= block.z_top_offset
        del points
        # Coordinates are longitude, latitude, depth
        points_geo = numpy.vstack((points_geo[1], points_geo[0], depth.ravel())).transpose()

        models = self.config["scec_ucvm"]["models"]
        values_tmp = self.api.ucvm_query(points_geo, models, DTYPE, converters)
        values = []
        for v in string_to_list(self.config["data"]["values"]):
            values += [values_tmp[v]]
        del values_tmp
        values = numpy.vstack(tuple(values)).transpose()
        values = values.reshape((points_shape[0], points_shape[1], points_shape[2], -1))
        return values


# End of file
