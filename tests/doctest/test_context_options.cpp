#include <doctest/doctest.h>

TEST_CASE("Context::setOption(bool) works for query flags") {
    doctest::Context ctx(0, nullptr);
    ctx.setOption("list-test-suites", true);
    CHECK(ctx.shouldExit());
}

TEST_CASE("Context::setOption(bool) works for help") {
    doctest::Context ctx(0, nullptr);
    ctx.setOption("help", true);
    CHECK(ctx.shouldExit());
}
