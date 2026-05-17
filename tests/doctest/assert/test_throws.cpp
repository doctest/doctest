#if !defined(DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS)

#include <doctest/doctest.h>

#include <stdexcept>

TEST_CASE("CHECK_THROWS vs CHECK_THROWS_WITH") {
    CHECK_THROWS(throw std::runtime_error("hello"));
    CHECK_THROWS_WITH(throw std::runtime_error("hello"), "hello");
    CHECK_FALSE(CHECK_THROWS_WITH(throw std::runtime_error("hello"), "world"));
}

#endif
