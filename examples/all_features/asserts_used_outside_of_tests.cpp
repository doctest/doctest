#ifndef DOCTEST_CONFIG_DISABLE
#define DOCTEST_CONFIG_SUPER_FAST_ASSERTS // defined so the asserts are crazy fast - both for compilation and execution
#endif

#include <doctest/doctest.h>

#include "header.h"

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <iostream>
#include <cstring>
#include <algorithm>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END
DOCTEST_GCC_SUPPRESS_WARNING("-Wmissing-declarations")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wmissing-prototypes")

// some function which uses asserts not just for unit testing but also for ensuring contracts in production code
static void some_func() {
    CHECK_EQ(true, false);
    CHECK_UNARY(false);
    CHECK_UNARY_FALSE(true);

    CHECK(false);
    CHECK_THROWS(std::cout << "hello! \n");
}

// std::mutex g_mut;

static void handler(const doctest::AssertData& ad) {
    using namespace doctest;

    // uncomment if asserts will be used in a multi-threaded context
    // std::lock_guard<std::mutex> lock(g_mut);

    // here we can choose what to do:
    // - log the failed assert
    // - throw an exception
    // - call std::abort() or std::terminate()

    std::cout << Color::LightGrey << skipPathFromFilename(ad.m_file) << "(" << ad.m_line << "): ";
    std::cout << Color::Red << failureString(ad.m_at) << ": ";

    // handling only normal (comparison and unary) asserts - exceptions-related asserts have been skipped
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

    std::cout << std::endl;
}

void some_program_code(int argc, char** argv) {
    // IGNORE THIS: return if the current test from the doctest CMake tests is not for this file
    if(std::find_if(argv, argv + argc, [](const char* str) { return strcmp(str, "-sf=*asserts_used_outside_of_tests.cpp") == 0; }) == argv + argc) return;

    // construct a context
    doctest::Context context(argc, argv);

    // sets the context as the default one - so asserts used outside of a testing context do not crash
    context.setAsDefaultForAssertsOutOfTestCases();

    // set a handler with a signature: void(const doctest::AssertData&)
    // without setting a handler we would get std::abort() called when an assert fails
    context.setAssertHandler(handler);

    // call the function with asserts out of a testing context - the above handler will be called on failure
    some_func();
}
