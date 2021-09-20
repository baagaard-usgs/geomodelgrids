# IRIS EMC Data Source

Data source for generating a GeoModelGrids model from an IRIS EMC model.
Currently, only conversion of vertex-based grids are supported.

## `iris_emc` parameters

+ **filename** *(string)* Relative or absolute path of IRIS EMC NetCDF file.

## Example

This example illustrates use of the IRIS EMC data source to convert the Moulik and others S362ANI+M 3D seismic velocity model from the IRIS EMC format to a GeoModelGrids model.
Most of the model metadata is extracted from the NetCDF file.

```{code-block} cfg
[geomodelgrids]
filename = seismic_moulik_etal_S362ANI+M.h5
data_source = geomodelgrids.create.data_srcs.iris_emc.datasrc.EMCNetCDF

repository_url = N/A
repository_name = None
repository_institution = None
repository_doi = None

[domain]
batch_size = 25000000

[block]
chunk_size = (15, 15, 25, 3)

[iris_emc]
filename = S362ANI+M_kmps.nc
```