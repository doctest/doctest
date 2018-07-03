#define DOCTEST_CONFIG_SUPER_FAST_ASSERTS // defined so the fast asserts are crazy fast - both for compilation and execution
#include "doctest.h"

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <iostream>
#include <cstring>
#include <algorithm>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

// some function which uses asserts not just for unit testing but also for ensuring contracts in production code
static void some_func() {
    FAST_CHECK_EQ(true, false);
    FAST_CHECK_UNARY(false);
    FAST_CHECK_UNARY_FALSE(true);

    CHECK_EQ(true, false);
    CHECK_UNARY(false);
    CHECK_UNARY_FALSE(true);

    CHECK(false);
    CHECK_THROWS(((void)false));
}

void some_program_code(int argc, char** argv); // obligatory fwd decl to silence a warning...
void some_program_code(int argc, char** argv) {
    // return if the current test from the doctest CMake tests is not for this file
    if(std::find_if(argv, argv + argc, [](const char* str) {
           return strcmp(str, "-sf=*asserts_used_outside_of_tests.cpp") == 0;
       }) == argv + argc)
        return;

    // construct a context
    doctest::Context context;
    context.applyCommandLine(argc, argv);

    // sets the context as the default one - so asserts used outside of a testing context do not crash
    context.setAsDefaultForAssertsOutOfTestCases();

    // set a handler with a signature: void(const doctest::AssertData&)
    // without setting a handler we would get std::abort() called when an assert fails
    context.setAssertHandler([](const doctest::AssertData& ad) {
        using namespace doctest;

        // here we can choose what to do:
        // - log the failed assert
        // - throw an exception
        // - call std::abort() or std::terminate()

        auto getFailString = [](assertType::Enum at) {
            if(at & assertType::is_warn)
                return "WARNING: ";
            if(at & assertType::is_check)
                return "ERROR: ";
            if(at & assertType::is_require)
                return "FATAL ERROR: ";
            return "";
        };

        std::cout << Color::LightGrey << ad.m_file << "(" << ad.m_line << "): ";
        std::cout << Color::Red << getFailString(ad.m_at);

        // handling only normal (comparison and unary) asserts - nothing exceptions-related
        if(ad.m_at & assertType::is_normal) {
            std::cout << Color::Cyan << assertString(ad.m_at) << "( " << ad.m_expr << " ) ";
            std::cout << Color::None << (ad.m_threw ? "THREW exception: " : "is NOT correct!\n");
            if(ad.m_threw)
                std::cout << ad.m_exception;
            else
                std::cout << "  values: " << assertString(ad.m_at) << "( " << ad.m_decomp << " )";
        } else {
            std::cout << Color::None << "an assert dealing with exceptions has failed!";
        }

        std::cout << "\n";
    });

    // call the function with asserts out of a testing context - the above handler will be called on failure
    some_func();
}
