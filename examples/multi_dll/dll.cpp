#define BUILDING_DLL
#include "common.h"

doctest_test(dll) {
    printf("I am a test from the dll!\n");
}

DLL_PUBLIC int call_tests_from_dll(int argc, char** argv) {
    void* params = doctest::createParams(argc, argv);
    int res = doctest::runTests(params);
    doctest::freeParams(params);
	return res;
}
