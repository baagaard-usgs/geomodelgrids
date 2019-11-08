#!/usr/bin/env python

import numpy
import h5py

class TestData:

    MODEL_ATTRS = (
        "title",
        "id",
        "description",
        "keywords",
        "creator_name",
        "creator_institution",
        "creator_email",
        "acknowledgments",
        "authors",
        "references",
        "doi",
        "version",
        "data_values",
        "data_units",
        "projection",
        "origin_x",
        "origin_y",
        "y_azimuth",
        "dim_x",
        "dim_y",
        "dim_z",
    )
    TOPOGRAPHY_ATTRS = (
        "resolution_horiz",
    )
    BLOCK_ATTRS = (
        "resolution_horiz",
        "resolution_vert",
        "z_top",
    )
    
    def create(self):
        h5 = h5py.File(self.filename, "w")

        # Model attributes
        attrs = h5.attrs
        for attr in self.MODEL_ATTRS:
            attrs[attr] = self.model[attr]

        # Topography
        if not self.topography is None:
            topo_dataset = h5.create_dataset("topography", data=self.topography["elevation"])
            attrs = topo_dataset.attrs
            for attr in self.TOPOGRAPHY_ATTRS:
                attrs[attr] = self.topography[attr]

        # Blocks
        h5.create_group("blocks")
        blocks_group = h5["blocks"]
        for block in self.blocks:
            block_dataset = blocks_group.create_dataset(block["name"], data=block["data"])
            attrs = block_dataset.attrs
            for attr in self.BLOCK_ATTRS:
                attrs[attr] = block[attr]
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
