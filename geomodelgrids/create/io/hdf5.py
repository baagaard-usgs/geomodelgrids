"""Model storage in a HDF5 file.
"""
import h5py
import numpy


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
        "acknowledgements",
        "authors",
        "references",
        "doi",
        "version",
        "data_values",
        "data_units",
        "crs",
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
            value = getattr(domain.metadata, attr)
            if isinstance(value, list) and isinstance(value[0], str):
                attrs[attr] = [numpy.string_(v) for v in value]
            else:
                attrs[attr] = value
        h5.close()

    def create_topography(self, topography):
        """Create topography in HDF5 file.

        Args:
            topography (Topography)
                Surface topography.
        """
        h5 = h5py.File(self.filename, "a")
        if "topography" in h5:
            del h5["topography"]
        topo_dataset = h5.create_dataset("topography", shape=topography.get_dims(), chunks=topography.chunk_size)
        attrs = topo_dataset.attrs
        for attr in self.TOPOGRAPHY_ATTRS:
            attrs[attr] = getattr(topography, attr)
        h5.close()

    def save_topography(self, elevation, batch=None):
        """Write topography to HDF5 file.

        Args:
            elevation (numpy.array)
                Elevation of ground surface associated with topography.
        """
        h5 = h5py.File(self.filename, "a")
        assert "topography" in h5
        topo_dataset = h5["topography"]
        if batch:
            x_start, x_end = batch.x_range
            y_start, y_end = batch.y_range
            topo_dataset[x_start:x_end, y_start:y_end, :] = elevation
        else:
            topo_dataset[:] = elevation
        h5.close()

    def load_topography(self, topography, batch=None):
        """Load topography from HDF5 file.

        Args:
            topography (Topography)
                Surface topography.
        """
        h5 = h5py.File(self.filename, "r")
        topo_dataset = h5["topography"]
        attrs = topo_dataset.attrs
        for attr in self.TOPOGRAPHY_ATTRS:
            if getattr(topography, attr) != attrs[attr]:
                raise ValueError("Inconsistency in topography attribute '{}': config value: {}, value from model: {}".format(
                    attr, getattr(topography, attr), attrs[attr]))

        if batch:
            x_start, x_end = batch.x_range
            y_start, y_end = batch.y_range
            elevation = topo_dataset[x_start:x_end, y_start:y_end]
        else:
            elevation = topo_dataset[:]
        h5.close()

        return elevation

    def create_block(self, block):
        """Create block in HDF5 file.

        Args:
            block (Block)
                Block associated with gridded data.
        """
        h5 = h5py.File(self.filename, "a")
        if not "blocks" in h5:
            h5.create_group("blocks")
        blocks_group = h5["blocks"]
        if block.name in blocks_group:
            del blocks_group[block.name]
        shape = list(block.get_dims()) + [len(block.model_metadata.data_values)]
        block_dataset = blocks_group.create_dataset(block.name, shape=shape, chunks=block.chunk_size)
        attrs = block_dataset.attrs
        for attr in self.BLOCK_ATTRS:
            attrs[attr] = getattr(block, attr)
        h5.close()

    def save_block(self, block, data, batch=None):
        """Write block data to HDF5 file.

        Args:
            block (Block)
                Block associated with gridded data.
            data (numpy.array)
                Numpy array [Nx,Ny,Nz,Nv] of gridded data.
            batch (BatchGenerator3D)
                Current batch of points in block.
        """
        h5 = h5py.File(self.filename, "a")
        assert "blocks" in h5
        blocks_group = h5["blocks"]
        assert block.name in blocks_group
        block_dataset = blocks_group[block.name]
        if batch:
            x_start, x_end = batch.x_range
            y_start, y_end = batch.y_range
            z_start, z_end = batch.z_range
            block_dataset[x_start:x_end, y_start:y_end, z_start:z_end, :] = data
        else:
            block_dataset[:] = data
        h5.close()


# End of file
