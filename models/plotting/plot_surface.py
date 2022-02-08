#!/usr/bin/env python3
"""Plot topography using only information in HDF5 geomodelgrids file.
"""

import os
import math
from importlib import import_module
import logging

#import matplotlib
#matplotlib.use("Agg")

import numpy
import h5py
import pyproj
import matplotlib.pyplot as pyplot
import matplotlib.colors as colors
import matplotlib.patches as patches
import matplotlib.ticker as ticker
from osgeo import osr
from cartopy import crs
from cartopy.mpl.gridliner import LONGITUDE_FORMATTER, LATITUDE_FORMATTER

from cartopy_extra_tiles import cached_tiler
import matplotlib_extras


# ----------------------------------------------------------------------
def _config_get_list(list_string):
    """Convert list as string to list.

    :type list_string: list
    :param list_string: List as string.
    :returns: List of strings.
    """
    l = [f.strip() for f in list_string[1:-1].split(",")]
    return l


# ----------------------------------------------------------------------
class TopographyApp(object):
    """Plot topography of model.
    """
    
    def __init__(self):
        """Constructor.
        """
        self.config = None

        self.cmap = "terrain" #"viridis" # plasma
        self.contours = [0, 500.0, 1000.0, 1500.0, 2000.0]

    def main(self):
        """Main entry point
        """
        # Initialization
        args = self._parseCommandLine()
        logLevel = logging.DEBUG if args.debug else logging.INFO
        logging.basicConfig(level=logLevel, filename="topography.log")
        if args.show_progress:
            self.showProgress = True
        self._initialize(args.config)

        if args.show_parameters:
            self._show_parameters()

        if args.plot:
            self._plot_topography(args.model, args.surface, args.filename)

    def _plot_topography(self, model_filename, surface_name, output_filename):
        COLORBAR_AXES = [0.15, 0.08, 0.02, 0.33]

        with h5py.File(model_filename, "r") as h5:
            surface = h5["surfaces"][surface_name]
            elev = surface[:]
            res_x = surface.attrs["x_resolution"]
            res_y = surface.attrs["y_resolution"]
            crsString = h5.attrs["crs"]
            origin_x = h5.attrs["origin_x"]
            origin_y = h5.attrs["origin_y"]
            y_azimuth = h5.attrs["y_azimuth"] / 180.0 * math.pi

        num_x = elev.shape[0]
        num_y = elev.shape[1]
        x_1d = numpy.arange(0.0, num_x*res_x, res_x)
        y_1d = numpy.arange(0.0, num_y*res_y, res_y)
        xModel,yModel = numpy.meshgrid(x_1d, y_1d, indexing="ij")
        xCRS = origin_x + xModel*math.cos(y_azimuth) + yModel*math.sin(y_azimuth)
        yCRS = origin_y - xModel*math.sin(y_azimuth) + yModel*math.cos(y_azimuth)

        proj = pyproj.Proj(crsString)
        lon,lat = proj(xCRS, yCRS, inverse=True)
        dataExtent = [numpy.min(lon.ravel()), numpy.max(lon.ravel()), numpy.min(lat.ravel()), numpy.max(lat.ravel())]
        modelCRS = crs.PlateCarree()

        figure = self._create_basemap(dataExtent)
        ax = figure.gca()
        norm = colors.Normalize(vmin=numpy.min(elev.ravel()), vmax=numpy.max(elev.ravel()))
        im = ax.pcolormesh(lon, lat, elev.squeeze(), norm=norm, transform=modelCRS, cmap=self.cmap, alpha=0.5)
        im.set_zorder(2)

        ax.set_title("Topography")
        ax.set_aspect("equal")
        cbax = figure.add_axes(COLORBAR_AXES)
        formatter = ticker.FormatStrFormatter("%.0f")
        colorbar = pyplot.colorbar(im, cax=cbax, ticks=self.contours, format=formatter)
        colorbar.set_label("Elevation (m)")

        figure.savefig(os.path.join("plots", output_filename), pad_inches=0.02)
        pyplot.close(figure)

    def _create_basemap(self, dataExtent):
        tilerPath = self.config.get("maps", "tiler").split(".")
        tilerObj = getattr(import_module(".".join(tilerPath[:-1])), tilerPath[-1])
        tilerStyle = self.config.get("maps", "tiler_style")
        tilerZoom = self.config.getint("maps", "zoom_level")
        tilesDir = self.config.get("maps", "tiler_cache_dir")
        tiler = cached_tiler.CachedTiler(tilerObj(desired_tile_form="L", style=tilerStyle), cache_dir=tilesDir)
        
        figWidthIn = self.config.getfloat("maps", "width_in")
        figHeightIn = self.config.getfloat("maps", "height_in")
        figure = pyplot.figure(figsize=(figWidthIn, figHeightIn), dpi=300)

        rectFactory = matplotlib_extras.axes.RectFactory(figure, margins=((0.45, 0, 0.05), (0.30, 0, 0.25)))
        ax = figure.add_axes(rectFactory.rect(), projection=tiler.crs)
        ax.set_extent(dataExtent)
        ax.add_image(tiler, tilerZoom, cmap="gray")

        tickSpacingDeg = self.config.getfloat("maps", "tick_spacing_deg")
        gridlines = ax.gridlines(crs=crs.PlateCarree(), draw_labels=True, linewidth=0.5, alpha=0.2)
        gridlines.xlabels_top = False
        gridlines.xlabels_bottom = True
        gridlines.ylabels_left = True
        gridlines.ylabels_right = False
        gridlines.xformatter = LONGITUDE_FORMATTER
        gridlines.yformatter = LATITUDE_FORMATTER
        gridlines.xlocator = ticker.MultipleLocator(tickSpacingDeg)
        gridlines.ylocator = ticker.MultipleLocator(tickSpacingDeg)
        return figure
    
    def _initialize(self, filename):
        """Set parameters from config file.

        :type config_filename: str
        :param config_filename: Name of configuration (INI) file with parameters.
        """
        import configparser
        import io
        config = configparser.ConfigParser()
        if self.showProgress:
            print("Fetching parameters from {}...".format(filename))
        config.read(filename)
        self.config = config
        return
    
    def _show_parameters(self):
        """Write parameters to stdout.
        """
        import sys
        self.config.write(sys.stdout)
        return

    def _parseCommandLine(self):
        """Parse command line arguments.
        """
        import argparse

        parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
        parser.add_argument("--config", action="store", dest="config", required=True, help="Configuration file")
        parser.add_argument("--model", action="store", dest="model", required=True, help="GeoModelGrids file")
        parser.add_argument("--surface", action="store", dest="surface", default="top_surface", help="Name of surface to plot")
        parser.add_argument("--filename", action="store", dest="filename", default="topography.jpg", help="Filename for plot output")
        parser.add_argument("--plot", action="store_true", dest="plot", default=True, help="Run plotting")
        parser.add_argument("--show-parameters", action="store_true", dest="show_parameters", help="Show parameters")
        parser.add_argument("--quiet", action="store_false", dest="show_progress", default=True, help="Do not show progress")
        parser.add_argument("--debug", action="store_true", dest="debug", default=True, help="Dump debugging information to log")
        return parser.parse_args()

# ======================================================================
if __name__ == "__main__":
    TopographyApp().main()

# End of file
