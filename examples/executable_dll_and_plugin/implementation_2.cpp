// note that DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL should not be defined here
#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <iostream>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

TEST_CASE("implementation_2") {
    std::cout << "I am a test from the implementation_2!" << std::endl;
}
