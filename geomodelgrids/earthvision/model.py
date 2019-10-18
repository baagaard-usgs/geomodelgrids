"""Models constructed using EarthVision.
"""

import os
import re
import sys
from importlib import import_module

import numpy

from .. import model
from .. import units
from .. import config
from . import api

class RulesModel(model.Model):
    """EarthVision model constructed from rules applies to fault blocks and zones.
    """

    def __init__(self, config):
        """Constructor."""
        self.model_dir = None
        self.api = None
        self.faultblock_ids = {}
        self.zone_ids = {}
        super().__init__(config)
    
    def initialize(self, config):
        """Initialize model.

        Args:
            config (dict)
                Configuration parameters.
        """
        super().initialize(config)

        self.model_dir = os.path.expanduser(config["earthvision"]["model_dir"])
        ev_env = config["earthvision.environment"]
        self.api = api.EarthVisionAPI(self.model_dir, ev_env)
        self._get_faultblocks_zones()

    def query_values(self, block):
        """Query EarthVision model for values at points.

        Args:
            block (Block)
                Block information.
        """
        POINTS_FILENAME = "block_points.dat" # Must have .dat suffix.
        VALUES_FILENAME = "block_values.dat" # Must have .data suffix.

        points_abspath = os.path.join(self.model_dir, POINTS_FILENAME)
        points = block.generate_points(self)

        scale = units.length_scale(self.config["earthvision"]["xy_units"])
        numpy.savetxt(points_abspath, points.reshape((-1, points.shape[3]))/scale, fmt="%16.8e")

        ev_model = self.config["earthvision"]["geologic_model"]
        data = self.api.ev_label(VALUES_FILENAME, POINTS_FILENAME, ev_model)

        topography_block = block.get_block_elevation(self.topography)
        depth = numpy.zeros(points.shape[:-1])
        for iz in range(depth.shape[-1]):
            depth[:, :, iz] = topography_block - points[:, :, iz, 2]
        depth[:, :, 0] -= block.z_top_offset
        del topography_block

        fn_path = self.config["earthvision"]["rules_fn"].split(".")
        if "rules_pythonpath" in self.config["earthvision"]:
            path = self.config["earthvision"]["rules_pythonpath"]
            if not path in sys.path:
                sys.path.append(path)
        rules_fn = getattr(import_module(".".join(fn_path[:-1])), fn_path[-1])
        
        values = numpy.array([rules_fn(pt["fault_block"], pt["zone"])(pt['x'], pt['y'], pt_depth) for (pt, pt_depth) in zip(data, depth.ravel())])
        del depth

        # Append fault block and zone id to values
        # :KLUDGE: Fault block and zone id are converted from int to float
        faultblock_id = numpy.array([self.faultblock_ids[name] for name in data["fault_block"]])
        zone_id = numpy.array([self.zone_ids[name] for name in data["zone"]])
        values = numpy.hstack((values, faultblock_id.reshape((-1,1)), zone_id.reshape((-1,1))))
        
        values = values.reshape((points.shape[0], points.shape[1], points.shape[2], -1))
        return values

    def query_topography(self, points):
        """Query EarthVision model for elevation of ground surface at points.

        Args:
            points (numpy.array [Nx,Ny,Nz])
                Numpy array with coordinates of points in model coordinates.
        """
        POINTS_FILENAME = "topography_points.dat" # Must have .dat suffix.
        ELEV_FILENAME = "topography_elev.dat" # Must have .data suffix.

        points_abspath = os.path.join(self.model_dir, POINTS_FILENAME)
        elev_abspath = os.path.join(self.model_dir, ELEV_FILENAME)
        scale = units.length_scale(self.config["earthvision"]["xy_units"])

        numpy.savetxt(points_abspath, points.reshape((-1, points.shape[2]))/scale, fmt="%16.8e")

        elev = -1.0e+20 * numpy.ones(points.shape[0:2])
        for grd_filename in config.string_to_list(self.config["earthvision"]["topography_2grd"]):
            formula = "{filename_out}<elev> = bakint({ev2grd}, {filename_in}<x>, {filename_in}<y>);".format(
                filename_in=POINTS_FILENAME, filename_out=ELEV_FILENAME, ev2grd=grd_filename)
            elev_grd = self.api.ev_fp(formula, elev_abspath).reshape(points.shape[0:2])
            elev_grd *= units.length_scale(self.config["earthvision"]["elev_units"])
            elev = numpy.maximum(elev_grd, elev)
        self.topography.set_elevation(elev)

        os.remove(points_abspath)
        os.remove(elev_abspath)

    def _get_faultblocks_zones(self):
        """Get fault block and zone ids from model using ev_facedump.
        """
        FAULTBLOCK_HEADING = "Fault Block Names"
        ZONE_HEADING = "Zone Names"
        NAME_PATTERN = r"^\[([0-9]+)\]\s([\S]+.+)"

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
                self.faultblock_ids = _read_names(lines, iline+1)
            if line.startswith(ZONE_HEADING):
                self.zone_ids = _read_names(lines, iline+1)


# End of file
