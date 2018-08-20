#include "doctest.h"

#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <thread>
#include <mutex>
#include <exception>
#include <stdexcept>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

std::exception_ptr g_exception_ptr = nullptr;
std::mutex         g_mutex;

TEST_CASE("threads...") {
    auto call_from_thread = [](int value) {
        INFO("print me!");
        // one of these has to fail
        CHECK(value == 1);
        CHECK(value == 2);
    };

    int data_1 = 1;
    int data_2 = 2;
    CAPTURE(data_1); // will not be used for assertions in other threads

    // subcases have to be used only in the main thread (where the test runner is)
    SUBCASE("test runner thread") {
        call_from_thread(data_1);
    }

    // normal threads which are assumed not to throw
    SUBCASE("spawned threads") {
        std::thread t1(call_from_thread, data_1);
        std::thread t2(call_from_thread, data_2);

        t1.join();
        t2.join();
    }

    // exceptions from threads (that includes failing REQUIRE asserts) have to be handled explicitly
    SUBCASE("spawned threads with exception propagation") {
        auto might_throw = []() {
            try {
                REQUIRE(1 == 1);
                REQUIRE(1 == 2); // will fail and throw an exception
                MESSAGE("not reached!");
            } catch(...) {
                // make sure there are no races when dealing with the global exception ptr
                std::lock_guard<std::mutex> lock(g_mutex);

                // set the global exception pointer in case of an exception - might overwrite
                // another exception but here we care about propagating any exception - not all
                g_exception_ptr = std::current_exception();
            }
        };
        std::thread t1(might_throw);
        std::thread t2(might_throw);

        t1.join();
        t2.join();

        // if any thread has thrown an exception - rethrow it
        if(g_exception_ptr)
            std::rethrow_exception(g_exception_ptr);
    }
}

#endif // DOCTEST_CONFIG_NO_EXCEPTIONS
