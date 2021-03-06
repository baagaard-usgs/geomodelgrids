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
        ("creator_name", str),
        ("creator_institution", str),
        ("creator_email", str),
        ("acknowledgements", str),
        ("authors", str),
        ("references", str),
        ("doi", str),
        ("version", str),
        ("data_values", str),
        ("data_units", str),
        ("crs", str),
        ("origin_x", float),
        ("origin_y", float),
        ("y_azimuth", float),
        ("dim_x", float),
        ("dim_y", float),
        ("dim_z", float),
    )
    TOPOGRAPHY_ATTRS = (
        ("resolution_horiz", float),
    )
    BLOCK_ATTRS = (
        ("resolution_horiz", float),
        ("resolution_vert", float),
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
        # Create fixed-length string attributes
        attrs["keywords"] = [numpy.string_(v) for v in self.model["keywords"]]
        attrs["creator_institution"] = numpy.string_(self.model["creator_institution"])
        if "auxiliary" in self.model:
            attrs["auxiliary"] = dict_as_str(self.model["auxiliary"])

        # Surfaces
        h5.create_group("surfaces")
        surfaces_group = h5["surfaces"]
        if not self.top_surface is None:
            top_dataset = surfaces_group.create_dataset("top_surface", data=self.top_surface["elevation"],
                                                        chunks=self.top_surface["chunk_size"])
            attrs = top_dataset.attrs
            for attr_name, map_fn in self.TOPOGRAPHY_ATTRS:
                attrs[attr_name] = self._hdf5_type(self.top_surface[attr_name], map_fn)
        if not self.topo_bathy is None:
            topobathy_dataset = surfaces_group.create_dataset("topography_bathymetry", data=self.topo_bathy["elevation"],
                                                              chunks=self.topo_bathy["chunk_size"])
            attrs = topobathy_dataset.attrs
            for attr_name, map_fn in self.TOPOGRAPHY_ATTRS:
                attrs[attr_name] = self._hdf5_type(self.topo_bathy[attr_name], map_fn)

        # Blocks
        h5.create_group("blocks")
        blocks_group = h5["blocks"]
        for block in self.blocks:
            block_dataset = blocks_group.create_dataset(block["name"], data=block["data"],
                                                        chunks=block["chunk_size"])
            attrs = block_dataset.attrs
            for attr_name, map_fn in self.BLOCK_ATTRS:
                attrs[attr_name] = self._hdf5_type(block[attr_name], map_fn)
        h5.close()

    @ staticmethod
    def create_groundsurf_xy(model, topography):
        resolution_horiz = topography["resolution_horiz"]

        x1 = numpy.arange(0.0, model["dim_x"] + 0.5 * resolution_horiz, resolution_horiz)
        y1 = numpy.arange(0.0, model["dim_y"] + 0.5 * resolution_horiz, resolution_horiz)
        x, y = numpy.meshgrid(x1, y1, indexing="ij")
        return (x, y)

    @ staticmethod
    def create_block_xyz(model, block):
        resolution_horiz = block["resolution_horiz"]
        resolution_vert = block["resolution_vert"]
        dim_z = block["dim_z"]
        z_top = block["z_top"]

        x1 = numpy.arange(0.0, model["dim_x"] + 0.5 * resolution_horiz, resolution_horiz)
        y1 = numpy.arange(0.0, model["dim_y"] + 0.5 * resolution_horiz, resolution_horiz)
        z1 = numpy.arange(z_top, z_top - dim_z - 0.5 * resolution_vert, -resolution_vert)
        x, y, z = numpy.meshgrid(x1, y1, z1, indexing="ij")
        return (x, y, z)


class OneBlockFlat(TestData):
    filename = "one-block-flat.h5"
    model = {
        "title": "One Block Flat",
        "id": "one-block-flat",
        "description": "Model with one block and no topography.",
        "keywords": ["key one", "key two", "key three"],
        "creator_name": "John Doe",
        "creator_institution": "Agency",
        "creator_email": "johndoe@agency.org",
        "acknowledgements": "Thank you!",
        "authors": ["Smith, Jim", "Doe, John", "Doyle, Sarah"],
        "references": ["Reference 1", "Reference 2"],
        "doi": "this.is.a.doi",
        "version": "1.0.0",
        "data_values": ["one", "two"],
        "data_units": ["m", "m/s"],
        "crs": 'EPSG:26910',
        "origin_x": 590000.0,
        "origin_y": 4150000.0,
        "y_azimuth": 90.0,
        "dim_x": 30.0e+3,
        "dim_y": 40.0e+3,
        "dim_z": 5.0e+3,
        "auxiliary": {0: "zero", 1: "one", },
    }

    top_surface = None
    topo_bathy = None

    blocks = [
        {
            "name": "block",
            "resolution_horiz": 10.0e+3,
            "resolution_vert": 5.0e+3,
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


class OneBlockTopo(TestData):
    filename = "one-block-topo.h5"
    model = {
        "title": "One Block Topography",
        "id": "one-block-topo",
        "description": "Model with one block and topography.",
        "keywords": ["key one", "key two", "key three"],
        "creator_name": "John Doe",
        "creator_institution": "Agency",
        "creator_email": "johndoe@agency.org",
        "acknowledgements": "Thank you!",
        "authors": ["Smith, Jim", "Doe, John", "Doyle, Sarah"],
        "references": ["Reference 1", "Reference 2"],
        "doi": "this.is.a.doi",
        "version": "2.0.0",
        "data_values": ["one", "two"],
        "data_units": ["m", "m/s"],
        "crs": 'EPSG:26910',
        "origin_x": 590000.0,
        "origin_y": 4150000.0,
        "y_azimuth": 90.0,
        "dim_x": 30.0e+3,
        "dim_y": 40.0e+3,
        "dim_z": 5.0e+3,
    }

    top_surface = {
        "resolution_horiz": 10.0e+3,
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
            "resolution_horiz": 10.0e+3,
            "resolution_vert": 5.0e+3,
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
            h5["surfaces"]["top_surface"].attrs["resolution_horiz"] *= 0.5


class ThreeBlocksFlat(TestData):
    filename = "three-blocks-flat.h5"
    model = {
        "title": "Three Blocks Flat",
        "id": "three-blocks-flat",
        "description": "Model with three blocks and no topography.",
        "keywords": ["key one", "key two", "key three"],
        "creator_name": "John Doe",
        "creator_institution": "Agency",
        "creator_email": "johndoe@agency.org",
        "acknowledgements": "Thank you!",
        "authors": ["Smith, Jim", "Doe, John", "Doyle, Sarah"],
        "references": ["Reference 1", "Reference 2"],
        "doi": "this.is.a.doi",
        "version": "1.0.0",
        "data_values": ["one", "two"],
        "data_units": ["m", "m/s"],
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
            "resolution_horiz": 10.0e+3,
            "resolution_vert": 5.0e+3,
            "z_top": 0.0,
            "dim_z": 5.0e+3,
            "chunk_size": (4, 4, 2, 2),
        },
        {
            "name": "middle",
            "resolution_horiz": 20.0e+3,
            "resolution_vert": 10.0e+3,
            "z_top": -5.0e+3,
            "dim_z": 20.0e+3,
            "chunk_size": (2, 2, 3, 2),
        },
        {
            "name": "bottom",
            "resolution_horiz": 30.0e+3,
            "resolution_vert": 10.0e+3,
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
        "creator_name": "John Doe",
        "creator_institution": "Agency",
        "creator_email": "johndoe@agency.org",
        "acknowledgements": "Thank you!",
        "authors": ["Smith, Jim", "Doe, John", "Doyle, Sarah"],
        "references": ["Reference 1", "Reference 2"],
        "doi": "this.is.a.doi",
        "version": "1.0.0",
        "data_values": ["one", "two"],
        "data_units": ["m", "m/s"],
        "crs": 'EPSG:3311',
        "origin_x": 200000.0,
        "origin_y": -400000.0,
        "y_azimuth": 330.0,
        "dim_x": 60.0e+3,
        "dim_y": 120.0e+3,
        "dim_z": 45.0e+3,
    }

    top_surface = {
        "resolution_horiz": 5.0e+3,
        "chunk_size": (4, 4, 1),
    }
    x, y = TestData.create_groundsurf_xy(model, top_surface)
    (nx, ny) = x.shape
    elevation = numpy.zeros((nx, ny, 1), dtype=numpy.float32)
    elevation[:, :, 0] = calc_top_surface(x, y)
    top_surface["elevation"] = elevation

    topo_bathy = {
        "resolution_horiz": 5.0e+3,
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
            "resolution_horiz": 10.0e+3,
            "resolution_vert": 5.0e+3,
            "z_top": 0.0e+3,
            "dim_z": 5.0e+3,
            "chunk_size": (4, 4, 2, 2),
        },
        {
            "name": "middle",
            "resolution_horiz": 20.0e+3,
            "resolution_vert": 10.0e+3,
            "z_top": -5.0e+3,
            "dim_z": 20.0e+3,
            "chunk_size": (2, 2, 3, 2),
        },
        {
            "name": "bottom",
            "resolution_horiz": 30.0e+3,
            "resolution_vert": 10.0e+3,
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
            blocks["bottom"].attrs["resolution_vert"] *= 0.75
            blocks["middle"].attrs["resolution_horiz"] -= 5.0e+5
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
            top = h5["surfaces"]["top_surface"]
            for attr in top.attrs:
                del top.attrs[attr]

    def inconsistent_units(self):
        self.filename = "three-blocks-topo-inconsistent-units.h5"
        self.create()
        with h5py.File(self.filename, "a") as h5:
            h5.attrs["data_units"] = ["km", "km/s"]


# ==============================================================================
if __name__ == "__main__":
    OneBlockFlat().create()
    OneBlockTopo().create()
    ThreeBlocksFlat().create()
    ThreeBlocksTopo().create()
    OneBlockTopo().bad_topo_metadata()
    ThreeBlocksTopo().bad_block_metadata()
    ThreeBlocksTopo().missing_metadata()
    ThreeBlocksTopo().inconsistent_units()


# End of file
