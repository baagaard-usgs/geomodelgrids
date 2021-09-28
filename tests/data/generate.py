#!/usr/bin/env python3

import h5py
import numpy
import json


def calc_top_surface(x, y):
    return 1.5e+2 + 2.0e-5 * x - 1.2e-5 * y + 5.0e-10 * x * y


def calc_topo_bathy(x, y):
    return 1.5e+2 + 2.0e-5 * x - 1.2e-5 * y - 5.0e-10 * x * y


def calc_one(x, y, z):
    return 2.0e+3 + 0.3 * x + 0.4 * y - 4.0 * z


def calc_two(x, y, z):
    return -1.2e+3 + 0.1 * x - 0.2 * y - 4.8 * z


def dict_as_str(value):
    return numpy.string_(json.dumps(value, sort_keys=True))


class TestData:

    MODEL_ATTRS = (
        ("title", str),
        ("id", str),
        ("description", str),
        ("keywords", str),
        ("history", str),
        ("comment", str),
        ("creator_name", str),
        ("creator_email", str),
        ("creator_institution", str),
        ("acknowledgement", str),
        ("authors", str),
        ("references", str),
        ("repository_name", str),
        ("repository_url", str),
        ("repository_doi", str),
        ("license", str),
        ("version", str),
        ("data_values", str),
        ("data_units", str),
        ("data_layout", str),
        ("crs", str),
        ("origin_x", float),
        ("origin_y", float),
        ("y_azimuth", float),
        ("dim_x", float),
        ("dim_y", float),
        ("dim_z", float),
    )
    SURFACE_ATTRS = (
        ("x_resolution", float),  # Only one of x_resolution or x_coordinates is used.
        ("x_coordinates", float),
        ("y_resolution", float),
        ("y_coordinates", float),
    )
    BLOCK_ATTRS = (
        ("x_resolution", float),  # Only one of x_resolution or x_coordinates is used.
        ("x_coordinates", float),
        ("y_resolution", float),
        ("y_coordinates", float),
        ("z_resolution", float),
        ("z_coordinates", float),
        ("z_top", float),
    )

    filename = None
    top_surface = None
    topo_bathy = None
    blocks = []
    model = {}

    @ staticmethod
    def _hdf5_type(value, map_fn):
        if type(value) in [list, tuple]:
            value_h5 = list(map(map_fn, value))
        else:
            value_h5 = map_fn(value)
        return value_h5

    def create(self):
        h5 = h5py.File(self.filename, "w")

        # Model attributes
        attrs = h5.attrs
        for attr_name, map_fn in self.MODEL_ATTRS:
            attrs[attr_name] = self._hdf5_type(self.model[attr_name], map_fn)
        if "auxiliary" in self.model:
            attrs["auxiliary"] = dict_as_str(self.model["auxiliary"])

        # Surfaces
        h5.create_group("surfaces")
        surfaces_group = h5["surfaces"]
        if not self.top_surface is None:
            top_dataset = surfaces_group.create_dataset("top_surface", data=self.top_surface["elevation"],
                                                        chunks=self.top_surface["chunk_size"])
            attrs = top_dataset.attrs
            for attr_name, map_fn in self.SURFACE_ATTRS:
                if attr_name in self.top_surface:
                    attrs[attr_name] = self._hdf5_type(self.top_surface[attr_name], map_fn)
        if not self.topo_bathy is None:
            topobathy_dataset = surfaces_group.create_dataset("topography_bathymetry", data=self.topo_bathy["elevation"],
                                                              chunks=self.topo_bathy["chunk_size"])
            attrs = topobathy_dataset.attrs
            for attr_name, map_fn in self.SURFACE_ATTRS:
                if attr_name in self.topo_bathy:
                    attrs[attr_name] = self._hdf5_type(self.topo_bathy[attr_name], map_fn)

        # Blocks
        h5.create_group("blocks")
        blocks_group = h5["blocks"]
        for block in self.blocks:
            block_dataset = blocks_group.create_dataset(block["name"], data=block["data"],
                                                        chunks=block["chunk_size"])
            attrs = block_dataset.attrs
            for attr_name, map_fn in self.BLOCK_ATTRS:
                if attr_name in block:
                    attrs[attr_name] = self._hdf5_type(block[attr_name], map_fn)
        h5.close()

    @ staticmethod
    def create_groundsurf_xy(model, topography):
        if "x_resolution" in topography:
            dx = topography["x_resolution"]
            x1 = numpy.arange(0.0, model["dim_x"] + 0.5 * dx, dx)
        else:
            x1 = topography["x_coordinates"]

        if "y_resolution" in topography:
            dy = topography["x_resolution"]
            y1 = numpy.arange(0.0, model["dim_y"] + 0.5 * dy, dy)
        else:
            y1 = topography["y_coordinates"]

        x, y = numpy.meshgrid(x1, y1, indexing="ij")
        return (x, y)

    @ staticmethod
    def create_block_xyz(model, block):
        if "x_resolution" in block:
            dx = block["x_resolution"]
            x1 = numpy.arange(0.0, model["dim_x"] + 0.5 * dx, dx)
        else:
            x1 = block["x_coordinates"]

        if "y_resolution" in block:
            dy = block["y_resolution"]
            y1 = numpy.arange(0.0, model["dim_y"] + 0.5 * dy, dy)
        else:
            y1 = block["y_coordinates"]

        if "z_resolution" in block:
            z_top = block["z_top"]
            dz = block["z_resolution"]
            z1 = numpy.arange(z_top, z_top - block["dim_z"] - 0.5 * dz, -dz)
        else:
            z_top = max(block["z_coordinates"])
            z1 = block["z_coordinates"]

        x, y, z = numpy.meshgrid(x1, y1, z1, indexing="ij")
        return (x, y, z)


