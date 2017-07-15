#!/usr/bin/env python
# ======================================================================
#
#                           Brad T. Aagaard
#                        U.S. Geological Survey
#
# ======================================================================
#

from geomodelgrids.Models import Models

import numpy
import datetime

class App(object):

    def __init__(self):
        self.model = None
        return


    def initialize(self):
        models = Models(self.models_dir)
        models.initialize()
        self.model = models.get(model_name)
        return


    def write_blocks(self, fileroot):
        for block in self.model.blocks:
            header = (
                "# Generated with %(script)s by %(user)s on %(date)s.\n"
                "#\n"
                "# Model: %(model)\n"
                "# Block: %(block)\n"
                "# res_horiz: $(res_horiz)f\n"
                "# res_vert: $(res_vert)f\n"
                "# z_top: $(z_top)f\n"
                "# num_x: $(num_x)f\n"
                "# num_y: $(num_y)f\n"
                "# num_z: $(num_z)f\n"
                % {"script": __file__,
                   "user": os.environ["USER"],
                   "date": datetime.datatime.now(),
                   "model": self.model.name,
                   "block": self.block.name,
                   "res_horiz": self.block.res_horiz,
                   "res_vert": self.block.res_vert,
                   "z_top": self.block.z_top,
                   "num_x": self.block.num_x,
                   "num_y": self.block.num_y,
                   "num_z": self.block.num_z,
                },)
            
            filename = "%s-%s.txt" % (self.filename, block.name)
            numpy.savetxt(fout, self.block.points(), fmt="%16.8e", header=header[0])
        return

# ======================================================================
if __name__ == "__main__":
    DESCRIPTION = "Application for generating files with xyz grid points "
    "associated with the points in the blocks of a grid-based model. One "
    "file is written for each block of points."

    parser = argparse.ArgumentParser(description=DESCRIPTION)
    parser.add_argument("--models-dir", action="store", dest="models_dir")
    parser.add_argument("--fileroot", action="store", dest="fileroot")
    parser.add_argument("--model", action="store", dest="model")
    parser.add_argument("--log", action="store", dest="logFilename", default="generate_blocksxyz.log")
    args = parser.parse_args()

    logging.basicConfig(level=logging.DEBUG, filename=args.logFilename)

    app = App(args.model, args.models_dir)
    app.initialize()
    app.write_blocks(args.filename)


# End of file    
