#include "common.h"

TEST_CASE("executable") {
    printf("I am a test from the executable!\n");
}

int main(int argc, char** argv) {
    doctest::Context context(argc, argv);
    int res = context.run();
    
    res += call_tests_from_dll(argc, argv);

    if(context.shouldExit()) // important - query flags (and --exit) rely on the user doing this
        return res;          // propagate the result of the tests

    return res; // the result from doctest is propagated here as well
}
