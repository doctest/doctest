#include "parts/doctest_fwd.h"

#include <iostream>
using namespace std;

//static int throws(bool in) { if(in) throw 42; return 0; }

TEST_CASE("dev stuff") {
    CHECK(4 == 5);
    //CHECK_THROWS(CHECK_THROWS(4 == 5));
    //CHECK_THROWS({throws(true);});
}
