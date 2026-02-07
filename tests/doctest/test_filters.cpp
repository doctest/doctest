#include <doctest/parts/private/filters.h>
#include <functional>
#include <vector>

using doctest::detail::wildcmp;
using doctest::detail::matchesAny;
using doctest::String;

TEST_SUITE("Comparing a single string to a glob") {

    using std::placeholders::_1;

    TEST_CASE("Empty glob") {
        const auto glob = std::bind(wildcmp, _1, "", true);
        CHECK(glob("")         == 1);
        CHECK(glob("nonempty") == 0);
    }

    TEST_CASE("Exact glob") {
        SUBCASE("Case-sensitive") {
            const auto glob = std::bind(wildcmp, _1, "test", true);
            CHECK(glob("")          == 0);
            CHECK(glob("tes")       == 0);
            CHECK(glob("test")      == 1);
            CHECK(glob("Test")      == 0);
            CHECK(glob("lest")      == 0);
            CHECK(glob("text")      == 0);
            CHECK(glob("tess")      == 0);
            CHECK(glob("test!")     == 0);
            CHECK(glob("retesting") == 0);
        }

        SUBCASE("Case-insensitive") {
            const auto glob = std::bind(wildcmp, _1, "test", false);
            CHECK(glob("")      == 0);
            CHECK(glob("tes")   == 0);
            CHECK(glob("test")  == 1);
            CHECK(glob("Test")  == 1);
            CHECK(glob("lest")  == 0);
            CHECK(glob("text")  == 0);
            CHECK(glob("tess")  == 0);
            CHECK(glob("test!") == 0);
        }
    }

    TEST_CASE("Single-char wildcard") {
        SUBCASE("Leading wildcard") {
            const auto glob = std::bind(wildcmp, _1, "?est", true);
            CHECK(glob("")     == 0);
            CHECK(glob("tes")  == 0);
            CHECK(glob("test") == 1);
            CHECK(glob("Test") == 1);
            CHECK(glob("best") == 1);
            CHECK(glob("this") == 0);
        }

        SUBCASE("Intermediate wildcard") {
            const auto glob = std::bind(wildcmp, _1, "te?t", true);
            CHECK(glob("")      == 0);
            CHECK(glob("tex")   == 0);
            CHECK(glob("test")  == 1);
            CHECK(glob("teSt")  == 1);
            CHECK(glob("text")  == 1);
            CHECK(glob("text!") == 0);
        }

        SUBCASE("Trailing wildcard") {
            const auto glob = std::bind(wildcmp, _1, "tes?", true);
            CHECK(glob("")      == 0);
            CHECK(glob("tes")   == 0);
            CHECK(glob("test")  == 1);
            CHECK(glob("tesT")  == 1);
            CHECK(glob("tess")  == 1);
            CHECK(glob("test!") == 0);
        }
    }

    TEST_CASE("Multi-char wildcard") {
        SUBCASE("Only wildcard") {
            const auto glob = std::bind(wildcmp, _1, "*", true);
            CHECK(glob("")       == 1);
            CHECK(glob("test")   == 1);
            CHECK(glob("string") == 1);
        }

        SUBCASE("Only multiple wildcards") {
            const auto glob = std::bind(wildcmp, _1, "**", true);
            CHECK(glob("")       == 1);
            CHECK(glob("test")   == 1);
            CHECK(glob("string") == 1);
        }

        SUBCASE("Leading wildcard") {
            const auto glob = std::bind(wildcmp, _1, "*.png", true);
            CHECK(glob("")        == 0);
            CHECK(glob(".png")    == 1);
            CHECK(glob("foo.png") == 1);
            CHECK(glob("foo.bmp") == 0);
        }

        SUBCASE("Intermediate wildcard") {
            const auto glob = std::bind(wildcmp, _1, "img-*.png", true);
            CHECK(glob("")           == 0);
            CHECK(glob("img-.png")   == 1);
            CHECK(glob("img-0.png")  == 1);
            CHECK(glob("img-42.png") == 1);
            CHECK(glob("bad-1.png")  == 0);
        }

        SUBCASE("Trailing wildcard") {
            const auto glob = std::bind(wildcmp, _1, "file.*", true);
            CHECK(glob("")         == 0);
            CHECK(glob("file.")    == 1);
            CHECK(glob("file.png") == 1);
            CHECK(glob("file.PNG") == 1);
            CHECK(glob("file.bmp") == 1);
            CHECK(glob("file.BMP") == 1);
        }

        SUBCASE("Leading and trailing wildcard") {
            const auto glob = std::bind(wildcmp, _1, "* 2 *", true);
            CHECK(glob("1")     == 0);
            CHECK(glob("2")     == 0);
            CHECK(glob("3")     == 0);
            CHECK(glob("1 2 3") == 1);
            CHECK(glob("_ 2 3") == 1);
            CHECK(glob("1 2 _") == 1);
        }
    }

}

TEST_SUITE("Comparing a single string to multiple globs") {

    using std::placeholders::_1;

    TEST_CASE("Empty set of filters") {
        const auto filters       = std::vector<String> { };
        const auto caseSensitive = true;

        SUBCASE("With matchesEmpty disabled") {
            const auto matchesEmpty = false;
            CHECK(matchesAny("test", filters, matchesEmpty, caseSensitive) == false);
        }

        SUBCASE("With matchesEmpty enabled") {
            const auto matchesEmpty = true;
            CHECK(matchesAny("test", filters, matchesEmpty, caseSensitive) == true);
        }
    }

    TEST_CASE("Non-empty set of filters") {
        const auto filters = std::vector<String> { "foo", "bar", "baz" };
        const auto matches = std::bind(matchesAny, _1, filters, false, true);
        CHECK(matches("foo")  == true);
        CHECK(matches("bar")  == true);
        CHECK(matches("baz")  == true);
        CHECK(matches("quux") == false);
    }

}
