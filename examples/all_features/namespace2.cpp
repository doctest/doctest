#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <cstdint>
#include <sstream>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

namespace user2 {
struct label
{
    label()
            : i(0) {}
    int         i;
    friend bool operator==(const user2::label& lhs, const user2::label& rhs) {
        return lhs.i == rhs.i;
    }
};
} // namespace user2

TEST_CASE("namespace 2 friend operator") {
    user2::label a;
    user2::label b;
    REQUIRE(a == b);
}
