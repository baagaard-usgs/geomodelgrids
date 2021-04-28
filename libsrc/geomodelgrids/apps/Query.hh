/// C++ application to query model.
#if !defined(geomodelgrids_apps_query_hh)
#define geomodelgrids_apps_query_hh

#include "appsfwd.hh" // forward declarations

#include "geomodelgrids/serial/Query.hh" // HASA SquashingEnum

#include <vector> // HASA std::std::vector
#include <string> // HASA std::string

class geomodelgrids::apps::Query {
    friend class TestQuery; // unit testing

    // PUBLIC METHODS /////////////////////////////////////////////////////////////////////////////
public:

    /// Constructor
    Query(void);

    /// Destructor
    ~Query(void);

    /**
     * Run query application.
     *
     * Arguments:
     *   --help
     *   --values=VALUE_0,...,VALUE_N
     *   --squash-min-elev=ELEV
     *   --squash-surface=top_surface|topography_bathymetry
     *   --models=FILE_0,...,FILE_M
     *   --points=FILE_POINTS
     *   --output=FILE_OUTPUT
     *   --log=FILE_LOG
     *   --points-coordsys=PROJ|EPSG|WKT
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
    std::vector<std::string> _valueNames;
    std::string _pointsFilename;
    std::string _pointsCRS;
    std::string _outputFilename;
    std::string _logFilename;
    double _squashMinElev;
    geomodelgrids::serial::Query::SquashingEnum _squash;
    bool _showHelp;

    // NOT IMPLEMENTED ////////////////////////////////////////////////////////////////////////////
private:

    Query(const Query&); ///< Not implemented
    const Query& operator=(const Query&); ///< Not implemented

}; // Query

#endif // geomodelgrids_apps_query_hh

// End of file
