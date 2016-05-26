#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest_proxy.h"

#include <cstdio>

my_testcase("custom macros") {
    printf("Hello world!\n");
    
    my_check(1 == 1);
    
    my_subcase("bar") {
        my_subcase("foo") {}
        my_subcase("baz") {}
        
        my_require(5 > 3);
    }
    
    // CHECK(1 == 1); <== ERROR - the default short macros are disabled from the proxy header
}
