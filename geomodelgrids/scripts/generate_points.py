#!/usr/bin/env python
# ======================================================================
#
#                           Brad T. Aagaard
#                        U.S. Geological Survey
#
# ======================================================================
#

import math
import logging
import numpy
import os
    
class Block(object):
    """Uniform resolution gridded block within a model.
    """

    def __init__(self, name, res_horiz, res_vert, z_top, z_bot, z_top_offset, domain_x, domain_y, domain_z):
        """Constructor.

        :param name: Name of block.
        :param res_horiz: Horizontal resolution of block in meters.
        :param res_vert: Vertical resolution of block in meters.
        :param z_top: Elevation of top of of block in meters.
        :param z_bot: Elevation of bottom of block in meters.
        :param z_top_offset: Amount to offset top elevation of block in meters.
        """
        self.name = name
        self.res_horiz = res_horiz
        self.res_vert = res_vert
        self.z_top = z_top
        self.z_bot = z_bot
        self.z_top_offset = z_top_offset
        self.domain_z = domain_z
        
        self.num_x = 1 + int(domain_x / self.res_horiz)
        self.num_y = 1 + int(domain_y / self.res_horiz)
        self.num_z = 1 + int((self.z_top-self.z_bot) / self.res_vert)
        return

    def points(self, y_azimuth, origin_x, origin_y, topography):
        """Create array of points spanning the block.
    
        :return: 3D array of points for the block.
        """
        logger = logging.getLogger(__name__)
        logger.info("Block '%s' contains %d points (%d x %d x %d)." % (self.name, self.num_x*self.num_y*self.num_z, self.num_x, self.num_y, self.num_z,))

        x1 = numpy.linspace(0.0, self.res_horiz*(self.num_x-1), self.num_x)
        y1 = numpy.linspace(0.0, self.res_horiz*(self.num_y-1), self.num_y)
        z1 = numpy.linspace(0.0, self.res_vert*(self.num_z-1), self.num_z)
        x,y,z = numpy.meshgrid(x1, y1, z1)

        domain_top = 0.0
        domain_bot = -self.domain_z
        if topography is not None:
            topoG = self._getBlockTopography(topography)
            for i in xrange(self.num_z):
                z[:,:,i] = domain_bot + (topoG-domain_bot)/(domain_top-domain_bot)*(self.z_top - z[:,:,i] - domain_bot)

            # Move top points down
            z[:,:,0] += self.z_top_offset
        else:
            z = self.z_top - z
        
        xyzG = numpy.vstack((x.ravel(), y.ravel(), z.ravel(),)).transpose()
        xyzP = numpy.zeros(xyzG.shape)
        azRad = y_azimuth * math.pi / 180.0
        xyzP[:,0] = origin_x + xyzG[:,0]*math.cos(azRad) + xyzG[:,1]*math.sin(azRad)
        xyzP[:,1] = origin_y - xyzG[:,0]*math.sin(azRad) + xyzG[:,1]*math.cos(azRad)
        xyzP[:,2] = xyzG[:,2]
        return xyzP
        
    def groundsurf_xy(self, y_azimuth, origin_x, origin_y):
        """Create array of points on the ground surface spanning the block.
    
        :return: 2D array of points on the ground surface.
        """
        logger = logging.getLogger(__name__)
        logger.info("Topography surface contains %d x %d points." % (self.num_x, self.num_y,))

        x1 = numpy.linspace(0.0, self.res_horiz*(self.num_x-1), self.num_x)
        y1 = numpy.linspace(0.0, self.res_horiz*(self.num_y-1), self.num_y)
        x,y = numpy.meshgrid(x1, y1)
        xyG = numpy.vstack((x.ravel(), y.ravel(),)).transpose()

        xyP = numpy.zeros(xyG.shape)
        azRad = y_azimuth*math.pi/180.0
        xyP[:,0] = origin_x + xyG[:,0]*math.cos(azRad) + xyG[:,1]*math.sin(azRad)
        xyP[:,1] = origin_y - xyG[:,0]*math.sin(azRad) + xyG[:,1]*math.cos(azRad)

        return xyP

    def _getBlockTopography(self, topography):
        """Get topography grid for block.
        """
        npts = topography.shape[0]
        n = self.num_x - 1
        m = self.num_y - 1
        skip  = int((-(n+m)+((n+m)**2-4*n*m*(1-npts))**0.5)/(2*n*m))
        
        if (n*skip+1)*(m*skip+1) != npts:
            raise ValueError("Nonintegral number of points for block relative to topography. Topography has %d points. Block has %d x %d points." % (npts, self.num_x, self.num_y,))

        topoG = topography.reshape((m*skip+1, n*skip+1,))
        topoG = topoG[::skip,::skip]
        return topoG
    

