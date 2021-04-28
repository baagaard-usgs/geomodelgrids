/// C++ application to query model via virtual borehole.
#if !defined(geomodelgrids_apps_borehole_hh)
#define geomodelgrids_apps_borehole_hh

#include "appsfwd.hh" // forward declarations

#include <vector> // HASA std::std::vector
#include <string> // HASA std::string

class geomodelgrids::apps::Borehole {
    friend class TestBorehole; // unit testing

    // PUBLIC METHODS /////////////////////////////////////////////////////////////////////////////
public:

    /// Constructor
    Borehole(void);

    /// Destructor
    ~Borehole(void);

    /**
     * Run query application.
     *
     * Arguments:
     *   --help
     *   --log=FILE_LOG
     *   --location=X,Y
     *   --models=FILE_0,...,FILE_M
     *   --output=FILE_OUTPUT
     *   --values=VALUE_0,...,VALUE_N
     *   --max-depth=DEPTH
     *   --dz=RESOLUTION
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
    std::string _pointsCRS;
    std::string _outputFilename;
    std::string _logFilename;
    double _maxDepth;
    double _location[2];
    double _dz;
    bool _showHelp;

    // NOT IMPLEMENTED ////////////////////////////////////////////////////////////////////////////
private:

    Borehole(const Borehole&); ///< Not implemented
    const Borehole& operator=(const Borehole&); ///< Not implemented

}; // Borehole

#endif // geomodelgrids_apps_borehole_hh

// End of file
