"""IRIS EMC data sources.
"""

import os
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
        self._read_model_metadata()
        self._read_domain_metadata()
        self._read_values_metadata()
        self._read_block_metadata()

    def _read_model_metadata(self):
        def list_from_attr(nc, name):
            return [getattr(nc, name)] if hasattr(nc, name) else []

        ATTRS = {
            "title": lambda nc: getattr(nc, "title", "unknown").strip(),
            "id": lambda nc: getattr(nc, "id", "unknown").strip(),
            "description": lambda nc: getattr(nc, "summary", "").strip(),
            "keywords": lambda nc: [kw.strip() for kw in getattr(nc, "keywords", "").split(",")],
            "history": lambda nc: getattr(nc, "history", "").strip(),
            "comment": lambda nc: getattr(nc, "comment", "").strip(),
            "version": lambda nc: getattr(nc, "data_revision", "unknown").strip(),
            "creator_name": lambda nc: getattr(nc, "author_name", getattr(nc, "creator_name", "")).strip(),
            "creator_email": lambda nc: getattr(nc, "author_email", getattr(nc, "creator_email", "")).strip(),
            "creator_institution": lambda nc: getattr(nc, "author_institution", getattr(nc, "creator_institution", "")).strip(),
            "authors": lambda nc: list_from_attr(nc, "author_name"),
            "acknowledgement": lambda nc: getattr(nc, "acknowledgement", "").strip(),
            "references": lambda nc: list_from_attr(nc, "reference"),
            "license": lambda nc: getattr(nc, "license", "").strip(),
        }

        import datetime

        metadata = {}
        for key, fn in ATTRS.items():
            metadata[key] = fn(self.emc)
        metadata["history"] += f"\n{datetime.date.today()} EMC NetCDF file converted to GeoModelGrids HDF5 file."
        self.config["geomodelgrids"].update(metadata)

        self.config["coordsys"] = {
            "crs": "EPSG:4979",
            "origin_x": self.emc.geospatial_lon_min,
            "origin_y": self.emc.geospatial_lat_min,
            "y_azimuth": 0.0,
        }

    def _read_domain_metadata(self):
        lon_max = self.emc.geospatial_lon_max
        lon_min = self.emc.geospatial_lon_min
        lat_max = self.emc.geospatial_lat_max
        lat_min = self.emc.geospatial_lat_min
        depth_max = self.emc.geospatial_vertical_max
        depth_min = self.emc.geospatial_vertical_min
        assert self.emc.geospatial_lon_units.strip() in ["degrees_east", "degrees"]
        assert self.emc.geospatial_lat_units.strip() in ["degrees_north", "degrees"]
        vertical_scale = units.length_scale(self.emc.geospatial_vertical_units)

        self.config["domain"].update({
            "dim_x": lon_max - lon_min,
            "dim_y": lat_max - lat_min,
            "dim_z": (depth_max - depth_min) * vertical_scale,
            "blocks": ["block"],
        })

    def _read_values_metadata(self):
        values = list(self.emc.variables.keys())
        for ignore in ("longitude", "latitude", "depth"):
            values.remove(ignore)

        units = []
        for value in values:
            units.append(self.emc.variables[value].units)
        self.config["data"] = {
            "values": values,
            "units": units,
        }

        long_names = {}
        for value in values:
            long_names[value] = self.emc.variables[value].long_name
        self.config["auxiliary"] = {"values": long_names}

    def _read_block_metadata(self):
        def is_uniform_resolution(z):
            return numpy.std(numpy.diff(z)) < 0.001

        lon_max = self.emc.geospatial_lon_max
        lon_min = self.emc.geospatial_lon_min
        lat_max = self.emc.geospatial_lat_max
        lat_min = self.emc.geospatial_lat_min
        depth_max = self.emc.geospatial_vertical_max
        depth_min = self.emc.geospatial_vertical_min
        vertical_scale = units.length_scale(self.emc.geospatial_vertical_units)

        info = {
            "x_resolution": (lon_max - lon_min) / (self.emc.variables["longitude"].size-1),
            "y_resolution": (lat_max - lat_min) / (self.emc.variables["latitude"].size-1),
            "z_top_offset": 0.0,
        }
        if is_uniform_resolution(self.emc.variables["depth"][:]):
            info["z_resolution"] = vertical_scale * (depth_max - depth_min) / (self.emc.variables["depth"].size-1),
        else:
            z_coordinates = -vertical_scale * self.emc.variables["depth"][:]
            info["z_coordinates"] = z_coordinates.tolist()
        self.config["block"].update(info)

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
        value_names = self.config["data"]["values"]
        if batch:
            nx = batch.x_range[1] - batch.x_range[0]
            ny = batch.y_range[1] - batch.y_range[0]
            nz = batch.z_range[1] - batch.z_range[0]
            values = numpy.zeros((nx, ny, nz, len(value_names)), dtype=numpy.float32)
            x_start, x_end = batch.x_range
            y_start, y_end = batch.y_range
            z_start, z_end = batch.z_range
            for i_value, name in enumerate(value_names):
                values_batch = self.emc.variables[name][z_start:z_end, y_start:y_end, x_start:x_end]
                values[:, :, :, i_value] = numpy.transpose(values_batch)
        else:
            nz, ny, nx = self.emc.variables[value_names[0]][:].shape
            values = numpy.zeros((nx, ny, nz, len(value_names)), dtype=numpy.float32)
            for i_value, name in enumerate(value_names):
                values[:, :, :, i_value] = numpy.transpose(self.emc.variables[name][:])
        return values


# End of file
