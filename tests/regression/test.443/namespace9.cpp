#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <cstdint>
#include <sstream>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

namespace user9a {
struct label
{
    label()
            : i(0) {}
    int i;
};
} // namespace user9a

DOCTEST_GCC_SUPPRESS_WARNING("-Wmissing-declarations")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wmissing-prototypes")
bool operator==(const user9a::label& lhs, const user9a::label& rhs) { return lhs.i == rhs.i; }

namespace user9b {
struct label
{
    label()
            : i(0) {}
    int i;
};
} // namespace user9b

DOCTEST_GCC_SUPPRESS_WARNING("-Wmissing-declarations")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wmissing-prototypes")

bool operator==(const user9b::label& lhs, const user9b::label& rhs) { return lhs.i == rhs.i; }

TEST_CASE("namespace 9 both global") {
    user9a::label a1;
    user9a::label a2;

    user9b::label b1;
    user9b::label b2;

    REQUIRE(a1 == a2);
    REQUIRE(b1 == b2);
}
