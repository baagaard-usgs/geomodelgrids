"""Get configuration information.
"""

import os
import configparser

def string_to_list(list_string):
    """Convert list as string into a list of objects.

    Args:
        list_string (str):
            List as string, e.g., "[a, b, c]"

    Returns:
        List of objects, e.g., ["a", "b", "c"]
    """
    values = [value.strip() for value in list_string[1:-1].split(",")]
    return values


def get_config(filenames, keep_case=True, verbose=False):
    """Get configuration from .cfg files.

    Args:
        filenames (list)
            List of .cfg files to read.
        keep_case (bool)
            If True, maintain case in section headings, otherwise convert to lowercase.
        verbose (bool)
            If True, print out progress.

    Returns:
        Dictionary with configuration.
    """
    config = configparser.ConfigParser()
    if keep_case:
        config.optionxform = str
    for filename in filenames:
        if not os.path.isfile(filename):
            raise IOError("Could not find configuration file '{}'.".format(filename))
        if verbose:
            print("Fetching parameters from {}...".format(filename))
        config.read(filename)

    return {s: dict(config.items(s)) for s in config.sections()}


# End of file
