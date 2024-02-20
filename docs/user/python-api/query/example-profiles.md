# Vertical Profiles Example

This example discusses a Python application that queries a 3D seismic velocity model for values on a vertical profile at seismic stations specified in a CSV file.
The output is written to a directory with the vertical profile for each station in a different file.
The command line interface for the Python script allows the user to specify which seismic velocity models in GeoModelGrids format to query, the name of the CSV file containing the information for the seismic stations, the maximum depth of the vertical profile, the resolution of the vertical profile, the name of the directory for the output, and whether to output the results as CSV files or text files (whitespace separated columns).

The source code is in `examples/python-api/station_profiles.py`.
The Python script includes application class `App` with public methods `initialize()` and `run_query()` to set up the query and run the query, respectively.
The Python script includes a function `cli()` that implements the command line interface.

```{code-block} bash
# Show usage
./station_profiles.py --help

# Output
usage: station_profiles.py [-h] --models MODELS --filename FILENAME_LOCATIONS --output-dir OUTPUT_DIR [--output-format {csv,txt}] --max-depth MAX_DEPTH
                           --resolution RESOLUTION [--log LOG_FILENAME] [--debug]

Application for running linear mixed effects regression on ground-motion records.

options:
  -h, --help            show this help message and exit
  --models MODELS       Comma separated list of GeoModelGrids files to query. (default: None)
  --filename FILENAME_LOCATIONS
                        CSV file with Network, StationCode, StationLongitude, and StationLatitude. (default: None)
  --output-dir OUTPUT_DIR
                        Directory for output. (default: None)
  --output-format {csv,txt}
                        Vertical resoluion in queries. (default: csv)
  --max-depth MAX_DEPTH
                        Maximum depth in kilometers. (default: None)
  --resolution RESOLUTION
                        Vertical resoluion in queries. (default: None)
  --log LOG_FILENAME
  --debug
```

## Demonstration with the USGS SF-CVM

