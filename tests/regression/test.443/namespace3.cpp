#include <doctest/doctest.h>

#include <cstdint>
#include <sstream>

namespace user3 {
struct label {
    label()
        : i(0) {}
    int i;
    bool operator==(const user3::label &rhs) const {
        return i == rhs.i;
    }
};
} // namespace user3

TEST_CASE("namespace 3 member operator") {
    const user3::label a;
    const user3::label b;
    REQUIRE(a == b);
}
