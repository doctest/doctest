#include <doctest/doctest.h>

#include "header.h"

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <vector>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

TEST_CASE("logging the counter of a loop") {
    std::vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(4);
    vec.push_back(8);
    vec.push_back(16);
    
    INFO("current iteration of loop:");
    for(unsigned i = 0; i < vec.size(); ++i) {
        CAPTURE(i);
        CHECK(vec[i] != (1 << i));
    }
}

static int someTests() {
    int some_var = 42;
    INFO("lots of captures - some on heap: " << some_var << " " << some_var << " " << some_var << ";");
    FAIL_CHECK("forcing the many captures (including those on the heap) to be stringified");
    return some_var;
}

TEST_CASE("a test case that will end from an exception") {
    int some_var = someTests();
    INFO("someTests() returned: " << some_var); // note that we have to use a local variable - cannot pass a temporary
    INFO("this should be printed if an exception is thrown even if no assert has failed: " << some_var);
    {
        INFO("in a nested scope this should be printed as well: " << some_var);
        {
            INFO("this should not be printed");
            CAPTURE(some_var);
        }

        CHECK_MESSAGE(some_var == 666, "why is this not 666 ?!");

        throw_if(true, 0);
    }
}

TEST_CASE("a test case that will end from an exception and should print the unprinted context") {
    INFO("should be printed even if an exception is thrown and no assert fails before that");
    throw_if(true, 0);
}

static void thirdPartyAssert(bool result, bool is_fatal, const char* file, int line) {
    if(result == false) {
        if(is_fatal)
            ADD_FAIL_AT(file, line, "MY_ASSERT_FATAL(" << result << ")");
        else
            ADD_FAIL_CHECK_AT(file, line, "MY_ASSERT(" << result << ")");
    }
}

#define MY_ASSERT(x) thirdPartyAssert(x, false, __FILE__, __LINE__)
#define MY_ASSERT_FATAL(x) thirdPartyAssert(x, true, __FILE__, __LINE__)

TEST_CASE("third party asserts can report failures to doctest") {
    MY_ASSERT(1 == 2);
    MY_ASSERT_FATAL(1 == 2);
}

TEST_CASE("explicit failures 1") {
    FAIL_CHECK("this should not end the test case, but mark it as failing");
    MESSAGE("reached!");
}

TEST_CASE("explicit failures 2") {
    FAIL("fail the test case and also end it");
    MESSAGE("never reached...");
}