class OneBlockFlat(TestData):
    filename = "one-block-flat.h5"
    model = {
        "title": "One Block Flat",
        "id": "one-block-flat",
        "description": "Model with one block and no topography.",
        "keywords": ["key one", "key two", "key three"],
        "history": "First version",
        "comment": "One comment",
        "creator_name": "John Doe",
        "creator_institution": "Agency",
        "creator_email": "johndoe@agency.org",
        "acknowledgement": "Thank you!",
        "authors": ["Smith, Jim", "Doe, John", "Doyle, Sarah"],
        "references": ["Reference 1", "Reference 2"],
        "repository_name": "Some repository",
        "repository_url": "http://somewhere.org",
        "repository_doi": "this.is.a.doi",
        "license": "CC0",
        "version": "1.0.0",
        "data_values": ["one", "two"],
        "data_units": ["m", "m/s"],
        "data_layout": "vertex",
        "crs": 'EPSG:26910',
        "origin_x": 590000.0,
        "origin_y": 4150000.0,
        "y_azimuth": 90.0,
        "dim_x": 32.0e+3,
        "dim_y": 40.0e+3,
        "dim_z": 5.0e+3,
        "auxiliary": {0: "zero", 1: "one", },
    }

    top_surface = None
    topo_bathy = None

    blocks = [
        {
            "name": "block",
            "x_resolution": 8.0e+3,
            "y_resolution": 10.0e+3,
            "z_resolution": 5.0e+3,
            "z_top": 0.0e+3,
            "dim_z": 5.0e+3,
            "chunk_size": (1, 1, 2, 2),
        }
    ]
    for block in blocks:
        x, y, z = TestData.create_block_xyz(model, block)
        (nx, ny, nz) = x.shape
        nvalues = len(model["data_values"])
        data = numpy.zeros((nx, ny, nz, nvalues), dtype=numpy.float32)
        data[:, :, :, 0] = calc_one(x, y, z)
        data[:, :, :, 1] = calc_two(x, y, z)
        block["data"] = data


