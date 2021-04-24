/// C++ application to query for elevation of isosurface.
#if !defined(geomodelgrids_apps_isosurface_hh)
#define geomodelgrids_apps_isosurface_hh

#include "appsfwd.hh" // forward declarations

#include "geomodelgrids/serial/Query.hh" // HASA SquashingEnum

#include <vector> // HASA std::std::vector
#include <string> // HASA std::string

class geomodelgrids::apps::Isosurface {
    friend class TestIsosurface; // unit testing
    friend class Isosurfacer; // utility class

    // PUBLIC METHODS //////////////////////////////////////////////////////////////////////////////////////////////////
public:

    /// Constructor
    Isosurface(void);

    /// Destructor
    ~Isosurface(void);

    /**
     * Run query application.
     *
     * Arguments:
     *   --help
     *   --log=FILE_LOG
     *   --bbox=XMIN,XMAX,YMIN,YMAX
     *   --hresolution=RESOLUTION
     *   --vresolution=RESOLUTION
     *   --isosurface=NAME,VALUE
     *   --depth-reference=SURFACE
     *   --num-search-points=NUM
     *   --max-depth=VALUE
     *   --models=FILE_0,...,FILE_M
     *   --output=FILE_OUTPUT
     *   --prefer-deep
     *   --bbox-coordsys=PROJ|EPSG|WKT
     *
     * @param argc[in] Number of arguments passed.
     * @param argv[in] Array of input arguments.
     *
     * @returns 1 if errors were detected, 0 otherwise.
     */
    int run(int argc,
            char* argv[]);

    // PRIVATE METHODS /////////////////////////////////////////////////////////////////////////////////////////////////
private:

    /** Parse command line arguments.
     *
     * @param argc[in] Number of arguments passed.
     * @param argv[in] Array of input arguments.
     */
    void _parseArgs(int argc,
                    char* argv[]);

    /// Print help information.
    void _printHelp(void);

    /** Create header for output file.
     *
     * @param argc[in] Number of arguments passed.
     * @param argv[in] Array of input arguments.
     */
    std::string _createOutputHeader(int argc,
                                    char* argv[]);

    // PRIVATE MEMBERS /////////////////////////////////////////////////////////////////////////////////////////////////
private:

    std::vector<std::string> _modelFilenames;
    std::vector< std::pair<std::string, double> > _isosurfaces;
    std::string _bboxCRS;
    std::string _outputFilename;
    std::string _logFilename;
    double _minX;
    double _maxX;
    double _minY;
    double _maxY;
    double _horizRes;
    double _vertRes;
    double _maxDepth;
    int _numSearchPoints;
    geomodelgrids::serial::Query::SquashingEnum _depthSurface;
    bool _preferShallow;
    bool _showHelp;

    // NOT IMPLEMENTED /////////////////////////////////////////////////////////////////////////////////////////////////
private:

    Isosurface(const Isosurface&); ///< Not implemented
    const Isosurface& operator=(const Isosurface&); ///< Not implemented

}; // Isosurface

#endif // geomodelgrids_apps_isosurface_hh

// End of file
