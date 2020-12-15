#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <iostream>
#include <iostream>
#include <thread>
#include <exception>
#include <functional>
#include <stdexcept>
using namespace std;
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

DOCTEST_GCC_SUPPRESS_WARNING("-Wterminate")
DOCTEST_GCC_SUPPRESS_WARNING("-Wdiv-by-zero")

DOCTEST_MSVC_SUPPRESS_WARNING(4722)
DOCTEST_MSVC_SUPPRESS_WARNING(4297)
DOCTEST_MSVC_SUPPRESS_WARNING(4723)
DOCTEST_MSVC_SUPPRESS_WARNING(4702)

// echo %errorlevel%

#define FROM_A_SEPARATE_THREAD 1

TEST_CASE("uncomment to test these out") {
    std::function<void(void)> f;

    //for(;;); // infinite loop - to test SIGTERM for CTRL+C ==> doesn't work!

    //SUBCASE("null pointer access") {
    //    f = []() { std::cout << *static_cast<int*>(nullptr); };
    //}

    //SUBCASE("div by zero") {
    //    f = []() {
    //        int a = 1;
    //        --a;
    //        std::cout << 5 / a;
    //    };
    //}

    //SUBCASE("call terminate") {
    //    f = []() { std::terminate(); };
    //}

    //SUBCASE("throw from destructor`") {
    //    f = []() {
    //        struct dtor
    //        {
    //            ~dtor() {
    //                //REQUIRE(1 == 2);
    //                throw 42;
    //            }
    //        };

    //        dtor();
    //    };
    //}

    //SUBCASE("escaping exception") {
    //    f = []() { throw 42; };
    //}

#if FROM_A_SEPARATE_THREAD
    std::thread t(f);
    t.join();
#else
    f();
#endif
}
