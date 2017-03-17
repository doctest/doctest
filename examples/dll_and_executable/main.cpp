#define DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#include "doctest.h"

#include <cstdio>

// throws an int(0) by default
template<typename T = int>
static int conditional_throw(bool in, const T& ex = T()) { if(in) throw ex; return 42; }

TEST_CASE("executable") {
    printf("I am a test from the executable!\n");
    conditional_throw(true, 'a');
}

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#ifdef _MSC_VER
#define LoadDynamicLib(lib) LoadLibrary(lib ".dll")
#else // _MSC_VER
#define LoadDynamicLib(lib) LoadLibrary("lib" lib ".dll")
#endif // _MSC_VER
#else // _WIN32
#include <dlfcn.h>
#ifdef __APPLE__
#define LoadDynamicLib(lib) dlopen("lib" lib ".dylib", RTLD_NOW)
#else // __APPLE__
#define LoadDynamicLib(lib) dlopen("lib" lib ".so", RTLD_NOW)
#endif // __APPLE__
#endif // _WIN32

// set an exception translator for double
REGISTER_EXCEPTION_TRANSLATOR(double& e) {
    return doctest::String("double: ") + doctest::toString(e);
}

int main(int argc, char** argv) {
    // force the use of a symbol from the dll so tests from it get registered
    DOCTEST_SYMBOL_IMPORT void from_dll(); from_dll();

    LoadDynamicLib("plugin"); // load the plugin so tests from it get registered

    doctest::Context context(argc, argv);
    int res = context.run();
    
    if(context.shouldExit()) // important - query flags (and --exit) rely on the user doing this
        return res;          // propagate the result of the tests

    return res; // the result from doctest is propagated here as well
}
