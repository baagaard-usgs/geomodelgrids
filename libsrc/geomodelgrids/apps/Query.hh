/// C++ application to query model.
#if !defined(geomodelgrids_apps_query_hh)
#define geomodelgrids_apps_query_hh

#include "appsfwd.hh" // forward declarations

#include <vector> // HASA std::std::vector
#include <string> // HASA std::string

class geomodelgrids::apps::Query {
    friend class TestQuery; // unit testing

    // PUBLIC METHODS //////////////////////////////////////////////////////////////////////////////////////////////////
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
     *   --values=VLAUE_0,...,VALUE_N
     *   --squash-min-elev=ELEV
     *   --models=FILE_0,...,FILE_M
     *   --points=FILE_POINTS
     *   --output=FILE_OUTPUT
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

    // PRIVATE MEMBERS /////////////////////////////////////////////////////////////////////////////////////////////////
private:

    std::vector<std::string> _modelFilenames;
    std::vector<std::string> _valueNames;
    std::string _pointsFilename;
    std::string _outputFilename;
    double _squashMinElev;
    bool _squash;
    bool _showHelp;

    // NOT IMPLEMENTED /////////////////////////////////////////////////////////////////////////////////////////////////
private:

    Query(const Query&); ///< Not implemented
    const Query& operator=(const Query&); ///< Not implemented

}; // Query

#endif // geomodelgrids_apps_query_hh

// End of file