class OneBlockFlatVarZ(TestData):
    filename = "one-block-flat-varz.h5"
    model = {
        "title": "One Block Flat",
        "id": "one-block-flat",
        "description": "Model with one block and no topography.",
        "keywords": ["key one", "key two", "key three"],
        "history": "First version",
        "comment": "One comment",
        "creator_name": "John Doe",
        "creator_institution": "Agency",
        "creator_email": "johndoe@agency.org",
        "acknowledgement": "Thank you!",
        "authors": ["Smith, Jim", "Doe, John", "Doyle, Sarah"],
        "references": ["Reference 1", "Reference 2"],
        "repository_name": "Some repository",
        "repository_url": "http://somewhere.org",
        "repository_doi": "this.is.a.doi",
        "license": "CC0",
        "version": "1.0.0",
        "data_values": ["one", "two"],
        "data_units": ["m", "m/s"],
        "data_layout": "vertex",
        "crs": 'EPSG:26910',
        "origin_x": 590000.0,
        "origin_y": 4150000.0,
        "y_azimuth": 90.0,
        "dim_x": 32.0e+3,
        "dim_y": 40.0e+3,
        "dim_z": 5.0e+3,
        "auxiliary": {0: "zero", 1: "one", },
    }

    top_surface = None
    topo_bathy = None

    blocks = [
        {
            "name": "block",
            "x_resolution": 8.0e+3,
            "y_resolution": 10.0e+3,
            "z_coordinates": [0.0e+3, -2.0e+3, -5.0e+3],
            "chunk_size": (1, 1, 3, 2),
        }
    ]
    for block in blocks:
        x, y, z = TestData.create_block_xyz(model, block)
        (nx, ny, nz) = x.shape
        nvalues = len(model["data_values"])
        data = numpy.zeros((nx, ny, nz, nvalues), dtype=numpy.float32)
        data[:, :, :, 0] = calc_one(x, y, z)
        data[:, :, :, 1] = calc_two(x, y, z)
        block["data"] = data
    blocks[0]["z_coordinates"] = blocks[0]["z_coordinates"][::-1]  # Reverse order for testing


class OneBlockTopo(TestData):
    filename = "one-block-topo.h5"
    model = {
        "title": "One Block Topography",
        "id": "one-block-topo",
        "description": "Model with one block and topography.",
        "keywords": ["key one", "key two", "key three"],
        "history": "First version",
        "comment": "One comment",
        "creator_name": "John Doe",
        "creator_institution": "Agency",
        "creator_email": "johndoe@agency.org",
        "acknowledgement": "Thank you!",
        "authors": ["Smith, Jim", "Doe, John", "Doyle, Sarah"],
        "references": ["Reference 1", "Reference 2"],
        "repository_name": "Some repository",
        "repository_url": "http://somewhere.org",
        "repository_doi": "this.is.a.doi",
        "license": "CC0",
        "version": "2.0.0",
        "data_values": ["one", "two"],
        "data_units": ["m", "m/s"],
        "data_layout": "vertex",
        "crs": 'EPSG:26910',
        "origin_x": 590000.0,
        "origin_y": 4150000.0,
        "y_azimuth": 90.0,
        "dim_x": 30.0e+3,
        "dim_y": 40.0e+3,
        "dim_z": 5.0e+3,
    }

    top_surface = {
        "x_resolution": 10.0e+3,
        "y_resolution": 10.0e+3,
        "chunk_size": (2, 2, 1),
    }
    x, y = TestData.create_groundsurf_xy(model, top_surface)
    (nx, ny) = x.shape
    elevation = numpy.zeros((nx, ny, 1), dtype=numpy.float32)
    elevation[:, :, 0] = calc_top_surface(x, y)
    top_surface["elevation"] = elevation

    topo_bathy = None

    blocks = [
        {
            "name": "block",
            "x_resolution": 10.0e+3,
            "y_resolution": 10.0e+3,
            "z_resolution": 5.0e+3,
            "z_top": 0.0,
            "dim_z": 5.0e+3,
            "chunk_size": (1, 1, 2, 2),
        }
    ]
    for block in blocks:
        x, y, z = TestData.create_block_xyz(model, block)
        (nx, ny, nz) = x.shape
        nvalues = len(model["data_values"])
        data = numpy.zeros((nx, ny, nz, nvalues), dtype=numpy.float32)
        data[:, :, :, 0] = calc_one(x, y, z)
        data[:, :, :, 1] = calc_two(x, y, z)
        block["data"] = data

    def bad_topo_metadata(self):
        self.filename = "one-block-topo-bad-topo.h5"
        self.create()
        with h5py.File(self.filename, "a") as h5:
            h5["surfaces"]["top_surface"].attrs["x_resolution"] *= 0.5
            h5.attrs["data_layout"] = "cell"


