# Set up environment for running EarthVision using GeoModelGrids development
# environment.
export DGIHOME=/opt/earthvision
export EVHOME=${DGIHOME}/ev11
export LM_LICENSE_FILE=${DGIHOME}/license/license.dat
export LD_LIBRARY_PATH=${EVHOME}/lib64:${LD_LIBRARY_PATH}
PATH=${EVHOME}/bin64:${EVHOME}/scripts:${PATH}
