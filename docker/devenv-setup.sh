#!/bin/bash

opttype="debug"
if [ $# == 1 ]; then
    opttype=$1
fi


if [ $opttype == "debug" ]; then
    export TOP_BUILDDIR=${BASE_DIR}/build/debug
    export INSTALL_DIR=${BASE_DIR}/dest/debug
elif [ $opttype == "opt" ]; then
    export TOP_BUILDDIR=${BASE_DIR}/build/opt
    export INSTALL_DIR=${BASE_DIR}/dest/opt
else
    echo "Unknown optimization '$opttype' setting requested."
    exit 1
fi

PATH=${INSTALL_DIR}/bin:/usr/sbin:/usr/bin:/sbin:/bin
export LD_LIBRARY_PATH=${INSTALL_DIR}/lib
export PYTHONPATH=${INSTALL_DIR}/lib/python${PYTHON_VERSION}/site-packages
export GEOMODELGRIDS_BUILDDIR=${TOP_BUILDDIR}

if [ "x$EVHOME" != "x" ]; then
    PATH=${PATH}:${EVHOME}/bin64:${EVHOME}/scripts
    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${EVHOME}/lib64
fi