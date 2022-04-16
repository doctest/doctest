#include <doctest/doctest.h>

class MoveOnly {
    public:
        MoveOnly(int iIn) : i(iIn) { }
        MoveOnly(MoveOnly&&) = default;
        MoveOnly(const MoveOnly&) = delete;
        MoveOnly& operator=(MoveOnly&&) = default;
        MoveOnly& operator=(const MoveOnly&) = default;
        operator bool() { // NOT const!
            return i == 42;
        }

    private:
        int i;
};

MoveOnly genType(bool b) {
    return MoveOnly(b ? 42 : 0);
}

TEST_CASE("Move Only Type") {
    CHECK(genType(true));
    CHECK(genType(false));
}
