"""Application for creating a GeoModelGrids model from data.
"""

import sys
import argparse
import logging
import configparser
from importlib import import_module

import geomodelgrids.create.core as core
from geomodelgrids.create.utils import config


class App():
    """Application for generating a GeoModelGrids model from data.
    """

    def __init__(self,
                 show_progress: bool = True,
                 debug=False,
                 log_filename: str = "create_model.log"):
        """Constructor."""
        self.config = None
        self.model = None
        self.show_progress = show_progress
        log_level = logging.DEBUG if debug else logging.INFO
        logging.basicConfig(level=log_level, filename=log_filename)

    def main(self,
             config_filenames: str,
             show_parameters: bool = False,
             import_domain: bool = False,
             import_surfaces: bool = False,
             import_blocks: bool = False,
             update_metadata: bool = False,
             all_steps: bool = False):
        """Main entry point.

        Arguments:
            config_filenames
                Comma separated list of configuration file(s).
            show_parameters
                If True, print parameters to stdout.
            import_domain
                If True, write domain information to model.
            import_surfaces
                If True, write surfaces information to model.
            import_blocks
                If True, write block information to model.
            update_metadata
                If True, update all metadata in model.
            all
                If True, equivalent to import_domain=True, import_surfaces=True, import_blocks=True
            show_progress
                If False, print progress to stdout.
            log_filename
                Name of log file.
            debug
                Print additional debugging information to log file.
        """
        self.initialize(config_filenames.split(","))

        if show_parameters:
            self.show_parameters()
            return

        if import_domain or import_surfaces or import_blocks or all_steps:
            data_path = self.config["geomodelgrids"]["data_source"].split(".")
            data_obj = getattr(import_module(".".join(data_path[:-1])), data_path[-1])
            datasrc = data_obj(self.config)
            datasrc.initialize()
        model = core.model.Model(self.config)

        if import_domain or all_steps:
            model.save_domain()

        if import_surfaces or all_steps:
            batch_size = int(self.config["domain"]["batch_size"]) if "batch_size" in self.config["domain"] else None
            if model.top_surface:
                model.init_top_surface()
                if batch_size:
                    for batch in model.top_surface.get_batches(batch_size):
                        points = model.top_surface.generate_points(batch)
                        elevation = datasrc.get_top_surface(points)
                        model.save_top_surface(elevation, batch)
                else:
                    points = model.top_surface.generate_points()
                    elevation = datasrc.get_top_surface()
                    model.save_top_surface(elevation)
            if model.topo_bathy:
                model.init_topography_bathymetry()
                if batch_size:
                    for batch in model.topo_bathy.get_batches(batch_size):
                        points = model.topo_bathy.generate_points(batch)
                        elevation = datasrc.get_topography_bathymetry(points)
                        model.save_topography_bathymetry(elevation, batch)
                else:
                    points = model.topo_bathy.generate_points()
                    elevation = datasrc.get_topography_bathymetry(points)
                    model.save_topography_bathymetry(elevation)

        if import_blocks or all_steps:
            batch_size = int(self.config["domain"]["batch_size"]) if "batch_size" in self.config["domain"] else None
            topo_depth = model.topo_bathy if model.topo_bathy else model.top_surface
            for block in model.blocks:
                model.init_block(block)
                if batch_size:
                    for batch in block.get_batches(batch_size):
                        values = datasrc.get_values(block, model.top_surface, topo_depth, batch)
                        model.save_block(block, values, batch)
                else:
                    values = datasrc.get_values(block, model.top_surface, topo_depth)
                    model.save_block(block, values)

        if update_metadata:
            model.update_metadata()

    def initialize(self, config_filenames):
        """Set parameters from config file and DEFAULTS.

        Args:
            config_filenames (list of str)
                Name of configuration (INI) file(s) with parameters.
        """
        self.config = config.get_config(config_filenames)

    def show_parameters(self):
        """Write parameters to stdout.
        """
        parser = configparser.ConfigParser()
        parser.read_dict(self.config)
        parser.write(sys.stdout)


def cli():
    """Parse command line arguments.
    """
    DESCRIPTION = (
        "Application for generating HDF5 file containing blocks of a grid-based model."
    )

    parser = argparse.ArgumentParser(description=DESCRIPTION)
    parser.add_argument("--config", action="store", dest="config", required=True)
    parser.add_argument("--show-parameters", action="store_true", dest="show_parameters")
    parser.add_argument("--import-domain", action="store_true", dest="import_domain")
    parser.add_argument("--import-surfaces", action="store_true", dest="import_surfaces")
    parser.add_argument("--import-blocks", action="store_true", dest="import_blocks")
    parser.add_argument("--update-metadata", action="store_true", dest="update_metadata")

    parser.add_argument("--all", action="store_true", dest="all")
    parser.add_argument("--quiet", action="store_false", dest="show_progress", default=True)
    parser.add_argument("--log", action="store", dest="log_filename", default="create_model.log")
    parser.add_argument("--debug", action="store_true", dest="debug")
    args = parser.parse_args()

    app = App(show_progress=True, debug=args.debug, log_filename=args.log_filename)
    kwargs = {
        "show_parameters": args.show_parameters,
        "import_domain": args.import_domain,
        "import_surfaces": args.import_surfaces,
        "import_blocks": args.import_blocks,
        "update_metadata": args.update_metadata,
    }
    app.main(**kwargs)


# End of file
