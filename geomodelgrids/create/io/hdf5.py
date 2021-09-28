"""Model storage in a HDF5 file.
"""
import h5py
import numpy


class HDF5Storage():
    """HDF5 file for storing gridded model.
    """

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
        for attr_info in domain.get_attributes():
            attr_name = attr_info[0]
            attrs[attr_name] = self._get_attribute(domain.metadata, attr_info)
        h5.close()

    def create_surface(self, surface):
        """Create surface in HDF5 file.

        Args:
            surface (Surface)
                Model surface.
        """
        h5 = h5py.File(self.filename, "a")
        if not "surfaces" in h5:
            h5.create_group("surfaces")
        surfaces_group = h5["surfaces"]
        if surface.name in surfaces_group:
            del surfaces_group[surface.name]
        surf_dataset = surfaces_group.create_dataset(surface.name, shape=surface.get_dims(),
                                                     chunks=surface.chunk_size, compression="gzip")
        h5.close()
        self.save_surface_metadata(surface)

    def save_surface_metadata(self, surface):
        """Write surface metadata to HDF5 file.

        Args:
            surface (Surface)
                Model surface
        """
        h5 = h5py.File(self.filename, "a")
        attrs = h5["surfaces"][surface.name].attrs
        for attr_info in surface.get_attributes():
            attr_name = attr_info[0]
            attrs[attr_name] = self._get_attribute(surface, attr_info)
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
        surfaces_group = h5["surfaces"]
        assert surface.name in surfaces_group
        surf_dataset = surfaces_group[surface.name]
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
        surf_dataset = h5["surfaces"][surface.name]
        attrs = surf_dataset.attrs
        for attr_info in surface.get_attributes():
            attr_name = attr_info[0]
            attr_type = attr_info[1]
            if attr_type == list or attr_type == tuple:
                config_attr = getattr(surface, attr_name)
                h5_attr = attrs[attr_name]
                if len(config_attr) != len(h5_attr):
                    raise ValueError(
                        f"Inconsistency in surface '{surface.name}' attribute '{attr_name}': config value: {config_attr}, value from model: {h5_attr}")
                for config_value, h5_value in zip(config_attr, h5_attr):
                    if config_value != h5_value:
                        raise ValueError(
                            f"Inconsistency in surface '{surface.name}' attribute '{attr_name}': config value: {config_attr}, value from model: {h5_attr}")
            else:
                if getattr(surface, attr_name) != attrs[attr_name]:
                    raise ValueError(
                        f"Inconsistency in surface '{surface.name}' attribute '{attr_name}': config value: {config_attr}, value from model: {h5_attr}")

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
        h5.close()
        self.save_block_metadata(block)

    def save_block_metadata(self, block):
        """Write block metadata to HDF5 file.

        Args:
            block (Block)
                Block associated with gridded data.
        """
        h5 = h5py.File(self.filename, "a")
        attrs = h5["blocks"][block.name].attrs
        for attr_info in block.get_attributes():
            attr_name = attr_info[0]
            attrs[attr_name] = self._get_attribute(block, attr_info)
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

    @staticmethod
    def _get_attribute(metadata, attr_info):
        result = None
        if len(attr_info) == 2:
            attr_name, attr_type = attr_info
            value = getattr(metadata, attr_name)
            if not isinstance(value, attr_type):
                raise ValueError(f"Expected type '{attr_type}' for attribute '{attr_name}'. Got type '{type(value)}'.")
            if attr_type == dict:
                import json
                result = numpy.string_(json.dumps(value, sort_keys=True))
            else:
                result = value
        else:
            attr_name, attr_type, value_type = attr_info
            value = getattr(metadata, attr_name)

            if not isinstance(value, attr_type):
                raise ValueError(f"Expected type '{attr_type}' for attribute '{attr_name}'. Got type '{type(value)}'.")
            if attr_type == list or attr_type == tuple:
                for list_value in value:
                    if not isinstance(list_value, value_type):
                        raise ValueError(
                            f"Expected type '{value_type}' for value of attribute '{attr_name}'. Got type '{type(iter_value)}'.")
                if value_type == str:
                    result = [numpy.string_(v) for v in value]
                else:
                    result = value
            else:
                raise NotImplementedError(f"Unexpected type '{attr_type}' for attribute '{attr_name}'.")
        return result

# End of file
