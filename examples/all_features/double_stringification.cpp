#define DOCTEST_CONFIG_DOUBLE_STRINGIFY
#include <doctest_fwd.h>

#include <string>

namespace App {
struct Foo {};
static std::string toString(Foo *) {
    return "Foo";
}
} // namespace App

TEST_CASE("toString std::string ret type") {
    App::Foo foo; // NOLINT(misc-const-correctness)
    CHECK(&foo != nullptr);
    CHECK_NE(&foo, nullptr);
    CHECK(&foo);
}