class Model(object):
    """Georeferenced model composed of uniformly gridded blocks.
    """

    def __init__(self):
        """Constructor.
        """
        self.config = None
        self.description = None
        self.key = None
        self.topography = None
        return

    def initialize(self, config_filename, data_dir=None):
        """Initialize the model.
        
        :param config_filename: Name of model configuration (INI format) file.
        """
        import ConfigParser
        parser = ConfigParser.ConfigParser()
        parser.read(config_filename)
        self.description = parser.get("geomodelgrids", "description")
        self.key = parser.get("geomodelgrids", "key")
        self.config = parser._sections

        self.domain_x = parser.getfloat("domain", "dim_x")
        self.domain_y = parser.getfloat("domain", "dim_y")
        self.domain_z = parser.getfloat("domain", "dim_z")
        self.y_azimuth = parser.getfloat("coordsys", "y_azimuth")
        self.origin_x = parser.getfloat("coordsys", "origin_x")
        self.origin_y = parser.getfloat("coordsys", "origin_y")
        
        self.blocks = []
        for block_name in self._config_list(parser.get("domain", "blocks")):
            res_horiz = parser.getfloat(block_name, "res_horiz")
            res_vert = parser.getfloat(block_name, "res_vert")
            z_top = parser.getfloat(block_name, "z_top")
            z_top_offset = parser.getfloat(block_name, "z_top_offset")
            z_bot = parser.getfloat(block_name, "z_bot")
            
            self.blocks.append(Block(block_name, res_horiz, res_vert, z_top, z_bot, z_top_offset, self.domain_x, self.domain_y, self.domain_z))

        if not os.path.isdir(data_dir):
            os.makedirs(data_dir)
        self.data_dir = data_dir
        return

    def write_surfxy(self):
        import os
        import datetime
        
        block = self.blocks[0]
        header = (
            "Generated with %(script)s by %(user)s on %(date)s.\n"
            "\n"
            "XY grid for topography\n"
            "\n"
            "Model: %(model)s\n"
            "res_horiz: %(res_horiz).1f m\n"
            "num_x: %(num_x)d\n"
            "num_y: %(num_y)d\n"
            % {"script": __file__,
               "user": os.environ["USER"],
               "date": datetime.datetime.now(),
               "model": self.key,
               "res_horiz": block.res_horiz,
               "num_x": block.num_x,
               "num_y": block.num_y,
            },)
            
        filename = "%s/%s-topo-xy.txt.gz" % (self.data_dir, self.key,)
        points = block.groundsurf_xy(self.y_azimuth, self.origin_x, self.origin_y)
        numpy.savetxt(filename, points, fmt="%16.8e", header=header[0])
        return
    
    def write_blocks(self):

        import os
        import datetime

        self._loadTopography()
        if "external_z_units" in self.config["domain"]:
            z_units = self.config["domain"]["external_z_units"]
        else:
            z_units = "m"

        
        for block in self.blocks:
            header = (
                "Generated with %(script)s by %(user)s on %(date)s.\n"
                "\n"
                "Model: %(model)s\n"
                "Block: %(block)s\n"
                "res_horiz: %(res_horiz).1f m\n"
                "res_vert: %(res_vert).1f m\n"
                "z_top: %(z_top).1f m\n"
                "z_top_offset: %(z_top_offset).1f m\n"
                "num_x: %(num_x)d\n"
                "num_y: %(num_y)d\n"
                "num_z: %(num_z)d\n"
                "z_units: %(z_units)s\n"
                % {"script": __file__,
                   "user": os.environ["USER"],
                   "date": datetime.datetime.now(),
                   "model": self.key,
                   "block": block.name,
                   "res_horiz": block.res_horiz,
                   "res_vert": block.res_vert,
                   "z_top": block.z_top,
                   "z_top_offset": block.z_top_offset,
                   "num_x": block.num_x,
                   "num_y": block.num_y,
                   "num_z": block.num_z,
                   "z_units": z_units,
                },)

            points = block.points(self.y_azimuth, self.origin_x, self.origin_y, self.topography)

            # Convert to output units
            if z_units in ["m", "meter", "meters"]:
                pass
            elif z_units in ["ft", "feet"]:
                points[:,2] /= 0.3048
            else:
                raise ValueError("Unknown units '%s' for external z coordinate." % z_units)
                
            filename = "%s/%s-%s-xyz.txt.gz" % (self.data_dir, self.key, block.name,)
            numpy.savetxt(filename, points, fmt="%20.12e, %20.12e, %14.6e", header=header[0])
        return

    def _loadTopography(self):
        if "topography" in self.config["domain"]:
            filename = "%s/%s" % (self.data_dir, self.config["domain"]["topography"])
            self.topography = numpy.loadtxt(filename)[:,2]

            # Convert to meters
            if "external_z_units" in self.config["domain"]:
                z_units = self.config["domain"]["external_z_units"]
                if z_units in ["m", "meter", "meters"]:
                    pass
                elif z_units in ["ft", "feet"]:
                    self.topography *= 0.3048
                else:
                    raise ValueError("Unknown units '%s' for external z coordinate." % z_units)

        else:
            self.topography = None
        return
    
    
    def _config_list(self, list_string):
        l = [f.strip() for f in list_string[1:-1].split(",")]
        return l

        
