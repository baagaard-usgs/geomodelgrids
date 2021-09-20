# CSV Data Source

Data source for generating a GeoModelGrids model from a model provided as a CSV file.

## `csv` parameters

+ **filename** *(string)* Relative or absolute path of CSV file.
+ **crs** *(str)* CRS of coordinates in CSV file.

## `csv.columns` parameters

Mapping of columns in the CSV file to data values. The first column is 0.

+ **x** *(integer)* Index of column with x coordinate values.
+ **y** *(integer)* Index of column with y coordinate values.
+ **z** *(integer)* Index of column with z coordinate values.
+ **VALUE** *(integer)* Index of column with values for `VALUE`.

:::{warning}
The CSV data source currently loads the entire CSV file into memory.
:::

## Example

This example illustrates use of the CSV data source to convert the Eberhart-Phillips and others 3D seismic velocity model from a CSV file to a GeoModelGrids model.
The model grid has a variable resolution in each of the coordinate directions.

```{code-block} cfg
[geomodelgrids]
title = New Zealand Wide model 2.3 Qs and Qp models for New Zealand, updated for Kaikoura
id = nz-cvm
description = The Qs and Qp New Zealand Wide models 2.3 incorporate the results from the Kaikoura region using aftershocks (Eberhart-Phillips, et al. 2021). The results from that study have been interpolated and merged into the previous New Zealand Wide model 2.2 (https://doi.org/10.5281/zenodo.3779523). There is no update of the Vp and Vp/Vs models.
keywords = [seismic velocity model, New Zealand, tomography]
history = 
comment = 
version = 2.3
creator_name = Donna Eberhart-Phillips
creator_institution = GNS Science, UC Davis
creator_email = eberhartphillips@ucdavis.edu
acknowledgement = 
authors = [ Eberhart-Phillips, Donna |  Bannister, Stephen |  Reyners, Martin |  Ellis, Susan |  Lanza, Federica]
references = [Eberhart-Phillips, D., S. Ellis, F. Lanza, and S. Bannister (2021), Heterogeneous material properties - as inferred from seismic attenuation - influenced multiple fault rupture and ductile creep of the Kaikoura Mw 7.8 earthquake, New Zealand, Geophys. J. Int., doi:10.1093/gji/ggab272.]
repository_name = 
repository_url = 
repository_doi = 
license = Creative Commons Attribution 4.0 International Public License

filename = seismic_nz_cvm-2-3.h5
data_source = geomodelgrids.create.data_srcs.csv.datasrc.CSVFile

[coordsys]
crs = EPSG:2193
origin_x = 1739903.44
origin_y = 3685845.31
y_azimuth = 310.0

[csv]
filename = nz_cvm-2-3.txt
crs = EPSG:4979

[csv.columns]
x = 1
y = 0
z = 2
density = 3
Vp = 4
Vs = 5
Qp = 6
Qs = 7


[data]
values = [density, Vp, Vs, Qp, Qs]
units = [kg/m**3, m/s, m/s, None, None]

[domain]
dim_x = 2400.0e+3
dim_y = 2400.0e+3
dim_z = 765.0e+3

blocks = [block]

[block]
x_coordinates = [0.0e+3, 464.0e+3, 514.0e+3, 561.0e+3, 600.0e+3, 633.0e+3, 663.0e+3, 693.0e+3, 723.0e+3, 752.0e+3, 782.0e+3, 812.0e+3, 842.0e+3, 887.0e+3, 931.0e+3, 976.0e+3, 1003.0e+3, 1013.0e+3, 1021.0e+3, 1029.0e+3, 1037.0e+3, 1045.0e+3, 1055.0e+3, 1070.0e+3, 1085.0e+3, 1100.0e+3, 1115.0e+3, 1130.0e+3, 1145.0e+3, 1160.0e+3, 1170.0e+3, 1180.0e+3, 1190.0e+3, 1200.0e+3, 1210.0e+3, 1220.0e+3, 1230.0e+3, 1240.0e+3, 1250.0e+3, 1260.0e+3, 1270.0e+3, 1280.0e+3, 1290.0e+3, 1305.0e+3, 1323.0e+3, 1348.0e+3, 1373.0e+3, 1398.0e+3, 1428.0e+3, 1453.0e+3, 1472.0e+3, 1491.0e+3, 1510.0e+3, 1529.0e+3, 1549.0e+3, 1579.0e+3, 1609.0e+3, 1639.0e+3, 1668.0e+3, 1694.0e+3, 1715.0e+3, 1736.0e+3, 1754.0e+3, 1773.0e+3, 1790.0e+3, 1810.0e+3, 1832.0e+3, 1853.0e+3, 1903.0e+3, 2400.0e+3]
y_coordinates = [0.0e+3, 850.0e+3, 900.0e+3, 940.0e+3, 970.0e+3, 995.0e+3, 1022.0e+3, 1039.0e+3, 1048.0e+3, 1055.0e+3, 1061.0e+3, 1067.0e+3, 1073.0e+3, 1079.0e+3, 1082.0e+3, 1086.0e+3, 1089.0e+3, 1093.0e+3, 1096.0e+3, 1100.0e+3, 1103.0e+3, 1107.0e+3, 1110.0e+3, 1114.0e+3, 1117.0e+3, 1121.0e+3, 1124.0e+3, 1128.0e+3, 1131.0e+3, 1135.0e+3, 1138.0e+3, 1142.0e+3, 1145.0e+3, 1149.0e+3, 1152.0e+3, 1156.0e+3, 1159.0e+3, 1163.0e+3, 1171.0e+3, 1178.0e+3, 1186.0e+3, 1193.0e+3, 1201.0e+3, 1208.0e+3, 1216.0e+3, 1223.0e+3, 1231.0e+3, 1238.0e+3, 1246.0e+3, 1253.0e+3, 1261.0e+3, 1268.0e+3, 1276.0e+3, 1286.0e+3, 1296.0e+3, 1306.0e+3, 1321.0e+3, 1336.0e+3, 1351.0e+3, 1400.0e+3, 1500.0e+3, 1600.0e+3, 1740.0e+3, 2400.0e+3]
z_coordinates = [15000.0, 1000.0, -1000.0, -3000.0, -5000.0, -8000.0, -15000.0, -23000.0, -30000.0, -34000.0, -38000.0, -42000.0, -48000.0, -55000.0, -65000.0, -85000.0, -105000.0, -130000.0, -155000.0, -185000.0, -225000.0, -275000.0, -370000.0, -620000.0, -750000.0]
z_top_offset = 0.0
chunk_size = (18, 16, 25, 5)
```