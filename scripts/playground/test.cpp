//#define DOCTEST_CONFIG_DISABLE
//#define DOCTEST_CONFIG_SUPER_FAST_ASSERTS
#include "parts/doctest_fwd.h"

#include <stdlib.h>
#include <iostream>
using namespace std;

void* operator new(std::size_t count) { return malloc(count); }
void* operator new[](std::size_t count) { return malloc(count); }

TEST_CASE("") {
    for(int i = 0; i < 10; ++i) {
        //FAST_CHECK_EQ(i, i);
        INFO(i);
        //MESSAGE(i);
        CHECK(i == i);
    }
}
