// examples/all_features/generators.cpp
#include <doctest/doctest.h>

#include "header.h"

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <sstream>
#include <vector>
#include <string>
using namespace std;
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

// ---------------------------------------------------------------------------
// Basic: single generator
// ---------------------------------------------------------------------------

TEST_CASE("GENERATE - single, two values") {
    auto x = GENERATE(10, 20);
    MESSAGE("x=", x);
    CHECK((x == 10 || x == 20));
}

TEST_CASE("GENERATE - single, three values") {
    auto x = GENERATE(1, 2, 3);
    MESSAGE("x=", x);
    CHECK(x >= 1);
    CHECK(x <= 3);
}

// ---------------------------------------------------------------------------
// Cartesian product: multiple generators, last cycles fastest
// ---------------------------------------------------------------------------

TEST_CASE("GENERATE - two generators, cartesian product") {
    static size_t iteration = 0;
    static const std::pair<int, int> expectations[] = {{1, 10}, {1, 20}, {2, 10}, {2, 20}};

    auto x = GENERATE(1, 2);   // outer: cycles slowest
    auto y = GENERATE(10, 20); // inner: cycles fastest
    MESSAGE("x=", x, " y=", y);
    REQUIRE(iteration < 2 * 2); // 2 values for x * 2 values for y
    auto expected = expectations[iteration++];
    CHECK(std::make_pair(x, y) == expected);
}

TEST_CASE("GENERATE - three generators") {
    auto a = GENERATE(1, 2);
    auto b = GENERATE(3, 4);
    auto c = GENERATE(5, 6);
    MESSAGE("a=", a, " b=", b, " c=", c);
    CHECK(a + b + c > 0);
}

TEST_CASE("GENERATE statements can depend on each other") {
    const int i = GENERATE(1, 2, 3);
    const int j = GENERATE(i + 1, i + 2, i + 3);
    MESSAGE("i=", i, " j=", j);
    CHECK(i <= j);
}

TEST_CASE("multiple GENERATEs on a single line") {
    // clang-format off
    // NOLINTNEXTLINE(readability-isolate-declaration)
    const int i = GENERATE(1, 2); const int j = GENERATE(10, 20);
    // clang-format on
    MESSAGE("i=", i, " j=", j);
}

// ---------------------------------------------------------------------------
// Non-int types
// ---------------------------------------------------------------------------

TEST_CASE("GENERATE - double values") {
    auto d = GENERATE(1.0, 2.5, 3.14);
    MESSAGE("d=", d);
    CHECK(d > 0.0);
}

TEST_CASE("GENERATE - const char* values") {
    auto s = GENERATE("hello", "world");
    CHECK(s != nullptr);
    CHECK(s[0] != '\0');
}

// ---------------------------------------------------------------------------
// Interaction with SUBCASE (generator-outer: all subcases per generator value)
// ---------------------------------------------------------------------------

TEST_CASE("GENERATE + SUBCASE - basic interaction") {
    auto x = GENERATE(1, 2);
    MESSAGE("test_case: x=", x);
    SUBCASE("A") {
        MESSAGE("A: x=", x);
        CHECK(x > 0);
    }
    SUBCASE("B") {
        MESSAGE("B: x=", x);
        CHECK(x > 0);
    }
}

TEST_CASE("GENERATE + nested SUBCASE") {
    auto x = GENERATE(1, 2);
    MESSAGE("test_case: x=", x);
    SUBCASE("outer") {
        MESSAGE("outer: x=", x);
        SUBCASE("inner A") {
            MESSAGE("inner A: x=", x);
            CHECK(x > 0);
        }
        SUBCASE("inner B") {
            MESSAGE("inner B: x=", x);
            CHECK(x > 0);
        }
    }
}

TEST_CASE("two GENERATEs + SUBCASE") {
    auto x = GENERATE(1, 2);
    auto y = GENERATE(10, 20);
    SUBCASE("s") {
        MESSAGE("s: x=", x, " y=", y);
        CHECK(x + y > 0);
    }
}

// ---------------------------------------------------------------------------
// Custom struct type
// ---------------------------------------------------------------------------

TEST_CASE("GENERATE - custom struct type") {
    struct Point {
        int x, y;
    };
    const Point p1{1, 2};
    const Point p2{3, 4};
    auto p = GENERATE(p1, p2, Point{5, 6});
    MESSAGE("p.x=", p.x, " p.y=", p.y);
    CHECK(p.x > 0);
    CHECK(p.y > 0);
}

// ---------------------------------------------------------------------------
// Single value (degenerate case)
// ---------------------------------------------------------------------------

TEST_CASE("GENERATE - single value (degenerate case)") {
    auto x = GENERATE(42);
    CHECK(x == 42);
    MESSAGE("x=", x);
}

// ---------------------------------------------------------------------------
// GENERATE inside SUBCASE
// ---------------------------------------------------------------------------

