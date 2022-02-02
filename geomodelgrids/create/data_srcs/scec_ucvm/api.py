"""API for running some UCVM query program.
"""

import os
import subprocess
import logging

import numpy


class UCVMAPI():
    """API to UCVM query program.
    """

    def __init__(self, filename_conf, env):
        """
        """
        self.filename_conf = filename_conf
        self.env = env

    def ucvm_query(self, points, models, dtype, converters, zrange=None):
        """Run 'ucvm_query -m MODELS -f FILENAME_CONF [-z zrange]'.

        Args:
        points (numpy.array)
            Coordinates (lon, lat, depth) of points to query.
        models (str)
            Comma delimited list of model names.
        zrange (tuple)
            Tuple with minimum and maximum depth range for interpolating between GTL and crustal model.
        """
        FILENAME_IN = "block_points.txt"
        FILENAME_OUT = "block_values.txt"

        cmd = ["ucvm_query", "-m", models, "-f", self.filename_conf]
        if zrange:
            cmd += ["-z", f"{zrange[0]:.6e},{zrange[1]:.6e}"]
        cmdstr = " ".join(cmd)
        logger = logging.getLogger(__name__)
        logger.info(f"Running UCVM command '{cmdstr}' with environment {self.env}.")

        numpy.savetxt(FILENAME_IN, points, fmt="%16.8e")
        with open(FILENAME_IN, "r") as fin, open(FILENAME_OUT, "w") as fout:
            subprocess.run(cmd, env=self.env, stdin=fin, stdout=fout, check=True)

        values = numpy.loadtxt(FILENAME_OUT, usecols=(14,15,16), dtype=dtype, converters=converters)
        os.remove(FILENAME_IN)
        os.remove(FILENAME_OUT)
        return values


    def ucvm_surface(self, points, models=None):
        """Run 'ucvm_query -m MODELS -f FILENAME_CONF [-z zrange]'.

        Args:
        filename_values (str)
            Name file for output.
        """
        FILENAME_IN = "surface_points.txt"
        FILENAME_OUT = "surface_elev.txt"

        cmd = ["ucvm_query", "-f", self.filename_conf]
        if models:
            cmd += ["-m", models]
        cmdstr = " ".join(cmd)
        logger = logging.getLogger(__name__)
        logger.info(f"Running UCVM command '{cmdstr}' with environment {self.env}.")
        numpy.savetxt(FILENAME_IN, points, fmt="%16.8e")
        with open(FILENAME_IN, "r") as fin, open(FILENAME_OUT, "w") as fout:
            subprocess.run(cmd, env=self.env, stdin=fin, stdout=fout, check=True)
        elev = numpy.loadtxt(FILENAME_OUT, usecols=(3,))
        os.remove(FILENAME_IN)
        os.remove(FILENAME_OUT)
        return elev


# End of file
