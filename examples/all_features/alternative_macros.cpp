#include "doctest_proxy.h"

my_testcase("custom macros") {
    my_check(1 == 1);
    
    my_fast_check_eq(1, 1);
    
    my_subcase("bar") {
        my_subcase("foo") {}
        my_subcase("baz") {}
        
        my_require(5 > 3);
    }
    // CHECK(1 == 1); <== ERROR - the default short macros are disabled from the proxy header
}