class OneBlockTopoVarXY(TestData):
    filename = "one-block-topo-varxy.h5"
    model = {
        "title": "One Block Topography",
        "id": "one-block-topo",
        "description": "Model with one block and topography.",
        "keywords": ["key one", "key two", "key three"],
        "history": "First version",
        "comment": "One comment",
        "creator_name": "John Doe",
        "creator_institution": "Agency",
        "creator_email": "johndoe@agency.org",
        "acknowledgement": "Thank you!",
        "authors": ["Smith, Jim", "Doe, John", "Doyle, Sarah"],
        "references": ["Reference 1", "Reference 2"],
        "repository_name": "Some repository",
        "repository_url": "http://somewhere.org",
        "repository_doi": "this.is.a.doi",
        "license": "CC0",
        "version": "2.0.0",
        "data_values": ["one", "two"],
        "data_units": ["m", "m/s"],
        "data_layout": "vertex",
        "crs": 'EPSG:26910',
        "origin_x": 590000.0,
        "origin_y": 4150000.0,
        "y_azimuth": 90.0,
        "dim_x": 30.0e+3,
        "dim_y": 40.0e+3,
        "dim_z": 5.0e+3,
    }

    top_surface = {
        "x_coordinates": [0.0e+3, 10.0e+3, 20.0e+3, 30.0e+3],
        "y_coordinates": [0.0e+3, 8.0e+3, 24.0e+3, 40.0e+3],
        "chunk_size": (2, 2, 1),
    }
    x, y = TestData.create_groundsurf_xy(model, top_surface)
    (nx, ny) = x.shape
    elevation = numpy.zeros((nx, ny, 1), dtype=numpy.float32)
    elevation[:, :, 0] = calc_top_surface(x, y)
    top_surface["elevation"] = elevation

    topo_bathy = None

    blocks = [
        {
            "name": "block",
            "x_coordinates": [0.0e+3, 10.0e+3, 20.0e+3, 30.0e+3],
            "y_coordinates": [0.0e+3, 8.0e+3, 16.0e+3, 40.0e+3],
            "z_resolution": 5.0e+3,
            "z_top": 0.0,
            "dim_z": 5.0e+3,
            "chunk_size": (1, 1, 2, 2),
        }
    ]
    for block in blocks:
        x, y, z = TestData.create_block_xyz(model, block)
        (nx, ny, nz) = x.shape
        nvalues = len(model["data_values"])
        data = numpy.zeros((nx, ny, nz, nvalues), dtype=numpy.float32)
        data[:, :, :, 0] = calc_one(x, y, z)
        data[:, :, :, 1] = calc_two(x, y, z)
        block["data"] = data

    def bad_topo_metadata(self):
        self.filename = "one-block-topo-bad-topo.h5"
        self.create()
        with h5py.File(self.filename, "a") as h5:
            h5["surfaces"]["top_surface"].attrs["x_coordinates"] *= 0.5

    def bad_topo_coordinates(self):
        self.filename = "one-block-topo-varxy-bad-surf-coords.h5"
        self.create()
        with h5py.File(self.filename, "a") as h5:
            attrs = h5["surfaces"]["top_surface"].attrs
            attrs["x_coordinates"] = [0.0, 10.0e+3, 30.0e+3]
            attrs["y_coordinates"] = [0.0, 1.0, 2.0, 3.0, 40.0e+3]

    def bad_block_coordinates(self):
        self.filename = "one-block-topo-varxy-bad-block-coords.h5"
        self.create()
        with h5py.File(self.filename, "a") as h5:
            attrs = h5["blocks"]["block"].attrs
            attrs["x_coordinates"] = [0.0, 10.0e+3, 30.0e+3]
            attrs["y_coordinates"] = [0.0, 1.0, 2.0, 3.0, 40.0e+3]
            attrs["z_coordinates"] = [0.0]
            del attrs["z_resolution"]


