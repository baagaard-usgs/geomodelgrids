# GeoModelGrids Docker Development Environment

The `geomodelgrids-devenv` Docker image <https://registry.gitlab.com/baagaard-usgs/geomodelgrids/geomodelgrids-devenv> provides all of the dependencies and defines the environment for GeoModelGrids development.
It is built using the Ubuntu 20.04 Linux distribution.
It is intended to be read only with a separate Docker volume for persistent storage and the GeoModelGrids development workspace.
We separate the development "environment" from the "workspace" so that we can update the development environment without affecting the workspace and easily maintain a persistent workspace while starting and stopping the Docker container that holds the development environment.

## Prerequisites

1. You need to have [Docker](https://www.docker.com/products/docker-desktop) installed and running on your computer.
2. You need to have a [GitHub](https://github.com) account.

## Setup

You only need to run these setup steps once.

### Fork repositories on GitHub

This creates a copy of the repository in your GitHub account.

1. Log in to your [GitHub](https://github.com) account, creating an account if you do not already have one.
2. Fork the GeoModelGrids repository: <https://github.com/baagaard-usgs/geomodelgrids>.

### Create Docker volume for persistent storage

On your local machine, create a Docker volume for persistent storage.

```{code-block} bash
docker volume create geomodelgrids-dev
```

### Start GeoModelGrids development Docker container

Running the command below will:

1. Start (run) the Docker container using the `geomodelgrids-devenv` Docker image and assign it the name `geomodelgrids-dev-workspace`.
2. Mount the docker volume with persistent storage at `/opt/geomodelgrids`. 
3. The `geomodelgrids-devenv` Docker image will be downloaded from the GitLab registry <registry.gitlab.com/baagaard-usgs/geomodelgrids>.

```{code-block} bash
# Without EarthVision
docker run --name geomodelgrids-dev-workspace --rm -it \
    -v geomodelgrids-dev:/opt/geomodelgrids \
    registry.gitlab.com/baagaard-usgs/geomodelgrids/geomodelgrids-devenv
# 
# With EarthVision in $TOOLS_DIR/earthvision-11
docker run --name geomodelgrids-dev-workspace --rm -it \
    -v geomodelgrids-dev:/opt/geomodelgrids \
    -v $TOOLS_DIR/earthvision-11:/opt/earthvision \
    -v $HOME/data/sfbay-geology:/data/sfbay-geology \
    -v $HOME/data/geomodelgrids/:/data/geomodelgrids \
    registry.gitlab.com/baagaard-usgs/geomodelgrids/geomodelgrids-devenv
```

### Setup directory structure

We will use the directory following directory structure for the persistent storage.

```{code-block} bash
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

This directory structure is setup for both a debugging version for development (debug directory) and an optimized version for performance testing (opt directory).
For now, we will only setup the debugging version.

```{code-block} bash
cd /opt/geomodelgrids
mkdir -p ${TOP_BUILDDIR}
mkdir -p ${INSTALL_DIR}
```

### Clone repositories

This creates a local copy of the repository in the persistent storage volume of the GeoModelGrids development container.

```{code-block} bash
cd /opt/geomodelgrids
git clone --recursive https://github.com/GITHUB_USERNAME/geomodelgrids.git src
```

### Configure and build for development

```{code-block} bash
cd ${TOP_BUILDDIR}
pushd ${TOP_SRCDIR} && autoreconf -if && popd
${TOP_SRCDIR}/configure \
  --prefix=${INSTALL_DIR} \
  --enable-python \
  --enable-gdal \
  --enable-testing \
  CPPFLAGS="-${HDF5_INCDIR}" \
  LDFLAGS="-L${HDF5_LIBDIR}" \
  CC=gcc CXX=g++ CFLAGS="-g -Wall" CXXFLAGS="-g -Wall"
make install -j$(nproc)
make check -j$(nproc)
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

## Running

### Start the GeoModelGrids development Docker container

Whenever you need to restart the `geomodelgrids-dev-workspace` Docker container, simply run

```{code-block} bash
# Without EarthVision
docker run --name geomodelgrids-dev-workspace --rm -it \
    -v geomodelgrids-dev:/opt/geomodelgrids \
    registry.gitlab.com/baagaard-usgs/geomodelgrids/geomodelgrids-devenv
```

:::{tip}
Make sure Docker is running before you start the container.
:::

### Attach VS Code to the Docker container

1. Start VS Code.
2. Click on the Docker extension in the Activity Bar on the far left hand side.
3. Find the `geomodelgrids-dev-workspace` container. Verify that it is running.
4. Right-click on the container and select `Attach Visual Studio Code`. This will open a new window. You should see `Container registry.gitlab.com/baagaard-usgs...` at the left side of the status bar at the bottom of the window.
