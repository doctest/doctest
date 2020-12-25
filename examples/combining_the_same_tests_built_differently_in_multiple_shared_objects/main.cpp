#define DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#include "doctest/doctest.h"

DOCTEST_SYMBOL_IMPORT void default_cpp_force_link();
DOCTEST_SYMBOL_IMPORT void return42_cpp_force_link();

TEST_CASE("main") { MESSAGE("hello from <main.cpp>"); }

int main(int argc, char** argv) {

    default_cpp_force_link();
    return42_cpp_force_link();

    doctest::Context context(argc, argv);
    int res = context.run();

    if(context.shouldExit()) // important - query flags (and --exit) rely on the user doing this
        return res;          // propagate the result of the tests

    int client_stuff_return_code = 0;
    // your program - if the testing framework is integrated in your production code

    return res + client_stuff_return_code; // the result from doctest is propagated here as well
}
