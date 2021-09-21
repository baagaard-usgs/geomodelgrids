"""EarthVision data sources.
"""

import os
import re
import sys
from importlib import import_module

import numpy

from geomodelgrids.create.core.datasrc import DataSrc
from geomodelgrids.create.utils import units
from geomodelgrids.create.utils.config import string_to_list
from geomodelgrids.create.data_srcs.earthvision import api
from geomodelgrids.create.core import NODATA_VALUE


class RulesDataSrc(DataSrc):
    """EarthVision model constructed from rules applies to fault blocks and zones.
    """
    @staticmethod
    def query_rule(rules_fn, data, depth):
        return rules_fn(data[3], data[4])(data[0], data[1], depth)

    @staticmethod
    def set_faultblock_id(faultblock_ids, name):
        return faultblock_ids[name]

    @staticmethod
    def set_zone_id(zone_ids, name):
        return zone_ids[name]

    def __init__(self, config):
        """Constructor.

        Args:
            config (dict)
                Configuration parameters.
        """
        self.config = config
        self.model_dir = None
        self.api = None
        self.faultblock_ids = {}
        self.zone_ids = {}
        super().__init__()

    def initialize(self):
        """Initialize model.
        """
        self.model_dir = os.path.expanduser(self.config["earthvision"]["model_dir"])
        ev_env = self.config["earthvision.environment"]
        self.api = api.EarthVisionAPI(self.model_dir, ev_env)
        self._get_faultblocks_zones()
        self.config["auxiliary"] = {
            "fault_block_ids": self.faultblock_ids,
            "zone_ids": self.zone_ids,
        }

    def get_top_surface(self, points):
        """Query EarthVision model for elevation of top surface at points.

        Args:
            points (numpy.array [Nx,Ny])
                Numpy array with coordinates of points in model coordinates.
        Returns:
            Numpy array [Nx,Ny] of elevation of top surface at points.
        """
        POINTS_FILENAME = "top_surface_points.dat"  # Must have .dat suffix.
        ELEV_FILENAME = "top_surface_elev.dat"  # Must have .dat suffix.

        points_abspath = os.path.join(self.model_dir, POINTS_FILENAME)
        elev_abspath = os.path.join(self.model_dir, ELEV_FILENAME)
        scale = units.length_scale(self.config["earthvision"]["xy_units"])

        numpy.savetxt(points_abspath, points.reshape((-1, points.shape[2])) / scale, fmt="%16.8e")

        elev = -1.0e+20 * numpy.ones(points.shape[0:2])
        for grd_filename in string_to_list(self.config["earthvision"]["top_surface_2grd"]):
            formula = "{filename_out}<elev> = bakint({ev2grd}, {filename_in}<x>, {filename_in}<y>);".format(
                filename_in=POINTS_FILENAME, filename_out=ELEV_FILENAME, ev2grd=grd_filename)
            elev_grd = self.api.ev_fp(formula, elev_abspath).reshape(points.shape[0:2])
            elev_grd *= units.length_scale(self.config["earthvision"]["elev_units"])
            elev = numpy.maximum(elev_grd, elev)

        os.remove(points_abspath)
        os.remove(elev_abspath)
        return elev.reshape((elev.shape[0], elev.shape[1], 1))

    def get_topography_bathymetry(self, points):
        """Query EarthVision model for elevation of topoggraphy or bathymetry at points.

        Args:
            points (numpy.array [Nx,Ny])
                Numpy array with coordinates of points in model coordinates.
        Returns:
            Numpy array [Nx,Ny] of elevation of topography or bathymetry at points.
        """
        POINTS_FILENAME = "topography_bathymetry_points.dat"  # Must have .dat suffix.
        ELEV_FILENAME = "topography_bathymetry_elev.dat"  # Must have .dat suffix.

        points_abspath = os.path.join(self.model_dir, POINTS_FILENAME)
        elev_abspath = os.path.join(self.model_dir, ELEV_FILENAME)
        scale = units.length_scale(self.config["earthvision"]["xy_units"])

        numpy.savetxt(points_abspath, points.reshape((-1, points.shape[2])) / scale, fmt="%16.8e")

        elev = -1.0e+20 * numpy.ones(points.shape[0:2])
        for grd_filename in string_to_list(self.config["earthvision"]["topography_bathymetry_2grd"]):
            formula = "{filename_out}<elev> = bakint({ev2grd}, {filename_in}<x>, {filename_in}<y>);".format(
                filename_in=POINTS_FILENAME, filename_out=ELEV_FILENAME, ev2grd=grd_filename)
            elev_grd = self.api.ev_fp(formula, elev_abspath).reshape(points.shape[0:2])
            elev_grd *= units.length_scale(self.config["earthvision"]["elev_units"])
            elev = numpy.maximum(elev_grd, elev)

        os.remove(points_abspath)
        os.remove(elev_abspath)
        return elev.reshape((elev.shape[0], elev.shape[1], 1))

    def get_values(self, block, top_surface, topo_bathy, batch=None):
        """Query EarthVision model for values at points.

        Args:
            block (Block)
                Block information.
            top_surface (Surface)
                Elevation of top surface of model.
            topo_bathy (Surface)
                Elevation of topography or bathymetry used to define depth.
            batch (BatchGenerator3D)
                Current batch of points in block.
        """
        POINTS_FILENAME = "block_points.dat"  # Must have .dat suffix.
        VALUES_FILENAME = "block_values.dat"  # Must have .data suffix.
        DTYPE = {
            "names": ("x", "y", "z", "fault_block", "zone"),
            "formats": ("f4", "f4", "f4", "<U32", "<U32")
        }
        hscale = units.length_scale(self.config["earthvision"]["xy_units"])
        vscale = units.length_scale(self.config["earthvision"]["elev_units"])
        converters = {
            0: lambda s: hscale*float(s),
            1: lambda s: hscale*float(s),
            2: lambda s: vscale*float(s),
            3: lambda s: s.decode("utf-8").strip('"'),
            4: lambda s: s.decode("utf-8").strip('"'),
        }

        points_abspath = os.path.join(self.model_dir, POINTS_FILENAME)
        points = block.generate_points(top_surface, batch)

        ev_points = points.reshape((-1, points.shape[3])).copy()
        ev_points[:, 0:2] /= hscale
        ev_points[:, 2] /= vscale
        numpy.savetxt(points_abspath, ev_points, fmt="%16.8e")
        del ev_points

        ev_model = self.config["earthvision"]["geologic_model"]
        data = self.api.ev_label(VALUES_FILENAME, POINTS_FILENAME, ev_model, DTYPE, converters)

        topo_bathy_elev = block.get_surface(topo_bathy, batch)
        depth = numpy.zeros(points.shape[:-1])
        for iz in range(depth.shape[-1]):
            depth[:, :, iz] = topo_bathy_elev - points[:, :, iz, 2]
        depth[:, :, 0] -= block.z_top_offset

        fn_path = self.config["earthvision"]["rules_fn"].split(".")
        if "rules_pythonpath" in self.config["earthvision"]:
            path = self.config["earthvision"]["rules_pythonpath"]
            if not path in sys.path:
                sys.path.append(path)
        rules_fn = getattr(import_module(".".join(fn_path[:-1])), fn_path[-1])
        rules_values = numpy.vectorize(self.query_rule)(rules_fn, data, depth.ravel())
        del depth

        # Append fault block and zone id to values
        # :KLUDGE: Fault block and zone id are converted from int to float
        faultblock_id = numpy.vectorize(self.set_faultblock_id)(self.faultblock_ids, data["fault_block"])
        zone_id = numpy.vectorize(self.set_zone_id)(self.zone_ids, data["zone"])
        values = numpy.vstack([v for v in rules_values] + [faultblock_id.reshape((1, -1)),
                                                           zone_id.reshape((1, -1))]).transpose()
        values = values.reshape((points.shape[0], points.shape[1], points.shape[2], -1))

        data_abspath = os.path.join(self.model_dir, VALUES_FILENAME)
        os.remove(points_abspath)
        os.remove(data_abspath)
        return values

    def _get_faultblocks_zones(self):
        """Get fault block and zone ids from model using ev_facedump.
        """
        FAULTBLOCK_HEADING = "Fault Block Names"
        ZONE_HEADING = "Zone Names"
        NAME_PATTERN = r"^\[([0-9]+)\]\s([\S]+.*)"

        def _read_names(lines, start):
            id_mapping = {}
            for line in lines[start:]:
                match = re.search(NAME_PATTERN, line.strip())
                if match and len(match.groups()) == 2:
                    groups = match.groups()
                    id_mapping[groups[1]] = int(groups[0])
                else:
                    break
            return id_mapping

        ev_faces = self.config["earthvision"]["geologic_model"].replace(".seq", ".faces")
        lines = self.api.ev_facedump(ev_faces)
        for iline, line in enumerate(lines):
            if line.startswith(FAULTBLOCK_HEADING):
                self.faultblock_ids = _read_names(lines, iline + 1)
            if line.startswith(ZONE_HEADING):
                self.zone_ids = _read_names(lines, iline + 1)
        if not "" in self.faultblock_ids:
            self.faultblock_ids[""] = NODATA_VALUE
        if not "" in self.zone_ids:
            self.zone_ids[""] = NODATA_VALUE


# End of file
