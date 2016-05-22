#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <iostream>
#include <vector>
using namespace std;

static int throws(bool in) {
    if(in)
        throw 5;
    return 42;
}

TEST_CASE("lots of nested subcases") {
    cout << endl << "root" << endl;
    SUBCASE("") {
        cout << "1" << endl;
        SUBCASE("") { cout << "1.1" << endl; }
    }
    SUBCASE("") {
        cout << "2" << endl;
        SUBCASE("") { cout << "2.1" << endl; }
        SUBCASE("") {
            // whops! all the subcases below shouldn't be discovered and executed!
            throws(true);

            cout << "2.2" << endl;
            SUBCASE("") {
                cout << "2.2.1" << endl;
                SUBCASE("") { cout << "2.2.1.1" << endl; }
                SUBCASE("") { cout << "2.2.1.2" << endl; }
            }
        }
        SUBCASE("") { cout << "2.3" << endl; }
        SUBCASE("") { cout << "2.4" << endl; }
    }
}

SCENARIO("vectors can be sized and resized") {
    GIVEN("A vector with some items") {
        std::vector<int> v(5);

        REQUIRE(v.size() == 5u);
        REQUIRE(v.capacity() >= 5u);

        WHEN("the size is increased") {
            v.resize(10u);

            THEN("the size and capacity change") {
                CHECK(v.size() == 20u);
                CHECK(v.capacity() >= 10u);
            }
        }
        WHEN("the size is reduced") {
            v.resize(0);

            THEN("the size changes but not capacity") {
                CHECK(v.size() == 0u);
                CHECK(v.capacity() >= 5u);
            }
        }
        WHEN("more capacity is reserved") {
            v.reserve(10);

            THEN("the capacity changes but not the size") {
                CHECK(v.size() == 5u);
                CHECK(v.capacity() >= 10u);
            }
        }
        WHEN("less capacity is reserved") {
            v.reserve(0);

            THEN("neither size nor capacity are changed") {
                CHECK(v.size() == 10u);
                CHECK(v.capacity() >= 5u);
            }
        }
    }
}

// to silence GCC warnings when inheriting from the class TheFixture which has no virtual destructor
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Weffc++"
#endif // __GNUC__

struct TheFixture
{
    int data;
    TheFixture()
            : data(42) {
        // setup here
    }

    ~TheFixture() {
        // teardown here
    }
};

TEST_CASE_FIXTURE(TheFixture, "test with a fixture - 1") {
    data /= 2;
    CHECK(data == 21);
}

TEST_CASE_FIXTURE(TheFixture, "test with a fixture - 2") {
    data *= 2;
    CHECK(data == 85);
}
