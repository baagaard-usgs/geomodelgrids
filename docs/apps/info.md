# geomodelgrids_info

The `geomodelgrids_info` command line program is used to print information about a model to stdout and verify a model conforms to the GeoModelGrids specification.

## Synopsis

Optional command line arguments are in square brackets.

```
geomodelgrids_info [--help] --models=FILE_0,...,FILE_M [--verify] [--description] [--coordsys] [--values] [--blocks] [--all]
```

### Required arguments

* **--models=FILE_0,...,FILE_M** Names of `M` model files to query. The
  information for each model is printed.

### Optional arguments

* **--help** Print help information to stdout and exit.
* **--verify** Verify a model conforms to the GeoModelGrids specification.
* **--description** Display model description.
* **--blocks** Display block grid information.
* **--coordsys** Display coordinate system information.
* **--values** Display names and units of values stored in the model.
* **--all** Display description, coordinate system, values, and blocks.

### Verification

Verification includes:

* checking for required metadata (HDF5 attributes);
* verifying that the topography and blocks span the horizontal dimensions; and
* verifying that the blocks span the vertical dimension of the domain.

## Example

Show all information for a model with three blocks and topography. The
file is `three-blocks-topo.h5` in the `tests/data` directory.

```bash
geomodelgrids_info --all --models=tests/data/three-blocks-topo.h5

# Output
Model: three-blocks-topo.h5
    Verification
        Verifying metadata...OK
        Verifying topography...OK
        Verifying block 'top'...OK
        Verifying block 'middle'...OK
        Verifying block 'bottom'...OK
        Verifying blocks span vertical dimension of domain...OK
    Title: Three Blocks Topo
    Id: three-blocks-topo
    Description: Model with three blocks and topography.
    Keywords: key one, key two, key three
    Creator: John Doe, Agency, johndoe@agency.org
    Authors: Smith, Jim; Doe, John; Doyle, Sarah
    References:
        Reference 1
        Reference 2
    Acknowledgments: Thank you!
    DOI: this.is.a.doi
    Version: 1.0.0
    Dimensions of model (m): x=60000, y=120000, z=45000
    Coordinate system:
        CRS (PROJ, EPSG, WKT): EPSG:3311
        Origin: x=200000, y=-400000
        Azimuth (degrees) of y axis from north: 330
    Values stored in model:
        0: one (m)
        1: two (m/s)
    Topography:
        Number of points: x=13, y=25
        Horizontal resolution (m): 5000
    Blocks (3)
        Block 'top'
            Resolution (m): horizontal=10000, vertical=5000
            Elevation (m) of top of block in logical space: 0
            Number of points: x=7, y=13, z=2
            Dimensions (m): x=60000, y=120000, z=5000
        Block 'middle'
            Resolution (m): horizontal=20000, vertical=10000
            Elevation (m) of top of block in logical space: -5000
            Number of points: x=4, y=7, z=3
            Dimensions (m): x=60000, y=120000, z=20000
        Block 'bottom'
            Resolution (m): horizontal=30000, vertical=10000
            Elevation (m) of top of block in logical space: -25000
            Number of points: x=3, y=5, z=3
            Dimensions (m): x=60000, y=120000, z=20000
```			
