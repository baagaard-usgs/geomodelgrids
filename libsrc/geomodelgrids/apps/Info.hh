/// C++ application to print model metadata to stdout.
#if !defined(geomodelgrids_apps_info_hh)
#define geomodelgrids_apps_info_hh

#include "appsfwd.hh" // forward declarations

#include "geomodelgrids/serial/serialfwd.hh" // USES Model

#include <vector> // USES std::std::vector
#include <string> // USES std::string

class geomodelgrids::apps::Info {
    friend class TestInfo; // unit testing

    // PUBLIC METHODS /////////////////////////////////////////////////////////////////////////////
public:

    /// Constructor
    Info(void);

    /// Destructor
    ~Info(void);

    /**
     * Run info application.
     *
     * Arguments:
     *   --help
     *   --description
     *   --blocks
     *   --coordsys
     *   --values
     *   --verify
     *   --models=FILE_0,...,FILE_M
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

    /** Print model description.
     *
     * @param[in] model Target model.
     */
    void _printDescription(geomodelgrids::serial::Model* const model);

    /** Print model coordinate system.
     *
     * @param[in] model Target model.
     */
    void _printCoordSys(geomodelgrids::serial::Model* const model);

    /** Print model coordinate system.
     *
     * @param[in] model Target model.
     */
    void _printValues(geomodelgrids::serial::Model* const model);

    /** Print description of model blocks.
     *
     * @param[in] model Target model.
     */
    void _printBlocks(geomodelgrids::serial::Model* const model);

    /** Verify presence of metadata and consistency of model.
     *
     * @param[in] model Target model.
     */
    void _verify(geomodelgrids::serial::Model* const model);

    // PRIVATE MEMBERS ////////////////////////////////////////////////////////////////////////////
private:

    std::vector<std::string> _modelFilenames;
    bool _showHelp;
    bool _showAll;
    bool _showDescription;
    bool _showBlocks;
    bool _showCoordSys;
    bool _showValues;
    bool _doVerification;

    // NOT IMPLEMENTED ////////////////////////////////////////////////////////////////////////////
private:

    Info(const Info&); ///< Not implemented
    const Info& operator=(const Info&); ///< Not implemented

}; // Info

#endif // geomodelgrids_apps_info_hh

// End of file
