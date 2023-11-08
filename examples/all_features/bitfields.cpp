#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <cstdint>
#include <sstream>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

DOCTEST_GCC_SUPPRESS_WARNING("-Wmissing-declarations")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wmissing-prototypes")

struct S {
    uint32_t a : 31;
    uint32_t b : 1;
};
TEST_CASE("bitfield") {
    S s;
    s.a = 0;
    s.b = 1;
    CHECK(s.a == 0); // error here
}
