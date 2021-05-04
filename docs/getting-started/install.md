# Installation

**TODO** Add intro here.

## Installing via binary package

**Note**: The binary package contains the query library and command line programs for accessing GeoModelGrids files. It does not contain the Python code to create GeoModelGrids files.

Binary packages are provided for Linux (GLIBC X.X and later) and MacOS (10.14 and later).
The binary packages are 200MB+ in size; the large size is a result of the inclusion of geographic coordinate datum information used by Proj.4.

## Installing via source

### Prerequisites

Most Linux distributions can provide all of the prerequisites via the package manager.
On MacOS systems the operating system supplies with XCode command line tools installed supply the compiler, Sqlite, libtiff, openssl, and libcurl. You can use a package manager to install Proj.4, HDF5, and CppUnit (if desired) or build them from source.

* C/C++ compiler supporting C++11
* HDF5 (version 1.10.0 or later)
* Sqlite (version 3 or later; required by Proj.4)
* Proj.4 (version 6.3.0 or later). Proj 7.0.0 and later also require:
  * libtiff
  * openssl
  * libcurl
* Python 3 (version 3.7 or later; if generating models)
  * h5py
  * numpy
* CppUnit (version 1.14.0 or later; if running test suite)
### Downloading the source code

**TODO** Add how to download.

We highly recommend building in a separate directory from the source tree.
In the following configuration, we will unpack the source code in `$HOME/src`, build the source code in `$HOME/build/geomodelgrids`, and install the code to `$HOME/geomodelgrids`.

### Running configure

Useful configure options (run `configure --help` to see all options):

* `--prefix=DIR` Install GeoModelGrids in directory `DIR`.
* `--enable-python` Enable building Python modules [default=no]
* `--enable-gdal` Enable GDAL support for writing GeoTiff files [default=no]
* `--enable-testing` Enable Python and C++ (requires cppunit) unit testing [default=no]
* `--with-cppunit-incdir` Specify location of cppunit header files [default=no]
* `--with-cppunit-libdir` Specify location of cppunit library [default=no]
* `--with-proj-incdir` Specify location of proj header files [default=no]
* `--with-proj-libdir` Specify location of proj library [default=no]
* `--with-hdf5-incdir` Specify location of hdf5 header files [default=no]
* `--with-hdf5-libdir` Specify location of hdf5 library [default=no]
* `--with-gdal-incdir` Specify location of gdal header files [default=no]
* `--with-gdal-libdir` Specify location of gdal library [default=no]

Creating GeoModelGrids files using `geomodelgrids_create_model` requires Python and configuring with `--enable-python`. Generating horizontal isosurfaces using `geomodelgrids_isosurface` requires the GDAL library and configuring with `--enable-gdal`.

```bash
mkdir -p $HOME/build/geomodelgrids
cd $HOME/build/geomodelgrids

# Typical configure on a Linux system with all dependencies installed by a package manager.
$HOME/src/geomodelgrids-1.0.0/configure --prefix=$HOME/geomodelgrids

# Typical configure on a MacOS system with HDF5 installed in $HOME/hdf5
# and Proj.4 installed in $HOME/proj.
$HOME/src/geomodelgrids-1.0.0/configure --prefix=$HOME/geomodelgrids \
--with-hdf5-incdir=$HOME/hdf5/include --with-hdf5-libdir=$HOME/hdf5/lib \
--with-proj-incdir=$HOME/proj/include --with-proj-libdir=$HOME/proj/lib
```

### Building and installing

```bash
make && make install
```

### Running tests

If GeoModelGrids is configured with `--enable-testing` (requires CppUnit), then the test suite can be run via `make check`. If GeoModelGrids was configured with Python enabled, then `make check` will also run the unit tests for the Python code.
