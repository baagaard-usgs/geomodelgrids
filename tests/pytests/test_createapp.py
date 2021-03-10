"""Test create.apps.create_model.App
"""

import unittest
import os

import h5py
import numpy

from geomodelgrids.create.apps.create_model import App
from geomodelgrids.create.core.model import ModelMetadata
from geomodelgrids.create.testing.datasrc import AnalyticDataSrc
from geomodelgrids.create.utils import config


class TestApp(unittest.TestCase):
    CONFIG_FILENAME = "test_createapp.cfg"

    def setUp(self):
        model_config = config.get_config([self.CONFIG_FILENAME])
        self.metadata = {
            "filename": model_config["geomodelgrids"]["filename"],
            "title": model_config["geomodelgrids"]["title"],
            "id": model_config["geomodelgrids"]["id"],
            "description": model_config["geomodelgrids"]["description"],
            "keywords": config.string_to_list(model_config["geomodelgrids"]["keywords"]),
            "version": model_config["geomodelgrids"]["version"],
            "creator_name": model_config["geomodelgrids"]["creator_name"],
            "creator_institution": model_config["geomodelgrids"]["creator_institution"],
            "creator_email": model_config["geomodelgrids"]["creator_email"],
            "acknowledgements": model_config["geomodelgrids"]["acknowledgements"],
            "authors": config.string_to_list(model_config["geomodelgrids"]["authors"], delimiter="|"),
            "references": config.string_to_list(model_config["geomodelgrids"]["references"], delimiter="|"),
            "doi": model_config["geomodelgrids"]["doi"],
            "data_values": config.string_to_list(model_config["data"]["values"]),
            "data_units": config.string_to_list(model_config["data"]["units"]),
            "crs": model_config["coordsys"]["crs"],
            "origin_x": float(model_config["coordsys"]["origin_x"]),
            "origin_y": float(model_config["coordsys"]["origin_y"]),
            "y_azimuth": float(model_config["coordsys"]["y_azimuth"]),
            "dim_x": float(model_config["domain"]["dim_x"]),
            "dim_y": float(model_config["domain"]["dim_y"]),
            "dim_z": float(model_config["domain"]["dim_z"]),
            "topography": {
                "resolution_horiz": float(model_config["topography"]["resolution_horiz"])
            },
            "blocks": {
                "top": {
                    "resolution_horiz": float(model_config["top"]["resolution_horiz"]),
                    "resolution_vert": float(model_config["top"]["resolution_vert"]),
                    "z_top": float(model_config["top"]["z_top"]),
                    "z_bot": float(model_config["top"]["z_bot"]),
                },
                "bottom": {
                    "resolution_horiz": float(model_config["bottom"]["resolution_horiz"]),
                    "resolution_vert": float(model_config["bottom"]["resolution_vert"]),
                    "z_top": float(model_config["bottom"]["z_top"]),
                    "z_bot": float(model_config["bottom"]["z_bot"]),
                }
            }
        }

    def test_domain(self):
        ARGS = {
            "config": self.CONFIG_FILENAME,
            "show_parameters": False,
            "import_domain": True,
            "import_topography": False,
            "import_blocks": False,
            "all": False,
            "show_progress": False,
            "log_filename": "test_createapp.log",
            "debug": True,
        }
        app = App()
        app.main(**ARGS)

        h5 = h5py.File(self.metadata["filename"], "r")
        attrs = h5.attrs
        self._check_attributes(ModelMetadata.__slots__, self.metadata, attrs)

    def test_topography(self):
        ARGS = {
            "config": self.CONFIG_FILENAME,
            "show_parameters": False,
            "import_domain": False,
            "import_topography": True,
            "import_blocks": False,
            "all": False,
            "show_progress": False,
            "log_filename": "test_createapp.log",
            "debug": True,
        }
        app = App()
        app.main(**ARGS)

        points = self._get_topography_xyz()
        topographyE = AnalyticDataSrc().get_topography(points)

        h5 = h5py.File(self.metadata["filename"], "r")
        topography = h5["topography"]

        toleranceV = numpy.maximum(1.0e-6, topographyE)
        valuesOk = numpy.abs(topographyE - topography) < toleranceV
        if numpy.sum(valuesOk.ravel()) != valuesOk.size:
            print("Mismatch in topography.")
            print("Expected values", topographyE[~valuesOk])
            print("Actual values", topographyE[~valuesOk])
            self.assertEqual(numpy.sum(valuesOk.ravel()), valuesOk.size())

    def test_blocks(self):
        ARGS = {
            "config": self.CONFIG_FILENAME,
            "show_parameters": False,
            "import_domain": False,
            "import_topography": False,
            "import_blocks": True,
            "all": False,
            "show_progress": False,
            "log_filename": "test_createapp.log",
            "debug": True,
        }
        app = App()
        app.main(**ARGS)

        h5 = h5py.File(self.metadata["filename"], "r")
        for block in self.metadata["blocks"]:
            points = self._get_block_xyz(block)
            valuesE = AnalyticDataSrc().get_values(points)

            values = h5["blocks"][block]

            toleranceV = numpy.maximum(1.0e-6, valuesE)
            valuesOk = numpy.abs(valuesE - values) < toleranceV
            if numpy.sum(valuesOk.ravel()) != valuesOk.size:
                print("Mismatch in topography.")
                print("Expected values", valuesE[~valuesOk])
                print("Actual values", valuesE[~valuesOk])
                self.assertEqual(numpy.sum(valuesOk.ravel()), valuesOk.size())

    def _check_attributes(self, names, attrsE, attrs):
        for attr in names:
            msg = f"Mismatch for attribute '{attr}'."
            if isinstance(attrsE[attr], list):
                attr_listE = attrsE[attr]
                attr_list = attrs.get(attr)
                self.assertEqual(len(attr_listE), len(attr_list), f"Mismatch in length of attribute list '{attr}'.")
                for itemE, item in zip(attr_listE, attr_list):
                    if isinstance(item, bytes):
                        self.assertEqual(itemE, item.decode("utf-8"), msg=msg)
                    else:
                        self.assertEqual(itemE, item, msg=msg)
            else:
                self.assertEqual(attrsE[attr], attrs.get(attr), msg=msg)

    def _get_topography_xyz(self):
        dx = self.metadata["topography"]["resolution_horiz"]
        x1 = self.metadata["origin_x"] + numpy.arange(0.0, self.metadata["dim_x"]+0.5*dx, dx)
        y1 = self.metadata["origin_y"] + numpy.arange(0.0, self.metadata["dim_y"]+0.5*dx, dx)
        x, y = numpy.meshgrid(x1, y1, indexing='ij')
        z = numpy.zeros(x.shape)
        xyz = numpy.stack((x, y, z), axis=2)
        return xyz


def test_classes():
    return [TestApp]


if __name__ == "__main__":
    suite = unittest.TestSuite()
    for cls in test_classes():
        suite.addTest(unittest.makeSuite(cls))
    unittest.TextTestRunner(verbosity=2).run(suite)


# End of file
