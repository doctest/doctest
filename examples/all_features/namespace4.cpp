#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <cstdint>
#include <sstream>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

namespace user4 {
struct label
{
    label()
            : i(0) {}
    int  i;
    bool operator==(const user4::label& rhs) const { return i == rhs.i; }
};
} // namespace user4

namespace user5 {
struct label
{
    label()
            : i(0) {}
    int  i;
    bool operator==(const user5::label& rhs) const { return i == rhs.i; }
};
} // namespace user5

TEST_CASE("namespace 4 member vs member") {
    user4::label a4;
    user4::label b4;

    user5::label a5;
    user5::label b5;

    REQUIRE(a4 == b4);
    REQUIRE(a5 == b5);
}
