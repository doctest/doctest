#include <doctest/doctest.h>
using doctest::Contains;

TEST_CASE("Stringification") {
    // clang-format off
    CHECK(doctest::toString(Contains(""))        == "Contains(  )");
    CHECK(doctest::toString(Contains("doctest")) == "Contains( doctest )");
    // clang-format on
}

TEST_CASE("Matching against string values") {
    CHECK("" == doctest::Contains(""));
    CHECK("" != doctest::Contains("doctest"));

    CHECK("doctest" == doctest::Contains(""));
    CHECK("doctest" == doctest::Contains("doc"));
    CHECK("doctest" == doctest::Contains("test"));
    CHECK("doctest" == doctest::Contains("doctest"));
    CHECK("doctest" != doctest::Contains("dctst"));
    CHECK("doctest" != doctest::Contains("DOCTEST"));
}