class ThreeBlocksFlat(TestData):
    filename = "three-blocks-flat.h5"
    model = {
        "title": "Three Blocks Flat",
        "id": "three-blocks-flat",
        "description": "Model with three blocks and no topography.",
        "keywords": ["key one", "key two", "key three"],
        "history": "First version",
        "comment": "One comment",
        "creator_name": "John Doe",
        "creator_institution": "Agency",
        "creator_email": "johndoe@agency.org",
        "acknowledgement": "Thank you!",
        "authors": ["Smith, Jim", "Doe, John", "Doyle, Sarah"],
        "references": ["Reference 1", "Reference 2"],
        "repository_name": "Some repository",
        "repository_url": "http://somewhere.org",
        "repository_doi": "this.is.a.doi",
        "license": "CC0",
        "version": "1.0.0",
        "data_values": ["one", "two"],
        "data_units": ["m", "m/s"],
        "data_layout": "vertex",
        "crs": 'EPSG:3311',
        "origin_x": 200000.0,
        "origin_y": -400000.0,
        "y_azimuth": 330.0,
        "dim_x": 60.0e+3,
        "dim_y": 120.0e+3,
        "dim_z": 45.0e+3,
    }

    top_surface = None
    topo_bathy = None

    blocks = [
        {
            "name": "top",
            "x_resolution": 10.0e+3,
            "y_resolution": 15.0e+3,
            "z_resolution": 5.0e+3,
            "z_top": 0.0,
            "dim_z": 5.0e+3,
            "chunk_size": (4, 4, 2, 2),
        },
        {
            "name": "middle",
            "x_resolution": 20.0e+3,
            "y_resolution": 20.0e+3,
            "z_resolution": 10.0e+3,
            "z_top": -5.0e+3,
            "dim_z": 20.0e+3,
            "chunk_size": (2, 2, 3, 2),
        },
        {
            "name": "bottom",
            "x_resolution": 30.0e+3,
            "y_resolution": 40.0e+3,
            "z_resolution": 10.0e+3,
            "z_top": -25.0e+3,
            "dim_z": 20.0e+3,
            "chunk_size": (1, 1, 3, 2),
        },
    ]
    for block in blocks:
        x, y, z = TestData.create_block_xyz(model, block)
        (nx, ny, nz) = x.shape
        nvalues = len(model["data_values"])
        data = numpy.zeros((nx, ny, nz, nvalues), dtype=numpy.float32)
        data[:, :, :, 0] = calc_one(x, y, z)
        data[:, :, :, 1] = calc_two(x, y, z)
        block["data"] = data


