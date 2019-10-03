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

  3. Correct coefficient (2.24 -> 2.48175) for constant term in
  Quaternary-Tertiary - sedimentary rocks Vp equation for 0.5 < d <
  10.0.

  4. Correct coefficient (4.46 -> 4.64) for constant term in Teriary
  sedimentary rocks (South Bay) and Cenozoic sedimentary rocks (Half
  Moon Bay region) for Vp equation 4 < d < 7.0.

  5. For density of Mafic and Great Valley ophiolite, use Vp0 if d <
  10.0, otherwise use same equation with Vp0 replaced with Vp(d).

  6. For density of Franciscan (Foothills, Napa-Sonoma, and Berkeley),
  use density0 if d < 3.0, otherwise use generic density.

Vp and Vs are in km/s. Density is in g/cm**3.

"""

UNKNOWN = -999

def default_vs(depth, vp):
    """Default rule for shear wave speed as a function of Vp.

    Args:
        depth (float)
            Depth of location in km.
        vp (float)
            P wave speed in km/s.

    Returns:
        S wave speed in km/s.
    """
    return 0.7858 - 1.2344*vp + 0.7949*vp**2 - 0.1238*vp**3 + 0.0064*vp**4


def default_density(depth, vp):
    """Default rule for density as a function of Vp.

    Args:
        depth (float)
            Depth of location in km.
        vp (float)
            P wave speed in km/s.

    Returns:
        Density in g/cm**3.
    """
    return 1.74*vp**0.25


def default_qs(depth, vs):
    """Default rule for Qs as a function of Vs.

    Args:
        depth (float)
            Depth of location in km.
        vs (float)
            S wave speed in km/s.

    Returns:
        Quality factor for S wave.
    """
    return -16.0 + 104.13*vs - 25.225*vs**2 + 8.2184*vs**3


def default_qp(depth, qs):
    """Default rule for Qp as a function of Qs.

    Args:
        depth (float)
            Depth of location in km.
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
            Depth of location in km.

    Returns:
        Tuple of density (g/cm**3), Vp (km/s), Vs (km/s), Qp, and Qs
    """
    vp = 7.77 if depth < 20.0 else 7.77 + 0.001*(depth-20)

    vs = 4.41 if depth < 20.0 else 4.41 + 0.0006*(depth-20)

    density = 3.3

    qs = 13.0 if vs < 0.3 else default_qs(depth, vs)
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
            Depth of location in km.

    Returns:
        Tuple of density (g/cm**3), Vp (km/s), Vs (km/s), Qp, and Qs
    """
    vp0 = 5.9

    vp = vp0 if depth < 10.0 else vp0 + 0.056*(depth-10)

    vs = default_vs(depth, vp)

    density = 0.227 + 1.6612*vp0 - 0.4721*vp0**2 + 0.0671*vp0**3 - 0.0043*vp0**4 + 0.0001*vp0**5 if depth < 10.0 else 0.227 + 1.6612*vp - 0.4721*vp**2 + 0.0671*vp**3 - 0.0043*vp**4 + 0.0001*vp**5

    qs = 13.0 if vs < 0.3 else default_qs(depth, vs)
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
            Depth of location in km.

    Returns:
        Tuple of density (g/cm**3), Vp (km/s), Vs (km/s), Qp, and Qs
    """
    a = 0.13
    vp0 = 5.4 + a
    density0 = 1.74*vp0**0.25

    if depth < 1.0:
        vp = a + 2.5 + 2.0*depth
    elif depth < 3.0:
        vp = a + 4.5 + 0.45*(depth-1.0)
    else:
        vp = a + 5.4 + 0.0588*(depth-3.0)

    vs = default_vs(depth, vp)

    density = density0 if depth < 3.0 else default_density(depth, vp)

    qs = 13.0 if vs < 0.3 else default_qs(depth, vs)
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
            Depth of location in km.

    Returns:
        Tuple of density (g/cm**3), Vp (km/s), Vs (km/s), Qp, and Qs
    """
    a = 0.03
    vp0 = 5.4 + a
    density0 = 1.74*vp0**0.25

    if depth < 1.0:
        vp = a + 2.5 + 2.0*depth
    elif depth < 3.0:
        vp = a + 4.5 + 0.45*(depth-1.0)
    else:
        vp = a + 5.4 + 0.0588*(depth-3.0)

    vs = default_vs(depth, vp)

    density = density0 if depth < 3.0 else default_density(depth, vp)

    qs = 13.0 if vs < 0.3 else default_qs(depth, vs)
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
            Depth of location in km.

    Returns:
        Tuple of density (g/cm**3), Vp (km/s), Vs (km/s), Qp, and Qs
    """
    a = -0.03
    vp0 = 5.4 + a
    density0 = 1.74*vp0**0.25

    if depth < 1.0:
        vp = a + 2.5 + 2.0*depth
    elif depth < 3.0:
        vp = a + 4.5 + 0.45*(depth-1.0)
    else:
        vp = a + 5.4 + 0.0588*(depth-3.0)

    vs = default_vs(depth, vp)

    density = density0 if depth < 3.0 else default_density(depth, vp)

    qs = 13.0 if vs < 0.3 else default_qs(depth, vs)
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
            Depth of location in km.

    Returns:
        Tuple of density (g/cm**3), Vp (km/s), Vs (km/s), Qp, and Qs
    """
    vp0 = 5.64
    density0 = 0.06 + 1.6612*vp0 - 0.4721*vp0**2 + 0.0671*vp0**3 - 0.0043*vp0**4 + 0.0001*vp0**5

    if depth < 0.5:
        vp = 1.5 + 5.0*depth
    elif depth < 1.5:
        vp = 4.0 + 1.3*(depth-0.5)
    elif depth < 2.5:
        vp = 5.3 + 0.3*(depth-1.5)
    elif depth < 5.0:
        vp = 5.6 + 0.08*(depth-2.5)
    else:
        vp = 5.8 + 0.06*(depth-5.0)

    vs = default_vs(depth, vp)

    density = density0 if depth < 3.0 else 0.06 + 1.6612*vp - 0.4721*vp**2 + 0.0671*vp**3 - 0.0043*vp**4 + 0.0001*vp**5

    qs = 13.0 if vs < 0.3 else default_qs(depth, vs)
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
            Depth of location in km.

    Returns:
        Tuple of density (g/cm**3), Vp (km/s), Vs (km/s), Qp, and Qs
    """
    vp0 = 4.25
    density0 = 1.74*vp0**0.25

    if depth < 3.6:
        vp = 2.5 + 0.5833*depth
    elif depth < 8.0:
        vp = 4.6 + 0.18182*(depth-3.6)
    elif depth < 11.0:
        vp = 5.4 + 0.166*(depth-8.0)
    else:
        vp = 5.9 + 0.0666*(depth-11.0)

    if depth < 1.0:
        vs = 0.6 + 1.183*depth
    elif depth < 5.0:
        vs = 1.5 + 0.2836*depth
    else:
        vs = default_vs(depth, vp)

    density = density0 if depth < 3.0 else default_density(depth, vp)

    qs = 13.0 if vs < 0.3 else default_qs(depth, vs)
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
            Depth of location in km.

    Returns:
        Tuple of density (g/cm**3), Vp (km/s), Vs (km/s), Qp, and Qs
    """
    if depth < 0.04:
        vp = 0.7 + 42.968*depth - 575.8*depth**2 + 2931.6*depth**3 - 3977.6*depth**4
    elif depth < 0.5:
        vp = 1.5 + 3.735*depth - 3.543*depth**2
    elif depth < 10.0:
        vp = 2.48175 + 0.6*depth
    else:
        vp = 2.48175+0.6*10.0

    if depth < 0.025:
        vs = 0.08 + 2.5*depth
    elif depth < 0.05:
        vs = (vp-1.36) / 1.16
    else:
        vs = default_vs(depth, vp)

    density = default_density(depth, vp)

    qs = 13.0 if vs < 0.3 else default_qs(depth, vs)
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
            Depth of location in km.

    Returns:
        Tuple of density (g/cm**3), Vp (km/s), Vs (km/s), Qp, and Qs
    """
    vp = 2.24 + 2.62*depth - 0.74432*depth**2 + 0.0707*depth**3 if depth < 3.0 else 5.32 + 0.027*(depth-3.0)

    vs = 0.5 + 6.633*depth if depth < 0.05 else default_vs(depth, vp)

    density = default_density(depth, vp)

    qs = 13.0 if vs < 0.3 else default_qs(depth, vs)
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
            Depth of location in km.

    Returns:
        Tuple of density (g/cm**3), Vp (km/s), Vs (km/s), Qp, and Qs
    """
    if depth < 0.75:
        vp = 1.80 + 1.2*depth
    elif depth < 4.0:
        vp = 2.70 + 0.597*(depth-0.75)
    elif depth < 7.0:
        vp = 4.64 + 0.417*(depth-4.0)
    else:
        vp = 5.891 + 0.06*(depth-7.0)

    vs = 0.5 + 0.4*depth if depth < 0.05 else default_vs(depth, vp)

    density = default_density(depth, vp)

    qs = 13.0 if vs < 0.3 else default_qs(depth, vs)
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
            Depth of location in km.

    Returns:
        Tuple of density (g/cm**3), Vp (km/s), Vs (km/s), Qp, and Qs
    """
    if depth < 0.05:
        vp = 0.7 + 31.4*depth
    elif depth < 4.0:
        vp = 2.24 + 0.6*(depth-0.05)
    elif depth < 7.0:
        vp = 4.64 + 0.417*(depth-4.0)
    else:
        vp = 5.891 + 0.06*(depth-7.0)

    vs = 0.5 + 0.4*depth if depth < 0.05 else default_vs(depth, vp)

    density = default_density(depth, vp)

    qs = 13.0 if vs < 0.3 else default_qs(depth, vs)
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
            Depth of location in km.

    Returns:
        Tuple of density (g/cm**3), Vp (km/s), Vs (km/s), Qp, and Qs
    """
    vp = 1.5

    vs = UNKNOWN

    density = 1.0

    qs = UNKNOWN
    qp = UNKNOWN
    return (density, vp, vs, qp, qs)


def outside_model(x, y, depth):
    """Rule for elastic properties outside the model.

    Args:
        x (float)
            Model x coordinate.
        y (float)
            Model y coordinate.
        depth (float)
            Depth of location in km.

    Returns:
        Tuple of density (g/cm**3), Vp (km/s), Vs (km/s), Qp, and Qs
    """
    vp = UNKNOWN
    vs = UNKNOWN
    density = UNKNOWN
    qs = UNKNOWN
    qp = UNKNOWN
    return (density, vp, vs, qp, qs)
