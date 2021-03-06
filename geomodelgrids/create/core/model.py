"""Georeferenced model of gridded data.
"""

import logging
import math
from dataclasses import dataclass, field
from typing import List, Dict

import numpy

from geomodelgrids.create.utils.config import string_to_list
from geomodelgrids.create.utils import batch
from geomodelgrids.create.io.hdf5 import HDF5Storage


class Surface():
    """Model surface.

    Used for top of the model (topography+water) and topography+bathymetry.
    """

    def __init__(self, name, model_metadata, config, storage):
        """Constructor.

        Args:
            name (str)
                Name of surface.
            model_metadata (ModelMetadata)
                Metadata for model domain associated with block.
            config (dict)
                True if use of surface is enabled, False otherwise.
                Keys:
                    - use_surface: Model uses surface
                    - resolution_horiz: Horizontal resolution in m
                    - chunk_size: Dimensions of dataset chunk (should be about 10Kb - 1Mb)
            storage (HDF5Storage)
                Storage interface.
        """
        self.name = name
        self.model_metadata = model_metadata
        self.enabled = "True" == config["use_surface"]
        self.resolution_horiz = float(config["resolution_horiz"]) if self.enabled else None
        self.chunk_size = tuple(map(int, string_to_list(config["chunk_size"]))) if self.enabled else None
        self.storage = storage

    def get_dims(self):
        """Get number of points in block along each dimension.

        Returns:
            Tuple of number of points along each dimension (num_x, num_y).
        """
        num_x = 1 + int(self.model_metadata.dim_x / self.resolution_horiz)
        num_y = 1 + int(self.model_metadata.dim_y / self.resolution_horiz)
        return (num_x, num_y, 1)

    def get_batches(self, batch_size):
        """Get iterator witch batches of points.

        Should not be called if surface is not enabled.
        """
        if not self.enabled:
            raise NotImplementedError("Surface.get_batches() not implemented if surface is not enabled.")

        num_x, num_y, _ = self.get_dims()
        return batch.BatchGenerator2D(num_x, num_y, batch_size)

    def generate_points(self, batch=None):
        """Generate points for surface.

        Should not be called if surface is not enabled.

        Args:
            batch (utils.BatchGenerator2D)
                Current batch of points in domain corresponding to elevation data.
        Returns:
            2D array (Nx*Ny,2) of point locations for ground surface.
        """
        if not self.enabled:
            raise NotImplementedError("Surface.generate_points() not implemented if surface is not enabled.")

        num_x, num_y, _ = self.get_dims()
        logger = logging.getLogger(__name__)
        logger.info("Surface for domain contains %d points (%d x %d).",
                    num_x * num_y, num_x, num_y)

        if batch:
            x_start, x_end = batch.x_range
            y_start, y_end = batch.y_range
            logger.info("Surface for batch contains %d points (%d x %d).",
                        (x_end - x_start) * (y_end - y_start), (x_end - x_start), (y_end - y_start))
        else:
            x_start = 0
            x_end = num_x
            y_start = 0
            y_end = num_y

        dx = self.resolution_horiz
        x1 = numpy.linspace(0.0, dx * (num_x - 1), num_x)[x_start:x_end]
        y1 = numpy.linspace(0.0, dx * (num_y - 1), num_y)[y_start:y_end]
        x, y = numpy.meshgrid(x1, y1, indexing="ij")
        z = numpy.zeros(x.shape)

        xyz_geo = numpy.stack((x, y, z), axis=2)
        xyz_model = numpy.zeros(xyz_geo.shape)
        az_rad = self.model_metadata.y_azimuth * math.pi / 180.0
        xyz_model[:, :, 0] = self.model_metadata.origin_x + xyz_geo[:, :, 0] * \
            math.cos(az_rad) + xyz_geo[:, :, 1] * math.sin(az_rad)
        xyz_model[:, :, 1] = self.model_metadata.origin_y - xyz_geo[:, :, 0] * \
            math.sin(az_rad) + xyz_geo[:, :, 1] * math.cos(az_rad)
        return xyz_model


