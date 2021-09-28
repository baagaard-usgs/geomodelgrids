#!/usr/bin/env python3

import pyproj
import numpy

CS_UTM = "+proj=utm +zone=59 +south +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs "
CS_TMERC = "+proj=tmerc +lat_0=0 +lon_0=171 +k=0.9996 +x_0=500000 +y_0=10000000 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs "
ORIGIN_UTM = (-339716.6068850297, 5326897.027243269)
ORIGIN_TMERC = (-339716.6068850297, 5326897.027243269)

#CS_UTM = "+proj=tmerc +lat_0=0 +lon_0=173 +k=0.9996 +x_0=1600000 +y_0=10000000 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs "
#CS_TMERC = "EPSG:2193"
#ORIGIN_UTM = (1591995.237075493, 5376443.768789517)
#ORIGIN_TMERC = (1591995.237075493, 5376443.768789517)

CS_GEO = "EPSG:4326"

Y_AZIMUTH = 310.0 / 180.0 * numpy.pi
ORIGIN_LL = (-41.7638, 172.9037)


pts = numpy.array([
    [-1200.0e+3, -1200.0e+3],
    [-1200.0e+3, +1200.0e+3],
    [+1200.0e+3, -1200.0e+3],
    [+1200.0e+3, +1200.0e+3],
    [0.0, 0.0],
    ], dtype=numpy.float64)



def convert(cs, origin):
    x_proj = +pts[:,0]*numpy.cos(Y_AZIMUTH) + pts[:,1]*numpy.sin(Y_AZIMUTH)
    y_proj = -pts[:,0]*numpy.sin(Y_AZIMUTH) + pts[:,1]*numpy.cos(Y_AZIMUTH)

    x_proj += origin[0]
    y_proj += origin[1]

    transformer = pyproj.Transformer.from_crs(crs_from=cs, crs_to=CS_GEO, always_xy=True)
    y_geo, x_geo = transformer.transform(x_proj, y_proj)

    return (x_geo, y_geo)

print("UTM",convert(CS_UTM, ORIGIN_UTM))
print("TMERC",convert(CS_TMERC, ORIGIN_TMERC))

def origin(cs):
    transformer = pyproj.Transformer.from_crs(crs_to=cs, crs_from=CS_GEO, always_xy=True)
    x_proj, y_proj = transformer.transform(ORIGIN_LL[1], ORIGIN_LL[0])
    return (x_proj, y_proj)

#print("ORIGIN")
#print("UTM", origin(CS_UTM))
#print("TMERC", origin(CS_TMERC))

