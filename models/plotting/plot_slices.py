#!/usr/bin/env python3
"""Plot horizontal and vertical slices for a geomodelgrids file.
"""

import os
import subprocess
from importlib import import_module
import logging

import numpy

import matplotlib.pyplot as pyplot
import matplotlib.colors as colors
import matplotlib.patches as patches
import matplotlib.ticker as ticker
from cartopy import crs
from cartopy.mpl.gridliner import LONGITUDE_FORMATTER, LATITUDE_FORMATTER

from cartopy_extra_tiles import cached_tiler
import matplotlib_extras

from geomodelgrids.create.core import NODATA_VALUE

import cmap_importxml

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
class SlicesApp(object):
    """Plot horizontal and vertical slices.
    """
    
    def __init__(self):
        """Constructor.
        """
        self.config = None

        cmap = cmap_importxml.make_cmap("sciviscolor-5w_heir1.xml")
        self.cmap = pyplot.get_cmap(cmap)
        return

    def main(self):
        """Main entry point
        """
        args = self._parseCommandLine()
        logLevel = logging.DEBUG if args.debug else logging.INFO
        logging.basicConfig(level=logLevel, filename="slices.log")
        if args.show_progress:
            self.showProgress = True
        self.initialize(args.config)
        self.model = args.model
        
        if args.show_parameters:
            self.show_parameters()

        if args.query_hslices:
            self.query_hslices()
        if args.query_vslices:
            self.query_vslices()

        pyplot.style.use(["color-lightbg", "size-presentation"])
        if args.plot_hslices:
            self.plot_hslices()
        if args.plot_vslices:
            self.plot_vslices()
        return

    def query_hslices(self):
        """Query model on horizontal slices.
        """
        slices = _config_get_list(self.config.get("horiz_slices", "slices"))
        domain = self.config.get("domain", "label")
        values = ",".join(v for v in _config_get_list(self.config.get("plots", "values")))
        for label in slices:
            if self.showProgress:
                print("Running query for horizontal slice '{}'...".format(label))

            points = self.hslice_points(label)
            numpy.savetxt("slice.in", points, fmt="%9.2f")

            output = os.path.join("data", "gmg-{}-hslice-{}.txt".format(domain, label))
            cmd = f"geomodelgrids_query --values={values} --models={self.model} --points=coordsys=EPSG:4326 --points=slice.in --output={output}"
            
            if self.config.getboolean(label, "squash"):
                squash_elev = 1000.0 * self.config.getfloat(label, "squash_elev_km")
                cmd += " --squash-min-elev={} --squash-surface=top_surface".format(squash_elev)
            subprocess.run(cmd.split())
            os.remove("slice.in")

    def query_vslices(self):
        """Query model on vertical slices.
        """
        slices = _config_get_list(self.config.get("vert_slices", "slices"))
        domain = self.config.get("domain", "label")
        values = ",".join(v for v in _config_get_list(self.config.get("plots", "values")))
        for label in slices:
            if self.showProgress:
                print("Running query for vertical slice '{}'...".format(label))

            (points, distH, elev) = self.vslice_points(label)
            numpy.savetxt("slice.in", points)

            output = os.path.join("data", "gmg-{}-vslice-{}.txt".format(domain, label))
            cmd = f"geomodelgrids_query --values={values} --models={self.model} --points=coordsys=EPSG:4326 --points=slice.in --output={output}"
            
            subprocess.run(cmd.split())
            os.remove("slice.in")

    def plot_hslices(self):
        slices = _config_get_list(self.config.get("horiz_slices", "slices"))
        for hslice in slices:
            if self.showProgress:
                print(f"Plotting horizontal slice {hslice}...")
            self.plot_hslice(hslice)
        return
    
    def plot_vslices(self):
        slices = _config_get_list(self.config.get("vert_slices", "slices"))
        for vslice in slices:
            if self.showProgress:
                print(f"Plotting vertical slice {vslice}...")
            self.plot_vslice(vslice)
            self.plot_vslice_icon(vslice)
        return
    
    def hslice_points(self, label):
        """Generate points for horizontal slice.
        """
        lonMin = self.config.getfloat("horiz_slices", "longitude_min")
        lonMax = self.config.getfloat("horiz_slices", "longitude_max")
        
        latMin = self.config.getfloat("horiz_slices", "latitude_min")
        latMax = self.config.getfloat("horiz_slices", "latitude_max")

        res = self.config.getfloat("horiz_slices", "resolution_deg")
        
        z0 = 1000.0 * self.config.getfloat(label, "z_km")

        lon1 = numpy.arange(lonMin, lonMax+0.1*res, res, dtype=numpy.float64)
        lat1 = numpy.arange(latMin, latMax+0.1*res, res, dtype=numpy.float64)

        logger = logging.getLogger(__name__)
        logger.info("Slice {}: numLon={}, numLat={}.".format(label, lon1.shape[-1], lat1.shape[-1]))
        
        lon, lat = numpy.meshgrid(lon1, lat1)
        z = z0 * numpy.ones(lon.shape, dtype=numpy.float64)
        points = numpy.vstack((lat.ravel(), lon.ravel(), z.ravel(),)).transpose()
        return points

    def vslice_points(self, label):
        """Generate points for vertical slice.
        """
        lonStart = self.config.getfloat(label, "longitude_start")
        lonEnd = self.config.getfloat(label, "longitude_end")
        latStart = self.config.getfloat(label, "latitude_start")
        latEnd = self.config.getfloat(label, "latitude_end")
        resH = self.config.getfloat("vert_slices", "resolution_horiz_m")

        elevMax = 1000.0 * self.config.getfloat("vert_slices", "elevation_max_km")
        elevMin = 1000.0 * self.config.getfloat("vert_slices", "elevation_min_km")
        resV = self.config.getfloat("vert_slices", "resolution_vert_m")

        dist = self.distance(lonStart, latStart, numpy.array([lonEnd]), numpy.array([latEnd]))
        nptsH = 1 + int(0.5 + dist / resH)
        nptsV = 1 + int(0.5 + (elevMax - elevMin) / resV)
        lon1 = numpy.linspace(lonStart, lonEnd, nptsH)
        lat1 = numpy.linspace(latStart, latEnd, nptsH)
        elev = numpy.linspace(elevMax, elevMin, nptsV)
        distH = self.distance(lonStart, latStart, lon1, lat1)

        points = numpy.zeros((nptsH*nptsV, 3), dtype=numpy.float64)
        points[:,0] = numpy.repeat(lat1, nptsV)
        points[:,1] = numpy.repeat(lon1, nptsV)
        points[:,2] = numpy.array(elev.tolist()*nptsH)
        return (points, distH, elev)

    def distance(self, refLon, refLat, ptsLon, ptsLat):
        """Get great circle distance in meters from reference point to points.

        Source: https://en.wikipedia.org/wiki/Great-circle_distance

        :type refLon: float
        :param refLon: Longitude of reference point in degrees.

        :type refLat: float
        :param refLat: Latitude of reference point in degrees.

        :type ptsLon: Numpy array
        :param ptsLon: Longitude of points in degrees.

        :type ptsLat: Numpy array
        :param ptsLat: Latitude of points in degrees.
        """
        EARTH_MEAN_RADIUS_M = 6371.0e+3
        DEG_TO_RAD = numpy.pi / 180.0

        refLonR = refLon * DEG_TO_RAD
        refLatR = refLat * DEG_TO_RAD
        ptsLonR = ptsLon * DEG_TO_RAD
        ptsLatR = ptsLat * DEG_TO_RAD

        p = numpy.sin(0.5*(ptsLatR-refLatR))**2 \
            + numpy.cos(refLatR)*numpy.cos(ptsLatR)*numpy.sin(0.5*(ptsLonR-refLonR))**2
        return EARTH_MEAN_RADIUS_M * 2.0*numpy.arcsin(p**0.5)

    def _create_basemap(self, ax, tiler, tiler_zoom):
        ax.add_image(tiler, tiler_zoom, zorder=0, cmap="gray")

        tick_spacing_deg = self.config.getfloat("maps", "tick_spacing_deg")
        gridlines = ax.gridlines(crs=crs.PlateCarree(), draw_labels=True, linewidth=0.5, alpha=0.2)
        gridlines.top_labels = False
        gridlines.bottom_labels = True
        gridlines.left_labels = True
        gridlines.right_labels = False
        gridlines.xformatter = LONGITUDE_FORMATTER
        gridlines.yformatter = LATITUDE_FORMATTER
        gridlines.xlocator = ticker.MultipleLocator(tick_spacing_deg)
        gridlines.ylocator = ticker.MultipleLocator(tick_spacing_deg)
        return

    def plot_hslice(self, depth):
        MARGINS = ((0.45, 0.45, 0.05), (0.30, 0, 0.25))

        res = self.config.getfloat("horiz_slices", "resolution_deg")

        domain = self.config.get("domain", "label")
        filename = os.path.join("data", f"{self.model}-{domain}-hslice-{depth}.txt")
        data = numpy.loadtxt(filename)

        latMin, latMax = numpy.min(data[:,0]), numpy.max(data[:,0])
        lonMin, lonMax = numpy.min(data[:,1]), numpy.max(data[:,1])
        data_extent = (lonMin, lonMax, latMin, latMax)
        numLon = 1 + int(0.5 + (lonMax - lonMin) / res)
        numLat = 1 + int(0.5 + (latMax - latMin) / res)
        gridShape = (numLat, numLon)

        lat = data[:,0].reshape(gridShape)
        lon = data[:,1].reshape(gridShape)
        elev = data[:,2].reshape(gridShape)
        data = numpy.ma.masked_values(data[:,3:], NODATA_VALUE)
        data = data.reshape(gridShape[0], gridShape[1], -1)

        tiler_path = self.config.get("maps", "tiler").split(".")
        tiler_obj = getattr(import_module(".".join(tiler_path[:-1])), tiler_path[-1])
        tiler_style = self.config.get("maps", "tiler_style")
        tiler_zoom = self.config.getint("maps", "zoom_level")
        tiles_dir = self.config.get("maps", "tiler_cache_dir")
        tiler = cached_tiler.CachedTiler(tiler_obj(desired_tile_form="L", style=tiler_style), cache_dir=tiles_dir)
        map_extent = (
            self.config.getfloat("horiz_slices", "longitude_min"),
            self.config.getfloat("horiz_slices", "longitude_max"),
            self.config.getfloat("horiz_slices", "latitude_min"),
            self.config.getfloat("horiz_slices", "latitude_max"),
            )
        
        values = _config_get_list(self.config.get("plots", "values"))
        for ivalue, value in enumerate(values):
            figWidthIn = self.config.getfloat("maps", "width_in")
            figHeightIn = self.config.getfloat("maps", "height_in")
            figure = pyplot.figure(figsize=(figWidthIn, figHeightIn), dpi=300)
            rectFactory = matplotlib_extras.axes.RectFactory(figure, margins=MARGINS)

            ax = figure.add_axes(rectFactory.rect(), projection=tiler.crs)
            ax.set_extent(map_extent)
            self._create_basemap(ax, tiler, tiler_zoom)
            self._plot_map_value(ax, data[:, :, ivalue], data_extent, value, show_colorbar=True)

            domain = self.config.get("domain", "label")
            filename = f"{self.model}-{domain}-hslice-{depth}_{value}.jpg"
            figure.savefig(os.path.join("plots", filename), pad_inches=0.02)
            pyplot.close(figure)
        
    def _plot_map_value(self, ax, data, data_extent, value, show_colorbar=False):
        COLORBAR_AXES = [0.15, 0.10, 0.02, 0.33]

        data_crs = crs.PlateCarree()
        if value in ["fault_block_id", "zone_id"]:
            norm = colors.Normalize(vmin=self.config.getfloat(value, "min"),
                                        vmax = self.config.getfloat(value, "max"))
        else:
            norm = colors.LogNorm(vmin=self.config.getfloat(value, "min"),
                                      vmax = self.config.getfloat(value, "max"))
        im = ax.imshow(data, norm=norm, extent=data_extent, transform=data_crs, origin="lower", cmap=self.cmap, alpha=0.5, zorder=2)

        #ax.set_title(label)
        if show_colorbar:
            cbax = ax.figure.add_axes(COLORBAR_AXES)
            formatter = ticker.FormatStrFormatter("%.0f")
            contours = list(map(float, _config_get_list(self.config.get(value, "contours"))))
            colorbar = pyplot.colorbar(im, cax=cbax, ticks=contours, format=formatter)
            colorbar.set_label(self.config.get(value, "label"))
        
    def plot_vslice(self, slice_loc):
        MARGINS = ((0.70, 0.0, 0.9), (0.45, 0.55, 0.20))

        (points, distH, elev) = self.vslice_points(slice_loc)
        numH = distH.shape[-1]
        numV = elev.shape[-1]
        grid_shape = (numH, numV)
        distH *= 1.0e-3
        elev *= 1.0e-3
        data_extent = [numpy.min(distH), numpy.max(distH), numpy.min(elev), numpy.max(elev)]

        domain = self.config.get("domain", "label")
        filename = os.path.join("data", f"{self.model}-{domain}-vslice-{slice_loc}.txt")
        data = numpy.loadtxt(filename)
        data = numpy.ma.masked_values(data[:,3:], NODATA_VALUE)
        data = data.reshape(grid_shape[0], grid_shape[1], -1)

        values = _config_get_list(self.config.get("plots", "values"))
        for ivalue, value in enumerate(values):
            figWidthIn = self.config.getfloat("profiles", "width_in")
            figHeightIn = self.config.getfloat("profiles", "height_in")
            figure = pyplot.figure(figsize=(figWidthIn, figHeightIn), dpi=300)
            rectFactory = matplotlib_extras.axes.RectFactory(figure, margins=MARGINS)

            ax = figure.add_axes(rectFactory.rect())
            self._plot_profile_value(ax, data[:, :, ivalue], data_extent, value)

            filename = f"{self.model}-{domain}-vslice-{slice_loc}_{value}.jpg"
            figure.savefig(os.path.join("plots", filename), pad_inches=0.02)
            pyplot.close(figure)

    def plot_vslice_icon(self, slice_loc):
        MARGINS = ((0, 0, 0), (0, 0, 0))

        tiler_path = self.config.get("maps", "tiler").split(".")
        tiler_obj = getattr(import_module(".".join(tiler_path[:-1])), tiler_path[-1])
        tiler_style = "terrain-background"
        tiler_zoom = self.config.getint("maps", "zoom_level") - 1
        tiles_dir = self.config.get("maps", "tiler_cache_dir")
        tiler = cached_tiler.CachedTiler(tiler_obj(desired_tile_form="RGB", style=tiler_style), cache_dir=tiles_dir)

        lonStart = self.config.getfloat(slice_loc, "longitude_start")
        lonEnd = self.config.getfloat(slice_loc, "longitude_end")
        latStart = self.config.getfloat(slice_loc, "latitude_start")
        latEnd = self.config.getfloat(slice_loc, "latitude_end")
        map_extent = (-124.75, -119.15, 36.0, 39.75)
        #map_extent = (-126.32, -118.98, 35.04, 41.46)

        figWidthIn = 0.5*self.config.getfloat("maps", "width_in")
        figHeightIn = 0.5*self.config.getfloat("maps", "height_in")
        figure = pyplot.figure(figsize=(figWidthIn, figHeightIn), dpi=300)
        rectFactory = matplotlib_extras.axes.RectFactory(figure, margins=MARGINS)
        ax = figure.add_axes(rectFactory.rect(), projection=tiler.crs)
        ax.set_extent(map_extent)
        ax.add_image(tiler, tiler_zoom, zorder=0)
        ax.plot((lonStart, lonEnd), (latStart, latEnd), "r-", lw=3, transform=crs.Geodetic(), zorder=5)

        domain = self.config.get("domain", "label")
        filename = f"{domain}-vslice-{slice_loc}_icon.jpg"
        figure.savefig(os.path.join("plots", filename), pad_inches=0.02)
        pyplot.close(figure)

            
    def _plot_profile_value(self, ax, data, data_extent, value, show_colorbar=True):
        COLORBAR_AXES = [0.88, 0.15, 0.015, 0.7]        

        ax.set_aspect("equal")
        norm = colors.LogNorm(vmin=self.config.getfloat(value, "min"), vmax=self.config.getfloat(value, "max"))
        im = ax.imshow(data.T, norm=norm, extent=data_extent, origin="upper", cmap=self.cmap, zorder=2)

        ax.set_xlabel("Distance (km)")
        ax.set_ylabel("Elevation (km)")
        ax.tick_params("both", direction="out")
        ax.spines["top"].set_visible(False)
        ax.spines["right"].set_visible(False)
        ax.xaxis.set_ticks_position("bottom")
        ax.yaxis.set_ticks_position("left")
        ax.xaxis.set_major_locator(ticker.MultipleLocator(20.0))
        ax.xaxis.set_minor_locator(ticker.MultipleLocator(5.0))
        ax.yaxis.set_major_locator(ticker.MultipleLocator(10.0))
        ax.yaxis.set_minor_locator(ticker.MultipleLocator(5.0))


        if show_colorbar:
            cbax = ax.figure.add_axes(COLORBAR_AXES)
            formatter = ticker.FormatStrFormatter("%.0f")
            contours = list(map(float, _config_get_list(self.config.get(value, "contours"))))
            colorbar = pyplot.colorbar(im, cax=cbax, ticks=contours, format=formatter)
            colorbar.set_label(self.config.get(value, "label"))
        
    def initialize(self, filename):
        """Set parameters from config file.

        :type config_filename: str
        :param config_filename: Name of configuration (INI) file with parameters.
        """
        import configparser
        config = configparser.ConfigParser()
        if self.showProgress:
            print("Fetching parameters from {}...".format(filename))
        config.read(filename)
        self.config = config
        return
    
    def show_parameters(self):
        """Write parameters to stdout.
        """
        import sys
        self.config.write(sys.stdout)
        return

    def _parseCommandLine(self):
        """Parse command line arguments.
        """
        import argparse

        parser = argparse.ArgumentParser()
        parser.add_argument("--config", action="store", dest="config", required=True)
        parser.add_argument("--model", action="store", dest="model", required=True)

        parser.add_argument("--query-hslices", action="store_true", dest="query_hslices")
        parser.add_argument("--query-vslices", action="store_true", dest="query_vslices")
        parser.add_argument("--plot-hslices", action="store_true", dest="plot_hslices")
        parser.add_argument("--plot-vslices", action="store_true", dest="plot_vslices")

        parser.add_argument("--show-parameters", action="store_true", dest="show_parameters")
        parser.add_argument("--quiet", action="store_false", dest="show_progress", default=True)
        parser.add_argument("--debug", action="store_true", dest="debug", default=True)
        return parser.parse_args()

# ======================================================================
if __name__ == "__main__":
    SlicesApp().main()


# End of file
