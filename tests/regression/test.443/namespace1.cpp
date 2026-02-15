#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <cstdint>
#include <sstream>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

namespace user1 {
struct label
{
    label()
            : i(0) {}
    int i;
};
} // namespace user1

DOCTEST_GCC_SUPPRESS_WARNING("-Wmissing-declarations")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wmissing-prototypes")

bool operator==(const user1::label& lhs, const user1::label& rhs) { return lhs.i == rhs.i; }


TEST_CASE("namespace 1 global operator") {
    user1::label a;
    user1::label b;
    CHECK(a == b);
}
