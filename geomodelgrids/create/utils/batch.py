"""Break domain points into batches to allow application of arbitrarily large domains.
"""

import logging
import math


class BatchGenerator2D():
    """Iterator for batches of points for 2D domains.

    Usage:
        for batch in BatchGenerator2D(num_x, num_y, max_nvalues):
            # Use batch
    """

    def __init__(self, num_x, num_y, max_nvalues=None):
        """Constructor.

        Args:
            num_x(int)
                Number of points in x direction.
            num_y(int)
                Number of points in y direction.
            max_nvalues(int)
                Maximum number of points in a batch.
        """
        self.num_x = num_x
        self.num_y = num_y

        if not max_nvalues or num_x * num_y <= max_nvalues:
            self.bnum_x = num_x
            self.bnum_y = num_y
        else:
            num_xy = round(max_nvalues**0.5)
            if num_x > num_xy and num_y > num_xy:
                self.bnum_x = self.bnum_y = num_xy
            elif num_x <= num_xy:
                self.bnum_x = num_x
                self.bnum_y = max_nvalues // num_x
            elif num_y <= num_xy:
                self.bnum_y = num_y
                self.bnum_x = max_nvalues // num_y
            else:
                raise ValueError("Unknown case.")
        self.nbatch_x = round(math.ceil(num_x / self.bnum_x))
        self.nbatch_y = round(math.ceil(num_y / self.bnum_y))

        self.ix = 0
        self.iy = 0

        self.x_range = (0, 0)
        self.y_range = (0, 0)

        logger = logging.getLogger(__name__)
        logger.info("2D batches -- size: x=%d, y=%d; number: x=%d, y=%d",
                    self.bnum_x, self.bnum_x, self.nbatch_x, self.nbatch_y)

    def __str__(self):
        return "Batch2D [{x0}:{x1}, {y0}:{y1}]".format(
            x0=self.x_range[0], x1=self.x_range[1],
            y0=self.y_range[0], y1=self.y_range[1])

    def __iter__(self):
        """Iteration.

        Returns:
            Batch object.
        """
        return self

    def __next__(self):
        """Get next batch.

        Returns:
            Batch object with next batch.
        """
        if self.ix >= self.nbatch_x:
            raise StopIteration()

        x_start = self.ix * self.bnum_x
        self.x_range = (x_start, min(x_start + self.bnum_x, self.num_x))

        y_start = self.iy * self.bnum_y
        self.y_range = (y_start, min(y_start + self.bnum_y, self.num_y))

        self.iy += 1
        if self.iy >= self.nbatch_y:
            self.iy = 0
            self.ix += 1
        return self


class BatchGenerator3D():
    """Iterator for batches of points for 3D domains.

    Usage:
        for batch in BatchGenerator3D(num_x, num_y, num_z, max_nvalues):
            # Use batch
    """

    def __init__(self, num_x, num_y, num_z, max_nvalues=None):
        """Constructor.

        Args:
            num_x(int)
                Number of points in x direction.
            num_y(int)
                Number of points in y direction.
            num_z(int)
                Number of points in z direction.
            max_nvalues(int)
                Maximum number of points in a batch.
        """
        self.num_x = num_x
        self.num_y = num_y
        self.num_z = num_z

        if not max_nvalues or num_x * num_y * num_z <= max_nvalues:
            self.bnum_x = num_x
            self.bnum_y = num_y
            self.bnum_z = num_z
        else:
            num_xyz = round(max_nvalues**(1.0 / 3.0))
            if num_x > num_xyz and num_y > num_xyz and num_z > num_xyz:
                self.bnum_x = self.bnum_y = self.bnum_z = num_xyz
            elif num_z <= num_xyz:
                self.bnum_z = num_z
                num_xy = round((max_nvalues // num_z)**0.5)
                if num_x > num_xy and num_y > num_xy:
                    self.bnum_x = self.bnum_y = num_xy
                elif num_x <= num_xy:
                    self.bnum_x = num_x
                    self.bnum_y = max_nvalues // (num_x * num_z)
                elif num_y <= num_xy:
                    self.bnum_y = num_y
                    self.bnum_x = max_nvalues // (num_y * num_z)
                else:
                    raise ValueError("Unknown case")
            elif num_x <= num_xyz:
                self.bnum_x = num_x
                num_yz = round((max_nvalues // num_x)**0.5)
                if num_y > num_yz and num_z > num_yz:
                    self.bnum_y = self.bnum_z = num_yz
                elif num_y <= num_yz:
                    self.bnum_y = num_y
                    self.bnum_z = max_nvalues // (num_x * num_y)
                else:
                    raise ValueError("Unknown case")
            elif num_y <= num_xyz:
                self.bnum_y = num_y
                num_xz = round((max_nvalues // num_y)**0.5)
                if num_x > num_xz and num_z > num_xz:
                    self.bnum_x = self.bnum_z = num_xz
                else:
                    raise ValueError("Unknown case")
            else:
                raise ValueError("Unknown case")
        self.nbatch_x = round(math.ceil(num_x / self.bnum_x))
        self.nbatch_y = round(math.ceil(num_y / self.bnum_y))
        self.nbatch_z = round(math.ceil(num_z / self.bnum_z))

        self.ix = 0
        self.iy = 0
        self.iz = 0

        self.x_range = (0, 0)
        self.y_range = (0, 0)
        self.z_range = (0, 0)

        logger = logging.getLogger(__name__)
        logger.info("3D batches -- size: x=%d, y=%d, z=%d; number: x=%d, y=%d, z=%d",
                    self.bnum_x, self.bnum_y, self.bnum_z, self.nbatch_x, self.nbatch_y, self.nbatch_z)

    def __str__(self):
        return "Batch3D [{x0}:{x1}, {y0}:{y1}, {z0}:{z1}]".format(
            x0=self.x_range[0], x1=self.x_range[1],
            y0=self.y_range[0], y1=self.y_range[1],
            z0=self.z_range[0], z1=self.z_range[1])

    def __iter__(self):
        """Iteration.

        Returns:
            Batch object.
        """
        return self

    def __next__(self):
        """Get next batch.

        Returns:
            Batch object with next batch.
        """
        if self.ix >= self.nbatch_x:
            raise StopIteration()

        x_start = self.ix * self.bnum_x
        self.x_range = (x_start, min(x_start + self.bnum_x, self.num_x))

        y_start = self.iy * self.bnum_y
        self.y_range = (y_start, min(y_start + self.bnum_y, self.num_y))

        z_start = self.iz * self.bnum_z
        self.z_range = (z_start, min(z_start + self.bnum_z, self.num_z))

        self.iz += 1
        if self.iz >= self.nbatch_z:
            self.iz = 0
            self.iy += 1
        if self.iy >= self.nbatch_y:
            self.iz = 0
            self.iy = 0
            self.ix += 1
        return self


# End of file
