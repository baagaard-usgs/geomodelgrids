# ======================================================================
#
#                           Brad T. Aagaard
#                        U.S. Geological Survey
#
# ======================================================================
#
# Generic block.

import numpy

class Block(object):
    """Block composed of a uniform resolution grid.
    """
    
    def __init__(self, name, res_horiz, res_vert, z_top, num_x, num_y, num_z):
        """Constructor.

        :param name: Name of block.
        :param res_horiz: Horizontal resolution of grid in meters.
        :param res_vert: Vertical resolution of grid in meters.
        :param z_top: Elevation of top of grid in meters.
        :param num_x: Number of points in block in block x-direction.
        :param num_y: Number of points in block in block y-direction.
        :param num_z: Number of points in block in block z-direction.
        """
        self.name = name
        self.res_horiz = res_horiz
        self.res_vert = res_vert
        self.z_top = z_top
        self.num_x = num_x
        self.num_y = num_y
        self.num_z = num_z
        return

    def points(self):
        """Create array of points spanning the block.

        Coordinates in this method for a generic block are in the local block coordinate system.
        """
        x1 = numpy.linspace(0.0, self.res_horiz*(self.num_x-1), self.num_x)
        y1 = numpy.linspace(0.0, self.res_horiz*(self.num_y-1), self.num_y)
        z1 = numpy.linspace(0.0, self.res_vert*(self.num_z-1), self.num_z)
        x,y,z = numpy.meshgrid(x1, y1, z1)
        xyz = numpy.vstack((x.ravel(), y.ravel(), z.ravel(),)).transpose()
        return xyz

    
# End of file
