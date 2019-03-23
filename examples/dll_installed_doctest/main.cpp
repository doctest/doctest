#define DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#include <doctest/doctest.h>

#include "dll.h"

int main(int argc, char** argv)
{
    doctest::Context context;
    context.applyCommandLine(argc, argv);

    int res = context.run(); // run doctest

    // important - query flags (and --exit) rely on the user doing this
    if(context.shouldExit()) {
        // propagate the result of the tests
        return res;
    }

    say_hello_dll(); // test dll func
}

int square(const int number) { return number * number; }

TEST_CASE("testing the square function") {
    CHECK(square(2) == 4);
    CHECK(square(4) == 16);
    CHECK(square(5) == 25);
    CHECK(square(8) == 64);
}

// running notes
// ./program --no-run (run normal program)
// ./program --exit (run tests then exit)
// ./program (run tests then run program)
// ./program --success (print successful test casts)