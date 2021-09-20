# Code layout

The root directory contains basic information files, such as `README` and `LICENSE.md`.
- **bin**: Command line programs.
- **ci-config**: Continuous integration configuration files.
- **developer**: Configuration files for developer related tools.
- **docker**: Docker image configuration files.
- **docs**: Files used to generate the documentation.
- **examples**: Source code illustrating use of the C and C++ APIs.
- **geomodelgrids**: Python API
- **libsrc**: C and C++ API
- **models**: Files used to generate some specific models using geomodelgrids.
- **tests**: Code for continuous integration testing.

## Command line programs

The `geomodelgrids_create_model` file is a Python script that uses the Python API. All of the other files are C++ files that use the C++ API.

```{code-block} bash
bin
├── Makefile.am
├── borehole.cc
├── geomodelgrids_create_model
├── info.cc
├── isosurface.cc
├── query.cc
└── queryelev.cc
```

## C/C++ API

The `apps` directory contains the code for the command line programs. The `serial` directory contains the code for the C/C++ serial API. The `utils` directory contains general C/C++ API utilities.

```{code-block} bash
libsrc/
├── Makefile.am
└── geomodelgrids
    ├── Makefile.am
    ├── apps
    │   ├── Borehole.cc
    │   ├── Borehole.hh
    │   ├── Info.cc
    │   ├── Info.hh
    │   ├── Isosurface.cc
    │   ├── Isosurface.hh
    │   ├── Makefile.am
    │   ├── Query.cc
    │   ├── Query.hh
    │   ├── QueryElev.cc
    │   ├── QueryElev.hh
    │   └── appsfwd.hh
    ├── geomodelgrids_serial.hh
    ├── serial
    │   ├── Block.cc
    │   ├── Block.hh
    │   ├── HDF5.cc
    │   ├── HDF5.hh
    │   ├── Hyperslab.cc
    │   ├── Hyperslab.hh
    │   ├── Makefile.am
    │   ├── Model.cc
    │   ├── Model.hh
    │   ├── ModelInfo.cc
    │   ├── ModelInfo.hh
    │   ├── Query.cc
    │   ├── Query.hh
    │   ├── Surface.cc
    │   ├── Surface.hh
    │   ├── cquery.cc
    │   ├── cquery.h
    │   └── serialfwd.hh
    └── utils
        ├── CRSTransformer.cc
        ├── CRSTransformer.hh
        ├── ErrorHandler.cc
        ├── ErrorHandler.hh
        ├── GeoTiff.cc
        ├── GeoTiff.hh
        ├── Indexing.cc
        ├── Indexing.hh
        ├── Makefile.am
        ├── TestDriver.cc
        ├── TestDriver.hh
        ├── cerrorhandler.cc
        ├── cerrorhandler.h
        ├── constants.hh
        └── utilsfwd.hh
```

## Python interface

```{code-block} bash
geomodelgrids
├── Makefile.am
├── __init__.py
└── create
    ├── __init__.py
    ├── apps
    │   ├── __init__.py
    │   └── create_model.py
    ├── core
    │   ├── __init__.py
    │   ├── datasrc.py
    │   └── model.py
    ├── data_srcs
    │   ├── __init__.py
    │   ├── csv
    │   │   ├── __init__.py
    │   │   └── datasrc.py
    │   ├── earthvision
    │   │   ├── __init__.py
    │   │   ├── api.py
    │   │   └── datasrc.py
    │   └── iris_emc
    │       ├── __init__.py
    │       └── datasrc.py
    ├── io
    │   ├── __init__.py
    │   ├── hdf5.py
    │   └── scec_cca.py
    ├── testing
    │   ├── __init__.py
    │   └── datasrc.py
    └── utils
        ├── __init__.py
        ├── batch.py
        ├── config.py
        └── units.py
```
