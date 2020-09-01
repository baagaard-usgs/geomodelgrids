// C++ driver for application to query model elevation.

#include "geomodelgrids/apps/QueryElev.hh" // USES QueryElev

int
main(int argc,
     char* argv[]) {
    geomodelgrids::apps::QueryElev query;
    return query.run(argc, argv);
} // main


// End of file
