# ======================================================================
#
#                           Brad T. Aagaard
#                        U.S. Geological Survey
#
# ======================================================================
#
# Graymer et al. 3-D seismic velocity model for a portion of the San
# Joaquin / Sacramento Delta.
#
# The domain is 100km x 75km and extends down to a depth of 45km.

from geomodelgrids.Block import Block
import math
    
    
class ModelBlock(Block):
    """Block for Graymer et al. Delta model.
    """
    DOMAIN_X = 75.0e+3
    DOMAIN_Y = 100.0e+3

    def __init__(self, name, res_horiz, res_vert, z_top, z_bot):
        """Constructor.

        :param name: Name of block.
        :param res_horiz: Horizontal resolution of grid in meters.
        :param res_vert: Vertical resolution of grid in meters.
        :param z_top: Elevation of top of grid in meters.
        :param z_bot: Elevation of bottom of grid in meters.
        """
        num_x = 1 + int(ModelBlock.DOMAIN_X / res_horiz)
        num_y = 1 + int(ModelBlock.DOMAIN_Y / res_horiz)
        num_z = 1 + int((z_top-z_bot) / res_vert)
        Block.__init__(name, res_horiz, res_vert, z_top, num_x, num_y, num_z)
        return

    def points(self):
        """Create array of points spanning the block.
        """
        xyzG = Block.points(self)
        xyzP = numpy.zeros(xyzB.shape)
        azRad = Model.Y_AZIMUTH*math.pi/180.0
        xyzP[:,0] = Model.X_ORIGIN + xyzG[:,0]*math.cos(azRad) + xyzG[:,1]*math.sin(azRad)
        xyzP[:,1] = Model.Y_ORIGIN - xyzG[:,0]*math.sin(azRad) + xyzG[:,1]*math.cos(azRad)
        xyzP[:,2] = Model.Z_TOP - xyzG[:,2]
        return xyzP
        
        

class Model(object):
    """Graymer et al. 3-D seismic velocity model for a portion of the San
    Joaquin / Sacramento Delta.
    """
    DESCRIPTION = "Graymer et al. 3-D seismic velocity model for the San Joaquin /
    Sacramento Delta."
    
    PROJECTION = {
        "proj": "utm",
        "zone": 10,
        "datum": "NAD83",
    }
    X_ORIGIN = 610000
    Y_ORIGIN = 4151100
    Y_AZIMUTH = 330

    VALUES = ["Vp", "Vs", "density", "Qp", "Qs"]
    UNITS = ["m/s", "m/s", "kg/m**3", "none", "none"]
    
    blocks = [
        #ModelBlock(name="vres25m", res_horiz=100.0, res_vert=25.0, z_top=1500.0, z_bot=-500.0),
        #ModelBlock(name="vres50m", res_horiz=200.0, res_vert=50.0, z_top=-500.0, z_bot=-3500.0),
        #ModelBlock(name="vres125m", res_horiz=500.0, res_vert=125.0, z_top=-3500.0, z_bot=-10.0e+3),
        #ModelBlock(name="vres250m", res_horiz=1000.0, res_vert=250.0, z_top=-10.0e+3, z_bot=-45.0e+3),
        ModelBlock(name="test", res_horiz=25.0e+3, res_vert=45.0e+3, z_top=0.0, z_bot=-45.0e+3),
    ]
    
    
def model():
    """
    Entry point for getting model.
    """
    return Model()



# End of file    
