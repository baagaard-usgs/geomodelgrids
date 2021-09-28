# Utilities Indexing Objects

## Classes

* [Indexing](cxx-api-utils-indexing)
* [IndexingUniform](cxx-api-utils-indexing-uniform)
* [IndexingVariable](cxx-api-utils-indexing-variable)

(cxx-api-utils-indexing)=
## Indexing

**Full name**: geomodelgrids::utils::Indexing

### Methods

+ [Indexing()](cxx-api-utils-indexing-Indexing)
+ [getIndex(const double x)](cxx-api-utils-indexing-getIndex)

(cxx-api-utils-indexing-Indexing)=
#### Indexing()

Constructor.

(cxx-api-utils-indexing-getIndex)=
#### getIndex(const double x)

Get index.

* **x[in]** Coordinate value.
* **returns** Index for coordinate value.

(cxx-api-utils-indexing-uniform)=
## IndexingUniform

**Full name**: geomodelgrids::utils::IndexingUniform

### Methods

+ [IndexingUniform()](cxx-api-utils-indexing-uniform-IndexingUniform)
+ [getIndex(const double x)](cxx-api-utils-indexing-uniform-getIndex)

(cxx-api-utils-indexing-uniform-IndexingUniform)=
#### IndexingUniform()

Constructor.

(cxx-api-utils-indexing-uniform-getIndex)=
#### getIndex(const double x)

Get index.

* **x[in]** Coordinate value.
* **returns** Index for coordinate value.

(cxx-api-utils-indexing-variable)=
## IndexingVariable

**Full name**: geomodelgrids::utils::IndexingVariable

### Enums

* **ASCENDING** Order coordinates in ascending order.
* **DESCENDING** Order coordinates in descending order.

### Methods

* [IndexingVariable(const double*, const size_t, SortOrder)](cxx-api-utils-indexing-Variable-IndexingVariable)
* [getIndex(const double x)](cxx-api-utils-indexing-Variable-getIndex)

(cxx-api-utils-indexing-Variable-IndexingVariable)=
#### IndexingVariable(const double* x, const size_t numX, SortOrder sortOrder=ASCENDING)

Constructor.

* **x[in]** Array of coordinates along axis.
* **numX[in]** Number of coordinates along axis.
* **sortOrder[in]** Order of coordinate indexing.

(cxx-api-utils-indexing-Variable-getIndex)=
#### getIndex(const double x)

Get index.

* **x[in]** Coordinate value.
* **returns** Index for coordinate value.
