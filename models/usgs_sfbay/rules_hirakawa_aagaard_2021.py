"""Rules for defining elastic properties given x, y, depth as given in
Hirakawa and Aagaard (20XX).

Hirakawa and Aagaard, B. T. (20XX), , Bulletin of the Seismological Society of
America, XXX(X), XXXX-XXXX, doi: 10.1785/XXXX.

The x and y coordinates are in the *unrotated* model coordinate system in units of m.
That is, the xy coordinates match the model CRS specified in the model configuration.

The rules were originally developed with Vp and Vs in km/s, density in
g/cm**3, and depth in km. Here all rules have been converted to SI
base units with Vp and Vs in m/s, density in kg/m**3, and depth in m.
"""

import math
from geomodelgrids.create.core import NODATA_VALUE
from rules_aagaard_etal_2010 import (
    default_vs,
    default_density,
    default_qp,
    default_qs)


MODEL_YAZIMUTH = 323.638


def compute_xy_refpoints():
    """Print coordinates of reference points in model coordinate system for subdividing fault blocks.

    These coordinates are used in the functions below.
    """
    REFERENCE_POINTS = {
        "valley_sequence_sanleandro": (37.57, -121.95),
        "franciscan_napa": (38.25, -122.28),
        "cenozoic_napa": (37.96, -122.07),
        "franciscan_sonoma": (38.25, -122.46),
        "cenozoic_sonoma": (38.25, -122.46),
    }
    import pyproj
    cs_geo = pyproj.CRS("EPSG:4326")
    cs_model = pyproj.CRS('PROJCS["unnamed",GEOGCS["NAD83",DATUM["North_American_Datum_1983",SPHEROID["GRS 1980",6378137,298.257222101,AUTHORITY["EPSG","7019"]],TOWGS84[0,0,0,0,0,0,0],AUTHORITY["EPSG","6269"]],PRIMEM["Greenwich",0,AUTHORITY["EPSG","8901"]],UNIT["degree",0.0174532925199433,AUTHORITY["EPSG","9122"]],AUTHORITY["EPSG","4269"]],PROJECTION["Transverse_Mercator"],PARAMETER["latitude_of_origin",35],PARAMETER["central_meridian",-123],PARAMETER["scale_factor",0.9996],PARAMETER["false_easting",0],PARAMETER["false_northing",0],UNIT["Meter",1]]')
    transformer = pyproj.Transformer.from_crs(cs_geo, cs_model)
    for name, (lon, lat) in REFERENCE_POINTS.items():
        x, y = transformer.transform(lon, lat)
        print(f"{name} (x0, y0) = ({x:.1f}, {y:.1f})")


def is_along_azimuth(x, y, x0, y0, azimuth):
    """Check if point is along azimuth from reference point.

    Args:
        x (float)
            Model x coordinate.
        y (float)
            Model y coordinate.
        x0 (float)
            X coordinate of reference point in model coordinate system.
        y0 (float)
            Y coordinate of reference point in model coordinate system.
        azimuth (float)
            Azimuth, in degrees CW, from y axis of model coordinate system.
    Returns: (bool)
        True if azimuth \dot (x-x0,y-y0) >= 0, False otherwize
    """
    azRad = azimuth / 180.0 * math.pi
    return (x-x0)*math.sin(azRad) + (y-y0)*math.cos(azRad) >= 0.0


def brocher2008_great_valley_sequence(x, y, depth):
    """Rule for elastic properties in Great Valley Sequence rocks. Brocher 2008.

    Args:
        x(float)
            Model x coordinate.
        y(float)
            Model y coordinate.
        depth(float)
            Depth of location in m.

    Returns:
        Tuple of density(kg/m**3), Vp(m/s), Vs(m/s), Qp, and Qs
    """
    if depth < 4.0e+3:
        vp = 2.75e+3 + 0.4725*depth
    elif depth < 7.0e+3:
        vp = 4.64e+3 + 0.3*(depth-4.0e+3)
    else:
        vp = 5.54e+3 + 0.06*(depth-7.0e+3)

    vs = default_vs(depth, vp)
    density = default_density(depth, vp)
    qs = default_qs(depth, vs)
    qp = default_qp(depth, qs)
    return (density, vp, vs, qp, qs)


def brocher2005_older_cenozoic_sedimentary(x, y, depth):
    """Rule for elastic properties in older Cenozoic sedimentary rocks. Brocher 2005.

    Args:
        x(float)
            Model x coordinate.
        y(float)
            Model y coordinate.
        depth(float)
            Depth of location in m.

    Returns:
        Tuple of density(kg/m**3), Vp(m/s), Vs(m/s), Qp, and Qs
    """
    if depth < 4.0e+3:
        vp = 2.24e+3 + 0.6*depth
    elif depth < 7.0e+3:
        vp = 4.64e+3 + 0.3*(depth-4.0e+3)
    else:
        vp = 5.54e+3 + 0.06*(depth-7.0e+3)

    vs = default_vs(depth, vp)
    density = default_density(depth, vp)
    qs = default_qs(depth, vs)
    qp = default_qp(depth, qs)
    return (density, vp, vs, qp, qs)


def franciscan_napa_sonoma(x, y, depth):
    """Rule for elastic properties in Franciscan(Napa-Sonoma variety) rocks.

    Args:
        x(float)
            Model x coordinate.
        y(float)
            Model y coordinate.
        depth(float)
            Depth of location in m.

    Returns:
        Tuple of density(kg/m**3), Vp(m/s), Vs(m/s), Qp, and Qs
    """
    a = 0.03e+3
    vp0 = 5.4e+3 + a
    density0 = 1.74e+3 * (vp0*1.0e-3)**0.25

    if depth < 1.0e+3:
        vp = a + 2.5e+3 + 2.0*depth
    elif depth < 3.0e+3:
        vp = a + 4.5e+3 + 0.45*(depth-1.0e+3)
    else:
        vp = a + 5.4e+3 + 0.0588*(depth-3.0e+3)

    vs = default_vs(depth, vp)
    density = density0 if depth < 3.0e+3 else default_density(depth, vp)
    qs = 13.0 if vs < 300.0 else default_qs(depth, vs)
    qp = default_qp(depth, qs)
    return (density, vp, vs, qp, qs)


