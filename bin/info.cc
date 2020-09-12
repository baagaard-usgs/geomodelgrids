// C++ driver for application to print model metadata to stdout.

#include "geomodelgrids/apps/Info.hh" // USES Info

#include <stdexcept> // USES std::exception
#include <iostream> // USES std::cerr

int
main(int argc,
     char* argv[]) {
    geomodelgrids::apps::Info info;

    int err = 0;
    try {
      err = info.run(argc, argv);
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
