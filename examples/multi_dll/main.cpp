#include "common.h"

doctest_test(executable) {
    printf("I am a test from the executable!\n");
}

int main(int argc, char** argv) {
    void* params = doctest::createParams(argc, argv);
    int res = doctest::runTests(params);
    doctest::freeParams(params);
    
    res += call_tests_from_dll(argc, argv);

    return res;
}
