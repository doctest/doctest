#include "parts/doctest_fwd.h"

#include <iostream>
using namespace std;

//static int throws(bool in) { if(in) throw 42; return 0; }
TEST_SUITE("omg")
//namespace
{
    TEST_CASE("dev stuff") {
        SUBCASE("sc1") {
            CHECK(4 == 5);
        }
        SUBCASE("sc2") {
            CHECK(4 == 5);
        }
        //CHECK_THROWS(CHECK_THROWS(4 == 5));
        //CHECK_THROWS({throws(true);});
    }
    TEST_CASE("asd 2") {
        CHECK(4 == 5);
    }
}