class Block():
    """Block of regular logically gridded points.
    """

    def __init__(self, name, model_metadata, config):
        """Constructor.

        Args:
            name (str)
                Name of block.
            model_metadata (ModelMetadata)
                Metadata for model domain associated with block.
            config (dict)
                Block parameters as dictionary.
                Keys:
                    - resolution_horiz: horizontal resolution (m)
                    - resolution_vert: vertical resolution (m)
                    - z_top: Elevation of top of block (m)
                    - z_bot: Elevation of bottom of block (m)
                    - z_top_offset: Vertical offset of top set of points below top of block (m)
                    - chunk_size: Dimensions of dataset chunk (should be about 10Kb - 1Mb)
        """
        self.name = name
        self.model_metadata = model_metadata
        self.resolution_horiz = float(config["resolution_horiz"])
        self.resolution_vert = float(config["resolution_vert"])
        self.z_top = float(config["z_top"])
        self.z_bot = float(config["z_bot"])
        self.z_top_offset = float(config["z_top_offset"])
        self.chunk_size = tuple(map(int, string_to_list(config["chunk_size"])))

    def get_dims(self):
        """Get number of points in block along each dimension.

        Returns:
            Tuple of number of points along each dimension (num_x, num_y, num_z).
        """
        num_x = 1 + int(self.model_metadata.dim_x / self.resolution_horiz)
        num_y = 1 + int(self.model_metadata.dim_y / self.resolution_horiz)
        num_z = 1 + int((self.z_top - self.z_bot) / self.resolution_vert)
        return (num_x, num_y, num_z)

    def get_batches(self, batch_size):
        num_x, num_y, num_z = self.get_dims()
        return batch.BatchGenerator3D(num_x, num_y, num_z, batch_size)

    def generate_points(self, top_surface, batch=None):
        """Generate grid of points in block.

        Args:
            top_surface (Surface)
                Elevation of top surface of model domain.
            batch (BatchGenerator3D)
                Current batch of points in block.
        Returns:
            3D array (Nx*Ny*Nz,3) of point locations in block.
        """
        (num_x, num_y, num_z) = self.get_dims()
        logger = logging.getLogger(__name__)
        logger.info("Entire block '%s' contains %d points (%d x %d x %d).",
                    self.name, num_x * num_y * num_z, num_x, num_y, num_z)

        if batch:
            x_start, x_end = batch.x_range
            y_start, y_end = batch.y_range
            z_start, z_end = batch.z_range
            logger.info("Current batch for block '%s' contains %d points (%d x %d x %d).",
                        self.name, (x_end - x_start) * (y_end - y_start) * (z_end - z_start),
                        (x_end - x_start), (y_end - y_start), (z_end - z_start))
        else:
            x_start = 0
            x_end = num_x
            y_start = 0
            y_end = num_y
            z_start = 0
            z_end = 0

        x1 = numpy.linspace(0.0, self.resolution_horiz * (num_x - 1), num_x)[x_start:x_end]
        y1 = numpy.linspace(0.0, self.resolution_horiz * (num_y - 1), num_y)[y_start:y_end]
        z1 = numpy.linspace(0.0, self.resolution_vert * (num_z - 1), num_z)[z_start:z_end]
        x, y, z = numpy.meshgrid(x1, y1, z1, indexing="ij")

        domain_top = 0.0
        domain_bot = -self.model_metadata.dim_z
        if top_surface.enabled:
            top_elev = self.get_surface(top_surface, batch)
            for iz in range(z.shape[-1]):
                z[:, :, iz] = domain_bot + (top_elev - domain_bot) / \
                    (domain_top - domain_bot) * (self.z_top - z[:, :, iz] - domain_bot)
        else:
            z = self.z_top - z
        z[:, :, 0] += self.z_top_offset  # Adjust top points by offset

        xyz_geo = numpy.stack((x, y, z), axis=3)
        xyz_model = numpy.zeros(xyz_geo.shape)
        az_rad = self.model_metadata.y_azimuth * math.pi / 180.0
        xyz_model[:, :, :, 0] = self.model_metadata.origin_x + xyz_geo[:, :, :, 0] * \
            math.cos(az_rad) + xyz_geo[:, :, :, 1] * math.sin(az_rad)
        xyz_model[:, :, :, 1] = self.model_metadata.origin_y - xyz_geo[:, :, :, 0] * \
            math.sin(az_rad) + xyz_geo[:, :, :, 1] * math.cos(az_rad)
        xyz_model[:, :, :, 2] = xyz_geo[:, :, :, 2]
        return xyz_model

    def get_surface(self, surface, batch=None):
        """Get surface grid for block.
        Args:
            surface (Surface)
                Surface in model domain.
            batch (BatchGenerator3D)
                Current batch of points in block.
        Returns:
            Numpy array [Nx,Ny] with elevation of surface for current batch in block.
        """
        TOLERANCE = 0.01

        class BatchSurface():

            def __init__(self, x_range, y_range):
                self.x_range = x_range
                self.y_range = y_range

        block_skip = int(0.01 + self.resolution_horiz / surface.resolution_horiz)
        if math.fabs(block_skip * surface.resolution_horiz - self.resolution_horiz) > TOLERANCE:
            raise ValueError("Block resolution ({}) must be a integer multiple of the surface resolution ({})".format(
                self.resolution_horiz, surface.resolution_horiz))

        if batch:
            x_range = (block_skip * batch.x_range[0], block_skip * batch.x_range[1])
            y_range = (block_skip * batch.y_range[0], block_skip * batch.y_range[1])
            surface_batch = BatchSurface(x_range, y_range)
        else:
            surface_batch = None

        elevation = surface.storage.load_surface(surface, surface_batch)
        return numpy.float64(elevation[::block_skip, ::block_skip, 0].squeeze())


