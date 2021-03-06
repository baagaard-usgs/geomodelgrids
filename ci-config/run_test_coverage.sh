#!/bin/bash

# Run tests to generate coverage information. Upload test coverage data.
# Must run codeocov script in top-level source directory.

SRC_DIR=`pwd`
BUILD_DIR=${SRC_DIR}/../../build/geomodelgrids

cd ${BUILD_DIR}

make check -C tests/data
if [ $? != 0 ]; then exit 1; fi


make -j$(nproc) check -C tests/libtests VERBOSE=1
if [ $? != 0 ]; then exit 1; fi

make coverage-libtests
if [ $? != 0 ]; then exit 1; fi

if [ -r coverage-libtests.info ]; then
  pushd ../src && \
      bash <(curl -s https://codecov.io/bash) -X gcov -f ../build/coverage-libtests.info -F libtests -y ci-config/codecov.yml \
	  || echo "Codecov did not collect libtests coverage reports." && \
      popd
fi

make check -C tests/pytests VERBOSE=1
if [ $? != 0 ]; then exit 1; fi

make coverage-pytests
if [ $? != 0 ]; then exit 1; fi

if [ -r coverage-pytests.xml ]; then
  pushd ${SRC_DIR} && \
      bash <(curl -s https://codecov.io/bash) -X gcov -f ${BUILD_DIR}/coverage-pytests.xml -F pytests -y ci-config/codecov.yml \
	  || echo "Codecov did not collect pytests coverage reports." && \
      popd
fi


exit 0
