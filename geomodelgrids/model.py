"""Georeferenced model of gridded data.
"""

import logging
import math
from abc import ABC

import numpy

from .config import string_to_list
from .io.hdf5 import HDF5Storage


class Block():
    """Block of regular logically gridded points.
    """

    def __init__(self, name, config):
        """Constructor.

        Args:
            name (str)
                Name of block.
            config (dict)
                Block parameters as dictionary.
                Keys:
                    resolution_horiz: horizontal resolution (m)
                    resolution_vert: vertical resolution (m)
                    z_top: Elevation of top of block (m)
                    z_bot: Elevation of bottom of block (m)
                    z_top_offset: Vertical offset of top set of points below top of block (m)
                    chunk_size: Dimensions of dataset chunk (should be about 10Kb - 1Mb)
        """
        self.name = name
        self.resolution_horiz = float(config["resolution_horiz"])
        self.resolution_vert = float(config["resolution_vert"])
        self.z_top = float(config["z_top"])
        self.z_bot = float(config["z_bot"])
        self.z_top_offset = float(config["z_top_offset"])
        self.chunk_size = tuple(map(int, string_to_list(config["chunk_size"])))

    def get_dims(self, domain):
        """Get number of points in block along each dimension.

        Args:
            domain (Model)
                Model domain.

        Returns:
            Array of points in block (Nx*Ny*Nz, 3)
        """
        num_x = 1 + int(domain.dim_x / self.resolution_horiz)
        num_y = 1 + int(domain.dim_y / self.resolution_horiz)
        num_z = 1 + int((self.z_top - self.z_bot) / self.resolution_vert)
        return (num_x, num_y, num_z)

    def generate_points(self, domain):
        """Generate grid of points in block.

        Args:
            domain (Model)
                Model domain.

        Returns:
            3D array (Nx*Ny*Nz,3) of point locations in block.
        """
        (num_x, num_y, num_z) = self.get_dims(domain)
        logger = logging.getLogger(__name__)
        logger.info("Block '{}' contains {} points ({} x {} x {}).".format(
            self.name, num_x * num_y * num_z, num_x, num_y, num_z,))

        x1 = numpy.linspace(0.0, self.resolution_horiz * (num_x - 1), num_x)
        y1 = numpy.linspace(0.0, self.resolution_horiz * (num_y - 1), num_y)
        z1 = numpy.linspace(0.0, self.resolution_vert * (num_z - 1), num_z)
        x, y, z = numpy.meshgrid(x1, y1, z1, indexing="ij")

        domain_top = 0.0
        domain_bot = -domain.dim_z
        if domain.topography is not None:
            topo_geo = self.get_block_elevation(domain.topography)
            for iz in range(z.shape[-1]):
                z[:, :, iz] = domain_bot + (topo_geo - domain_bot) / \
                  (domain_top - domain_bot) * (self.z_top - z[:, :, iz] - domain_bot)

            # Move top points down
            z[:, :, 0] += self.z_top_offset
        else:
            z = self.z_top - z

        xyz_geo = numpy.stack((x, y, z), axis=3)
        xyz_model = numpy.zeros(xyz_geo.shape)
        az_rad = domain.y_azimuth * math.pi / 180.0
        xyz_model[:, :, :, 0] = domain.origin_x + xyz_geo[:, :, :, 0] * \
            math.cos(az_rad) + xyz_geo[:, :, :, 1] * math.sin(az_rad)
        xyz_model[:, :, :, 1] = domain.origin_y - xyz_geo[:, :, :, 0] * \
            math.sin(az_rad) + xyz_geo[:, :, :, 1] * math.cos(az_rad)
        xyz_model[:, :, :, 2] = xyz_geo[:, :, :, 2]
        return xyz_model

    def get_block_elevation(self, topography):
        """Get topography grid for block.
        Args:
            domain (Model)
                Model domain.

        Returns:
            2D array (Nx*Ny*Nz,3) of point locations in block.
        """
        TOLERANCE = 0.01
        num_skip = int(0.01 + self.resolution_horiz / topography.resolution_horiz)
        if math.fabs(num_skip * topography.resolution_horiz - self.resolution_horiz) > TOLERANCE:
            raise ValueError("Block resolution ({}) must be a integer multiple of the topography resolution ({})".format(
                self.resolution_horiz, topography.resolution_horiz))

        return topography.elevation[::num_skip, ::num_skip, 0].squeeze()


