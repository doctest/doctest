#define BUILDING_DLL
#include "common.h"

TESTCASE("dll") {
    printf("I am a test from the dll!\n");
}

DLL_PUBLIC int call_tests_from_dll(int argc, char** argv) {
    doctest::Context context(argc, argv);
    return context.run();
}
