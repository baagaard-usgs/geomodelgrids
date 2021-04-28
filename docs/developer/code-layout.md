# Code layout

- The root directory contains basic information files, such as `README` and `LICENSE.md`.
- **bin**: Command line programs.
- **libsrc**: C and C++ API
- **geomodelgrids**: Python API
- **docs**: Files used to generate the documentation.
- **models**: Files used to generate some specific models using geomodelgrids.

## Command line programs

The `geomodelgrids_create_model` file is a Python script that uses the Python API. All of the other files are C++ files that use the C++ API.

```bash
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

```bash
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
        ├── Makefile.am
        ├── TestDriver.cc
        ├── TestDriver.hh
        ├── cerrorhandler.cc
        ├── cerrorhandler.h
        ├── constants.hh
        └── utilsfwd.hh
```

## Python interface

```bash
geomodelgrids
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
    ├── earthvision
    │   ├── __init__.py
    │   ├── api.py
    │   └── datasrc.py
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