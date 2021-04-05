"""Model storage in a HDF5 file.
"""
import h5py
import numpy


class HDF5Storage():
    """HDF5 file for storing gridded model.
    """
    MODEL_ATTRS = (
        ("title", str),
        ("id", str),
        ("description", str),
        ("keywords", list),
        ("creator_name", str),
        ("creator_email", str),
        ("creator_institution", str),
        ("acknowledgements", str),
        ("authors", list),
        ("references", list),
        ("doi", str),
        ("version", str),
        ("data_values", list),
        ("data_units", list),
        ("crs", str),
        ("origin_x", float),
        ("origin_y", float),
        ("y_azimuth", float),
        ("dim_x", float),
        ("dim_y", float),
        ("dim_z", float),
    )
    SURFACE_ATTRS = (
        ("resolution_horiz", float),
    )
    BLOCK_ATTRS = (
        ("resolution_horiz", float),
        ("resolution_vert", float),
        ("z_top", float),
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
        for attr, typeE in self.MODEL_ATTRS:
            value = getattr(domain.metadata, attr)
            if not isinstance(value, typeE):
                raise ValueError(f"Expected type '{typeE}' for domain attribute '{attr}'. Got type '{type(value)}'")
            if isinstance(value, list) and isinstance(value[0], str):
                attrs[attr] = [numpy.string_(v) for v in value]
            else:
                attrs[attr] = value
        h5.close()

    def create_surface(self, surface):
        """Create surface in HDF5 file.

        Args:
            surface (Surface)
                Model surface.
        """
        h5 = h5py.File(self.filename, "a")
        if surface.name in h5:
            del h5[surface.name]
        surf_dataset = h5.create_dataset(surface.name, shape=surface.get_dims(),
                                         chunks=surface.chunk_size, compression="gzip")
        attrs = surf_dataset.attrs
        for attr, typeE in self.SURFACE_ATTRS:
            value = getattr(surface, attr)
            if not isinstance(value, typeE):
                raise ValueError(
                    f"Expected type '{typeE}' for surface '{surface.name}' attribute '{attr}'. Got type '{type(value)}'")
            attrs[attr] = value
        h5.close()

    def save_surface(self, surface, elevation, batch=None):
        """Write surface to HDF5 file.

        Args:
            surface (Surface)
                Model surface
            elevation (numpy.array)
                Elevation of surface in model.
            batch (utils.BatchGenerator2D)
                Current batch of points in domain corresponding to elevation data.
        """
        h5 = h5py.File(self.filename, "a")
        assert surface.name in h5
        surf_dataset = h5[surface.name]
        if batch:
            x_start, x_end = batch.x_range
            y_start, y_end = batch.y_range
            surf_dataset[x_start:x_end, y_start:y_end, :] = elevation
        else:
            surf_dataset[:] = elevation
        h5.close()

    def load_surface(self, surface, batch=None):
        """Load surface from HDF5 file.

        Args:
            surface (Surface)
                Model surface.
            batch (utils.BatchGenerator2D)
                Current batch of points in domain corresponding to elevation data.
        """
        h5 = h5py.File(self.filename, "r")
        surf_dataset = h5[surface.name]
        attrs = surf_dataset.attrs
        for attr, _ in self.SURFACE_ATTRS:
            if getattr(surface, attr) != attrs[attr]:
                raise ValueError("Inconsistency in surface '{surface.name}' attribute '{}': config value: {}, value from model: {}".format(
                    attr, getattr(surface, attr), attrs[attr]))

        if batch:
            x_start, x_end = batch.x_range
            y_start, y_end = batch.y_range
            elevation = surf_dataset[x_start:x_end, y_start:y_end]
        else:
            elevation = surf_dataset[:]
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
        block_dataset = blocks_group.create_dataset(
            block.name, shape=shape, chunks=block.chunk_size, compression="gzip")
        attrs = block_dataset.attrs
        for attr, _ in self.BLOCK_ATTRS:
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