TEST_CASE("GENERATE inside SUBCASE - single sibling") {
    // SUBCASE("A") runs twice (for x=1 and x=2); SUBCASE("B") runs once.
    SUBCASE("A") {
        auto x = GENERATE(1, 2);
        MESSAGE("A x=", x);
        CHECK(x > 0);
    }
    SUBCASE("B") {
        MESSAGE("B");
        CHECK(true);
    }
}

TEST_CASE("GENERATE inside sibling SUBCASEs - different counts") {
    // A runs 3 times, B runs 2 times: 5 total.
    SUBCASE("A") {
        auto x = GENERATE(1, 2, 3);
        MESSAGE("A x=", x);
        CHECK(x > 0);
    }
    SUBCASE("B") {
        auto y = GENERATE(10, 20);
        MESSAGE("B y=", y);
        CHECK(y > 0);
    }
}

TEST_CASE("GENERATE inside SUBCASE - with nested child subcases") {
    // Each generator value causes both child subcases to re-run.
    SUBCASE("outer") {
        auto x = GENERATE(1, 2);
        MESSAGE("outer x=", x);
        SUBCASE("inner A") {
            MESSAGE("  inner A x=", x);
            CHECK(x > 0);
        }
        SUBCASE("inner B") {
            MESSAGE("  inner B x=", x);
            CHECK(x > 0);
        }
    }
}

TEST_CASE("top-level GENERATE composes with GENERATE inside SUBCASE") {
    auto n = GENERATE(10, 20);
    SUBCASE("s") {
        auto k = GENERATE(1, 2);
        MESSAGE("s: n=", n, " k=", k);
        CHECK(n + k > 0);
    }
}

TEST_CASE("top-level GENERATE + two sibling SUBCASEs each with GENERATE") {
    // For each n, subcase A runs twice (k=10,20), subcase B runs twice (k=100,200).
    // 2 * (2 + 2) = 8 runs total, 8 assertions.
    auto n = GENERATE(1, 2);
    SUBCASE("A") {
        auto k = GENERATE(10, 20);
        MESSAGE("A n=", n, " k=", k);
        CHECK(n + k > 0);
    }
    SUBCASE("B") {
        auto k = GENERATE(100, 200);
        MESSAGE("B n=", n, " k=", k);
        CHECK(n + k > 0);
    }
}

TEST_CASE("GENERATE at multiple nesting depths") {
    // top-level GENERATE is the outermost loop.
    // Inside outer1: inner1a/deep1 has 2 values, inner1b has 2 values.
    // Inside outer2: 2 values.
    // 2 * (2 + 2 + 2) = 12 runs total, 12 assertions.
    auto i = GENERATE(1, 2);
    SUBCASE("A") {
        auto j = GENERATE(3, 4);
        SUBCASE("1") {
            auto k = GENERATE(5, 6);
            SUBCASE("a") {
                auto l = GENERATE(7, 8);
                MESSAGE("A1a: i=", i, " j=", j, " k=", k, " l=", l);
            }
        }
        SUBCASE("2") {
            auto m = GENERATE(9, 10);
            MESSAGE("A2 i=", i, " m=", m);
        }
    }
    SUBCASE("B") {
        auto n = GENERATE(1000, 2000);
        MESSAGE("B: i=", i, " n=", n);
    }
    MESSAGE("test_case: i=", i);
}

TEST_CASE("nested GENERATE does not affect sibling SUBCASE") {
    static int b_enter_count = 0;

    SUBCASE("A") {
        auto j = GENERATE(10, 20);
        SUBCASE("1") {
            auto k = GENERATE(100, 200);
            MESSAGE("A1 j=", j, " k=", k);
        }
    }
    SUBCASE("B") {
        MESSAGE("B enter=", b_enter_count);
        CHECK(b_enter_count == 0);
        b_enter_count++;
        (void)b_enter_count;
    }
}

// ---------------------------------------------------------------------------
// GENERATE with REQUIRE
// ---------------------------------------------------------------------------

TEST_CASE("all items are handled" * doctest::expected_failures(2)) {
    const int i = GENERATE(1, 2, 3, 4);
    MESSAGE("before: i=", i);
    REQUIRE(i % 2);
    MESSAGE("after: i=", i);
}

TEST_CASE("failures continue cartesian iteration" * doctest::expected_failures(4)) {
    const int i = GENERATE(1, 2);
    const int j = GENERATE(1 - i, 2 - i, 3 - i);
    MESSAGE("before: i=", i, " j=", j);
    REQUIRE(i + j == 2);
    MESSAGE("after: i=", i, " j=", j);
}

// ---------------------------------------------------------------------------
// GENERATE with control flow
// ---------------------------------------------------------------------------

DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(5045) // Spectre mitigation warning can surface at GENERATE call sites

