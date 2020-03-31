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

    // PUBLIC CONSTANTS ------------------------------------------------------------------------------------------------
public:

    static const hid_t H5_NULL;

    // PUBLIC METHODS --------------------------------------------------------------------------------------------------
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
     * @param[out[names Names of datasets.
     * @param[in] group Full path of group.
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
    void readAttribute(const char* path,
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

    /** Read hyperslab (subset of values) from dataset.
     *
     * @param[out values Values of hyperslab.
     * @param[in] path Full path to dataset.
     * @param[in] origin Origin of hyperslab in dataset.
     * @param[in] dims Dimensions of hyperslab.
     * @param[in] ndims Number of dimensions of hyperslab.
     * @param[in] datatype Type of data.
     */
    void readDatasetHyperslab(void* values,
                              const char* path,
                              const hsize_t* const origin,
                              const hsize_t* const dims,
                              int ndims,
                              hid_t datatype);

    // PRIVATE MEMBERS ------------------------------------------------------
private:

    hid_t _file; ///< HDF5 file

}; // HDF5

#endif // geomodelgrids_serial_hdf5_hh

// End of file
