"""CSV data source.
"""

from importlib import import_module

import numpy

from geomodelgrids.create.core.datasrc import DataSrc
from geomodelgrids.create.utils.config import string_to_list
from geomodelgrids.create.core import NODATA_VALUE


class CSVFile(DataSrc):
    """CSV file data source.
    """

    def __init__(self, config):
        """Constructor.

        Args:
            config (dict)
                Configuration parameters.
        """
        super().__init__()
        self.config = config

    def initialize(self):
        """Initialize model.
        """
        import datetime

        history = self.config["geomodelgrids"].get("history", "")
        if len(history) > 0:
            history += "\n"
        history += f"{datetime.date.today()} Converted to GeoModelGrids HDF5 file."
        self.config["geomodelgrids"]["history"] = history

    def get_top_surface(self, points):
        return None

    def get_topography_bathymetry(self, points):
        return None

    def get_values(self, block, top_surface, topo_bathy, batch=None):
        """Get values at points.

        Args:
            block (Block)
                Block information.
            top_surface (Surface)
                Elevation of top surface of model.
            topo_bathy (Surface)
                Elevation of topography or bathymetry used to define depth.
            batch (BatchGenerator3D)
                Current batch of points in block.
        """
        def _varxy_index_fn(coordinates):
            x_grid = numpy.array(coordinates)
            x_grid.sort()
            x_round = -int(numpy.log10(numpy.min(numpy.diff(x_grid))))
            return x_grid.searchsorted(round(x, x_round))

        if batch:
            raise NotImplementedError("Batches with CSV files are not implemented.")

        comment_flag = self.config["csv"].get("comment_flag", "#")
        csv_data = numpy.loadtxt(self.config["csv"]["filename"], comments=comment_flag)
        model_x, model_y, model_z = self._to_model_xyz(csv_data)

        value_names = string_to_list(self.config["data"]["values"])
        value_columns = [int(self.config["csv.columns"][value]) for value in value_names]

        if block.x_resolution:
            def get_xindex(x, block): return round(x / block.x_resolution)
        else:
            def get_xindex(x, block): return _varxy_index_fn(block.x_coordinates)
        if block.y_resolution:
            def get_yindex(y, block): return round(y / block.y_resolution)
        else:
            def get_yindex(y, block): return _varxy_index_fn(block.y_coordinates)
        if block.z_resolution:
            def get_zindex(z, block): return round(z / block.z_resolution)
        else:
            z_grid = numpy.array(block.z_coordinates)
            z_grid.sort()
            z_round = -int(numpy.log10(numpy.min(numpy.diff(z_grid))))
            def get_zindex(z, block): return len(z_grid) - 1 - z_grid.searchsorted(round(z, z_round))

        nx, ny, nz = block.get_dims()
        values = NODATA_VALUE * numpy.ones((nx, ny, nz, len(value_names)), dtype=numpy.float32)
        for row, x, y, z in zip(csv_data, model_x, model_y, model_z):
            ix = get_xindex(x, block)
            iy = get_yindex(y, block)
            iz = get_zindex(z, block)
            row_values = row[value_columns]
            values[ix, iy, iz, :] = row_values
        return values

    def _to_model_xyz(self, csv_data):
        import pyproj

        transformer = pyproj.Transformer.from_crs(
            crs_to=self.config["coordsys"]["crs"], crs_from=self.config["csv"]["crs"], always_xy=True)

        ix = int(self.config["csv.columns"]["x"])
        iy = int(self.config["csv.columns"]["y"])
        iz = int(self.config["csv.columns"]["z"])
        (tmp_x, tmp_y, model_z) = transformer.transform(csv_data[:, ix], csv_data[:, iy], csv_data[:, iz])

        origin_x = float(self.config["coordsys"]["origin_x"])
        origin_y = float(self.config["coordsys"]["origin_y"])
        azimuth_rad = float(self.config["coordsys"]["y_azimuth"]) / 180.0 * numpy.pi
        model_x = (tmp_x-origin_x)*numpy.cos(azimuth_rad) - (tmp_y-origin_y)*numpy.sin(azimuth_rad)
        model_y = (tmp_x-origin_x)*numpy.sin(azimuth_rad) + (tmp_y-origin_y)*numpy.cos(azimuth_rad)

        return (model_x, model_y, model_z)


# End of file
