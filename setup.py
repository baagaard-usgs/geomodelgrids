from distutils.core import setup
import glob

setup(
    name="geomodelgrids",
    description="USGS Georeferenced Gridded Models",
    author=("Brad Aagaard"),
    author_email=("baagaard@usgs.gov"),
    url="https://github.com/baagaard-usgs/geomodelgrids",
    version="1.0.0",
    packages=[
        "gmprocess",
    },
    scripts=glob.glob("bin/*"),
)
