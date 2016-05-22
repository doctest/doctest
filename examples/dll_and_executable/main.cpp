#include "common.h"

TEST_CASE("executable") {
    printf("I am a test from the executable!\n");
}

int main(int argc, char** argv) {
    doctest::Context context(argc, argv);
    int res = context.run();
    
    res += call_tests_from_dll(argc, argv);

    return res;
}
