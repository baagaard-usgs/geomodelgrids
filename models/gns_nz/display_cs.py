#!/usr/bin/env python3

import pyproj
import numpy

CS_MODEL = "EPSG:2193"
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


def origin():
    transformer = pyproj.Transformer.from_crs(crs_to=CS_MODEL, crs_from=CS_GEO, always_xy=True)
    x_proj, y_proj = transformer.transform(ORIGIN_LL[1], ORIGIN_LL[0])
    return (x_proj, y_proj)


def convert():
    origin_x, origin_y = origin()
    
    x_proj = +pts[:,0]*numpy.cos(Y_AZIMUTH) + pts[:,1]*numpy.sin(Y_AZIMUTH)
    y_proj = -pts[:,0]*numpy.sin(Y_AZIMUTH) + pts[:,1]*numpy.cos(Y_AZIMUTH)

    x_proj += origin_x
    y_proj += origin_y

    transformer = pyproj.Transformer.from_crs(crs_from=CS_MODEL, crs_to=CS_GEO, always_xy=True)
    y_geo, x_geo = transformer.transform(x_proj, y_proj)

    return (x_geo, y_geo)


def origin_sw():
    AZ = 50.0 / 180.0*numpy.pi

    origin_x, origin_y = origin()
    
    origin_x2 = origin_x + 1200.0e+3*(-numpy.cos(AZ) + numpy.sin(AZ))
    origin_y2 = origin_y + 1200.0e+3*(-numpy.cos(AZ) - numpy.sin(AZ))
    return (origin_x2, origin_y2)
    

print("CORNERS",convert())
print("ORIGIN", origin())
print("ORIGIN_SW", origin_sw())

X_GRID = numpy.array([
      -1200.00,
       -703.00,
       -653.00,
       -632.00,
       -610.00,
       -590.00,
       -573.00,
       -554.00,
       -536.00,
       -515.00,
       -494.00,
       -468.00,
       -439.00,
       -409.00,
       -379.00,
       -349.00,
       -329.00,
       -310.00,
       -291.00,
       -272.00,
       -253.00,
       -228.00,
       -198.00,
       -173.00,
       -148.00,
       -123.00,
       -105.00,
        -90.00,
        -80.00,
        -70.00,
        -60.00,
        -50.00,
        -40.00,
        -30.00,
        -20.00,
        -10.00,
          0.00,
         10.00,
         20.00,
         30.00,
         40.00,
         55.00,
         70.00,
         85.00,
        100.00,
        115.00,
        130.00,
        145.00,
        155.00,
        163.00,
        171.00,
        179.00,
        187.00,
        197.00,
        224.00,
        269.00,
        313.00,
        358.00,
        388.00,
        418.00,
        448.00,
        477.00,
        507.00,
        537.00,
        567.00,
        600.00,
        639.00,
        686.00,
        736.00,
       1200.00,
   ])

Y_GRID = numpy.array([
  -1200.00,
   -540.00,
   -400.00,
   -300.00,
   -200.00,
   -151.00,
   -136.00,
   -121.00,
   -106.00,
    -96.00,
    -86.00,
    -76.00,
    -68.00,
    -61.00,
    -53.00,
    -46.00,
    -38.00,
    -31.00,
    -23.00,
    -16.00,
     -8.00,
     -1.00,
      7.00,
     14.00,
     22.00,
     29.00,
     37.00,
     41.00,
     44.00,
     48.00,
     51.00,
     55.00,
     58.00,
     62.00,
     65.00,
     69.00,
     72.00,
     76.00,
     79.00,
     83.00,
     86.00,
     90.00,
     93.00,
     97.00,
    100.00,
    104.00,
    107.00,
    111.00,
    114.00,
    118.00,
    121.00,
    127.00,
    133.00,
    139.00,
    145.00,
    152.00,
    161.00,
    178.00,
    205.00,
    230.00,
    260.00,
    300.00,
    350.00,
   1200.00,
   ])

Z_GRID = -numpy.array([
    -15.00,
     -1.00,
      1.00,
      3.00,
      5.00,
      8.00,
     15.00,
     23.00,
     30.00,
     34.00,
     38.00,
     42.00,
     48.00,
     55.00,
     65.00,
     85.00,
    105.00,
    130.00,
    155.00,
    185.00,
    225.00,
    275.00,
    370.00,
    620.00,
    750.00,
    ])

# Reverse x and y and put origin at SW corner
X_GRID = -X_GRID - X_GRID[-1]
Y_GRID = -Y_GRID - Y_GRID[-1]

print("x_coordinates = [" + ", ".join([f"{v:.1f}e+3" for v in sorted(X_GRID)]) + "]")
print("y_coordinates = [" + ", ".join([f"{v:.1f}e+3" for v in sorted(Y_GRID)]) + "]")
print("z_coordinates = [" + ", ".join([f"{v:.1f}e+3" for v in Z_GRID]) + "]")
