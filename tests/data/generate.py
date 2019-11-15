#!/usr/bin/env python

import numpy
import h5py


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
        ("projection", unicode),
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
            topo_dataset = h5.create_dataset("topography", data=self.topography["elevation"])
            attrs = topo_dataset.attrs
            for attr_name, map_fn in self.TOPOGRAPHY_ATTRS:
                attrs[attr_name] = self._hdf5_type(self.topography[attr_name], map_fn)

        # Blocks
        h5.create_group("blocks")
        blocks_group = h5["blocks"]
        for block in self.blocks:
            block_dataset = blocks_group.create_dataset(block["name"], data=block["data"])
            attrs = block_dataset.attrs
            for attr_name, map_fn in self.BLOCK_ATTRS:
                attrs[attr_name] = self._hdf5_type(block[attr_name], map_fn)
        h5.close()


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
        "doi": ["this.is.a.doi"],
        "version": "1.0.0",
        "data_values": ["one", "two"],
        "data_units": ["m", "m/s"],
        "projection": "",
        "origin_x": 100.0,
        "origin_y": 200.0,
        "y_azimuth": 90.0,
        "dim_x": 30.0,
        "dim_y": 40.0,
        "dim_z": 5.0,
    }

    topography = None

    blocks = [
        {
            "name": "block",
            "data": numpy.array([
                [[[-1.1, -1.2], [-2.1, -2.2], [-3.1, -3.2], [-4.1, -4.2]],
                 [[-5.1, -5.2], [-6.1, -6.2], [-7.1, -7.2], [-8.1, -8.2]],
                    [[-9.1, -9.2], [-10.1, -10.2], [-11.1, -11.2], [-12.1, -12.2]]],
                [[[1.1, 1.2], [2.1, 2.2], [3.1, 3.2], [4.1, 4.2]],
                 [[5.1, 5.2], [6.1, 6.2], [7.1, 7.2], [8.1, 8.2]],
                    [[9.1, 9.2], [10.1, 10.2], [11.1, 11.2], [12.1, 12.2]]],
            ]),
            "resolution_horiz": 10.0,
            "resolution_vert": 5.0,
            "z_top": 0.0,
        }
    ]


if __name__ == "__main__":
    OneBlockFlat().create()


# End of file
