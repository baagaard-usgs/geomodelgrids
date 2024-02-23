#!/usr/bin/env python3
"""Python script analogous to geomodelgrids_borehole. This script provides additional
functionality by querying for vertical profiles at multiple sites, which are specified
in a CSV file.

In this case, we associate the CSV file with seismic stations, so we require the CSV
file to include the columns 'Network', 'StationCode', 'StationLatitude', and
'StationLongitude'. The script will ignore any additional columns.

Output will be one file per site with columns of `Elevation`, `Depth` and the query
values with the name of each site of the form `{NetworkCode}.{StationCode}.{Format}`,
where `Format` is `csv` (comma separated values) or `txt` (whitespace separated values
with `#` as first character of the header line).
"""

import pathlib
import logging
import csv

import numpy

import geomodelgrids


class App:
    """Application for querying for properties along vertical profiles at seismic stations."""

    CRS = "EPSG:4326"

    def __init__(self):
        """Constructor."""
        self.query = None
        self.max_depth_m = 25.0e+3
        self.resolution_m = 25.0
        self.values = []

    def initialize(self,
                   models: list,
                   max_depth_m: float,
                   resolution_m: float,
                   values: tuple = ("Density", "Vp", "Vs", "Qp", "Qs")):
        """Initialize query.

        :param models: List of GeoModelGrids files (in query order).
        :param max_depth_m: Maximum depth (meters) of vertical profiles.
        :param resolution_m: Resolution (meters) of vertical profiles.
        :param values: Values to return in queries.
        """
        self.query = geomodelgrids.Query()
        self.query.initialize(models, values, self.CRS)
        self.max_depth_m = max_depth_m
        self.resolution_m = resolution_m
        self.values = values

    def run_query(self, filename: str, output_dir: pathlib.Path, output_format: str):
        """Run query reading locations from CSV file 'filename' and writing results
        to `output_dir` in format `output_format`.

        :param filename: CSV file with locations of seismic stations.
        :param output_dir: Name of directory where output is written.
        :param output_format: Output format (`csv` or `txt`).
        """
        logger = logging.getLogger(__name__)
        write = self._write_csv if output_format == "csv" else self._write_txt

        locations = self._load_locations(filename)

        output_dir.mkdir(exist_ok=True)
        for row in locations:
            name = f"{row['Network']}.{row['StationCode']}"
            logger.info("Working on %s...", name)
            names, data = self._query(row["StationLatitude"], row["StationLongitude"])
            if data is None:
                logger.info("No data for %s. Skipping profile.", name)
                continue
            output_filepath = output_dir / f"{name}.{output_format}"
            write(output_filepath, names, data)

        self.query.finalize()
        self.query = None

    def _load_locations(self, filename) -> list:
        """Read locations of seismic stations from CSV file.

        :param filename: CSV file with seismic station information.
        :returns: List of seismic stations.
        """
        data = []
        with open(filename, encoding="utf-8") as csvfile:
            reader = csv.DictReader(csvfile, delimiter=",")
            for row in reader:
                data.append(row)
        return data

    def _query(self, latitude, longitude) -> numpy.ndarray:
        """Run vertical profile query for a single site at latitude and longitude.

        :param latitude: Latitude (degrees, WGS84) of site.
        :param longitude: Longitude (degrees, WGS84) of site.
        :returns: Array with elevation, depth, and profile values.
        """
        point = numpy.array([[latitude, longitude]])
        ground_surf = self.query.query_top_elevation(point)[0]
        if ground_surf == geomodelgrids.Query.NODATA_VALUE:
            return (None, None)
        if ground_surf != 0.0:
            # Account for roundoff errors and drop point a negligible amount.
            ground_surf -= 1.0e-6

        # Create a vertical profile of points.
        min_elev = ground_surf-self.max_depth_m-0.5*self.resolution_m
        elev = numpy.arange(ground_surf, min_elev, -self.resolution_m)
        depth = ground_surf - elev
        points = numpy.zeros((len(elev), 3))
        points[:, 0] = latitude
        points[:, 1] = longitude
        points[:, 2] = elev

        # Query for points in vertical profile and then package values into a NumPy array.
        values = self.query.query(points)[0]
        return self._package_values(elev, depth, values)

    def _package_values(self, elev, depth, values) -> tuple:
        """Package values into a single NumPy array.

        :param elev: Array of elevation values.
        :param depth: Array of depth values.
        :param values: Array of query values.
        :returns: Names and array of values.
        """
        names = ["Elevation", "Depth"] + list(self.values)
        data = numpy.stack([elev, depth] + list(values.T)).T
        return (names, data)

    def _write_csv(self, filename: str, names: list, data: numpy.ndarray):
        """Write data to CSV file.

        :param filename: Name of output file.
        :param names: List of names of data fields.
        :param data: Array of data fields.
        """
        with open(filename, "w", encoding="utf-8") as csvfile:
            writer = csv.writer(csvfile)
            writer.writerow(names)
            for row in data:
                writer.writerow([f"{col:.2f}" for col in row])

    def _write_txt(self, filename, names, data):
        """Write data to text file (compatible with numpy.loadtxt).

        :param filename: Name of output file.
        :param names: List of names of data fields.
        :param data: Array of data fields.
        """
        with open(filename, "w", encoding="utf-8") as fout:
            header = "# " + " ".join(names) + "\n"
            fout.write(header)
            numpy.savetxt(fout, data)


def cli():
    """Command line interface."""
    import argparse

    DESCRIPTION = "Application for running linear mixed effects regression on ground-motion records."

    parser = argparse.ArgumentParser(description=DESCRIPTION,
                                     formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("--models", action="store", dest="models", required=True,
                        help="Comma separated list of GeoModelGrids files to query.")
    parser.add_argument("--filename", action="store", dest="filename_locations", required=True,
                        help="CSV file with Network, StationCode, StationLongitude, and StationLatitude.")
    parser.add_argument("--output-dir", action="store", dest="output_dir", required=True, help="Directory for output.")
    parser.add_argument("--output-format", action="store", dest="output_format", default="csv",
                        choices=("csv", "txt"), help="Vertical resoluion in queries.")

    parser.add_argument("--max-depth", action="store", dest="max_depth",
                        required=True, type=float, help="Maximum depth in kilometers.")
    parser.add_argument("--resolution", action="store", dest="resolution", required=True,
                        type=float, help="Vertical resoluion in queries.")

    # Logging options
    parser.add_argument("--log", action="store", dest="log_filename", default="station_profiles.log")
    parser.add_argument("--debug", action="store_true", dest="debug")

    args = parser.parse_args()
    if args.debug:
        logging.basicConfig(level=logging.DEBUG, filename=args.log_filename)
    else:
        logging.basicConfig(level=logging.INFO, filename=args.log_filename)

    app = App()
    app.initialize(models=args.models.split(","), max_depth_m=args.max_depth*1.0e+3, resolution_m=args.resolution)
    output_dir = pathlib.Path(args.output_dir)
    app.run_query(filename=args.filename_locations,
                  output_dir=output_dir,
                  output_format=args.output_format)


if __name__ == "__main__":
    cli()
