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
                Keys:
                    - x_resolution: Resolution in x-direction (m) if uniform resolution in x-direction.
                    - x_coordinates: Array of x coordinates (m) if variable resolution in x-direction.
                    - y_resolution: Resolution in y-direction (m) if uniform resolution in y-direction.
                    - y_coordinates: Array of y coordinates (m) if variable resolution in y-direction.
                    - chunk_size: Dimensions of dataset chunk (should be about 10Kb - 1Mb)
            storage (HDF5Storage)
                Storage interface.
        """
        self.name = name
        self.model_metadata = model_metadata
        if "x_resolution" in config:
            self.x_resolution = float(config["x_resolution"])
            self.x_coordinates = None
        else:
            self.x_resolution = None
            self.x_coordinates = tuple(map(float, string_to_list(config["x_coordinates"])))
        if "y_resolution" in config:
            self.y_resolution = float(config["y_resolution"])
            self.y_coordinates = None
        else:
            self.y_resolution = None
            self.y_coordinates = tuple(map(float, string_to_list(config["y_coordinates"])))

        self.chunk_size = tuple(map(int, string_to_list(config["chunk_size"])))
        self.storage = storage

    def get_dims(self):
        """Get number of points in surface along each dimension.

        Returns:
            Tuple of number of points along each dimension (num_x, num_y).
        """
        num_x = 1 + int(self.model_metadata.dim_x / self.x_resolution) if self.x_resolution else len(self.x_coordinates)
        num_y = 1 + int(self.model_metadata.dim_y / self.y_resolution) if self.y_resolution else len(self.y_coordinates)
        return (num_x, num_y, 1)

    def get_batches(self, batch_size):
        """Get generator for batches of points.
        """
        num_x, num_y, _ = self.get_dims()
        return batch.BatchGenerator2D(num_x, num_y, batch_size)

    def generate_points(self, batch=None):
        """Generate points for surface.

        Args:
            batch (utils.BatchGenerator2D)
                Current batch of points in domain corresponding to elevation data.
        Returns:
            2D array (Nx*Ny,2) of point locations for ground surface.
        """
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

        if self.x_resolution:
            x1 = numpy.linspace(0.0, self.x_resolution * (num_x - 1), num_x)[x_start:x_end]
        else:
            x1 = self.x_coordinates[x_start:x_end]
        if self.y_resolution:
            y1 = numpy.linspace(0.0, self.y_resolution * (num_y - 1), num_y)[y_start:y_end]
        else:
            y1 = self.y_coordinates[y_start:y_end]
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

    def get_attributes(self):
        attrs = []
        if self.x_resolution:
            attrs.append(("x_resolution", float))
        else:
            attrs.append(("x_coordinates", tuple, float))
        if self.y_resolution:
            attrs.append(("y_resolution", float))
        else:
            attrs.append(("y_coordinates", tuple, float))
        return attrs


class Block():
    """Grid of points on a logically regular grid.
    """

    def __init__(self, name, model_metadata, config):
        """Constructor.

        Args:
            name (str)
                Name of block.
            model_metadata (ModelMetadata)
                Metadata for model domain containing the block.
            config (dict)
                Block parameters as dictionary.
                Keys:
                    - x_resolution: Resolution in x-direction (m) if uniform resolution in x-direction.
                    - x_coordinates: Array of x coordinates (m) if variable resolution in x-direction.
                    - y_resolution: Resolution in y-direction (m) if uniform resolution in y-direction.
                    - y_coordinates: Array of y coordinates (m) if variable resolution in y-direction.
                    - z_resolution: Resolution in z-direction (m) if uniform resolution in z-direction.
                    - z_top: Elevation of top of block (m) if uniform resolution in z-direction.
                    - z_bot: Elevation of bottom of block (m) if uniform resolution in z-direction.
                    - z_coordinates: Array of z coordinates (m) if variable resolution in z-direction.
                    - z_top_offset: Vertical offset of top set of points below top of block (m) (used to avoid roundoff errors).
                    - chunk_size: Dimensions of dataset chunk (should be about 10Kb - 1Mb)
        """
        self.name = name
        self.model_metadata = model_metadata

        if "x_resolution" in config:
            self.x_resolution = float(config["x_resolution"])
            self.x_coordinates = None
        else:
            self.x_resolution = None
            self.x_coordinates = tuple(map(float, string_to_list(config["x_coordinates"])))
        if "y_resolution" in config:
            self.y_resolution = float(config["y_resolution"])
            self.y_coordinates = None
        else:
            self.y_resolution = None
            self.y_coordinates = tuple(map(float, string_to_list(config["y_coordinates"])))
        if "z_resolution" in config:
            self.z_resolution = float(config["z_resolution"])
            self.z_coordinates = None
            self.z_top = float(config["z_top"])
            self.z_bot = float(config["z_bot"])
        else:
            self.z_resolution = None
            self.z_coordinates = tuple(map(float, string_to_list(config["z_coordinates"])))
            self.z_top = numpy.max(self.z_coordinates)
            self.z_bot = numpy.min(self.z_coordinates)

        self.z_top_offset = float(config["z_top_offset"])
        self.chunk_size = tuple(map(int, string_to_list(config["chunk_size"])))

    def get_dims(self):
        """Get number of points in block along each dimension.

        Returns:
            Tuple of number of points along each dimension (num_x, num_y, num_z).
        """
        if self.x_resolution:
            num_x = 1 + int(self.model_metadata.dim_x / self.x_resolution)
        else:
            num_x = len(self.x_coordinates)
        if self.y_resolution:
            num_y = 1 + int(self.model_metadata.dim_y / self.y_resolution)
        else:
            num_y = len(self.y_coordinates)
        if self.z_resolution:
            num_z = 1 + int((self.z_top - self.z_bot) / self.z_resolution)
        else:
            num_z = len(self.z_coordinates)
        return (num_x, num_y, num_z)

    def get_batches(self, batch_size):
        """Get batch generator for block.

        Returns:
            BatchGenerator3D for block.
        """
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

        if self.x_resolution:
            x1 = numpy.linspace(0.0, self.x_resolution * (num_x - 1), num_x)[x_start:x_end]
        else:
            x1 = self.x_coordinates[x_start:x_end]
        if self.y_resolution:
            y1 = numpy.linspace(0.0, self.y_resolution * (num_y - 1), num_y)[y_start:y_end]
        else:
            y1 = self.y_coordinates[y_start:y_end]
        if self.z_resolution:
            z1 = numpy.linspace(0.0, self.z_resolution * (num_z - 1), num_z)[z_start:z_end]
        else:
            z1 = self.z_coordinates[z_start:z_end]
        x, y, z = numpy.meshgrid(x1, y1, z1, indexing="ij")

        domain_top = 0.0
        domain_bot = -self.model_metadata.dim_z
        if top_surface:
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

        def _get_slice_uniform(block_resolution, surfae_dim, surface_resolution, batch_range):
            block_skip = int(0.01 + block_resolution / surface_resolution)
            if math.fabs(block_skip * surface_resolution - block_resolution) > TOLERANCE:
                raise ValueError(
                    f"Block resolution ({block_resolution}) must be a integer multiple of the surface resolution ({surface_resolution})")
            if batch:
                surf_range = (block_skip * batch_range[0], block_skip * batch_range[1])
                indices = numpy.arange(0, surf_range[1]-surf_range[0], step=block_skip, dtype=numpy.int)
            else:
                surf_range = None
                indices = numpy.arange(0, surfae_dim+0.5, step=block_skip, dtype=numpy.int)
            return (surf_range, indices)

        def _get_slice_variable(block_coordinates, surface_coordinates, batch_range):
            if batch:
                block_values = block_coordinates[batch_range[0]:batch_range[1]]
                scoords_sorted = numpy.array(surface_coordinates)
                scoords_sorted.sort()
                indices = scoords_sorted.searchsorted(block_values)
                for index, block_coord in zip(indices, block_values):
                    if math.fabs(scoords_sorted[index] - block_coord) > TOLERANCE:
                        raise ValueError(
                            f"Could not find surface coordinate matching block coordinate ({block_coord}).")
                indices -= indices[0]
                surf_range = (numpy.min(indices), numpy.max(indices)+1)
            else:
                scoords_sorted = numpy.array(surface_coordinates)
                scoords_sorted.sort()
                indices = scoords_sorted.searchsorted(block_coordinates)
                for index, block_coord in zip(indices, block_coordinates):
                    if abs(scoords_sorted[index] - block_coord) > TOLERANCE:
                        raise ValueError(
                            f"Could not find surface coordinate matching block coordinate ({block_coord}).")
                surf_range = (numpy.min(indices), numpy.max(indices)+1)
            return (surf_range, indices)

        if self.x_resolution and not surface.x_resolution:
            raise ValueError("Cannot use uniform x-resolution block with variable x-resolution surface.")
        elif self.x_coordinates and not surface.x_coordinates:
            raise ValueError("Cannot use variable x-resolution block with uniform x-resolution surface.")
        if self.y_resolution and not surface.y_resolution:
            raise ValueError("Cannot use uniform y-resolution block with variable y-resolution surface.")
        elif self.y_coordinates and not surface.y_coordinates:
            raise ValueError("Cannot use variable y-resolution block with uniform y-resolution surface.")

        surface_dims = surface.get_dims()
        if self.x_resolution:
            (surf_xrange, x_indices) = _get_slice_uniform(self.x_resolution, surface_dims[0],
                                                          surface.x_resolution, batch.x_range)
        else:
            (surf_xrange, x_indices) = _get_slice_variable(self.x_coordinates, surface.x_coordinates, batch.x_range)
        if self.y_resolution:
            (surf_yrange, y_indices) = _get_slice_uniform(self.y_resolution, surface_dims[1],
                                                          surface.y_resolution, batch.y_range)
        else:
            (surf_yrange, y_indices) = _get_slice_variable(self.y_coordinates, surface.y_coordinates, batch.y_range)

        if batch:
            surface_batch = BatchSurface(surf_xrange, surf_yrange)
        else:
            surface_batch = None

        elevation = surface.storage.load_surface(surface, surface_batch)
        return numpy.float64(elevation[numpy.ix_(x_indices, y_indices, [0])].squeeze())

    def get_attributes(self):
        """Get attributes associated with block.

        Returns:
            Array of tuples with attributes for block.
        """
        attrs = []
        if self.x_resolution:
            attrs.append(("x_resolution", float))
        else:
            attrs.append(("x_coordinates", tuple, float))
        if self.y_resolution:
            attrs.append(("y_resolution", float))
        else:
            attrs.append(("y_coordinates", tuple, float))
        if self.z_resolution:
            attrs.append(("z_resolution", float))
            attrs.append(("z_top", float))
        else:
            attrs.append(("z_coordinates", tuple, float))
        return attrs


@dataclass
class ModelMetadata:
    """Metadata describing model.
    """
    # Description
    title: str
    id: str
    description: str
    keywords: List[str]
    history: str
    comment: str
    version: str

    # Attribution
    creator_name: str
    creator_institution: str
    creator_email: str
    acknowledgement: str
    authors: List[str]
    references: List[str]
    repository_name: str
    repository_url: str
    repository_doi: str
    license: str

    # Data
    data_values: List[str]
    data_units: List[str]
    data_layout: str

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
                    - history: Description of model creation.
                    - comment: Additional comments about model.
                    - version: Model version number.
                    - creator_name: Name of person creating GeoModelGrids model.
                    - creator_email: Email of creator.
                    - creator_institution: Institution of creator.
                    - acknowledgement: Acknowledgement for model.
                    - authors: List of model authors.
                    - references: List of references for model.
                    - repository_name: Name of repository holding model.
                    - repository_url: URL of repository.
                    - repository_doi: Digital Object Identifier for repository.
                    - license: Name of the license for model.
                    - coordsys
                        - crs
                        - origin_x
                        - origin_y
                        - y_azimuth
                    - data
                        - values
                        - units
                        - layout
                    - domain
                        - dim_x
                        - dim_y
                        - dim_z
        """
        self.title = config["geomodelgrids"]["title"]
        self.id = config["geomodelgrids"]["id"]
        self.description = config["geomodelgrids"]["description"]
        self.keywords = string_to_list(config["geomodelgrids"]["keywords"])
        self.history = config["geomodelgrids"]["history"]
        self.comment = config["geomodelgrids"]["comment"]
        self.version = config["geomodelgrids"]["version"]

        self.creator_name = config["geomodelgrids"]["creator_name"]
        self.creator_email = config["geomodelgrids"]["creator_email"]
        self.creator_institution = config["geomodelgrids"]["creator_institution"]
        self.acknowledgement = config["geomodelgrids"]["acknowledgement"]
        self.authors = string_to_list(config["geomodelgrids"]["authors"], delimiter="|")
        self.references = string_to_list(config["geomodelgrids"]["references"], delimiter="|")
        self.repository_name = config["geomodelgrids"]["repository_name"]
        self.repository_url = config["geomodelgrids"]["repository_url"]
        self.repository_doi = config["geomodelgrids"]["repository_doi"]
        self.license = config["geomodelgrids"]["license"]

        # Data
        self.data_values = string_to_list(config["data"]["values"])
        self.data_units = string_to_list(config["data"]["units"])
        if config["data"]["layout"] in ["vertex"]:
            self.data_layout = config["data"]["layout"]
        else:
            raise NotImplementedError("Currently only vertex-based data are implemented.")

        # Coordinate system
        self.crs = config["coordsys"]["crs"]
        self.origin_x = float(config["coordsys"]["origin_x"])
        self.origin_y = float(config["coordsys"]["origin_y"])
        self.y_azimuth = float(config["coordsys"]["y_azimuth"])

        # Domain dimensions
        self.dim_x = float(config["domain"]["dim_x"])
        self.dim_y = float(config["domain"]["dim_y"])
        self.dim_z = float(config["domain"]["dim_z"])

        # Auxiliary data
        if "auxiliary" in config["data"]:
            import json
            self.auxiliary = json.loads(config["data"]["auxiliary"])
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
                Numpy array [Nx,Ny,Nz,Nv] of gridded data associated with block.
            batch (utils.BatchGenerator3D)
                Current batch of points in domain.
        """
        self.storage.save_block(block, values, batch)

    def update_metadata(self):
        """Update all metadata for model using current model configuration.
        """
        self.storage.save_domain(self)
        if self.top_surface:
            self.storage.save_surface_metadata(self.top_surface)
        if self.topo_bathy:
            self.storage.save_surface_metadata(self.topo_bathy)
        for block in self.blocks:
            self.storage.save_block_metadata(block)

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
        if "top_surface" in config:
            self.top_surface = Surface("top_surface", self.metadata, config["top_surface"], self.storage)
        else:
            self.top_surface = None
        if "topography_bathymetry" in config:
            self.topo_bathy = Surface("topography_bathymetry", self.metadata,
                                      config["topography_bathymetry"], self.storage)
        else:
            self.topo_bathy = None

    @staticmethod
    def get_attributes():
        """Get attributes for model.
        """
        return (
            ("title", str),
            ("id", str),
            ("description", str),
            ("keywords", list, str),
            ("history", str),
            ("comment", str),
            ("creator_name", str),
            ("creator_email", str),
            ("creator_institution", str),
            ("acknowledgement", str),
            ("authors", list, str),
            ("references", list, str),
            ("repository_name", str),
            ("repository_url", str),
            ("repository_doi", str),
            ("license", str),
            ("version", str),
            ("data_values", list, str),
            ("data_units", list, str),
            ("data_layout", str),
            ("crs", str),
            ("origin_x", float),
            ("origin_y", float),
            ("y_azimuth", float),
            ("dim_x", float),
            ("dim_y", float),
            ("dim_z", float),
            ("auxiliary", dict),
        )


# End of file
