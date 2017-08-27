# ======================================================================
#
#                           Brad T. Aagaard
#                        U.S. Geological Survey
#
# ======================================================================
#
# Generic georeferenced model composed of uniformly gridded blocks.

import math
import numpy
    
class Block(object):
    """Uniform resolution gridded block within a model.
    """

    def __init__(self, name, config):
        """Constructor.

        :param name: Name of block.
        :param res_horiz: Horizontal resolution of grid in meters.
        :param res_vert: Vertical resolution of grid in meters.
        :param z_top: Elevation of top of grid in meters.
        :param z_bot: Elevation of bottom of grid in meters.
        """
        self.config = config

        self.name = name
        self.res_horiz = float(config[name]["res_horiz"])
        self.res_vert = float(config[name]["res_vert"])
        self.z_top = float(config[name]["z_top"])
        self.z_bot = float(config[name]["z_bot"])

        dim_x = float(config["domain"]["dim_x"])
        dim_y = float(config["domain"]["dim_y"])
        
        self.num_x = 1 + int(dim_x / self.res_horiz)
        self.num_y = 1 + int(dim_y / self.res_horiz)
        self.num_z = 1 + int((self.z_top-self.z_bot) / self.res_vert)
        return

    def points(self):
        """Create array of points spanning the block.
    
        :return: 3D array of points for the block.
        """
        x1 = numpy.linspace(0.0, self.res_horiz*(self.num_x-1), self.num_x)
        y1 = numpy.linspace(0.0, self.res_horiz*(self.num_y-1), self.num_y)
        z1 = numpy.linspace(0.0, self.res_vert*(self.num_z-1), self.num_z)
        x,y,z = numpy.meshgrid(x1, y1, z1)
        xyzG = numpy.vstack((x.ravel(), y.ravel(), z.ravel(),)).transpose()

        coordsys = self.config["coordsys"]
        y_azimuth = float(coordsys["y_azimuth"])
        origin_x = float(coordsys["origin_x"])
        origin_y = float(coordsys["origin_y"])
        
        xyzP = numpy.zeros(xyzG.shape)
        azRad = y_azimuth * math.pi / 180.0
        xyzP[:,0] = origin_x + xyzG[:,0]*math.cos(azRad) + xyzG[:,1]*math.sin(azRad)
        xyzP[:,1] = origin_y - xyzG[:,0]*math.sin(azRad) + xyzG[:,1]*math.cos(azRad)
        xyzP[:,2] = self.z_top - xyzG[:,2]
        return xyzP
        
    def groundsurf_xy(self):
        """Create array of points on the ground surface spanning the block.
    
        :return: 2D array of points on the ground surface.
        """
        x1 = numpy.linspace(0.0, self.res_horiz*(self.num_x-1), self.num_x)
        y1 = numpy.linspace(0.0, self.res_horiz*(self.num_y-1), self.num_y)
        x,y = numpy.meshgrid(x1, y1)
        xyG = numpy.vstack((x.ravel(), y.ravel(),)).transpose()

        coordsys = self.config["coordsys"]
        y_azimuth = float(coordsys["y_azimuth"])
        origin_x = float(coordsys["origin_x"])
        origin_y = float(coordsys["origin_y"])

        xyP = numpy.zeros(xyG.shape)
        azRad = y_azimuth*math.pi/180.0
        xyP[:,0] = origin_x + xyG[:,0]*math.cos(azRad) + xyG[:,1]*math.sin(azRad)
        xyP[:,1] = origin_y - xyG[:,0]*math.sin(azRad) + xyG[:,1]*math.cos(azRad)
        return xyP
                

class Model(object):
    """Georeferenced model composed of uniformly gridded blocks.
    """

    def __init__(self):
        """Constructor.
        """
        self.config = None
        self.description = None
        self.key = None
        return

    def initialize(self, config_filename):
        """Initialize the model.
        
        :param config_filename: Name of model configuration (INI format) file.
        """
        import ConfigParser
        
        parser = ConfigParser.ConfigParser()
        parser.read(config_filename)
        self.description = parser.get("geomodelgrids", "description")
        self.key = parser.get("geomodelgrids", "key")
        self.config = parser._sections

        self.blocks = []
        for block_name in self._config_list(parser.get("domain", "blocks")):
            self.blocks.append(Block(block_name, self.config))
        return
        

    def _config_list(self, list_string):
        l = [f.strip() for f in list_string[1:-1].split(",")]
        return l

        
# End of file    
