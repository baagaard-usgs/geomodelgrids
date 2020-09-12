// C++ driver for application to query model with virtual borehole.

#include "geomodelgrids/apps/Borehole.hh" // USES Borehole

#include <stdexcept> // USES std::exception
#include <iostream> // USES std::cerr

int
main(int argc,
     char* argv[]) {
    geomodelgrids::apps::Borehole borehole;

    int err = 0;
    try {
      err = borehole.run(argc, argv);
    } catch (const std::exception& ex) {
	std::cerr << ex.what() << std::endl;
	err = 1;
    } catch (...) {
      std::cerr << "Caught unknown exception." << std::endl;
      err = 2;
    } // try/catch

    return err;
} // main


// End of file
