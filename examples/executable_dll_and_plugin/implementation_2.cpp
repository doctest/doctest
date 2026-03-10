// note that DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL should not be defined here
#include <doctest/doctest.h>

#include <iostream>

TEST_CASE("implementation_2") {
    std::cout << "I am a test from the implementation_2!" << std::endl;
}