@dataclass
class ModelMetadata:
    """Metadata describing model.
    """
    # Description
    title: str
    id: str
    description: str
    keywords: List[str]
    version: str

    # Attribution
    creator_name: str
    creator_institution: str
    creator_email: str
    acknowledgements: str
    authors: List[str]
    references: List[str]
    doi: str

    # Data
    data_values: List[str]
    data_units: List[str]

    # Coordinate system
    crs: str
    origin_x: float
    origin_y: float
    y_azimuth: float

    # Domain dimensions
    dim_x: float
    dim_y: float
    dim_z: float

    # Auxiliary data
    auxiliary: Dict[str, dict]

    def __init__(self, config):
        """Constructor.

        Args:
            config (dict)
                Dictionary with model configuration.
                Keys:
                    - title: Title of model.
                    - id: Model id.
                    - description: Description of model.
                    - keywords: List of keywords describing model.
                    - version: Model version number.
                    - creator_name: Name of person creating GeoModelGrids model.
                    - creator_email: Email of creator.
                    - creator_institution: Institution of creator.
                    - acknowledgements: acknowledgements for model.
                    - authors: List of model authors.
                    - references: List of references for model.
                    - doi: Digital Object Identifier
                    - coordsys
                        - crs
                        - origin_x
                        - origin_y
                        - y_azimuth
                    - data
                        - values
                        - units
                    - domain
                        - dim_x
                        - dim_y
                        - dim_z
        """
        self.title = config["geomodelgrids"]["title"]
        self.id = config["geomodelgrids"]["id"]
        self.description = config["geomodelgrids"]["description"]
        self.keywords = string_to_list(config["geomodelgrids"]["keywords"])
        self.version = config["geomodelgrids"]["version"]

        self.creator_name = config["geomodelgrids"]["creator_name"]
        self.creator_email = config["geomodelgrids"]["creator_email"]
        self.creator_institution = config["geomodelgrids"]["creator_institution"]
        self.acknowledgements = config["geomodelgrids"]["acknowledgements"]
        self.authors = string_to_list(config["geomodelgrids"]["authors"], delimiter="|")
        self.references = string_to_list(config["geomodelgrids"]["references"], delimiter="|")
        self.doi = config["geomodelgrids"]["doi"]

        # Data
        self.data_values = string_to_list(config["data"]["values"])
        self.data_units = string_to_list(config["data"]["units"])

        # Coordinate system
        self.crs = config["coordsys"]["crs"]
        self.origin_x = float(config["coordsys"]["origin_x"])
        self.origin_y = float(config["coordsys"]["origin_y"])
        self.y_azimuth = float(config["coordsys"]["y_azimuth"])

        # Domain dimensions
        self.dim_x = float(config["domain"]["dim_x"])
        self.dim_y = float(config["domain"]["dim_y"])
        self.dim_z = float(config["domain"]["dim_z"])

        # Auxilary data
        if "auxiliary" in config:
            self.auxiliary = config["auxiliary"]
        else:
            self.auxiliary = {}


class Model():
    """Georeferenced model composed of logical grids, potentially warped by topography.
    """

    def __init__(self, config):
        """Constructor.
        """
        self.top_surface = None
        self.topo_bathy = None
        self.blocks = []
        self.config = None
        self.storage = None

        self.metadata = None

        self._initialize(config)

    def save_domain(self):
        """Write domain information to storage."""
        self.storage.save_domain(self)

    def init_top_surface(self):
        """Create top_surface in storage.
        """
        self.storage.create_surface(self.top_surface)

    def save_top_surface(self, elevation, batch=None):
        """Write top_surface information to storage.

        Args:
            elevation (numpy_array [Nx,Ny])
                Numpy array with elevation of top surface.
            batch (BatchGenerator2D)
                Current batch of points in domain corresponding to elevation data.
        """
        self.storage.save_surface(self.top_surface, elevation, batch)

    def init_topography_bathymetry(self):
        """Create topography/bathymetry in storage.
        """
        self.storage.create_surface(self.topo_bathy)

    def save_topography_bathymetry(self, elevation, batch=None):
        """Write topography/bathymetry information to storage.

        Args:
            elevation (numpy_array [Nx,Ny])
                Numpy array with elevation of ground surface and sea floor.
            batch (BatchGenerator2D)
                Current batch of points in domain corresponding to elevation data.
        """
        self.storage.save_surface(self.topo_bathy, elevation, batch)

    def init_block(self, block):
        """Create block in storage.

        Args:
            block (Block)
                Block information.
        """
        self.storage.create_block(block)

    def save_block(self, block, values, batch=None):
        """Write block information to storage.

        Args:
            block (Block)
                Block information.
            values (numpy.array)
                Numpy array [Nx,Ny,Nz,Nv] of gridded data asociated with block.
            batch (utils.BatchGenerator3D)
                Current batch of points in domain corresponding to elevation data.
        """
        self.storage.save_block(block, values, batch)

    def _initialize(self, config):
        """Setup model.

        Args:
            config (dict)
                Model configuration.
        """
        self.metadata = ModelMetadata(config)

        for name in string_to_list(config["domain"]["blocks"]):
            block = Block(name, self.metadata, config[name])
            self.blocks.append(block)

        self.storage = HDF5Storage(config["geomodelgrids"]["filename"])
        self.top_surface = Surface("top_surface", self.metadata, config["top_surface"], self.storage)
        self.topo_bathy = Surface("topography_bathymetry", self.metadata, config["topography_bathymetry"], self.storage)


# End of file
