#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

TEST_CASE("alpha;beta") { CHECK(true); }
TEST_CASE("regular") { CHECK(true); }
TEST_CASE("two;;semicolons") { CHECK(true); }
TEST_CASE("comma,semi;colon") { CHECK(true); }
