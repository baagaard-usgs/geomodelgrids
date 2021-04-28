/// C++ application to query for model elevation.
#if !defined(geomodelgrids_apps_queryelev_hh)
#define geomodelgrids_apps_queryelev_hh

#include "appsfwd.hh" // forward declarations

#include <vector> // HASA std::std::vector
#include <string> // HASA std::string

class geomodelgrids::apps::QueryElev {
    friend class TestQueryElev; // unit testing

    // PUBLIC METHODS /////////////////////////////////////////////////////////////////////////////
public:

    /// Constructor
    QueryElev(void);

    /// Destructor
    ~QueryElev(void);

    /**
     * Run query application.
     *
     * Arguments:
     *   --help
     *   --models=FILE_0,...,FILE_M
     *   --points=FILE_POINTS
     *   --output=FILE_OUTPUT
     *   --log=FILE_LOG
     *   --points-coordsys=PROJ|EPSG|WKT
     *   --surface=SURFACE ["top_surface" (default) | "topography_bathymetry"]
     *
     * @param argc[in] Number of arguments passed.
     * @param argv[in] Array of input arguments.
     *
     * @returns 1 if errors were detected, 0 otherwise.
     */
    int run(int argc,
            char* argv[]);

    // PRIVATE METHODS ////////////////////////////////////////////////////////////////////////////
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

    // PRIVATE MEMBERS ////////////////////////////////////////////////////////////////////////////
private:

    std::vector<std::string> _modelFilenames;
    std::string _pointsFilename;
    std::string _pointsCRS;
    std::string _outputFilename;
    std::string _logFilename;
    bool _useTopoBathy;
    bool _showHelp;

    // NOT IMPLEMENTED ////////////////////////////////////////////////////////////////////////////
private:

    QueryElev(const QueryElev&); ///< Not implemented
    const QueryElev& operator=(const QueryElev&); ///< Not implemented

}; // QueryElev

#endif // geomodelgrids_apps_queryelev_hh

// End of file
