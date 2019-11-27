"""Model storage in a HDF5 file.
"""

import h5py


class HDF5Storage():
    """HDF5 file for storing gridded model.
    """
    MODEL_ATTRS = (
        "title",
        "id",
        "description",
        "keywords",
        "creator_name",
        "creator_email",
        "creator_institution",
        "acknowledgments",
        "authors",
        "references",
        "doi",
        "version",
        "data_values",
        "data_units",
        "projection",
        "origin_x",
        "origin_y",
        "y_azimuth",
        "dim_x",
        "dim_y",
        "dim_z",
    )
    TOPOGRAPHY_ATTRS = (
        "resolution_horiz",
    )
    BLOCK_ATTRS = (
        "resolution_horiz",
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
        for attr in self.MODEL_ATTRS:
            attrs[attr] = getattr(domain, attr)
        h5.close()

    def save_topography(self, topography):
        """Write topography to HDF5 file.

        Args:
            topography (Topography)
                Surface topography.
        """
        h5 = h5py.File(self.filename, "a")
        if "topography" in h5:
            del h5["topography"]
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
        if not "blocks" in h5:
            h5.create_group("blocks")
        blocks_group = h5["blocks"]
        if block.name in blocks_group:
            del blocks_group[block.name]
        block_dataset = blocks_group.create_dataset(block.name, data=data)
        attrs = block_dataset.attrs
        for attr in self.BLOCK_ATTRS:
            attrs[attr] = getattr(block, attr)
        h5.close()


# End of file
