#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <cstdint>
#include <sstream>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

namespace user6 {
struct label
{
    label()
            : i(0) {}
    int  i;
    bool operator==(const user6::label& rhs) const { return i == rhs.i; }
};
} // namespace user6

namespace user7 {
struct label
{
    label()
            : i(0) {}
    int i;
};
} // namespace user7

DOCTEST_GCC_SUPPRESS_WARNING("-Wmissing-declarations")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wmissing-prototypes")

bool operator==(const user7::label& lhs, const user7::label& rhs) { return lhs.i == rhs.i; }

TEST_CASE("namespace 7 member vs global") {
    user6::label a6;
    user6::label b6;

    user7::label a7;
    user7::label b7;

    REQUIRE(a6 == b6);
    REQUIRE(a7 == b7);
}
