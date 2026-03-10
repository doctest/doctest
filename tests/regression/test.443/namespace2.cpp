#include <doctest/doctest.h>

#include <cstdint>
#include <sstream>

namespace user2 {
struct label {
    label()
        : i(0) {}
    int i;
    friend bool operator==(const user2::label &lhs, const user2::label &rhs) {
        return lhs.i == rhs.i;
    }
};
} // namespace user2

TEST_CASE("namespace 2 friend operator") {
    const user2::label a;
    const user2::label b;
    REQUIRE(a == b);
}
