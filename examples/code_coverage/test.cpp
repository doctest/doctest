#include "doctest.h"

using doctest::Approx;

static int throws(bool in) {
    if(in)
        throw "whops!";
    return 42;
}

TEST_SUITE("ts1");

TEST_CASE("ops") {
    throws(false);



}

TEST_SUITE_END();


TEST_SUITE("ts2");

