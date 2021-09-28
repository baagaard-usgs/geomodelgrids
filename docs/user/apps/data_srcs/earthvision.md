# EarthVision Data Source

Data source for generating a GeoModelGrids model from an EarthVision model.

## `earthvision` parameters

+ **model_dir** *(string)* Full absolute path to the directory containing the EarthVision model.
+ **top_surface_2grd** *(str, optional)* Filename of `2grd` file for the top surface of the model.
+ **topography_bathymetry_2grd** *(str, optional)* Filename of `2grd` file for the topography/bathymetry surface of the model.
+ **geologic_model** *(string)* Filename of the sequence file corresponding to the EarthVision geologic model.
+ **elev_units** *(string)* Units of elevation in the EarthVision geologic model.
+ **xy_units** *(string)* Units of x and y coordinates in the EarthVision geologic model.
+ **rules_module** *(string)* Path to the Python object used to assign material properties to the geologic units.
+ **rules_pythonpath** `PYTHONPATH` for `rules_module`.

## `earthvision.environment` parameters

Bash environment variables that include locations of the EarthVision files.

+ **PATH** *(string)* Value of `PATH` environment variable for executables.
+ **LD_LIBRARY_PATH** *(string)* Value of `LD_LIBRARY_PATH` environment variable for dynamic libraries.
+ **LM_LICENSE_FILE** *(string)* Absolute path of the EarthVision license file.

## Example

This example illustrates use of the EarthVision data source to generate a 3D seismic velocity model from a 3D geologic model in EarthVision.
Rules are applied on each grid point to assign elastic properties based on the geologic unit and depth.
The model includes two surfaces: one for the top of the model and one for the top of the solid material.
The model includes two blocks, each with a uniform grid resolution.

```{code-block} cfg
[geomodelgrids]
title = USGS 3D seismic velocity model for the San Francisco Bay region (regional domain)
id = usgs-sfcvm-regional
description = USGS 3D seismic velocity model for the San Francisco Bay region (regional domain)
keywords = [seismic velocity model, San Francisco Bay region]
history = Version 08.3.0 re-released as a GeoModelGrids model.
creator_name = Brad Aagaard
creator_institution = U.S. Geological Survey
creator_email = baagaard@usgs.gov
acknowledgement = None
authors = [Brocher, Thomas | Jachens, Robert | Simpson, Robert | Aagaard, Brad]
references = [None]
repository_name = ScienceBase
repository_url = https://sciencebase.gov
repository_doi = TBD
version = 21.0.0
license = CC0

filename = seismic_sfbay_regional-21.0.0.h5
data_source = geomodelgrids.create.earthvision.datasrc.RulesDataSrc

[coordsys]
crs = PROJCS["unnamed",GEOGCS["NAD83",DATUM["North_American_Datum_1983",SPHEROID["GRS 1980",6378137,298.257222101,AUTHORITY["EPSG","7019"]],TOWGS84[0,0,0,0,0,0,0],AUTHORITY["EPSG","6269"]],PRIMEM["Greenwich",0,AUTHORITY["EPSG","8901"]],UNIT["degree",0.0174532925199433,AUTHORITY["EPSG","9122"]],AUTHORITY["EPSG","4269"]],PROJECTION["Transverse_Mercator"],PARAMETER["latitude_of_origin",35],PARAMETER["central_meridian",-123],PARAMETER["scale_factor",0.9996],PARAMETER["false_easting",0],PARAMETER["false_northing",0],UNIT["Meter",1]]
origin_x = 97513.8
origin_y = 4562.2
y_azimuth = 323.638

[data]
values = [density, Vp, Vs, Qp, Qs, fault_block_id, zone_id]
units = [kg/m**3, m/s, m/s, None, None, None, None]

[domain]
dim_x = 325.0e+3
dim_y = 645.0e+3
dim_z = 45.0e+3

blocks = [vres125m, vres250m]

batch_size = 2500000

[earthvision]
model_dir = /data/sfbay-geology/regional_domain
top_surface_2grd = [bigbay_reg_topo_km_prj.2grd, zero.2grd]
topography_bathymetry_2grd = [bigbay_reg_topo_km_prj.2grd]
geologic_model = CenCal_Reg_13b.seq
elev_units = km
xy_units = km
rules_fn = seismic_regional_08_03_00.get_rules
rules_pythonpath = .

[earthvision.environment]
PATH = /bin:/usr/bin:/sbin:/usr/sbin:/opt/earthvision/ev11/bin64
LD_LIBRARY_PATH = /opt/earthvision/ev11/lib64
LM_LICENSE_FILE = /opt/earthvision/license/license.dat

[top_surface]
use_surface = True
resolution_x = 500.0
resolution_y = 500.0
chunk_size = (150, 150, 1)

[topography_bathymetry]
use_surface = True
resolution_x = 500.0
resolution_y = 500.0
chunk_size = (150, 150, 1)

[vres125m]
resolution_x = 500.0
resolution_y = 500.0
resolution_z = 125.0
z_top = 0.0
z_bot = -10.0e+3
z_top_offset = -0.1
chunk_size = (12, 12, 81, 7)

[vres250m]
resolution_horiz = 1000.0
resolution_vert = 250.0
z_top = -10.0e+3
z_bot = -45.0e+3
z_top_offset = 0.0
chunk_size = (8, 8, 141, 7)
```