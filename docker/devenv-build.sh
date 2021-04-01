#!/bin/bash

opttype="debug"
if [ $# == 1 ]; then
    opttype=$1
fi


if [ $opttype == "debug" ]; then
    CFLAGS="-g -Wterminate -Wall --coverage"
    CXXFLAGS="-std=c++11 -g -Wall --coverage"
elif [ $opttype == "opt" ]; then
    CFLAGS="-O3 -march=native"
    CXXFLAGS="-std=c++11 -O3 -march=native -g"
    CPPFLAGS_OPT="-DNDEBUG"
else
    echo "Unknown optimization '$opttype' setting requested."
    exit 1
fi


${TOP_SRCDIR}/configure --prefix=${INSTALL_DIR} --enable-python --enable-testing --enable-test-coverage \
	     CPPFLAGS="-I${HDF5_INCDIR} $CPPFLAGS_OPT" \
	     LDFLAGS="-L${HDF5_LIBDIR} --coverage" \
	     CFLAGS="$CFLAGS" \
	     CXXFLAGS="$CXXFLAGS"
