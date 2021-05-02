MODELS_DIR=/data/geomodelgrids
DETAILED_VERSION=21-1-0
REGIONAL_VERSION=21-0-0

# detailed+regional
geomodelgrids_isosurface \
    --bbox=35.0,41.50,-126.50,-118.90 \
    --hresolution=0.01 \
    --isosurface=Vs,1000.0 \
    --isosurface=Vs,2500.0 \
    --max-depth=10.0e+3 \
    --models=${MODELS_DIR}/seismic_sfbay_detailed-${DETAILED_VERSION}.h5,${MODELS_DIR}/seismic_sfbay_regional-${REGIONAL_VERSION}.h5 \
    --output=${MODELS_DIR}/seismic_sfbay_regional+detailed_vsisosurfaces.tiff \
    --log=isosurface.log
