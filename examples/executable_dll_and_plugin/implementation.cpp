#define DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

#include <iostream>

TEST_CASE("implementation") {
    std::cout << "I am a test from the implementation!" << std::endl;
}
