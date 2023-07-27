## The ```main()``` entry point

The usual way of writing tests in C++ has always been into separate source files from the code they test that form an executable containing only tests. In that scenario the default ```main()``` provided by **doctest** is usually sufficient:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
```

This should be done in exactly one source file and is even a good idea to do this in a separate file with nothing else in it.

However if you need more control - want to set options with code to the execution context or want to integrate the framework in your production code - then the default ```main()``` just won't do the job. In that case use [**```DOCTEST_CONFIG_IMPLEMENT```**](configuration.md#doctest_config_implement).

All the [**command line**](commandline.md) options can be set like this (flags cannot because it wouldn't make sense). Filters can only be appended or cleared with the ```addFilter()``` or ```clearFilters()``` method of a ```doctest::Context``` object - the user cannot remove a specific filter with code.

```c++
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

int main(int argc, char** argv) {
    doctest::Context context;

    // !!! THIS IS JUST AN EXAMPLE SHOWING HOW DEFAULTS/OVERRIDES ARE SET !!!

    // defaults
    context.addFilter("test-case-exclude", "*math*"); // exclude test cases with "math" in their name
    context.setOption("abort-after", 5);              // stop test execution after 5 failed assertions
    context.setOption("order-by", "name");            // sort the test cases by their name

    context.applyCommandLine(argc, argv);

    // overrides
    context.setOption("no-breaks", true);             // don't break in the debugger when assertions fail

    int res = context.run(); // run

    if(context.shouldExit()) // important - query flags (and --exit) rely on the user doing this
        return res;          // propagate the result of the tests
    
    int client_stuff_return_code = 0;
    // your program - if the testing framework is integrated in your production code
    
    return res + client_stuff_return_code; // the result from doctest is propagated here as well
}

```

Note the call to ```.shouldExit()``` on the context - that is very important - it will be set when a query flag has been used (or the ```--no-run``` option is set to ```true```) and it is the user's responsibility to exit the application in a normal way.

### Dealing with shared objects (DLLs)

The framework can be used separately in binaries (executables / shared objects) with each having its own test runner - this way even different versions of doctest can be used - but there will be no simple way to execute the tests from all loaded binaries and have the results aggregated and summarized.

There is also an option to have the test runner (implementation) built in a binary and shared with others (so there is a single test registry) by exporting its public symbols (the ones needed for writing tests by the user - all the forward declarations of the framework).

For more info on that checkout the [**```DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL```**](configuration.md#doctest_config_implementation_in_dll) config identifier and [**this example**](../../examples/executable_dll_and_plugin/).

---------------

[Home](readme.md#reference)

<p align="center"><img src="../../scripts/data/logo/icon_2.svg"></p>
