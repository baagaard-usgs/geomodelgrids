#include <portinfo>

#include "Projection.hh" // implementation of class methods

#include <sstream> // USES std::ostringstream
#include <cassert> // USES assert()

// ---------------------------------------------------------------------------------------------------------------------
// Default constructor.
geomodelgrids::utils::Projection::Projection(void) {} // constructor


// ---------------------------------------------------------------------------------------------------------------------
// Destructor
geomodelgrids::utils::Projection::~Projection(void) {} // destructor


// ---------------------------------------------------------------------------------------------------------------------
// Create projection from WKT string.
void
geomodelgrids::utils::Projection::fromWKT(const char* wkt) {}


// ---------------------------------------------------------------------------------------------------------------------
// Initialize projection.
void
geomodelgrids::utils::Projection::initialize(void) {}


// ---------------------------------------------------------------------------------------------------------------------
// Compute xy coordinates in geographic projection.
void
geomodelgrids::utils::Projection::project(double* x,
                                          double* y,
                                          const double longitude,
                                          const double latitude) {}


// End of file
