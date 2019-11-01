// C++ driver for application to print model metadata to stdout.

#include "geomodelgrids/apps/Info.hh" // USES Info

int
main(int argc,
     char* argv[]) {
    geomodelgrids::apps::Info info;
    return info.run(argc, argv);
} // main


// End of file
