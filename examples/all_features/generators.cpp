// examples/all_features/generators.cpp
#include <doctest/doctest.h>

#include "header.h"

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <iostream>
#include <vector>
#include <string>
using namespace std;
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

// ---------------------------------------------------------------------------
// Basic: single generator
// ---------------------------------------------------------------------------

TEST_CASE("GENERATE - single, two values") {
    auto x = GENERATE(10, 20);
    cout << "x=" << x << endl;
    CHECK((x == 10 || x == 20));
}

TEST_CASE("GENERATE - single, three values") {
    auto x = GENERATE(1, 2, 3);
    cout << "x=" << x << endl;
    CHECK(x >= 1);
    CHECK(x <= 3);
}

// ---------------------------------------------------------------------------
// Cartesian product: multiple generators, last cycles fastest
// ---------------------------------------------------------------------------

TEST_CASE("GENERATE - two generators, cartesian product") {
    auto x = GENERATE(1, 2);   // outer: cycles slowest
    auto y = GENERATE(10, 20); // inner: cycles fastest
    cout << "x=" << x << " y=" << y << endl;
    CHECK(x >= 1);
    CHECK(y >= 10);
}

TEST_CASE("GENERATE - three generators") {
    auto a = GENERATE(1, 2);
    auto b = GENERATE(3, 4);
    auto c = GENERATE(5, 6);
    cout << "a=" << a << " b=" << b << " c=" << c << endl;
    CHECK(a + b + c > 0);
}

TEST_CASE("GENERATE statements can depend on each other") {
    int i = GENERATE(1, 2, 3);
    int j = GENERATE(i + 1, i + 2, i + 3);
    cout << "i=" << i << " j=" << j << endl;
    CHECK(i <= j);
}

TEST_CASE("multiple GENERATEs on a single line") {
    // clang-format off
    // NOLINTNEXTLINE(readability-isolate-declaration)
    int i = GENERATE(1, 2), j = GENERATE(10, 20);
    // clang-format on
    cout << "i=" << i << " j=" << j << endl;
}

// ---------------------------------------------------------------------------
// Non-int types
// ---------------------------------------------------------------------------

TEST_CASE("GENERATE - double values") {
    auto d = GENERATE(1.0, 2.5, 3.14);
    cout << "d=" << d << endl;
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
    cout << "test_case: x=" << x << endl;
    SUBCASE("A") {
        cout << "A: x=" << x << endl;
        CHECK(x > 0);
    }
    SUBCASE("B") {
        cout << "B: x=" << x << endl;
        CHECK(x > 0);
    }
}

TEST_CASE("GENERATE + nested SUBCASE") {
    auto x = GENERATE(1, 2);
    cout << "test_case: x=" << x << endl;
    SUBCASE("outer") {
        cout << "outer: x=" << x << endl;
        SUBCASE("inner A") {
            cout << "inner A: x=" << x << endl;
            CHECK(x > 0);
        }
        SUBCASE("inner B") {
            cout << "inner B: x=" << x << endl;
            CHECK(x > 0);
        }
    }
}

TEST_CASE("two GENERATEs + SUBCASE") {
    auto x = GENERATE(1, 2);
    auto y = GENERATE(10, 20);
    SUBCASE("s") {
        cout << "s: x=" << x << " y=" << y << endl;
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
    cout << "p.x=" << p.x << " p.y=" << p.y << endl;
    CHECK(p.x > 0);
    CHECK(p.y > 0);
}

// ---------------------------------------------------------------------------
// Single value (degenerate case)
// ---------------------------------------------------------------------------

TEST_CASE("GENERATE - single value (degenerate case)") {
    auto x = GENERATE(42);
    CHECK(x == 42);
    cout << "x=" << x << endl;
}

// ---------------------------------------------------------------------------
// GENERATE inside SUBCASE
// ---------------------------------------------------------------------------

TEST_CASE("GENERATE inside SUBCASE - single sibling") {
    // SUBCASE("A") runs twice (for x=1 and x=2); SUBCASE("B") runs once.
    SUBCASE("A") {
        auto x = GENERATE(1, 2);
        cout << "A x=" << x << endl;
        CHECK(x > 0);
    }
    SUBCASE("B") {
        cout << "B" << endl;
        CHECK(true);
    }
}

TEST_CASE("GENERATE inside sibling SUBCASEs - different counts") {
    // A runs 3 times, B runs 2 times: 5 total.
    SUBCASE("A") {
        auto x = GENERATE(1, 2, 3);
        cout << "A x=" << x << endl;
        CHECK(x > 0);
    }
    SUBCASE("B") {
        auto y = GENERATE(10, 20);
        cout << "B y=" << y << endl;
        CHECK(y > 0);
    }
}

TEST_CASE("GENERATE inside SUBCASE - with nested child subcases") {
    // Each generator value causes both child subcases to re-run.
    SUBCASE("outer") {
        auto x = GENERATE(1, 2);
        cout << "outer x=" << x << endl;
        SUBCASE("inner A") {
            cout << "  inner A x=" << x << endl;
            CHECK(x > 0);
        }
        SUBCASE("inner B") {
            cout << "  inner B x=" << x << endl;
            CHECK(x > 0);
        }
    }
}

TEST_CASE("top-level GENERATE composes with GENERATE inside SUBCASE") {
    auto n = GENERATE(10, 20);
    SUBCASE("s") {
        auto k = GENERATE(1, 2);
        cout << "s: n=" << n << " k=" << k << endl;
        CHECK(n + k > 0);
    }
}

TEST_CASE("top-level GENERATE + two sibling SUBCASEs each with GENERATE") {
    // For each n, subcase A runs twice (k=10,20), subcase B runs twice (k=100,200).
    // 2 * (2 + 2) = 8 runs total, 8 assertions.
    auto n = GENERATE(1, 2);
    SUBCASE("A") {
        auto k = GENERATE(10, 20);
        cout << "A n=" << n << " k=" << k << endl;
        CHECK(n + k > 0);
    }
    SUBCASE("B") {
        auto k = GENERATE(100, 200);
        cout << "B n=" << n << " k=" << k << endl;
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
                cout << "A1a: i=" << i << " j=" << j << " k=" << k << " l=" << l << endl;
            }
        }
        SUBCASE("2") {
            auto m = GENERATE(9, 10);
            cout << "A2 i=" << i << " m=" << m << endl;
        }
    }
    SUBCASE("B") {
        auto n = GENERATE(1000, 2000);
        cout << "B: i=" << i << " n=" << n << endl;
    }
    cout << "test_case: i=" << i << endl;
}