def quaternary_livermore(x, y, depth):
    """
    Rule for elastic properties in Shallow livermore sediments obtained by trial and error

    Args:
        x(float)
            Model x coordinate.
        y(float)
            Model y coordinate.
        depth(float)
            Depth of location in m.

    Returns:
        Tuple of density(kg/m**3), Vp(m/s), Vs(m/s), Qp, and Qs
    """
    if depth < 6444.44:
        vp = 1.64e+3 + 0.6*depth
    else:
        vp = 5.506667e+3 + 0.06*(depth-6444.44)

    vs = default_vs(depth, vp)
    density = default_density(depth, vp)
    qs = default_qs(depth, vs)
    qp = default_qp(depth, qs)
    return (density, vp, vs, qp, qs)


def cenozoic_walnutcreek(x, y, depth):
    """Rule for elastic properties in southernmost part of Napa Block

    * above 7km, same as 'tertiary_sedimentary_southbay' from aagaard_etal_2010.py
    * below 7km, same as Brocher GVS and OlderCenozoicSeds

    Args:
        x(float)
            Model x coordinate.
        y(float)
            Model y coordinate.
        depth(float)
            Depth of location in m.

    Returns:
        Tuple of density(kg/m**3), Vp(m/s), Vs(m/s), Qp, and Qs
    """
    if depth < 750.0:
        vp = 1.80e+3 + 1.2*depth
    elif depth < 4.0e+3:
        vp = 2.70e+3 + 0.597*(depth-750.0)
    elif depth < 7.0e+3:
        vp = 4.64e+3 + 0.3*(depth-4.0e+3)
    else:
        vp = 5.54e+3 + 0.06*(depth-7.0e+3)

    vs = 500.0 + 0.4*depth if depth < 50.0 else default_vs(depth, vp)
    density = default_density(depth, vp)
    qs = 13.0 if vs < 300.0 else default_qs(depth, vs)
    qp = default_qp(depth, qs)
    return (density, vp, vs, qp, qs)


def valley_sequence_sanleandro(x, y, depth):
    """Rule for elastic properties in zone 'Valley Sequence', block 'San Leandro'

    Args:
        x(float)
            Model x coordinate.
        y(float)
            Model y coordinate.
        depth(float)
            Depth of location in m.

    Returns:
        Tuple of density(kg/m**3), Vp(m/s), Vs(m/s), Qp, and Qs
    """
    (x0, y0) = (92724.2, 285582.4)
    if is_along_azimuth(x, y, x0, y0, 323.638):
        return brocher2008_great_valley_sequence(x, y, depth)
    else:
        return brocher2005_older_cenozoic_sedimentary(x, y, depth)


def franciscan_napa(x, y, depth):
    """Rule for elastic properties in Franciscan rock, Napa Block

    Args:
        x(float)
            Model x coordinate.
        y(float)
            Model y coordinate.
        depth(float)
            Depth of location in m.

    Returns:
        Tuple of density(kg/m**3), Vp(m/s), Vs(m/s), Qp, and Qs
    """
    (x0, y0) = (62999.3, 360755.6)
    if is_along_azimuth(x, y, x0, y0, 323.638):
        return franciscan_napa_sonoma(x, y, depth)
    else:
        return brocher2008_great_valley_sequence(x, y, depth)


def cenozoic_napa(x, y, depth):
    """Rule for elastic properties in Cenozoic zone, Napa Block

    Args:
        x(float)
            Model x coordinate.
        y(float)
            Model y coordinate.
        depth(float)
            Depth of location in m.

    Returns:
        Tuple of density(kg/m**3), Vp(m/s), Vs(m/s), Qp, and Qs
    """
    (x0, y0) = (81696.7, 328741.7)
    if is_along_azimuth(x, y, x0, y0, 323.638):
        return brocher2005_older_cenozoic_sedimentary(x, y, depth)
    else:
        return cenozoic_walnutcreek(x, y, depth)


def franciscan_sonoma(x, y, depth):
    """Rule for elastic properties of Franciscan zone, Sonoma Block

    Args:
        x(float)
            Model x coordinate.
        y(float)
            Model y coordinate.
        depth(float)
            Depth of location in m.

    Returns:
        Tuple of density(kg/m**3), Vp(m/s), Vs(m/s), Qp, and Qs
    """
    (x0, y0) = (47249.3, 360648.4)
    if is_along_azimuth(x, y, x0, y0, 323.638):
        return franciscan_napa_sonoma(x, y, depth)
    else:
        return brocher2008_great_valley_sequence(x, y, depth)


def cenozoic_sonoma(x, y, depth):
    """Rule for elastic properties of Cenozoic zone, Sonoma Block

    Args:
        x(float)
            Model x coordinate.
        y(float)
            Model y coordinate.
        depth(float)
            Depth of location in m.

    Returns:
        Tuple of density(kg/m**3), Vp(m/s), Vs(m/s), Qp, and Qs
    """
    (x0, y0) = (47249.3, 360648.4)
    if is_along_azimuth(x, y, x0, y0, 323.638):
        return brocher2008_great_valley_sequence(x, y, depth)
    else:
        return brocher2005_older_cenozoic_sedimentary(x, y, depth)
