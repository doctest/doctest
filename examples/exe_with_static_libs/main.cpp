namespace A {
    struct Bar {
        int i;
    };
}

static bool operator==(const A::Bar& lhs, const A::Bar& rhs) {
    return lhs.i == rhs.i;
}

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_CASE("main") { printf("hello from <main.cpp>\n"); }


TEST_CASE("equality of namespaced type") {
    const auto b0 = A::Bar{};
    const auto b1 = A::Bar{};
    CHECK(b0 == b1);
    CHECK_EQ(b0, b1);
}
