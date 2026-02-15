#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <cstdint>
#include <sstream>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

namespace user3 {
struct label
{
    label()
            : i(0) {}
    int  i;
    bool operator==(const user3::label& rhs) const { return i == rhs.i; }
};
} // namespace user3

TEST_CASE("namespace 3 member operator") {
    user3::label a;
    user3::label b;
    REQUIRE(a == b);
}
