"""Reader for Southern California Earthquake Center (SCEC) Central California (CCA06)
3D seismic velocity model.
"""

import logging

import numpy

from geomodelgrids.create.core.datasrc import DataSrc


class CCA06(DataSrc):
    """Reader for SCEC CCA06.
    """
    NUM_VALUES = 3

    def __init__(self, config):
        """Constructor.

        Args:
            config ():
                Config with name of data file.
        """
        self.filename = config["scec_cca"]["data_file"]

    def get_topography(self, points):
        """Write domain attributes to HDF5 file.

        Args:
            points (numpy.array [Nx,Ny])
                Numpy array with coordinates of points in model coordinates.
        Returns:
            None
        """
        return

    def get_values(self, block, topography, batch=None):
        """Get model values.

        Args:
            block (Block)
                Block information.
            topography ()
            batch (BatchGenerator3D)
                Current batch of points in block.
        """
        DIM = 3
        x_start, x_end = batch.x_range
        y_start, y_end = batch.y_range
        z_start, z_end = batch.z_range
        num_x = x_end - x_start
        num_y = y_end - y_start
        num_z = z_end - z_start

        logger = logging.getLogger(__name__)
        logger.info("Getting values for batch %s from SCEC CCA06 '%s'.",
                    batch, self.filename)

        data = numpy.zeros((num_x, num_y, num_z, self.NUM_VALUES))
        with open(self.filename, "r") as fin:
            for line in fin:
                values = line.split()
                ix, iy, iz = map(int, values[0:DIM])
                ix -= 1
                iy -= 1
                iz -= 1
                if ix >= x_start and ix < x_end and \
                        iy >= y_start and iy < y_end and \
                        iz >= z_start and iz < z_end:
                    data[ix - x_start, iy - y_start, iz - z_start, :] = \
                        tuple(map(float, values[DIM:DIM + self.NUM_VALUES]))
        return data


# End of file