class App(object):
    """Create xyz grid files, one file per block with points in physical space.

    The intent is that external modeling tools provide the values at
    these points. This step can be skipped if the model is already
    discretized in a suite of blocks.
    """

    def __init__(self, model_config, data_dir):
        """Constructor.
        """
        self.model_config = model_config
        self.data_dir = data_dir
        return

    
    def run(self, points):
        """Generate points in the model blocks.

        :param points: If points == "groundsurf", generate points on ground surface. If points == "blocks", generate points in each block.
        """
        self.model = Model()
        self.model.initialize(self.model_config, self.data_dir)
        
        if points == "groundsurf":
            self.model.write_surfxy()
            
        elif points == "blocks":
            self.model.write_blocks()

        else:
            raise ValueError("Unknown value '%s' for points." % points)
        return



# ======================================================================
if __name__ == "__main__":
    DESCRIPTION = "Application for generating files with xyz grid points "
    "associated with the points in the blocks of a grid-based model. One "
    "file is written for each block of points."

    import argparse
    import logging
    parser = argparse.ArgumentParser(description=DESCRIPTION)
    parser.add_argument("--data-dir", action="store", dest="data_dir", default=".")
    parser.add_argument("--model", action="store", dest="model_config", required=True)
    parser.add_argument("--points", action="store", dest="points", choices=["blocks","groundsurf"], required=True)
    parser.add_argument("--log", action="store", dest="logFilename", default="generate_points.log")
    args = parser.parse_args()

    logging.basicConfig(level=logging.DEBUG, filename=args.logFilename)

    app = App(args.model_config, args.data_dir)
    app.run(args.points)


# End of file    
