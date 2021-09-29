# Installation

There are two basic means of installing the GeoModelGrids software.

1. Use the Linux or macOS binary package.
2. Build from source.

The binary packages provide the command line programs. If you want to use the C/C++ API in your own code, then we recommend building from source to insure compatibility of the compiled code.
For anyone wanting to contribute to GeoModelGrids software development, we offer a Docker development environment.
See [Developer Environment](../developer/docker-devenv.md) for more information.

## Installing the binary package

:::{note}
The binary package contains the query library and command line programs for accessing GeoModelGrids files.
It does not contain the Python code to create GeoModelGrids files.
:::

Binary packages are provided for Linux (GLIBC 2.17 and later; use `ldd --version` to check your version of GLIBC) and macOS (10.14 and later).
The macOS binary package has not been tested on machines with Apple silicon processors.
Windows users can use the Linux binary package within the Windows Subsystem for Linux.
The binary packages are 200MB+ in size; the large size is a result of the inclusion of geographic coordinate datum information used by Proj.4.

Open a terminal window and \item Open a terminal window and change to the directory where you want to place the binary distribution.

```{code-block} bash
cd  $HOME
mkdir geomodelgrids
cd geomodelgrids
```

Download the Linux or macOS tarball from GeoModelGrids GitHub [releases](https://github.com/baagaard-usgs/geomodelgrids/releases) and save it to the desired location, e.g., `$HOME/geomodelgrids`.

Unpack the tarball.

```{code-block} bash
# Linux 64-bit
$ tar -xf geomodelgrids-1.0.0-Linux_x86_64.tar.gz

# macOS
$ tar -xf geomodelgrids-1.0.0-Darwin_x86_64.tar.gz
```

Set environment variables.
The provided `setup.sh` script in the top-level directory script only works if you are using bash shell.
If you are using a different shell, you will need to alter how the environment variables are set in `setup.sh`.

```{code-block} bash
# Linux 64-bit
cd geomodelgrids-1.0.0-Linux_x86_64

# macOS
cd geomodelgrids-1.0.0-Darwin_x86_64

source setup.sh
```

:::{important}
You will need to either source the `setup.sh` script each time you open a new bash shell (terminal) window and want to use GeoModelGrids or add the environment variables to your shell setup script (for example, `.bashrc`).
:::

:::{warning}
The binary distribution contains the GeoModelGrids software and all of its dependencies (Proj4, HDF5, OpenSSL, sqlite, curl, and tiff).
If you have any of this software already installed on your system, you need to be careful in setting up your environment so that preexisting software does not conflict with the GeoModelGrids binary.
By default the `setup.sh` script will prepend to the `PATH` (for Linux and macOS) and `LD_LIBRARY_PATH` (for Linux) environment variables.
This will prevent most conflicts.
:::

## Installing from source

### Prerequisites

Most Linux distributions can provide all of the prerequisites via the package manager.
On macOS systems the operating system supplies with XCode command line tools installed supply the compiler, Sqlite, libtiff, openssl, and libcurl.
You can use a package manager to install Proj.4, HDF5, and CppUnit (if desired) or build them from source.
You can also use the `build_binary.py` Python script in the `docker` directory of the GeoModelGrids source code to install the software and any prerequisites that you do not have.

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

We highly recommend building in a separate directory from the source tree.
In the following configuration, we will unpack the source code in `$HOME/src`, build the source code in `$HOME/build/geomodelgrids`, and install the code to `$HOME/geomodelgrids`.

 Download the source code for the latest release from GeoModelGrids GitHub [release](https://github.com/baagaard-usgs/geomodelgrids/releases) and unpack the tarball.

 ```{code-block} bash
 mkdir $HOME/src
 mv geomodelgrids-1.0.0.tar.gz $HOME/src
 cd $HOME/src
 tar -xf geomodelgrids-1.0.0.tar.gz
 ```

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

Creating GeoModelGrids files using `geomodelgrids_create_model` requires Python and configuring with `--enable-python`.
Generating horizontal isosurfaces using `geomodelgrids_isosurface` requires the GDAL library and configuring with `--enable-gdal`.

```{code-block} bash
mkdir -p $HOME/build/geomodelgrids
cd $HOME/build/geomodelgrids

# Typical configure on a Linux system with all dependencies installed by a package manager.
# We usually have to specify the location of HDF5 because multiple variations can be present.
$HOME/src/geomodelgrids-1.0.0/configure --prefix=$HOME/geomodelgrids \
--with-hdf5-incdir=/usr/include/hdf5/serial --with-hdf5-libdir=/usr/lib/x86_64-linux-gnu/hdf5/serial

# Typical configure on a macOS system with HDF5 installed in $HOME/hdf5
# and Proj.4 installed in $HOME/proj.
$HOME/src/geomodelgrids-1.0.0/configure --prefix=$HOME/geomodelgrids \
--with-hdf5-incdir=$HOME/hdf5/include --with-hdf5-libdir=$HOME/hdf5/lib \
--with-proj-incdir=$HOME/proj/include --with-proj-libdir=$HOME/proj/lib
```

### Building and installing

```{code-block} bash
make && make install
```

### Running tests (optional)

If GeoModelGrids is configured with `--enable-testing` (requires CppUnit), then the test suite can be run via `make check`.
If GeoModelGrids was configured with Python enabled, then `make check` will also run the unit tests for the Python code.

### Setting environment variables

Set environment variables for use of the bash shell:

```{code-block} bash
PATH=$HOME/geomodelgrids/bin:$PATH
export LD_LIBRARY_PATH=$HOME/geomodelgrids/lib:$LD_LIBRARY_PATH
```
