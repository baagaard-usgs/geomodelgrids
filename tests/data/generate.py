#!/usr/bin/env python

import h5py
import numpy


class TestData:

    MODEL_ATTRS = (
        ("title", unicode),
        ("id", unicode),
        ("description", unicode),
        ("keywords", unicode),
        ("creator_name", unicode),
        ("creator_institution", unicode),
        ("creator_email", unicode),
        ("acknowledgments", unicode),
        ("authors", unicode),
        ("references", unicode),
        ("doi", unicode),
        ("version", unicode),
        ("data_values", unicode),
        ("data_units", unicode),
        ("crs", unicode),
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

    @staticmethod
    def _hdf5_type(value, map_fn):
        if type(value) in [list, tuple]:
            value_h5 = map(map_fn, value)
        else:
            value_h5 = map_fn(value)
        return value_h5

    def create(self):
        h5 = h5py.File(self.filename, "w")

        # Model attributes
        attrs = h5.attrs
        for attr_name, map_fn in self.MODEL_ATTRS:
            attrs[attr_name] = self._hdf5_type(self.model[attr_name], map_fn)

        # Topography
        if not self.topography is None:
            topo_dataset = h5.create_dataset("topography", data=self.topography["elevation"],
                                             chunks=self.topography["chunk_size"])
            attrs = topo_dataset.attrs
            for attr_name, map_fn in self.TOPOGRAPHY_ATTRS:
                attrs[attr_name] = self._hdf5_type(self.topography[attr_name], map_fn)

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

    @staticmethod
    def create_groundsurf_xy(model, topography):
        resolution_horiz = topography["resolution_horiz"]

        x1 = numpy.arange(0.0, model["dim_x"] + 0.5 * resolution_horiz, resolution_horiz)
        y1 = numpy.arange(0.0, model["dim_y"] + 0.5 * resolution_horiz, resolution_horiz)
        x, y = numpy.meshgrid(x1, y1, indexing="ij")
        return (x, y)

    @staticmethod
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
        "acknowledgments": "Thank you!",
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
    }

    topography = None

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
        data[:, :, :, 0] = 2.0e+3 + 1.0 * x + 0.4 * y - 0.5 * z
        data[:, :, :, 1] = -1.2e+3 + 2.1 * x - 0.9 * y + 0.3 * z
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
        "acknowledgments": "Thank you!",
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

    topography = {
        "resolution_horiz": 10.0e+3,
        "chunk_size": (2, 2, 1),
    }
    x, y = TestData.create_groundsurf_xy(model, topography)
    (nx, ny) = x.shape
    elevation = numpy.zeros((nx, ny, 1), dtype=numpy.float32)
    elevation[:, :, 0] = 1.5e+2 + 2.0e-5 * x - 1.2e-5 * y + 5.0e-10 * x * y
    topography["elevation"] = elevation

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
        data[:, :, :, 0] = 2.0e+3 + 1.0 * x + 0.4 * y - 0.5 * z
        data[:, :, :, 1] = -1.2e+3 + 2.1 * x - 0.9 * y + 0.3 * z
        block["data"] = data


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
        "acknowledgments": "Thank you!",
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

    topography = None

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
        data[:, :, :, 0] = 2.0e+3 + 1.0 * x + 0.4 * y - 0.5 * z
        data[:, :, :, 1] = -1.2e+3 + 2.1 * x - 0.9 * y + 0.3 * z
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
        "acknowledgments": "Thank you!",
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

    topography = {
        "resolution_horiz": 5.0e+3,
        "chunk_size": (4, 4, 1),
    }
    x, y = TestData.create_groundsurf_xy(model, topography)
    (nx, ny) = x.shape
    elevation = numpy.zeros((nx, ny, 1), dtype=numpy.float32)
    elevation[:, :, 0] = 1.5e+2 + 2.0e-5 * x - 1.2e-5 * y + 5.0e-10 * x * y
    topography["elevation"] = elevation

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
        data[:, :, :, 0] = 2.0e+3 + 1.0 * x + 0.4 * y - 0.5 * z
        data[:, :, :, 1] = -1.2e+3 + 2.1 * x - 0.9 * y + 0.3 * z
        block["data"] = data


# ==============================================================================
if __name__ == "__main__":
    OneBlockFlat().create()
    OneBlockTopo().create()
    ThreeBlocksFlat().create()
    ThreeBlocksTopo().create()


# End of file
