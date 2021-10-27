"""Rules for defining elastic properties given x, y, depth as given in
the electronic supplement of Aagaard et al. (2010) with some minor corrections.
http://www.seismosoc.org/Publications/BSSA_html/bssa_100-6/2009379-esupp/velocitymodel.html

Aagaard, B. T., R. W. Graves, A. Rodgers, T. M. Brocher,
R. W. Simpson, D. Dreger, N. A. Petersson, S. C. Larsen, S. Ma, and
R. C. Jachens (2010, December), Ground motion modeling of Hayward
fault scenario earthquakes, Part II: Simulation of long-period and
broadband ground motions, Bulletin of the Seismological Society of
America, 100(6), 2945-2977, doi: 10.1785/0120090379.

Corrections:

  1. Fix typos in Vp equation for Franciscan for d >= 3.0 (0.0. -> 0.)

  2. Correct coefficient (0.00064 -> 0.0064) for 4th order term in
  generic Vs equation.

  3. Correct coefficient (4.46 -> 4.64) for constant term in Tertiary
  sedimentary rocks (South Bay) and Cenozoic sedimentary rocks (Half
  Moon Bay region) for Vp equation 4 < d < 7.0.

  4. For density of Mafic and Great Valley ophiolite, use Vp0 if d <
  10.0, otherwise use same equation with Vp0 replaced with Vp(d).

  5. For density of Franciscan (Foothills, Napa-Sonoma, and Berkeley),
  use density0 if d < 3.0, otherwise use generic density.

The rules were originally developed with Vp and Vs in km/s, density in
g/cm**3, and depth in km. Here all rules have been converted to SI
base units with Vp and Vs in m/s, density in kg/m**3, and depth in m.

"""

from geomodelgrids.create.core import NODATA_VALUE


def default_vs(depth, vp):
    """Default rule for shear wave speed as a function of Vp.

    Args:
        depth (float)
            Depth of location in m.
        vp (float)
            P wave speed in m/s.

    Returns:
        S wave speed in m/s.
    """
    return 785.8 - 1.2344*vp + 0.7949e-3*vp**2 - 0.1238e-6*vp**3 + 0.0064e-9*vp**4


def default_density(depth, vp):
    """Default rule for density as a function of Vp.

    Args:
        depth (float)
            Depth of location in m.
        vp (float)
            P wave speed in m/s.

    Returns:
        Density in kg/m**3.
    """
    return 1.74e+3 * (vp*1.0e-3)**0.25


def default_qs(depth, vs):
    """Default rule for Qs as a function of Vs.

    Args:
        depth (float)
            Depth of location in m.
        vs (float)
            S wave speed in m/s.

    Returns:
        Quality factor for S wave.
    """
    return -16.0 + 104.13e-3*vs - 25.225e-6*vs**2 + 8.2184e-9*vs**3


def default_qp(depth, qs):
    """Default rule for Qp as a function of Qs.

    Args:
        depth (float)
            Depth of location in m.
        qs (float)
            Quality factor for S wave.

    Returns:
        Quality factor for P wave.
    """
    return 2.0*qs


def upper_mantle(x, y, depth):
    """Rule for elastic properties in the upper mantle.

    Args:
        x (float)
            Model x coordinate.
        y (float)
            Model y coordinate.
        depth (float)
            Depth of location in m.

    Returns:
        Tuple of density (kg/m**3), Vp (m/s), Vs (m/s), Qp, and Qs
    """
    vp = 7.77e+3 if depth < 20.0e+3 else 7.77e+3 + 0.001*(depth-20.0e+3)

    vs = 4.41e+3 if depth < 20.0e+3 else 4.41e+3 + 0.0006*(depth-20.0e+3)

    density = 3.3e+3

    qs = 13.0 if vs < 300.0 else default_qs(depth, vs)
    qp = default_qp(depth, qs)
    return (density, vp, vs, qp, qs)


def mafic_great_valley_ophiolite(x, y, depth):
    """Rule for elastic properties in Mafic Great Valley Ophiolite rocks.

    Args:
        x (float)
            Model x coordinate.
        y (float)
            Model y coordinate.
        depth (float)
            Depth of location in m.

    Returns:
        Tuple of density (kg/m**3), Vp (m/s), Vs (m/s), Qp, and Qs
    """
    vp0 = 5.9e+3

    vp = vp0 if depth < 10.0e+3 else vp0 + 0.056*(depth-10e+3)

    vs = default_vs(depth, vp)

    vd = vp0 if depth < 10.0e+3 else vp
    density = 227.0 + 1.6612*vd - 0.4721e-3*vd**2 + 0.0671e-6*vd**3 - 0.0043e-9*vd**4 + 0.0001e-12*vd**5

    qs = 13.0 if vs < 300.0 else default_qs(depth, vs)
    qp = default_qp(depth, qs)
    return (density, vp, vs, qp, qs)


