#define DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <cstdio>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

template<typename T>
static int conditional_throw(bool in, const T& ex) {
    if(in)
#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
        throw ex;
#else // DOCTEST_CONFIG_NO_EXCEPTIONS
        ((void)ex);
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS
    return 42;
}

TEST_CASE("executable") {
    printf("I am a test from the executable!\n");
    conditional_throw(true, 'a');
}

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <windows.h>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END
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

DOCTEST_SYMBOL_IMPORT void from_dll();

int main(int argc, char** argv) {
    // force the use of a symbol from the dll so tests from it get registered
    from_dll();

    LoadDynamicLib("plugin"); // load the plugin so tests from it get registered

    doctest::Context context(argc, argv);
    int res = context.run();
    
    if(context.shouldExit()) // important - query flags (and --exit) rely on the user doing this
        return res;          // propagate the result of the tests

    int client_stuff_return_code = 0;
    // your program - if the testing framework is integrated in your production code

    return res + client_stuff_return_code; // the result from doctest is propagated here as well
}
