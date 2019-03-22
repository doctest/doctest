#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

#include "header.h"

int program();
void some_program_code(int argc, char** argv);

int main(int argc, char** argv) {
    doctest::Context context;

    // !!! THIS IS JUST AN EXAMPLE SHOWING HOW DEFAULTS/OVERRIDES ARE SET !!!

    // defaults
    context.addFilter("test-case-exclude", "*math*"); // exclude test cases with "math" in the name
    context.setOption("rand-seed", 324);              // if order-by is set to "rand" use this seed
    context.setOption("order-by", "file");            // sort the test cases by file and line

    context.applyCommandLine(argc, argv);

    // overrides
    context.setOption("no-breaks", true); // don't break in the debugger when assertions fail

    int res = context.run(); // run queries, or run tests unless --no-run is specified

    if(context.shouldExit()) // important - query flags (and --exit) rely on the user doing this
        return res;          // propagate the result of the tests

    context.clearFilters(); // removes all filters added up to this point

    int client_stuff_return_code = program();
    some_program_code(argc, argv);
    // your program - if the testing framework is integrated in your production code

    return res + client_stuff_return_code; // the result from doctest is propagated here as well
}

TEST_CASE("[string] testing std::string") {
    std::string a("omg");
    CHECK(a == "omg");
}

TEST_CASE("[math] basic stuff") {
    CHECK(6 > 5);
    CHECK(6 > 7);
}

int program() {
    printf("Program code.\n");
    return EXIT_SUCCESS;
}
