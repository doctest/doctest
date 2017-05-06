//#define DOCTEST_CONFIG_DISABLE
#include "parts/doctest_fwd.h"

#include <iostream>
using namespace std;

TEST_CASE("asd 4"
          * doctest::test_suite("override")
          * doctest::description("override")
          * doctest::skip(false)
          //* doctest::should_fail()
          * doctest::expected_failures(2)
) {
    FAIL_CHECK("");
    //FAIL_CHECK("");
}

//TEST_CASE("") {
//    CHECK(true);
//}

//TEST_SUITE_BEGIN("stack"
//    * doctest::skip_if(true)
//    * doctest::description("stack")
//);
//
//TEST_SUITE("scope"
//    * doctest::skip_if(false)
//    * doctest::description("scope")
//) {
//    TEST_CASE("asd 1"
//        * doctest::test_suite("override")
//        * doctest::description("override")
//        * doctest::skip_if(false)
//    ) {
//        FAIL("");
//    }
//    TEST_CASE("asd 2"
//              //* doctest::test_suite("override")
//              //* doctest::description("override")
//              //* doctest::skip_if(false)
//              ) {
//        FAIL("");
//    }
//
//} // scope ts end
//
//TEST_CASE("asd 3"
//          //* doctest::test_suite("override")
//          //* doctest::description("override")
//          //* doctest::skip_if(false)
//          ) {
//    FAIL("");
//}
//
//TEST_SUITE_END;
//
//TEST_CASE("asd 4"
//          //* doctest::test_suite("override")
//          //* doctest::description("override")
//          //* doctest::skip_if(false)
//          ) {
//    FAIL("");
//}
