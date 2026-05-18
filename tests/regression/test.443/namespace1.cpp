#include <doctest/doctest.h>

#include <cstdint>
#include <sstream>

namespace user1 {
struct label {
    label()
        : i(0) {}
    int i;
};
} // namespace user1

DOCTEST_GCC_SUPPRESS_WARNING("-Wmissing-declarations")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wmissing-prototypes")

bool operator==(const user1::label &lhs, const user1::label &rhs) {
    return lhs.i == rhs.i;
}

TEST_CASE("namespace 1 global operator") {
    const user1::label a;
    const user1::label b;
    CHECK(a == b);
}