class ThreeBlocksTopo(TestData):
    filename = "three-blocks-topo.h5"
    model = {
        "title": "Three Blocks Topo",
        "id": "three-blocks-topo",
        "description": "Model with three blocks and topography.",
        "keywords": ["key one", "key two", "key three"],
        "history": "First version",
        "comment": "One comment",
        "creator_name": "John Doe",
        "creator_institution": "Agency",
        "creator_email": "johndoe@agency.org",
        "acknowledgement": "Thank you!",
        "authors": ["Smith, Jim", "Doe, John", "Doyle, Sarah"],
        "references": ["Reference 1", "Reference 2"],
        "repository_name": "Some repository",
        "repository_url": "http://somewhere.org",
        "repository_doi": "this.is.a.doi",
        "license": "CC0",
        "version": "1.0.0",
        "data_values": ["one", "two"],
        "data_units": ["m", "m/s"],
        "data_layout": "vertex",
        "crs": 'EPSG:3311',
        "origin_x": 200000.0,
        "origin_y": -400000.0,
        "y_azimuth": 330.0,
        "dim_x": 60.0e+3,
        "dim_y": 120.0e+3,
        "dim_z": 45.0e+3,
    }

    top_surface = {
        "x_resolution": 5.0e+3,
        "y_resolution": 5.0e+3,
        "chunk_size": (4, 4, 1),
    }
    x, y = TestData.create_groundsurf_xy(model, top_surface)
    (nx, ny) = x.shape
    elevation = numpy.zeros((nx, ny, 1), dtype=numpy.float32)
    elevation[:, :, 0] = calc_top_surface(x, y)
    top_surface["elevation"] = elevation

    topo_bathy = {
        "x_resolution": 5.0e+3,
        "y_resolution": 5.0e+3,
        "chunk_size": (4, 4, 1),
    }
    x, y = TestData.create_groundsurf_xy(model, topo_bathy)
    (nx, ny) = x.shape
    elevation = numpy.zeros((nx, ny, 1), dtype=numpy.float32)
    elevation[:, :, 0] = calc_topo_bathy(x, y)
    topo_bathy["elevation"] = elevation

    blocks = [
        {
            "name": "top",
            "x_resolution": 10.0e+3,
            "y_resolution": 10.0e+3,
            "z_resolution": 5.0e+3,
            "z_top": 0.0e+3,
            "dim_z": 5.0e+3,
            "chunk_size": (4, 4, 2, 2),
        },
        {
            "name": "middle",
            "x_resolution": 20.0e+3,
            "y_resolution": 20.0e+3,
            "z_resolution": 10.0e+3,
            "z_top": -5.0e+3,
            "dim_z": 20.0e+3,
            "chunk_size": (2, 2, 3, 2),
        },
        {
            "name": "bottom",
            "x_resolution": 30.0e+3,
            "y_resolution": 30.0e+3,
            "z_resolution": 10.0e+3,
            "z_top": -25.0e+3,
            "dim_z": 20.0e+3,
            "chunk_size": (1, 1, 3, 2),
        },
    ]
    for block in blocks:
        x, y, z = TestData.create_block_xyz(model, block)
        (nx, ny, nz) = x.shape
        nvalues = len(model["data_values"])
        data = numpy.zeros((nx, ny, nz, nvalues), dtype=numpy.float32)
        data[:, :, :, 0] = calc_one(x, y, z)
        data[:, :, :, 1] = calc_two(x, y, z)
        block["data"] = data

    def bad_block_metadata(self):
        self.filename = "three-blocks-topo-bad-blocks.h5"
        self.create()
        with h5py.File(self.filename, "a") as h5:
            blocks = h5["blocks"]
            blocks["bottom"].attrs["y_resolution"] *= 0.75
            blocks["middle"].attrs["z_resolution"] -= 5.0e+3
            blocks["middle"].attrs["z_top"] += 10.0
            blocks["top"].attrs["z_top"] -= 4.0

    def missing_metadata(self):
        self.filename = "three-blocks-topo-missing-metadata.h5"
        self.create()
        with h5py.File(self.filename, "a") as h5:
            for attr in h5.attrs:
                del h5.attrs[attr]
            blocks = h5["blocks"]
            for attr in blocks["middle"].attrs:
                del blocks["middle"].attrs[attr]
            del blocks["bottom"].attrs["z_top"]
            top = h5["surfaces"]["top_surface"]
            for attr in top.attrs:
                del top.attrs[attr]

    def inconsistent_units(self):
        self.filename = "three-blocks-topo-inconsistent-units.h5"
        self.create()
        with h5py.File(self.filename, "a") as h5:
            h5.attrs["data_units"] = ["km", "km/s"]