TEST_CASE("GENERATE with fixed for loop") {
    SUBCASE("loop with fixed name") {
        // N.B. this produces 2 reruns, not 6: the loop reenters the same subcase
        // three times per rerun, and the single generator depth chosen for that
        // rerun is reused across all three iterations. State is preserved across
        // the repeated subcase entries, so the vector grows up to 3 elements.
        std::vector<int> js;
        for (int i = 0; i < 3; ++i) {
            SUBCASE("fixed name") {
                js.push_back(3 + 2 * i + GENERATE(0, 1));
                std::ostringstream oss;
                oss << "i=" << i << " js=[ ";
                for (auto j: js) {
                    oss << j << " ";
                }
                oss << "]";
                MESSAGE(oss.str());
            }
        }
    }
    SUBCASE("loop with dynamic name") {
        std::vector<int> js;
        // N.B. this produces 3 * 2 = 6 traversal leaves: each loop iteration has a
        // distinct subcase identity, and each of those subcases has its own
        // GENERATE(0, 1). State resets for each rerun, so the vector contains
        // only the value for the current dynamic subcase.
        // This is analogous to:
        // int i = GENERATE(0, 1, 2);
        // int j = GENERATE(0, 1);
        for (int i = 0; i < 3; ++i) {
            SUBCASE("dynamic " + std::to_string(i)) {
                js.push_back(3 + 2 * i + GENERATE(0, 1));
                std::ostringstream oss;
                oss << "i=" << i << " js=[ ";
                for (auto j: js) {
                    oss << j << " ";
                }
                oss << "]";
                MESSAGE(oss.str());
            }
        }
    }
    SUBCASE("loop without subcase") {
        // N.B. this produces 2^3 = 8 traversal leaves, because the loop contains
        // three separate generator encounters. It behaves like:
        // int j1 = GENERATE(3, 4);
        // int j2 = GENERATE(5, 6);
        // int j3 = GENERATE(7, 8);
        // with one generator depth per loop iteration.
        std::vector<int> js;
        js.reserve(3);
        for (int i = 0; i < 3; ++i) {
            js.push_back(GENERATE(3 + 2 * i + 0, 3 + 2 * i + 1));
        }
        std::ostringstream oss;
        oss << "js=[ ";
        for (auto j: js) {
            oss << j << " ";
        }
        oss << "]";
        MESSAGE(oss.str());
    }
}

TEST_CASE("GENERATE with dynamic loop") {
    const int i = GENERATE(1, 2);
    MESSAGE("# i=", i);
    for (int j = 0; j < i; ++j) {
        SUBCASE(std::to_string(j)) {
            const int k = GENERATE(10 - i - j, 20 - i - j);
            MESSAGE("j=", j, " k=", k);
            CHECK(((i + j + k) % 10) == 0);
        }
    }
}

DOCTEST_MSVC_SUPPRESS_WARNING_POP // context for spectre warning suppression

TEST_CASE("GENERATE with dynamic if") {
    const int i = GENERATE(1, 2, 3);
    if (i != 2) {
        const int j = GENERATE(10, 20);
        MESSAGE("i=", i, " j=", j);
    }
}

// ---------------------------------------------------------------------------
// Regression: stale traversal depth metadata
// ---------------------------------------------------------------------------

TEST_CASE("stale generator depth does not create extra reruns") {
    static bool discover_deeper_generator = true;
    static int run_count = 0;

    SUBCASE("outer") {
        MESSAGE("run=", run_count, " discover=", discover_deeper_generator);

        ++run_count;
        (void)run_count;

        if (discover_deeper_generator) {
            discover_deeper_generator = false;
            const int j = GENERATE(10, 20, 30);
            MESSAGE("j=", j);
        }

        CHECK(run_count <= 2);
    }
}

TEST_CASE("GENERATE with dynamic switch") {
    const int i = GENERATE(1, 2, 3);
    switch (i) {
        case 1: {
            const int j = GENERATE(10, 20);
            MESSAGE("i=", i, " j=", j);
            break;
        }
        case 2: {
            const int k = GENERATE(100, 200);
            MESSAGE("i=", i, " k=", k);
            break;
        }
        case 3: {
            const int m = GENERATE(1000, 2000);
            MESSAGE("i=", i, " m=", m);
            break;
        }
        default:;
    }
}

// ---------------------------------------------------------------------------
// GENERATE with filters
// ---------------------------------------------------------------------------

TEST_CASE("GENERATE filter test-case target") {
    const int x = GENERATE(1, 2);
    const int y = GENERATE(10, 20);
    MESSAGE("tc target x=", x, " y=", y);
    CHECK(x + y > 0);
}

TEST_CASE("GENERATE filter test-case excluded") {
    const int x = GENERATE(3, 4);
    MESSAGE("tc excluded x=", x);
    CHECK(x > 0);
}

TEST_CASE("GENERATE filter subcase target") {
    const int x = GENERATE(1, 2);
    MESSAGE("sc root x=", x);

    SUBCASE("keep") {
        MESSAGE("keep x=", x);
        CHECK(x > 0);
    }

    SUBCASE("drop") {
        MESSAGE("drop x=", x);
        CHECK(x > 0);
    }
}
