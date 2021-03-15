"""Get conversion factors for units to base SI units (meter, second,
kilogram).
"""


def length_scale(name):
    """Get length scale associated with units.

    Args:
        name (str):
           Units of length, e.g., "m", "meter", "km", "kilometer", "ft", "feet".

    Returns:
        Length of unit in meters.
    """
    value = 1.0
    if name in ["m", "meter", "meters"]:
        value = 1.0
    elif name in ["km", "kilometer", "kilometers"]:
        value = 1000.0
    elif name in ["ft", "foot", "feet"]:
        value = 0.3048
    else:
        raise ValueError("Unknown units '{}' for topography elevation.".format(name))
    return value


# End of file
