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
    int         i;
    friend bool operator==(const user6::label& lhs, const user6::label& rhs) {
        return lhs.i == rhs.i;
    }
};
} // namespace user6

namespace user8 {
struct label
{
    label()
            : i(0) {}
    int i;
};
} // namespace user8


DOCTEST_GCC_SUPPRESS_WARNING("-Wmissing-declarations")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wmissing-prototypes")

bool operator==(const user8::label& lhs, const user8::label& rhs) { return lhs.i == rhs.i; }


TEST_CASE("namespace 8 friend vs global") {
    user6::label a6;
    user6::label b6;

    user8::label a8;
    user8::label b8;

    REQUIRE(a6 == b6);
    REQUIRE(a8 == b8);
}
