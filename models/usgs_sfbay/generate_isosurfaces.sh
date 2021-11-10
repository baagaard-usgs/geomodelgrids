MODELS_DIR=/data/geomodelgrids
DETAILED_VERSION=v21-1
REGIONAL_VERSION=v21-0

# detailed+regional
geomodelgrids_isosurface \
    --bbox=35.0,41.50,-126.50,-118.90 \
    --hresolution=0.01 \
    --isosurface=Vs,1000.0 \
    --isosurface=Vs,2500.0 \
    --max-depth=10.0e+3 \
    --models=${MODELS_DIR}/USGS_SFCVM_${DETAILED_VERSION}_detailed.h5,${MODELS_DIR}/USGS_SFCVM_${REGIONAL_VERSION}_regional.h5 \
    --output=${MODELS_DIR}/USGS_SFCVM_${DETAILED_VERSION}_z10z25.tiff \
    --log=isosurface.log
