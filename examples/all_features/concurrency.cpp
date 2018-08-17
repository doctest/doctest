#include "doctest.h"

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <thread>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

static void call_from_thread() {
    INFO("trololo");
    CHECK(1 == 1);
    CHECK(1 == 2);
}
 
TEST_CASE("threads...") {
    std::thread t1(call_from_thread);
    std::thread t2(call_from_thread);
 
    t1.join();
    t2.join();
}
