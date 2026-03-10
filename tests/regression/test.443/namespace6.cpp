#include <doctest/doctest.h>

#include <cstdint>
#include <sstream>

namespace user6 {
struct label {
    label()
        : i(0) {}
    int i;
    friend bool operator==(const user6::label &lhs, const user6::label &rhs) {
        return lhs.i == rhs.i;
    }
};
} // namespace user6

namespace user7 {
struct label {
    label()
        : i(0) {}
    int i;
    friend bool operator==(const user7::label &lhs, const user7::label &rhs) {
        return lhs.i == rhs.i;
    }
};
} // namespace user7

TEST_CASE("namespace 6 friend vs friend") {
    const user6::label a6;
    const user6::label b6;

    const user7::label a7;
    const user7::label b7;

    REQUIRE(a6 == b6);
    REQUIRE(a7 == b7);
}
