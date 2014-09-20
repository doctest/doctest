#define BUILDING_DLL
#include "common.h"

doctest_test(dll) {
    printf("I am a test from the dll!\n");
}

DLL_PUBLIC void call_tests_from_dll(int argc, char** argv) {
    DOCTEST_INVOKE_ALL_TEST_FUNCTIONS(argc, argv);
}
