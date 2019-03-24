#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

int main(int argc, char **argv) {
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    
    int res = context.run(); // run doctest
    
    // important - query flags (and --exit) rely on the user doing this
    if (context.shouldExit()) {
        // propagate the result of the tests
        return res;
    }
    
    printf("%s", "Hello, World!");
}

int factorial(const int number) {
    return number < 1 ? 1 : number <= 1 ? number : factorial(number - 1) * number;
}

TEST_CASE("testing the factorial function") {
    CHECK(factorial(0) == 1);
    CHECK(factorial(1) == 1);
    CHECK(factorial(2) == 2);
    CHECK(factorial(3) == 6);
    CHECK(factorial(10) == 3628800);
}

// running notes
// ./example_exe --no-run (run normal program)
// ./example_exe --exit (run tests then exit)
// ./example_exe (run tests then run program)
// ./example_exe --success (print successful test casts)