class ThreeBlocksTopoVarXYZ(TestData):
    filename = "three-blocks-topo-varxyz.h5"
    model = {
        "title": "Three Blocks Topo",
        "id": "three-blocks-topo",
        "description": "Model with three blocks and topography.",
        "keywords": ["key one", "key two", "key three"],
        "history": "First version",
        "comment": "One comment",
        "creator_name": "John Doe",
        "creator_institution": "Agency",
        "creator_email": "johndoe@agency.org",
        "acknowledgement": "Thank you!",
        "authors": ["Smith, Jim", "Doe, John", "Doyle, Sarah"],
        "references": ["Reference 1", "Reference 2"],
        "repository_name": "Some repository",
        "repository_url": "http://somewhere.org",
        "repository_doi": "this.is.a.doi",
        "license": "CC0",
        "version": "1.0.0",
        "data_values": ["one", "two"],
        "data_units": ["m", "m/s"],
        "data_layout": "vertex",
        "crs": 'EPSG:3311',
        "origin_x": 200000.0,
        "origin_y": -400000.0,
        "y_azimuth": 330.0,
        "dim_x": 60.0e+3,
        "dim_y": 120.0e+3,
        "dim_z": 45.0e+3,
    }

    top_surface = {
        "x_coordinates": [0.0e+3, 5.0e+3, 10.0e+3, 30.0e+3, 40.0e+3, 60.0e+3],
        "y_coordinates": [0.0e+3, 5.0e+3, 10.0e+3, 20.0e+3, 30.0e+3, 40.0e+3, 60.0e+3, 80.0e+3, 100.0e+3, 120.0e+3],
        "chunk_size": (3, 3, 1),
    }
    x, y = TestData.create_groundsurf_xy(model, top_surface)
    (nx, ny) = x.shape
    elevation = numpy.zeros((nx, ny, 1), dtype=numpy.float32)
    elevation[:, :, 0] = calc_top_surface(x, y)
    top_surface["elevation"] = elevation

    topo_bathy = {
        "x_coordinates": [0.0e+3, 5.0e+3, 10.0e+3, 30.0e+3, 40.0e+3, 60.0e+3],
        "y_coordinates": [0.0e+3, 5.0e+3, 10.0e+3, 20.0e+3, 30.0e+3, 40.0e+3, 60.0e+3, 80.0e+3, 100.0e+3, 120.0e+3],
        "chunk_size": (4, 4, 1),
    }
    x, y = TestData.create_groundsurf_xy(model, topo_bathy)
    (nx, ny) = x.shape
    elevation = numpy.zeros((nx, ny, 1), dtype=numpy.float32)
    elevation[:, :, 0] = calc_topo_bathy(x, y)
    topo_bathy["elevation"] = elevation

    blocks = [
        {
            "name": "top",
            "x_coordinates": [0.0e+3, 5.0e+3, 10.0e+3, 30.0e+3, 40.0e+3, 60.0e+3],
            "y_coordinates": [0.0e+3, 5.0e+3, 10.0e+3, 20.0e+3, 30.0e+3, 40.0e+3, 60.0e+3, 80.0e+3, 100.0e+3, 120.0e+3],
            "z_coordinates": [0.0e+3, -1.0e+3, -3.0e+3, -5.0e+3],
            "chunk_size": (3, 3, 4, 2),
        },
        {
            "name": "middle",
            "x_coordinates": [0.0e+3, 0.0e+3, 30.0e+3, 40.0e+3, 60.0e+3],
            "y_coordinates": [0.0e+3, 10.0e+3, 20.0e+3, 40.0e+3, 60.0e+3, 80.0e+3, 100.0e+3, 120.0e+3],
            "z_coordinates": [-5.0e+3, -10.0e+3, -20.0e+3, -25.0e+3],
            "chunk_size": (2, 2, 4, 2),
        },
        {
            "name": "bottom",
            "x_coordinates": [0.0e+3, 30.0e+3, 60.0e+3],
            "y_coordinates": [0.0e+3, 10.0e+3, 20.0e+3, 40.0e+3, 80.0e+3, 120.0e+3],
            "z_coordinates": [-25.0e+3, -30.0e+3, -45.0e+3],
            "chunk_size": (1, 1, 3, 2),
        },
    ]
    for block in blocks:
        x, y, z = TestData.create_block_xyz(model, block)
        (nx, ny, nz) = x.shape
        nvalues = len(model["data_values"])
        data = numpy.zeros((nx, ny, nz, nvalues), dtype=numpy.float32)
        data[:, :, :, 0] = calc_one(x, y, z)
        data[:, :, :, 1] = calc_two(x, y, z)
        block["data"] = data


# ==============================================================================
if __name__ == "__main__":
    OneBlockFlat().create()
    OneBlockTopo().create()
    ThreeBlocksFlat().create()
    ThreeBlocksTopo().create()

    OneBlockFlatVarZ().create()
    OneBlockTopoVarXY().create()
    OneBlockTopoVarXY().bad_topo_coordinates()
    OneBlockTopoVarXY().bad_block_coordinates()
    ThreeBlocksTopoVarXYZ().create()

    OneBlockTopo().bad_topo_metadata()
    ThreeBlocksTopo().bad_block_metadata()
    ThreeBlocksTopo().missing_metadata()
    ThreeBlocksTopo().inconsistent_units()


# End of file
