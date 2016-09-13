## The ```main()``` entry point

The usual way of writing tests in C++ has always been into separate source files from the code they test that form an executable containing only tests. In that scenario the default ```main()``` provided by **doctest** is usually sufficient:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
```

This should be done in exactly one source file and is even a good idea to do this in a separate file with nothing else in it.

However if you need more control - want to set options with code to the execution context or want to integrate the framework in your production code - then the default ```main()``` just won't do the job. In that case use **```DOCTEST_CONFIG_IMPLEMENT```**.

All the [**command line**](commandline.md) options can be set like this (flags cannot because it wouldn't make sense). Filters can only be appended - the user cannot remove a filter given from the command line with code. 

```c++
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

int main(int argc, char** argv) {
    doctest::Context context; // initialize

    // defaults
    context.addFilter("test-case-exclude", "*math*"); // exclude test cases with "math" in their name
    context.setOption("abort-after", 5);              // stop test execution after 5 failed assertions
    context.setOption("sort", "name");                // sort the test cases by their name

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

When integrating the framework in production code which gets built as a shared object (dll) everything still works. Many shared objects and an executable can have tests in them and can even use different versions of the **doctest** framework.

Check out [**this**](../../examples/dll_and_executable/) example which showcases how to call the tests in a shared object from the executable (and it also showcases that if a file with a test case is included both in the shared object and the executable then the test is registered in both places). 

---------------

[Home](readme.md#reference)
