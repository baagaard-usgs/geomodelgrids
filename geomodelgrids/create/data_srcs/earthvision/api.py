"""API for running some specific EarthVision programs.
"""

import os
import subprocess
import logging

import numpy


class EarthVisionAPI():
    """API to EarthVision programs.
    """

    def __init__(self, model_dir, env):
        """
        """
        self.model_dir = model_dir
        self.env = env

    def ev_facedump(self, filename_faces):
        """Run 'ev_facedump {filename_faces}'.

        Args:
        filename_faces (str)
            Name of faces file for model.
        """
        cmd = "ev_facedump {ev_faces}".format(ev_faces=filename_faces)
        logger = logging.getLogger(__name__)
        logger.info("Running EarthVision command '%s' in directory '%s' and environment %s.",
                    cmd, self.model_dir, self.env)
        result = subprocess.run(cmd.split(), cwd=self.model_dir, env=self.env, stdout=subprocess.PIPE, check=True)
        return result.stdout.decode().split("\n")

    def ev_label(self, filename_values, filename_points, filename_model, dtype, converters):
        """Run 'ev_label -m FILENAME_MODEL -o FILENAME_VALUES FILE'.

        Args:
            filename_values (str)
                Name of file for output values.
            filename_points (str)
                Name of file with input points.
            filename_model (str)
                Name of EarthVision model (.seq) file.
            dtype (dict)
                Dictionary containing types for output.
            converters (dict)
                Dictionary for converting output to dtype.
        """
        cmd = "ev_label -m {ev_model} -o {filename_out} -suppress VolumeIndex {filename_in}".format(
            filename_in=filename_points, filename_out=filename_values, ev_model=filename_model)
        logger = logging.getLogger(__name__)
        logger.info("Running EarthVision command '%s' in directory '%s' and environment %s.",
                    cmd, self.model_dir, self.env)
        subprocess.run(cmd.split(), cwd=self.model_dir, env=self.env, check=True)
        values_abspath = os.path.join(self.model_dir, filename_values)
        return numpy.loadtxt(values_abspath, delimiter="\t", dtype=dtype, converters=converters)

    def ev_fp(self, formula, filename_out=None):
        """Run 'ev_fp < {formula}'.

        Args;
            formula (str)
                Formula for EarthVision formula processor.
            filename_out (str)
                Name of output file in formula.
        """
        logger = logging.getLogger(__name__)
        logger.info("Running EarthVision command '%s' with input '%s' in directory '%s' and environment %s.",
                    "ev_fp", formula, self.model_dir, self.env)
        result = subprocess.run(["ev_fp"], input=formula.encode(
            "utf-8"), cwd=self.model_dir, env=self.env, stdout=subprocess.PIPE, check=True)
        return numpy.loadtxt(filename_out) if filename_out else result.stdout


# End of file
