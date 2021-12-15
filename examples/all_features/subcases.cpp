#include <doctest/doctest.h>

#include "header.h"

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <iostream>
#include <string>
#include <vector>
using namespace std;
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

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
            FAIL("");

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

static void call_func() {
    SUBCASE("from function...") {
        MESSAGE("print me twice");
        SUBCASE("sc1") {
            MESSAGE("hello! from sc1");
        }
        SUBCASE("sc2") {
            MESSAGE("hello! from sc2");
        }
    }
}

TEST_CASE("subcases can be used in a separate function as well") {
    call_func();
    MESSAGE("lala");
}

SCENARIO("vectors can be sized and resized") {
    GIVEN("A vector with some items") {
        std::vector<int> v(5);

        REQUIRE(v.size() == 5);
        REQUIRE(v.capacity() >= 5);

        WHEN("the size is increased") {
            v.resize(10);

            THEN("the size and capacity change") {
                CHECK(v.size() == 20);
                CHECK(v.capacity() >= 10);
            }
        }
        WHEN("the size is reduced") {
            v.resize(0);

            THEN("the size changes but not capacity") {
                CHECK(v.size() == 0);
                CHECK(v.capacity() >= 5);
            }
        }
        WHEN("more capacity is reserved") {
            v.reserve(10);

            THEN("the capacity changes but not the size") {
                CHECK(v.size() == 5);
                CHECK(v.capacity() >= 10);
            }
        }
        WHEN("less capacity is reserved") {
            v.reserve(0);

            THEN("neither size nor capacity are changed") {
                CHECK(v.size() == 10);
                CHECK(v.capacity() >= 5);
            }
        }
    }
}

TEST_CASE("test case should fail even though the last subcase passes") {
    SUBCASE("one") {
        CHECK(false);
    }
    SUBCASE("two") {
        CHECK(true);
    }
}

TEST_CASE("fails from an exception but gets re-entered to traverse all subcases") {
    SUBCASE("level zero") {
        SUBCASE("one") {
            CHECK(false);
        }
        SUBCASE("two") {
            CHECK(false);
        }

        throw_if(true, "failure... but the show must go on!");
    }
}

static void checks(int data)
{
    DOCTEST_SUBCASE("check data 1") { REQUIRE(data % 2 == 0); }
    DOCTEST_SUBCASE("check data 2") { REQUIRE(data % 4 == 0); }
}

TEST_CASE("Nested - related to https://github.com/doctest/doctest/issues/282")
{
    DOCTEST_SUBCASE("generate data variant 1")
    {
        int data(44);
        
        // checks
        checks(data);
    }
    DOCTEST_SUBCASE("generate data variant 1")
    {
        int data(80);
        
        // checks (identical in both variants)
        checks(data);
    }
}

DOCTEST_MSVC_SUPPRESS_WARNING(5045) // Spectre mitigation stuff
DOCTEST_GCC_SUPPRESS_WARNING("-Wuseless-cast") // for the std::string() cast
#undef SUBCASE
#define SUBCASE(...) DOCTEST_SUBCASE(std::string(__VA_ARGS__).c_str())

TEST_CASE("subcases with changing names") {
    for(int i = 0; i < 2; ++i) {
        SUBCASE("outer " + std::to_string(i)) {
            for(int k = 0; k < 2; ++k) {
                SUBCASE("inner " + std::to_string(k)) {
                    MESSAGE("msg!");
                }
            }
        }
    }
    SUBCASE("separate") {
        MESSAGE("separate msg!");
    }
}
