#!/bin/bash -x

src_dir=${TOP_SRCDIR}/geomodelgrids
build_dir=${TOP_BUILDDIR}/geomodelgrids

mkdir -p ${src_dir}
mkdir -p ${build_dir} 
cp -r . ${src_dir}
if [ $? != 0 ]; then exit 1; fi

cd ${src_dir}
autoreconf --install --verbose --force
if [ $? != 0 ]; then exit 1; fi

cd ${build_dir}
${src_dir}/configure --prefix=${INSTALL_DIR} --enable-python --enable-testing \
    --enable-test-coverage=${TEST_COVERAGE} --with-python-coverage=${PYTHON_COVERAGE} \
    --enable-valgrind=${ENABLE_VALGRIND} --enable-gdal=${ENABLE_GDAL} \
    CPPFLAGS="-I${INSTALL_DIR}/include -I${HDF5_INCDIR}" \
    LDFLAGS="-L${INSTALL_DIR}/lib -L${INSTALL_DIR}/lib64 -L${HDF5_LIBDIR}"  \
    CFLAGS="-g -O --coverage" CXXFLAGS="-g -O --coverage"
if [ $? != 0 ]; then exit 1; fi

make -j$(nproc) install
if [ $? != 0 ]; then exit 1; fi

cd ${src_dir}

exit 0