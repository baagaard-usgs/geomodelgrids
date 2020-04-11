#!/usr/bin/env python

import pyproj
import numpy

from generate import OneBlockFlat
from generate import ThreeBlocksFlat


def printBoundingBox(model):
    csBB = pyproj.Proj("EPSG:4326")

    x0 = model["origin_x"]
    y0 = model["origin_y"]
    dx = model["dim_x"]
    dy = model["dim_y"]
    y_azimuth = model["y_azimuth"] / 180.0 * numpy.pi

    xyModel = numpy.array([
        [0.0, 0.0],
        [dx, 0.0],
        [dx, dy],
        [0.0, dy]
    ])

    xProj = xyModel[:, 0] * numpy.cos(y_azimuth) + xyModel[:, 1] * numpy.sin(y_azimuth)
    yProj = -xyModel[:, 0] * numpy.sin(y_azimuth) + xyModel[:, 1] * numpy.cos(y_azimuth)
    xProj += model["origin_x"]
    yProj += model["origin_y"]

    xGeo, yGeo = pyproj.transform(model["crs"], csBB, xProj, yProj)

    print("latitude, longitude, title")
    labels = ["A", "B", "C", "D"]
    for xi, yi, ti in zip(xGeo, yGeo, labels):
        print("{:.4f}, {:.4f}, {}".format(xi, yi, ti))


if __name__ == "__main__":
    print("One Block")
    printBoundingBox(OneBlockFlat().model)

    print("Three Blocks")
    printBoundingBox(ThreeBlocksFlat().model)

# End of file
