#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

TEST_CASE("throws") {
    CHECK(1 == 2);
}

int main(int argc, char** argv) {
    doctest::Context context;

    context.addFilter("test-case-exclude", "*math*");
    context.setOption("no-breaks", true);
    context.setOption("sort", "name");

    context.applyCommandLine(argc, argv);

    int res = context.run();

    if(context.shouldExit())
        return res;

    return res;
}
