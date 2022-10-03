#define DOCTEST_CONFIG_DOUBLE_STRINGIFY
#include <doctest_fwd.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <string>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

namespace App {
    struct Foo { };
    static std::string toString(Foo*) { return "Foo"; }
}

TEST_CASE("toString std::string ret type") {
    App::Foo foo;
    CHECK(&foo != nullptr);
    CHECK_NE(&foo, nullptr);
    CHECK(&foo);
}
