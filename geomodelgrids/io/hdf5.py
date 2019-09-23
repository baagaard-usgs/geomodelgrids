"""Model storage in a HDF5 file.
"""

import h5py


class HDF5Storage():
    """HDF5 file for storing gridded model.
    """
    DOMAIN_ATTRS = (
        "description",
        "version",
        "projection",
        "origin_x",
        "origin_y",
        "y_azimuth",
        "data_values",
        "data_units",
        "dim_x",
        "dim_y",
        "dim_z",
    )
    TOPOGRAPHY_ATTRS = (
        "resolution_horiz",
    )
    BLOCK_ATTRS = (
        "ressolution_horiz",
        "resolution_vert",
        "z_top",
    )

    def __init__(self, filename):
        """Constructor.

        Args:
            filename (str):
                Name for HDF5 file
        """
        self.filename = filename

    def save_domain(self, domain):
        """Write domain attributes to HDF5 file.

        Args:
            domain (Model):
                Model domain.
        """
        h5 = h5py.File(self.filename, "a")
        attrs = h5.attrs
        for attr in self.DOMAIN_ATTRS:
            attrs[attr] = getattr(domain, attr)
        h5.close()

    def save_topography(self, topography):
        """Write topography to HDF5 file.

        Args:
            topography (Topography)
                Surface topography.
        """
        h5 = h5py.File(self.filename, "a")
        topo_dataset = h5.create_dataset("topography", data=topography.elevation)
        attrs = topo_dataset.attrs
        for attr in self.TOPOGRAPHY_ATTRS:
            attrs[attr] = getattr(topography, attr)
        h5.close()

    def load_topography(self, topography):
        """Load topography from HDF5 file.

        Args:
            topography (Topography)
                Surface topography.
        """
        h5 = h5py.File(self.filename, "r")
        topo_dataset = h5["topography"]
        topography.elevation = topo_dataset[:]
        attrs = topo_dataset.attrs
        for attr in self.TOPOGRAPHY_ATTRS:
            if getattr(topography, attr) != attrs[attr]:
                raise ValueError("Inconsistency in topography attribute '{}': config value: {}, value from model: {}".format(
                    attr, getattr(topography, attr), attrs[attr]))
        h5.close()

    def save_block(self, block, data):
        """Write block to HDF5 file.

        Args:
            block (Block)
                Block associated with gridded data.
            data (numpy.array)
                Numpy array [Nx,Ny,Nz,Nv] of gridded data.
        """
        h5 = h5py.File(self.filename, "a")
        block_dataset = h5.create_dataset(block.name, data=data)
        attrs = block_dataset.attrs
        for attr in self.BLOCK_ATTRS:
            attrs[attr] = getattr(block, attr)
        h5.close()


# End of file
