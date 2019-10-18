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
        logger.info("Running EarthVision command '{}' in directory '{}' and environment {}.".format(
            cmd, self.model_dir, self.env))
        result = subprocess.run(cmd.split(), cwd=self.model_dir, env=self.env, stdout=subprocess.PIPE)
        return result.stdout.decode().split("\n")


    def ev_label(self, filename_values, filename_points, filename_model):
        """Run 'ev_label -m FILENAME_MODEL -o FILENAME_VALUES FILE

        Args:
            filename_values (str)
                Name of file for output values.
            filename_points (str)
                Name of file with input points.
            filename_model (str)
                Name of EarthVision model (.seq) file.
        """
        CONVERTERS = {
            0: float,
            1: float,
            2: float,
            3: lambda s: s.decode("utf-8").strip('"'),
            4: lambda s: s.decode("utf-8").strip('"'),
        }
        DTYPE = {
            "names": ("x", "y", "z", "fault_block", "zone"),
            "formats": ("f4", "f4", "f4", "<U32", "<U32")
        }

        cmd = "ev_label -m {ev_model} -o {filename_out} -suppress VolumeIndex {filename_in}".format(
            filename_in=filename_points, filename_out=filename_values, ev_model=filename_model)
        logger = logging.getLogger(__name__)
        logger.info("Running EarthVision command '{}' in directory '{}' and environment {}.".format(
            cmd, self.model_dir, self.env))
        subprocess.run(cmd.split(), cwd=self.model_dir, env=self.env)
        values_abspath = os.path.join(self.model_dir, filename_values)
        return numpy.loadtxt(values_abspath, delimiter="\t", dtype=DTYPE, converters=CONVERTERS)


    def ev_fp(self, formula, filename_out=None):
        """Run 'ev_fp < {formula}'.

        Args;
            formula (str)
                Formula for EarthVision formula processor.
            filename_out (str)
                Name of output file in formula.
        """
        logger = logging.getLogger(__name__)
        logger.info("Running EarthVision command '{}' with input '{}' in directory '{}' and environment {}.".format(
            "ev_fp", formula, self.model_dir, self.env))
        result = subprocess.run(["ev_fp"], input=formula.encode("utf-8"), cwd=self.model_dir, env=self.env, stdout=subprocess.PIPE)
        return numpy.loadtxt(filename_out) if filename_out else result.stdout


# End of file
