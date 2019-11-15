#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/TextOutputter.h>

#include <stdlib.h> // USES abort()

int
main(int argc,
     char* argv[]) {
    CppUnit::TestResultCollector result;

    try {
        // Create event manager and test controller
        CppUnit::TestResult controller;

        // Add listener to collect test results
        controller.addListener(&result);

        // Add listener to show progress as tests run
        CppUnit::BriefTestProgressListener progress;
        controller.addListener(&progress);

        // Add top suite to test runner
        CppUnit::TestRunner runner;
        runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
        runner.run(controller);

        // Print tests
        CppUnit::TextOutputter outputter(&result, std::cerr);
        outputter.write();

    } catch (...) {
        abort();
    } // catch

    return (result.wasSuccessful() ? 0 : 1);
} // main


// End of file