class Topography():
    """Surface topography.
    """

    def __init__(self, config):
        """Constructor.

        Args:
           config (dict)
             True if use of topography is enabled, False otherwise.
             Keys:
                 use_topography: Model uses topography
                 resolution_horiz: Horizontal resolution in m
                 chunk_size: Dimensions of dataset chunk (should be about 10Kb - 1Mb)
        """
        self.elevation = None
        self.enabled = bool(config["use_topography"])
        self.resolution_horiz = float(config["resolution_horiz"]) if self.enabled else None
        self.chunk_size = tuple(map(int, string_to_list(config["chunk_size"]))) if self.enabled else None

    def set_elevation(self, elev):
        """Set topography values.

        Args:
            elev (numpy.array)
                Numpy array [Nx*Ny] with elevation at points.
        """
        if not self.enabled:
            return

        self.elevation = elev

    def generate_points(self, domain):
        """Generate points for topography.

        Args:
            domain (Model)
                Model with topography.
        Returns:
            2D array (Nx*Ny,2) of point locations for ground surface.
        """
        dx = self.resolution_horiz

        num_x = 1 + int(domain.dim_x / dx)
        num_y = 1 + int(domain.dim_y / dx)
        logger = logging.getLogger(__name__)
        logger.info("Topography contains {} points ({} x {}).".format(
            num_x * num_y, num_x, num_y))

        x1 = numpy.linspace(0.0, dx * (num_x - 1), num_x)
        y1 = numpy.linspace(0.0, dx * (num_y - 1), num_y)
        x, y = numpy.meshgrid(x1, y1, indexing="ij")
        z = numpy.zeros(x.shape)

        xyz_geo = numpy.stack((x, y, z), axis=2)
        xyz_model = numpy.zeros(xyz_geo.shape)
        az_rad = domain.y_azimuth * math.pi / 180.0
        xyz_model[:, :, 0] = domain.origin_x + xyz_geo[:, :, 0] * math.cos(az_rad) + xyz_geo[:, :, 1] * math.sin(az_rad)
        xyz_model[:, :, 1] = domain.origin_y - xyz_geo[:, :, 0] * math.sin(az_rad) + xyz_geo[:, :, 1] * math.cos(az_rad)
        return xyz_model


class Model(ABC):
    """Georeferenced model composed of logical grids, potentially warped by topography.
    """

    def __init__(self, config):
        """Constructor.
        """
        self.topography = None
        self.blocks = []

        self.initialize(config)

    def initialize(self, config):
        """Setup model, creating blocks.

        Args:
            config (dict)
                Model configuration.
        """
        self.title = config["geomodelgrids"]["title"]
        self.id = config["geomodelgrids"]["id"]
        self.description = config["geomodelgrids"]["description"]
        self.keywords = string_to_list(config["geomodelgrids"]["keywords"])
        self.creator_name = config["geomodelgrids"]["creator_name"]
        self.creator_email = config["geomodelgrids"]["creator_email"]
        self.creator_institution = config["geomodelgrids"]["creator_institution"]
        self.acknowledgments = config["geomodelgrids"]["acknowledgments"]
        self.authors = string_to_list(config["geomodelgrids"]["authors"], delimiter="|")
        self.references = string_to_list(config["geomodelgrids"]["references"], delimiter="|")
        self.doi = config["geomodelgrids"]["doi"]
        self.version = config["geomodelgrids"]["version"]
        self.crs = config["coordsys"]["crs"]
        self.origin_x = float(config["coordsys"]["origin_x"])
        self.origin_y = float(config["coordsys"]["origin_y"])
        self.y_azimuth = float(config["coordsys"]["y_azimuth"])
        self.data_values = string_to_list(config["data"]["values"])
        self.data_units = string_to_list(config["data"]["units"])
        self.dim_x = float(config["domain"]["dim_x"])
        self.dim_y = float(config["domain"]["dim_y"])
        self.dim_z = float(config["domain"]["dim_z"])

        self.config = config

        self.topography = Topography(config["topography"])
        for name in string_to_list(config["domain"]["blocks"]):
            block = Block(name, config[name])
            self.blocks.append(block)

        self.storage = HDF5Storage(config["geomodelgrids"]["filename"])

    def import_domain(self):
        """Write domain information to storage."""
        self.storage.save_domain(self)

    def import_topography(self):
        """Write topography information to storage.
        """
        self.storage.save_topography(self.topography)

    def load_topography(self):
        """Load topography from model file."""
        self.storage.load_topography(self.topography)

    def import_block(self, block, values):
        """Write block information to storage.

        Args:
            block (Block)
                Block information.
            values (numpy.array)
                Numpy array [Nx,Ny,Nz,Nv] of gridded data asociated with block.
        """
        self.storage.save_block(block, values)

    @abstractmethod
    def query_topography(self, points):
        """Query EarthVision model for elevation of ground surface at points.

        Args:
            points (numpy.array [Nx,Ny,Nz])
                Numpy array with coordinates of points in model coordinates.
        """

    @abstractmethod
    def query_values(self, block):
        """Query EarthVision model for values at points.

        Args:
            block (Block)
                Block information.
        """


# End of file