We demonstrate use of this Python application using the USGS San Francisco Bay region 3D seismic velocity model.
We use version 21.1 of the model, `USGS_SFCVM_v21-1_detailed.h5`, which can be downloaded from [USGS ScienceBase](https://www.sciencebase.gov/catalog/item/61817394d34e9f2789e3c36c).

We create an input file `stations.csv` with information for two seismic stations, `BK.BKS` and `NC.JGR`.
Station `BK.BKS` is located at 37.87622 degrees north and 122.23558 degrees west, and station `NC.JGR` is located at 37.51604 degrees north and 122.45815 degrees west in the WGS84 datum.

```{code-block} text
---
caption: Contents of `stations.csv`.
---
Network,StationCode,StationLatitude,StationLongitude
BK,BKS,37.87622,-122.23558
NC,JGR,37.51604,-122.45815
```

We run the application, specifying the seismic velocity model, station information in `stations.csv`, output in CSV files in a `profiles` directory, and vertical profiles 1.0 km deep with a resolution of 25 m.

```{code-block} bash
./station_profiles.py --models=USGS_SFCVM_v21-1_detailed.h5 --filename=stations.csv --output-dir=profiles --max-depth=1.0 --resolution=25.0
```

The `profiles` directory contains files `BK.BKS.csv` and `NC.JGR.csv`.
The values are all in SI units (`m` for elevation and depth, `kg/m**3` for density, and `m/s` for Vp and Vs).

```{code-block} text
---
caption: Contents of `BK.BKS.csv` in the `profiles` output directory.
---
Elevation,Depth,Density,Vp,Vs,Qp,Qs
298.11,0.00,2240.71,2750.09,1194.08,172.73,86.37
273.11,25.00,2243.10,2761.81,1204.19,174.33,87.16
248.11,50.00,2245.49,2773.62,1214.38,175.94,87.97
223.11,75.00,2247.88,2785.44,1224.60,177.56,88.78
198.11,100.00,2250.26,2797.25,1234.83,179.19,89.59
173.11,125.00,2252.63,2809.06,1245.08,180.82,90.41
148.11,150.00,2254.99,2820.87,1255.35,182.45,91.23
123.11,175.00,2257.35,2832.69,1265.62,184.09,92.04
98.11,200.00,2259.70,2844.50,1275.92,185.73,92.87
73.11,225.00,2262.04,2856.31,1286.22,187.38,93.69
48.11,250.00,2264.38,2868.13,1296.54,189.03,94.52
23.11,275.00,2266.70,2879.94,1306.87,190.69,95.35
-1.89,300.00,2269.03,2891.75,1317.22,192.36,96.18
-26.89,325.00,2271.34,2903.56,1327.58,194.02,97.01
-51.89,350.00,2273.65,2915.38,1337.94,195.70,97.85
-76.89,375.00,2275.95,2927.19,1348.32,197.37,98.69
-101.89,400.00,2278.24,2939.00,1358.71,199.06,99.53
-126.89,425.00,2280.52,2950.81,1369.11,200.75,100.37
-151.89,450.00,2282.80,2962.63,1379.52,202.44,101.22
-176.89,475.00,2285.07,2974.44,1389.93,204.14,102.07
-201.89,500.00,2287.34,2986.25,1400.36,205.84,102.92
-226.89,525.00,2289.60,2998.07,1410.79,207.56,103.78
-251.89,550.00,2291.85,3009.88,1421.23,209.27,104.63
-276.89,575.00,2294.09,3021.69,1431.68,210.99,105.50
-301.89,600.00,2296.33,3033.50,1442.13,212.71,106.36
-326.89,625.00,2298.56,3045.32,1452.59,214.45,107.22
-351.89,650.00,2300.79,3057.13,1463.06,216.18,108.09
-376.89,675.00,2303.01,3068.94,1473.53,217.93,108.96
-401.89,700.00,2305.22,3080.75,1484.00,219.67,109.84
-426.89,725.00,2307.43,3092.57,1494.48,221.43,110.71
```

```{code-block} text
---
caption: Contents of `NC.JGR.csv` in the `profiles` output directory.
---
Elevation,Depth,Density,Vp,Vs,Qp,Qs
260.71,0.00,2669.73,1501.00,337.70,33.21,16.60
235.71,25.00,2669.73,1625.01,392.36,42.94,21.47
210.71,50.00,2669.73,1750.00,456.56,54.13,27.07
185.71,75.00,2669.73,1875.00,528.97,66.48,33.24
160.71,100.00,2669.73,2000.00,608.68,79.78,39.89
135.71,125.00,2669.73,2125.00,694.81,93.85,46.93
110.71,150.00,2669.73,2250.00,786.53,108.59,54.29
85.71,175.00,2669.73,2375.00,883.06,123.88,61.94
60.71,200.00,2669.73,2500.00,983.63,139.68,69.84
35.71,225.00,2669.73,2625.00,1087.52,155.96,77.98
10.71,250.00,2669.73,2750.00,1194.06,172.73,86.37
-14.29,275.00,2669.73,2875.00,1302.60,190.02,95.01
-39.29,300.00,2669.73,3000.00,1412.53,207.85,103.93
-64.29,325.00,2669.73,3125.00,1523.29,226.29,113.15
-89.29,350.00,2669.73,3250.00,1634.33,245.39,122.70
-114.29,375.00,2669.73,3375.00,1745.18,265.20,132.60
-139.29,400.00,2669.73,3500.00,1855.36,285.75,142.87
-164.29,425.00,2669.73,3625.00,1964.46,307.08,153.54
-189.29,450.00,2669.73,3750.00,2072.11,329.21,164.61
-214.29,475.00,2669.73,3875.00,2177.95,352.14,176.07
-239.29,500.00,2669.73,3990.52,2273.91,374.02,187.01
-264.29,525.00,2669.73,4032.44,2308.26,382.11,191.06
-289.29,550.00,2669.73,4064.94,2334.75,388.43,194.21
-314.29,575.00,2669.73,4097.44,2360.94,394.83,197.42
-339.29,600.00,2669.73,4129.93,2387.09,401.25,200.62
-364.29,625.00,2669.73,4162.43,2412.93,407.75,203.87
-389.29,650.00,2669.73,4194.93,2438.72,414.26,207.13
-414.29,675.00,2669.73,4227.42,2464.20,420.84,210.42
-439.29,700.00,2669.73,4259.92,2489.61,427.44,213.72
-464.29,725.00,2669.73,4292.42,2514.71,434.11,217.05
-489.29,750.00,2669.73,4324.92,2539.74,440.79,220.40
-514.29,775.00,2669.73,4357.41,2564.45,447.54,223.77
-539.29,800.00,2669.73,4389.91,2589.08,454.30,227.15
-564.29,825.00,2669.73,4422.41,2613.39,461.13,230.56
-589.29,850.00,2669.73,4454.90,2637.62,467.96,233.98
-614.29,875.00,2669.73,4487.40,2661.52,474.85,237.42
-639.29,900.00,2669.73,4519.90,2685.33,481.75,240.88
-664.29,925.00,2669.73,4552.40,2708.81,488.70,244.35
-689.29,950.00,2669.73,4584.89,2732.20,495.66,247.83
-714.29,975.00,2669.73,4617.39,2755.26,502.66,251.33
-739.29,1000.00,2669.73,4649.89,2778.23,509.68,254.84
```