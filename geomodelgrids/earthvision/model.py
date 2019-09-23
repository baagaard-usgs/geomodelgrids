"""Models constructed using EarthVision.
"""

import os
import re

import numpy

import geomodelgrids.model
import geomodelgrids.units
import api

class RulesModel(geomodelgrids.model.Model):
    """EarthVision model constructed from rules applies to fault blocks and zones.
    """

    def __init__(self):
        """Constructor."""
        self.model_dir = None
        self.api = None
        self.faultblock_ids = {}
        self.zone_ids = {}
    
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

    def query_values(self, points):
        """Query EarthVision model for values at points.

        Args:
            points (numpy.array [Nx,Ny,Nz])
                Numpy array with coordinates of points in model coordinates.
        """
        POINTS_FILENAME = "block_points.dat" # Must have .dat suffix.
        VALUES_FILENAME = "block_values.dat" # Must have .data suffix.

        points_abspath = os.path.join(self.model_dir, POINTS_FILENAME)

        scale = units.length_scale(self.config["earthvision"]["xy_units"])
        numpy.savetxt(points_abspath, points.reshape((-1, points.shape[2]))/scale, fmt="%16.8e")

        ev_model = self.config["earthvision"]["geologic_model"]
        data = self.api.ev_label(VALUES_FILENAME, POINTS_FILENAME, ev_model)

        faultblock_id = numpy.array([self.faultblock_ids[name] for name in data["fault_block"]])
        zone_id = numpy.array([self.zone_ids[name] for name in data["zone"]])
        z_depth = self.topography.elevation - data["z"]

        values = numpy.array([self.rules[(fb_id, z_id)](pt['x'], pt['y'], pt_depth) for (pt, fb_id, z_id, pt_depth) in zip(data, faultblock_id, zone_id, z_depth)])
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

        topo_2grd = self.config["earthvision"]["topography_2grd"]
        formula = "{filename_out}<elev> = bakint({ev2grd}, {filename_in}<x>, {filename_in}<y>);".format(
            filename_in=POINTS_FILENAME, filename_out=ELEV_FILENAME, ev2grd=topo_2grd)
        elev = self.api.ev_fp(formula, elev_abspath).reshape(points.shape[0:2])
        elev *= units.length_scale(self.config["earthvision"]["elev_units"])
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
                    id_mapping[groups[1]] = groups[0]
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
