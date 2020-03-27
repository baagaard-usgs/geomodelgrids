#!/usr/bin/env python

import pyproj
import numpy

Y_AZIMUTH = 330.0 / 180.0 * numpy.pi
ORIGIN_X = 200000.0
ORIGIN_Y = -400000.0

POINTS = numpy.array([
    [34.0, -116.5],
    [34.5, -117.8],
    [35.5, -118.0],
    [37.0, -122.0],
    [38.0, -123.0],
    ])

csSrc = pyproj.Proj("EPSG:4326")
csDst = pyproj.Proj("EPSG:3311")
xG, yG = pyproj.transform(csSrc, csDst, POINTS[:,0], POINTS[:,1])
x = xG - ORIGIN_X
y = yG - ORIGIN_Y

xR = x * numpy.cos(Y_AZIMUTH) - y * numpy.sin(Y_AZIMUTH)
yR = x * numpy.sin(Y_AZIMUTH) + y * numpy.cos(Y_AZIMUTH)

print(xR, yR)
