#!/usr/bin/env python
# ======================================================================
#
#                           Brad T. Aagaard
#                        U.S. Geological Survey
#
# ======================================================================
#

from geomodelgrids.Model import Model

import numpy
import datetime
import os

class App(object):

    def __init__(self):
        """Constructor.
        """
        self.model = None
        return


    def initialize(self, model_config):
        self.model = Model()
        self.model.initialize(model_config)
        return


    def write_blocks(self, fileroot):
        if fileroot is None:
            fileroot = self.model.key
        for block in self.model.blocks:
            header = (
                "Generated with %(script)s by %(user)s on %(date)s.\n"
                "\n"
                "Model: %(model)s\n"
                "Block: %(block)s\n"
                "res_horiz: %(res_horiz).1f\n"
                "res_vert: %(res_vert).1f\n"
                "z_top: %(z_top).1f\n"
                "num_x: %(num_x)d\n"
                "num_y: %(num_y)d\n"
                "num_z: %(num_z)d\n"
                % {"script": __file__,
                   "user": os.environ["USER"],
                   "date": datetime.datetime.now(),
                   "model": self.model.key,
                   "block": block.name,
                   "res_horiz": block.res_horiz,
                   "res_vert": block.res_vert,
                   "z_top": block.z_top,
                   "num_x": block.num_x,
                   "num_y": block.num_y,
                   "num_z": block.num_z,
                },)
            
            filename = "%s-%s.txt" % (fileroot, block.name)
            numpy.savetxt(filename, block.points(), fmt="%16.8e", header=header[0])
        return

# ======================================================================
if __name__ == "__main__":
    DESCRIPTION = "Application for generating files with xyz grid points "
    "associated with the points in the blocks of a grid-based model. One "
    "file is written for each block of points."

    import argparse
    import logging
    parser = argparse.ArgumentParser(description=DESCRIPTION)
    parser.add_argument("--fileroot", action="store", dest="fileroot")
    parser.add_argument("--model", action="store", dest="model_config", required=True)
    parser.add_argument("--log", action="store", dest="logFilename", default="generate_blocksxyz.log")
    args = parser.parse_args()

    logging.basicConfig(level=logging.DEBUG, filename=args.logFilename)

    app = App()
    app.initialize(args.model_config)
    app.write_blocks(args.fileroot)


# End of file    
