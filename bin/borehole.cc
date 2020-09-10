// C++ driver for application to query model with virtual borehole.

#include "geomodelgrids/apps/Borehole.hh" // USES Borehole

int
main(int argc,
     char* argv[]) {
    geomodelgrids::apps::Borehole borehole;
    return borehole.run(argc, argv);
} // main


// End of file
