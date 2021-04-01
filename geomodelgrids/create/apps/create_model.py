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

    def __init__(self):
        """Constructor."""
        self.config = None
        self.model = None
        self.show_progress = False

    def main(self, **kwargs):
        """Main entry point.

        Keyword arguments:
            config (str)
                Name of configuration (INI) file.
            show_parameters (bool), default: False
                If True, print parameters to stdout.
            import_domain (bool), default: False
                If True, write domain information to model.
            import_topography (bool), default: False
                If True, write topography information to model.
            import_blocks (bool), default: False
                If True, write block information to model.
            all (bool), default: False
                If True, equivalent to import_domain=True, import_topography=True, import_blocks=True
            show_progress (bool), default: True
                If False, print progress to stdout.
            log_filename (str), default: rasterize.log
                Name of log file.
            debug (bool), default: False
                Print additional debugging information to log file.
        """
        args = argparse.Namespace(**kwargs) if kwargs else self._parse_command_line()
        log_level = logging.DEBUG if args.debug else logging.INFO
        logging.basicConfig(level=log_level, filename=args.log_filename)
        if args.show_progress:
            self.show_progress = True
        self.initialize(args.config.split(","))

        if args.show_parameters:
            self.show_parameters()
            return

        data_path = self.config["geomodelgrids"]["data_source"].split(".")
        data_obj = getattr(import_module(".".join(data_path[:-1])), data_path[-1])
        datasrc = data_obj(self.config)
        model = core.model.Model(self.config)

        if args.import_domain or args.all:
            model.save_domain()

        batch_size = int(self.config["domain"]["batch_size"])
        datasrc.initialize()

        if args.import_topography or args.all:
            if model.topography.enabled:
                model.init_topography()
                for batch in model.topography.get_batches(batch_size):
                    points = model.topography.generate_points(batch)
                    elevation = datasrc.get_topography(points)
                    model.save_topography(elevation, batch)

        if args.import_blocks or args.all:
            for block in model.blocks:
                model.init_block(block)
                for batch in block.get_batches(batch_size):
                    values = datasrc.get_values(block, model.topography, batch)
                    model.save_block(block, values, batch)

    def initialize(self, config_filenames):
        """Set parameters from config file and DEFAULTS.

        Args:
            config_filename (str)
                Name of configuration (INI) file(s) with parameters.
        """
        self.config = config.get_config(config_filenames)

    def show_parameters(self):
        """Write parameters to stdout.
        """
        parser = configparser.ConfigParser()
        parser.read_dict(self.config)
        parser.write(sys.stdout)

    def _parse_command_line(self):
        """Parse command line arguments.
        """
        DESCRIPTION = (
            "Application for generating HDF5 file containing blocks of a grid-based model."
        )

        parser = argparse.ArgumentParser(description=DESCRIPTION)
        parser.add_argument("--config", action="store", dest="config", required=True)
        parser.add_argument("--show-parameters", action="store_true", dest="show_parameters")
        parser.add_argument("--import-domain", action="store_true", dest="import_domain")
        parser.add_argument("--import-topography", action="store_true", dest="import_topography")
        parser.add_argument("--import-blocks", action="store_true", dest="import_blocks")

        parser.add_argument("--all", action="store_true", dest="all")
        parser.add_argument("--quiet", action="store_false", dest="show_progress", default=True)
        parser.add_argument("--log", action="store", dest="log_filename", default="create_model.log")
        parser.add_argument("--debug", action="store_true", dest="debug")
        args = parser.parse_args()

        if args.debug:
            logging.basicConfig(level=logging.DEBUG, filename=args.log_filename)
        else:
            logging.basicConfig(level=logging.INFO, filename=args.log_filename)
        return args


# End of file
