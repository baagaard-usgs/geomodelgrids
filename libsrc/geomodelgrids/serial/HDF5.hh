/** Model stored as HDF5 file.
 */

#if !defined(geomodelgrids_serial_hdf5_hh)
#define geomodelgrids_serial_hdf5_hh

// Include directives ---------------------------------------------------
#include "serialfwd.hh" // forward declarations

#include <hdf5.h> // USES hid_t
#include <vector> // USES std::std::vector
#include <string> // USGS std::string

class geomodelgrids::serial::HDF5 {
    friend class TestHDF5; // Unit testing

    // PUBLIC METHODS
    // ------------------------------------------------------------------------------------------------------
public:

    /// Default constructor.
    HDF5(void);

    /// Destructor
    ~HDF5(void);

    /** Open HDF5.
     *
     * @param[in] filename Name of HDF5 file
     * @param[in] mode Mode for HDF5 file
     */
    void open(const char* filename,
              hid_t mode);

    /// Close HDF5 file.
    void close(void);

    /** Check if HDF5 file is open.
     *
     * @returns True if HDF5 file is open, false otherwise.
     */
    bool isOpen(void) const;

    /** Check if HDF5 file has group.
     *
     * @param name Full name of group.
     * @returns True if group exists, false otherwise.
     */
    bool hasGroup(const char* name);

    /** Check if HDF5 file has dataset.
     *
     * @param name Full name of dataset.
     * @returns True if dataset exists, false otherwise.
     */
    bool hasDataset(const char* name);

    /** Get dimensions of dataset.
     *
     * @param[out] dims Array of dimensions.
     * @param[out] ndims Number of dimensions.
     * @param[in] path Full path to dataset.
     */
    void getDatasetDims(hsize_t** dims,
                        int* ndims,
                        const char* path);

    /** Get names of datasets in group.
     *
     * @param names Names of datasets.
     * @param group Name of parent.
     */
    void getGroupDatasets(std::vector<std::string>* names,
                          const char* parent);

    /** Read scalar attribute.
     *
     * @param[in] path Full path to object with attribute.
     * @param[in] name Name of attribute.
     * @param[in] datatype Datatype of scalar.
     * @param[out] value Attribute value.
     */
    void readAttribute(const char* parent,
                       const char* name,
                       hid_t datatype,
                       void* value);

    /** Read string attribute.
     *
     * @param[in] path Full path to object with attribute.
     * @param[in] name Name of attribute.
     * @returns value String value
     */
    std::string readAttribute(const char* path,
                              const char* name);

    /** Read strings attribute.
     *
     * @param[in] path Full path to object with attribute.
     * @param[in] name Name of attribute.
     * @param[out] value Array of strings.
     */
    void readAttribute(const char* path,
                       const char* name,
                       std::vector<std::string>* values);

    /** Read dataset chunk.
     *
     * Currently this method assumes the chunk size (slice along dim=0).
     *
     * @param parent Full path of parent group for dataset.
     * @param name Name of dataset.
     * @param data Data.
     * @param dims Dimensions of chunk.
     * @param ndims Number of dimensions of chunk.
     * @param islice Index of data slice.
     * @param datatype Type of data.
     */
    void readDatasetChunk(const char* parent,
                          const char* name,
                          char** const data,
                          hsize_t** const dimsChunk,
                          int* const ndims,
                          const int chunk,
                          hid_t datatype);

    // PRIVATE MEMBERS ------------------------------------------------------
private:

    hid_t _file; ///< HDF5 file

}; // HDF5

#endif // geomodelgrids_serial_hdf5_hh

// End of file