TEST_CASE("nested GENERATE does not affect sibling SUBCASE") {
    static int b_enter_count = 0;

    SUBCASE("A") {
        auto j = GENERATE(10, 20);
        SUBCASE("1") {
            auto k = GENERATE(100, 200);
            cout << "A1 j=" << j << " k=" << k << endl;
        }
    }
    SUBCASE("B") {
        cout << "B enter=" << b_enter_count << endl;
        CHECK(b_enter_count++ == 0);
    }
}

// ---------------------------------------------------------------------------
// GENERATE with REQUIRE
// ---------------------------------------------------------------------------

TEST_CASE("all items are handled" * doctest::expected_failures(2)) {
    int i = GENERATE(1, 2, 3, 4);
    cout << "before: i=" << i << endl;
    REQUIRE(i % 2);
    cout << "after: i=" << i << endl;
}

TEST_CASE("failures continue cartesian iteration" * doctest::expected_failures(4)) {
    int i = GENERATE(1, 2);
    int j = GENERATE(1 - i, 2 - i, 3 - i);
    cout << "before: i=" << i << " j=" << j << endl;
    REQUIRE(i + j == 2);
    cout << "after: i=" << i << " j=" << j << endl;
}

// ---------------------------------------------------------------------------
// GENERATE with control flow
// ---------------------------------------------------------------------------

DOCTEST_MSVC_SUPPRESS_WARNING_PUSH  // context for spectre warning suppression
DOCTEST_MSVC_SUPPRESS_WARNING(5045) // Spectre mitigation warning can surface at GENERATE call sites

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
                cout << "i=" << i;
                for (auto j: js) {
                    cout << " j=" << j;
                }
                cout << endl;
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
            SUBCASE(("dynamic " + std::to_string(i)).c_str()) {
                js.push_back(3 + 2 * i + GENERATE(0, 1));
                cout << "i=" << i;
                for (auto j: js) {
                    cout << " j=" << j;
                }
                cout << endl;
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
        cout << "js=[ ";
        for (auto j: js) {
            cout << j << " ";
        }
        cout << "]" << endl;
    }
}

TEST_CASE("GENERATE with dynamic loop") {
    int i = GENERATE(1, 2);
    cout << "# i=" << i << endl;
    for (int j = 0; j < i; ++j) {
        SUBCASE(std::to_string(j).c_str()) {
            int k = GENERATE(10 - i - j, 20 - i - j);
            cout << "j=" << j << " k=" << k << endl;
            CHECK(((i + j + k) % 10) == 0);
        }
    }
}

DOCTEST_MSVC_SUPPRESS_WARNING_POP // context for spectre warning suppression

TEST_CASE("GENERATE with dynamic if") {
    int i = GENERATE(1, 2, 3);
    if (i != 2) {
        int j = GENERATE(10, 20);
        cout << "i=" << i << " j=" << j << endl;
    }
}

TEST_CASE("GENERATE with dynamic switch") {
    int i = GENERATE(1, 2, 3);
    switch (i) {
        case 1: {
            int j = GENERATE(10, 20);
            cout << "i=" << i << " j=" << j << endl;
            break;
        }
        case 2: {
            int k = GENERATE(100, 200);
            cout << "i=" << i << " k=" << k << endl;
            break;
        }
        case 3: {
            int m = GENERATE(1000, 2000);
            cout << "i=" << i << " m=" << m << endl;
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
    cout << "tc target x=" << x << " y=" << y << endl;
    CHECK(x + y > 0);
}

TEST_CASE("GENERATE filter test-case excluded") {
    const int x = GENERATE(3, 4);
    cout << "tc excluded x=" << x << endl;
    CHECK(x > 0);
}

TEST_CASE("GENERATE filter subcase target") {
    const int x = GENERATE(1, 2);
    cout << "sc root x=" << x << endl;

    SUBCASE("keep") {
        cout << "keep x=" << x << endl;
        CHECK(x > 0);
    }

    SUBCASE("drop") {
        cout << "drop x=" << x << endl;
        CHECK(x > 0);
    }
}
