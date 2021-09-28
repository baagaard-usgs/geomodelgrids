"""Test create.apps.create_model.App
"""

import unittest
import os

import h5py
import numpy
import json

from geomodelgrids.create.apps.create_model import App
from geomodelgrids.create.core.model import ModelMetadata
from geomodelgrids.create.testing.datasrc import AnalyticDataSrc
from geomodelgrids.create.utils import config


class TestApp(unittest.TestCase):
    CONFIG_FILENAME = "test_createapp.cfg"
    TOLERANCE = 1.0e-6

    def setUp(self):
        def _surface_metadata(sconfig):
            metadata = {}
            if "x_resolution" in sconfig:
                metadata["x_resolution"] = float(sconfig["x_resolution"])
            else:
                metadata["x_coordinates"] = tuple(map(float, config.string_to_list(sconfig["x_coordinates"])))
            if "y_resolution" in sconfig:
                metadata["y_resolution"] = float(sconfig["y_resolution"])
            else:
                metadata["y_coordinates"] = tuple(map(float, config.string_to_list(sconfig["y_coordinates"])))
            return metadata

        def _block_metadata(bconfig):
            metadata = {}
            if "x_resolution" in bconfig:
                metadata["x_resolution"] = float(bconfig["x_resolution"])
            else:
                metadata["x_coordinates"] = tuple(map(float, config.string_to_list(bconfig["x_coordinates"])))
            if "y_resolution" in bconfig:
                metadata["y_resolution"] = float(bconfig["y_resolution"])
            else:
                metadata["y_coordinates"] = tuple(map(float, config.string_to_list(bconfig["y_coordinates"])))
            if "z_resolution" in bconfig:
                metadata["z_resolution"] = float(bconfig["z_resolution"])
                metadata["z_top"] = float(bconfig["z_top"])
                metadata["z_bot"] = float(bconfig["z_bot"])
            else:
                metadata["z_coordinates"] = tuple(map(float, config.string_to_list(bconfig["z_coordinates"])))
                metadata["z_top"] = numpy.max(metadata["z_coordinates"])
            return metadata

        model_config = config.get_config([self.CONFIG_FILENAME])
        auxiliary = {"int_value": 1, "float_value": 2.0, "str_value": "abc"}
        self.metadata = {
            "filename": model_config["geomodelgrids"]["filename"],
            "title": model_config["geomodelgrids"]["title"],
            "id": model_config["geomodelgrids"]["id"],
            "description": model_config["geomodelgrids"]["description"],
            "keywords": config.string_to_list(model_config["geomodelgrids"]["keywords"]),
            "history": model_config["geomodelgrids"]["history"],
            "comment": model_config["geomodelgrids"]["comment"],
            "version": model_config["geomodelgrids"]["version"],
            "creator_name": model_config["geomodelgrids"]["creator_name"],
            "creator_institution": model_config["geomodelgrids"]["creator_institution"],
            "creator_email": model_config["geomodelgrids"]["creator_email"],
            "acknowledgement": model_config["geomodelgrids"]["acknowledgement"],
            "authors": config.string_to_list(model_config["geomodelgrids"]["authors"], delimiter="|"),
            "references": config.string_to_list(model_config["geomodelgrids"]["references"], delimiter="|"),
            "repository_name": model_config["geomodelgrids"]["repository_name"],
            "repository_url": model_config["geomodelgrids"]["repository_url"],
            "repository_doi": model_config["geomodelgrids"]["repository_doi"],
            "license": model_config["geomodelgrids"]["license"],
            "data_values": config.string_to_list(model_config["data"]["values"]),
            "data_units": config.string_to_list(model_config["data"]["units"]),
            "data_layout": model_config["data"]["layout"],
            "crs": model_config["coordsys"]["crs"],
            "origin_x": float(model_config["coordsys"]["origin_x"]),
            "origin_y": float(model_config["coordsys"]["origin_y"]),
            "y_azimuth": float(model_config["coordsys"]["y_azimuth"]),
            "dim_x": float(model_config["domain"]["dim_x"]),
            "dim_y": float(model_config["domain"]["dim_y"]),
            "dim_z": float(model_config["domain"]["dim_z"]),
            "auxiliary": numpy.string_(json.dumps(auxiliary, sort_keys=True)),
            "top_surface": _surface_metadata(model_config["top_surface"]),
            "topography_bathymetry": _surface_metadata(model_config["topography_bathymetry"]),
            "blocks": {
                "top": _block_metadata(model_config["top"]),
                "bottom": _block_metadata(model_config["bottom"]),
            }
        }

    def test_domain(self):
        ARGS = {
            "config": self.CONFIG_FILENAME,
            "show_parameters": False,
            "import_domain": True,
            "import_surfaces": False,
            "import_blocks": False,
            "update_metadata": False,
            "all": False,
            "show_progress": False,
            "log_filename": "test_createapp.log",
            "debug": True,
        }
        app = App()
        app.main(**ARGS)

        h5 = h5py.File(self.metadata["filename"], "r")
        attrs = h5.attrs
        self._check_attributes(ModelMetadata.__dataclass_fields__, self.metadata, attrs)

    def test_surfaces(self):
        ARGS = {
            "config": self.CONFIG_FILENAME,
            "show_parameters": False,
            "import_domain": True,
            "import_surfaces": True,
            "import_blocks": False,
            "update_metadata": False,
            "all": False,
            "show_progress": False,
            "log_filename": "test_createapp.log",
            "debug": True,
        }
        app = App()
        app.main(**ARGS)

        points = self._get_surface_xyz()
        elevationE = AnalyticDataSrc().get_top_surface(points)

        h5 = h5py.File(self.metadata["filename"], "r")
        elevation = h5["surfaces"]["top_surface"][:]

        toleranceV = numpy.maximum(self.TOLERANCE, numpy.abs(elevationE)*self.TOLERANCE)
        valuesOk = numpy.abs(elevationE - elevation) < toleranceV
        if numpy.sum(valuesOk.ravel()) != valuesOk.size:
            msg = (
                "Mismatch in elevation.",
                "Expected values: %s" % elevationE[~valuesOk],
                "Actual values: %s" % elevation[~valuesOk],
            )
            h5.close()
            self.assertEqual(numpy.sum(valuesOk.ravel()), valuesOk.size, msg="\n".join(msg))

        elevationE = AnalyticDataSrc().get_topography_bathymetry(points)
        h5 = h5py.File(self.metadata["filename"], "r")
        elevation = h5["surfaces"]["topography_bathymetry"][:]

        toleranceV = numpy.maximum(self.TOLERANCE, numpy.abs(elevationE)*self.TOLERANCE)
        valuesOk = numpy.abs(elevationE - elevation) < toleranceV
        if numpy.sum(valuesOk.ravel()) != valuesOk.size:
            msg = (
                "Mismatch in elevation.",
                "Expected values: %s" % elevationE[~valuesOk],
                "Actual values: %s" % elevation[~valuesOk],
            )
            h5.close()
            self.assertEqual(numpy.sum(valuesOk.ravel()), valuesOk.size, msg="\n".join(msg))

    def test_blocks(self):
        ARGS = {
            "config": self.CONFIG_FILENAME,
            "show_parameters": False,
            "import_domain": True,
            "import_surfaces": True,
            "import_blocks": True,
            "update_metadata": False,
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
            npts = points.shape
            valuesE = numpy.zeros((npts[0], npts[1], npts[2], 2), dtype=numpy.float32)
            valuesE[:, :, :, 0] = AnalyticDataSrc._get_values_one(points)
            valuesE[:, :, :, 1] = AnalyticDataSrc._get_values_two(points)

            values = h5["blocks"][block][:]

            toleranceV = numpy.maximum(self.TOLERANCE, numpy.abs(valuesE)*self.TOLERANCE)
            valuesOk = numpy.abs(valuesE - values) < toleranceV
            if numpy.sum(valuesOk.ravel()) != valuesOk.size:
                msg = (
                    "Mismatch in data values.",
                    "Expected values: %s" % valuesE[~valuesOk],
                    "Actual values: %s" % values[~valuesOk],
                )
                h5.close()
                self.assertEqual(numpy.sum(valuesOk.ravel()), valuesOk.size, msg="\n".join(msg))

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

    def _get_surface_xyz(self):
        if "x_resolution" in self.metadata["top_surface"]:
            dx = self.metadata["top_surface"]["x_resolution"]
            x1 = self.metadata["origin_x"] + numpy.arange(0.0, self.metadata["dim_x"]+0.5*dx, dx)
        else:
            x1 = self.metadata["origin_x"] + numpy.array(self.metadata["top_surface"]["x_coordinates"])
        if "y_resolution" in self.metadata["top_surface"]:
            dy = self.metadata["top_surface"]["y_resolution"]
            y1 = self.metadata["origin_y"] + numpy.arange(0.0, self.metadata["dim_y"]+0.5*dy, dy)
        else:
            y1 = self.metadata["origin_y"] + numpy.array(self.metadata["top_surface"]["y_coordinates"])

        x, y = numpy.meshgrid(x1, y1, indexing='ij')
        z = numpy.zeros(x.shape)
        xyz = numpy.stack((x, y, z), axis=2)
        return xyz

    def _get_block_xyz(self, block):
        block_metadata = self.metadata["blocks"][block]
        if "x_resolution" in block_metadata:
            dx = block_metadata["x_resolution"]
            x1 = self.metadata["origin_x"] + numpy.arange(0.0, self.metadata["dim_x"]+0.5*dx, dx)
        else:
            x1 = self.metadata["origin_x"] + numpy.array(block_metadata["x_coordinates"])
        if "y_resolution" in block_metadata:
            dy = block_metadata["y_resolution"]
            y1 = self.metadata["origin_y"] + numpy.arange(0.0, self.metadata["dim_y"]+0.5*dy, dy)
        else:
            y1 = self.metadata["origin_y"] + numpy.array(block_metadata["y_coordinates"])

        # Get top surface
        x, y = numpy.meshgrid(x1, y1, indexing='ij')
        z = numpy.zeros(x.shape)
        xyz = numpy.stack((x, y, z), axis=2)
        top_surface = AnalyticDataSrc.get_top_surface(xyz).squeeze()

        # Create domain points
        if "z_resolution" in block_metadata:
            dz = block_metadata["z_resolution"]
            block_dimz = block_metadata["z_top"] - block_metadata["z_bot"]
            z1 = numpy.arange(0.0, block_dimz+0.5*dz, dz)
        else:
            z1 = block_metadata["z_coordinates"]
        x, y, z = numpy.meshgrid(x1, y1, z1, indexing='ij')

        # Adjust domain points to conform to top surface
        domain_top = 0.0
        domain_bot = -self.metadata["dim_z"]
        for iz in range(z.shape[-1]):
            z[:, :, iz] = domain_bot + (top_surface - domain_bot) / \
                (domain_top - domain_bot) * (block_metadata["z_top"] - z[:, :, iz] - domain_bot)

        xyz = numpy.stack((x, y, z), axis=3)
        return xyz


class TestAppBatch(TestApp):
    CONFIG_FILENAME = "test_createapp_batch.cfg"


class TestAppVarZ(TestApp):
    CONFIG_FILENAME = "test_createapp_varz.cfg"


class TestAppVarXYZ(TestApp):
    CONFIG_FILENAME = "test_createapp_varxyz.cfg"

    def test_metadata(self):
        ARGS = {
            "config": self.CONFIG_FILENAME,
            "show_parameters": False,
            "import_domain": False,
            "import_surfaces": False,
            "import_blocks": False,
            "update_metadata": False,
            "all": True,
            "show_progress": False,
            "log_filename": "test_createapp.log",
            "debug": True,
        }
        app = App()
        app.main(**ARGS)
        ARGS.update({
            "config": "test_updatemetadata_varxyz.cfg",
            "all": False,
            "update_metadata": True,
        })
        app.main(**ARGS)

        model_metadata = {
            "references": ["lots"],
            "repository_name": "Yet another repository too",
            "license": "CC0123",
            "data_values": ["one", "three"],
        }
        h5 = h5py.File(self.metadata["filename"], "r")
        self._check_attributes(model_metadata.keys(), model_metadata, h5.attrs)


def test_classes():
    return [TestApp, TestAppBatch, TestAppVarZ, TestAppVarXYZ]


if __name__ == "__main__":
    suite = unittest.TestSuite()
    for cls in test_classes():
        suite.addTest(unittest.makeSuite(cls))
    unittest.TextTestRunner(verbosity=2).run(suite)


# End of file
