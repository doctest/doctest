#define DOCTEST_CONFIG_COLORS_ANSI
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

/** @return the text with the escape char stripped */
inline std::string escape(std::string text) noexcept {
    if (!text.empty() && text.front() == '\033') {
        return text.substr(1);
    } else {
        return text;
    }
}

} // namespace

DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4866)
TEST_CASE("Colorizing a stream") {
    using namespace doctest;

    doctest::detail::g_cs->force_colors = true;
    CHECK(escape(text(Color::None))        == "[0m");
    CHECK(escape(text(Color::White))       == "[0m");
    CHECK(escape(text(Color::Red))         == "[0;31m");
    CHECK(escape(text(Color::Green))       == "[0;32m");
    CHECK(escape(text(Color::Blue))        == "[0;34m");
    CHECK(escape(text(Color::Cyan))        == "[0;36m");
    CHECK(escape(text(Color::Yellow))      == "[0;33m");
    CHECK(escape(text(Color::Grey))        == "[1;30m");
    CHECK(escape(text(Color::BrightRed))   == "[1;31m");
    CHECK(escape(text(Color::BrightGreen)) == "[1;32m");
    CHECK(escape(text(Color::LightGrey))   == "[0;37m");
    CHECK(escape(text(Color::BrightWhite)) == "[1;37m");
    doctest::detail::g_cs->force_colors = false;
}
DOCTEST_MSVC_SUPPRESS_WARNING_POP