def franciscan_foothills(x, y, depth):
    """Rule for elastic properties in Franciscan (Foothills variety) rocks.

    Args:
        x (float)
            Model x coordinate.
        y (float)
            Model y coordinate.
        depth (float)
            Depth of location in m.

    Returns:
        Tuple of density (kg/m**3), Vp (m/s), Vs (m/s), Qp, and Qs
    """
    a = 0.13e+3
    vp0 = 5.4e+3 + a
    density0 = 1.74e+3*(vp0*1.0e-3)**0.25

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


def franciscan_napa_sonoma(x, y, depth):
    """Rule for elastic properties in Franciscan (Napa-Sonoma variety) rocks.

    Args:
        x (float)
            Model x coordinate.
        y (float)
            Model y coordinate.
        depth (float)
            Depth of location in m.

    Returns:
        Tuple of density (kg/m**3), Vp (m/s), Vs (m/s), Qp, and Qs
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


def franciscan_berkeley(x, y, depth):
    """Rule for elastic properties in Franciscan (Berkeley variety) rocks.

    Args:
        x (float)
            Model x coordinate.
        y (float)
            Model y coordinate.
        depth (float)
            Depth of location in m.

    Returns:
        Tuple of density (kg/m**3), Vp (m/s), Vs (m/s), Qp, and Qs
    """
    a = -0.03e+3
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


def salinian_granitic(x, y, depth):
    """Rule for elastic properties in Salinian Granitic rocks.

    Args:
        x (float)
            Model x coordinate.
        y (float)
            Model y coordinate.
        depth (float)
            Depth of location in m.

    Returns:
        Tuple of density (kg/m**3), Vp (m/s), Vs (m/s), Qp, and Qs
    """
    vp0 = 5.64e+3

    if depth < 0.5e+3:
        vp = 1.5e+3 + 5.0*depth
    elif depth < 1.5e+3:
        vp = 4.0e+3 + 1.3*(depth-0.5e+3)
    elif depth < 2.5e+3:
        vp = 5.3e+3 + 0.3*(depth-1.5e+3)
    elif depth < 5.0e+3:
        vp = 5.6e+3 + 0.08*(depth-2.5e+3)
    else:
        vp = 5.8e+3 + 0.06*(depth-5.0e+3)

    vs = default_vs(depth, vp)

    vd = vp0 if depth < 3.0e+3 else vp
    density = 60.0 + 1.6612*vd - 0.4721e-3*vd**2 + 0.0671e-6*vd**3 - 0.0043e-9*vd**4 + 0.0001e-12*vd**5

    qs = 13.0 if vs < 300.0 else default_qs(depth, vs)
    qp = default_qp(depth, qs)
    return (density, vp, vs, qp, qs)


def great_valley_sequence_sedimentary(x, y, depth):
    """Rule for elastic properties in Great Valley sedimentary rocks.

    Args:
        x (float)
            Model x coordinate.
        y (float)
            Model y coordinate.
        depth (float)
            Depth of location in m.

    Returns:
        Tuple of density (kg/m**3), Vp (m/s), Vs (m/s), Qp, and Qs
    """
    vp0 = 4.25e+3
    density0 = 1.74e+3 * (vp0*1.0e-3)**0.25

    if depth < 3.6e+3:
        vp = 2.5e+3 + 0.5833*depth
    elif depth < 8.0e+3:
        vp = 4.6e+3 + 0.18182*(depth-3.6e+3)
    elif depth < 11.0e+3:
        vp = 5.4e+3 + 0.166*(depth-8.0e+3)
    else:
        vp = 5.9e+3 + 0.0666*(depth-11.0e+3)

    if depth < 1.0e+3:
        vs = 0.6e+3 + 1.183*depth
    elif depth < 5.0e+3:
        vs = 1.5e+3 + 0.2836*depth
    else:
        vs = default_vs(depth, vp)

    density = density0 if depth < 3.0e+3 else default_density(depth, vp)

    qs = 13.0 if vs < 300.0 else default_qs(depth, vs)
    qp = default_qp(depth, qs)
    return (density, vp, vs, qp, qs)


def quaternary_tertiary_sedimentary(x, y, depth):
    """Rule for elastic properties in Quaternary-Tertiary sedimentary rocks.

    Args:
        x (float)
            Model x coordinate.
        y (float)
            Model y coordinate.
        depth (float)
            Depth of location in m.

    Returns:
        Tuple of density (kg/m**3), Vp (m/s), Vs (m/s), Qp, and Qs
    """
    if depth < 40.0:
        vp = 700.0 + 42.968*depth - 575.8e-3*depth**2 + 2931.6e-6*depth**3 - 3977.6e-9*depth**4
    elif depth < 500.0:
        vp = 1.5e+3 + 3.735*depth - 3.543e-3*depth**2
    elif depth < 10.0e+3:
        vp = 2.24e+3 + 0.6*depth
    else:
        vp = 2.24e+3 + 0.6*10.0e+3

    if depth < 25.0:
        vs = 80.0 + 2.5*depth
    elif depth < 50.0:
        vs = (vp-1360.0) / 1.16
    else:
        vs = default_vs(depth, vp)

    density = default_density(depth, vp)

    qs = 13.0 if vs < 300.0 else default_qs(depth, vs)
    qp = default_qp(depth, qs)
    return (density, vp, vs, qp, qs)


def tertiary_sedimentary_lahondabasin(x, y, depth):
    """Rule for elastic properties in Tertiary sedimentary rocks (La Honda basin).

    Args:
        x (float)
            Model x coordinate.
        y (float)
            Model y coordinate.
        depth (float)
            Depth of location in m.

    Returns:
        Tuple of density (kg/m**3), Vp (m/s), Vs (m/s), Qp, and Qs
    """
    vp = 2.24e+3 + 2.62*depth - 0.74432e-3*depth**2 + 0.0707e-6 * \
        depth**3 if depth < 3.0e+3 else 5.32e+3 + 0.027*(depth-3.0e+3)

    vs = 500.0 + 6.633*depth if depth < 50.0 else default_vs(depth, vp)

    density = default_density(depth, vp)

    qs = 13.0 if vs < 300.0 else default_qs(depth, vs)
    qp = default_qp(depth, qs)
    return (density, vp, vs, qp, qs)


def tertiary_sedimentary_southbay(x, y, depth):
    """Rule for elastic properties in Tertiary sedimentary rocks (South Bay).

    Args:
        x (float)
            Model x coordinate.
        y (float)
            Model y coordinate.
        depth (float)
            Depth of location in m.

    Returns:
        Tuple of density (kg/m**3), Vp (m/s), Vs (m/s), Qp, and Qs
    """
    if depth < 750.0:
        vp = 1.80e+3 + 1.2*depth
    elif depth < 4.0e+3:
        vp = 2.70e+3 + 0.597*(depth-750.0)
    elif depth < 7.0e+3:
        vp = 4.64e+3 + 0.417*(depth-4.0e+3)
    else:
        vp = 5.891e+3 + 0.06*(depth-7.0e+3)

    vs = 500.0 + 0.4*depth if depth < 50.0 else default_vs(depth, vp)

    density = default_density(depth, vp)

    qs = 13.0 if vs < 300.0 else default_qs(depth, vs)
    qp = default_qp(depth, qs)
    return (density, vp, vs, qp, qs)


def cenozoic_sedimentary_halfmoonbay(x, y, depth):
    """Rule for elastic properties in Cenozoic sedimentary rocks (Half Moon Bay region).

    Args:
        x (float)
            Model x coordinate.
        y (float)
            Model y coordinate.
        depth (float)
            Depth of location in m.

    Returns:
        Tuple of density (kg/m**3), Vp (m/s), Vs (m/s), Qp, and Qs
    """
    if depth < 50.0:
        vp = 700.0 + 31.4*depth
    elif depth < 4.0e+3:
        vp = 2.24e+3 + 0.6*(depth-50.0)
    elif depth < 7.0e+3:
        vp = 4.64e+3 + 0.417*(depth-4.0e+3)
    else:
        vp = 5.891e+3 + 0.06*(depth-7.0e+3)

    vs = 500.0 + 0.4*depth if depth < 50.0 else default_vs(depth, vp)

    density = default_density(depth, vp)

    qs = 13.0 if vs < 300.0 else default_qs(depth, vs)
    qp = default_qp(depth, qs)
    return (density, vp, vs, qp, qs)


def seawater(x, y, depth):
    """Rule for elastic properties in sea water.

    Args:
        x (float)
            Model x coordinate.
        y (float)
            Model y coordinate.
        depth (float)
            Depth of location in m.

    Returns:
        Tuple of density (kg/m**3), Vp (m/s), Vs (m/s), Qp, and Qs
    """
    vp = 1500.0
    density = 1030.0

    vs = NODATA_VALUE
    qs = NODATA_VALUE
    qp = 63000.0
    return (density, vp, vs, qp, qs)


def outside_model(x, y, depth):
    """Rule for elastic properties outside the model.

    Args:
        x (float)
            Model x coordinate.
        y (float)
            Model y coordinate.
        depth (float)
            Depth of location in m.

    Returns:
        Tuple of density (kg/m**3), Vp (m/s), Vs (m/s), Qp, and Qs
    """
    vp = NODATA_VALUE
    vs = NODATA_VALUE
    density = NODATA_VALUE
    qs = NODATA_VALUE
    qp = NODATA_VALUE
    return (density, vp, vs, qp, qs)
