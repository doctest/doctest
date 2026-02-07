#define DOCTEST_CONFIG_COLORS_NONE
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <string>
#include <sstream>

namespace {

/** @return the text that the color stringizes to */
inline std::string text(doctest::Color::Enum color) noexcept {
    std::ostringstream oss { };
    oss << color;
    return oss.str();
}

} // namespace

TEST_CASE("Colorizing a stream") {
    using namespace doctest;

    CHECK(text(Color::None)        == "");
    CHECK(text(Color::White)       == "");
    CHECK(text(Color::Red)         == "");
    CHECK(text(Color::Green)       == "");
    CHECK(text(Color::Blue)        == "");
    CHECK(text(Color::Cyan)        == "");
    CHECK(text(Color::Yellow)      == "");
    CHECK(text(Color::Grey)        == "");
    CHECK(text(Color::BrightRed)   == "");
    CHECK(text(Color::BrightGreen) == "");
    CHECK(text(Color::LightGrey)   == "");
    CHECK(text(Color::BrightWhite) == "");
}
