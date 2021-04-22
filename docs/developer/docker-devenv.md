# GeoModelGrids Docker Development Environment

The `geomodelgrids-devenv` Docker image (https://registry.gitlab.com/baagaard-usgs/geomodelgrids/geomodelgrids-devenv) provides all of the dependencies and defines the environment for GeoModelGrids development. It is intended to be read only with a separate Docker volume for persistent storage and the GeoModelGrids development workspace.

## Setup

You only need to run these setup steps once.

### Fork repositories on GitHub

This creates a copy of the repository in your GitHub account.

1. Log in to your [GitHub](https://github.com) account, creating an account if you do not already have one.
2. Fork the GeoModelGrids repository: https://github.com/baagaard-usgs/geomodelgrids

### Create Docker volume for persistent storage

```
docker volume create geomodelgrids-dev
```

### Start GeoModelGrids development Docker container

```
# Without EarthVision
docker run --name geomodelgrids-dev-workspace --rm -t \
    -v geomodelgrids-dev:/opt/geomodelgrids \
    registry.gitlab.com/baagaard-usgs/geomodelgrids/geomodelgrids-devenv
# 
# With EarthVision
docker run --name geomodelgrids-dev-workspace --rm -it \
    -v geomodelgrids-dev:/opt/geomodelgrids \
    -v $TOOLS_DIR/earthvision-11:/opt/earthvision \
    -v $HOME/data/sfbay-geology:/data/sfbay-geology \
    -v $HOME/data/geomodelgrids/:/data/geomodelgrids \
    registry.gitlab.com/baagaard-usgs/geomodelgrids/geomodelgrids-devenv
```

### Setup directory structure

We will use the directory following directory structure for the persistent storage.

```
/opt/geomodelgrids
    ├── src
    ├── build
    │   ├── debug
    │   └── opt
    └── dest
        ├── debug
        │   ├── bin
        │   ├── include
        │   ├── lib
        │   └── share
        └── opt
            ├── bin
            ├── include
            ├── lib
            └── share
```

This directory structure is setup for both a debugging version for development (debug directory) and an optimized version for performance testing (opt directory). For now, we will only setup the debugging version.

```
cd /opt/geomodelgrids
mkdir -p ${TOP_BUILDDIR}
mkdir -p ${INSTALL_DIR}
```

### Clone repositories

This creates a local copy of the repository in the persistent storage volume of the GeoModelGrids development container.

```
cd /opt/geomodelgrids
git clone --recursive https://github.com/GITHUB_USERNAME/geomodelgrids.git src
```

### Configure and build for development

```
cd ${TOP_BUILDDIR}
pushd ${TOP_SRCDIR} && autoreconf -if && popd
${TOP_SRCDIR}/configure --prefix=${INSTALL_DIR} --enable-python --enable-testing \
    CPPFLAGS="-${HDF5_INCDIR}" LDFLAGS="-L${HDF5_LIBDIR}" \
    CC=gcc CXX=g++ CFLAGS="-g -Wall" CXXFLAGS="-std=c++11 -g -Wall"
make install -j${nproc}
make check -j${nproc}
```


### Install Visual Studio Code

Install [VS Code](https://code.visualstudio.com/) for your computer.

Install the following extensions:
* Remote - Containers
* C/C++ 
* Docker
* Live Share
* Python
* Uncrustify

Additionally, we recommend also installing the following extensions:
* GitHub Pull Requests and Issues
* GitLens -- Git supercharged
* Material Icon Theme
