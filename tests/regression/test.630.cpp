#include <doctest/doctest.h>

namespace {

DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wunused-member-function")
class MoveOnly {
public:
    MoveOnly(int iIn)
        : i(iIn) {}
    MoveOnly(MoveOnly &&) = default;
    MoveOnly(const MoveOnly &) = delete;
    MoveOnly &operator=(MoveOnly &&) = default;
    MoveOnly &operator=(const MoveOnly &) = default;
    ~MoveOnly() = default;
    // NOLINTNEXTLINE(readability-make-member-function-const)
    operator bool() const { // both non-const and const can call
        return i == 42;
    }

private:
    int i;
};
DOCTEST_CLANG_SUPPRESS_WARNING_POP

MoveOnly genType(bool b) {
    return {b ? 42 : 0};
}

} // namespace

TEST_CASE("Move Only Type") {
    CHECK(genType(true));
    CHECK_FALSE(genType(false));

    MoveOnly a{0}; // NOLINT(misc-const-correctness)
    CHECK_FALSE(a);
}
