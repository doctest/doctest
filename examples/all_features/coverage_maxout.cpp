#include "doctest.h"

#include "header.h"

#include <ostream>
#include <sstream>

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 6)
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif // > gcc 4.6

#ifndef DOCTEST_CONFIG_DISABLE

// =================================================================================================
// !!! THESE ARE NOT PROPER EXAMPLES OF LIBRARY USAGE !!! THESE ARE MEANT FOR CODE COVERAGE ONLY !!!
// =================================================================================================

TEST_CASE("doctest internals") {
    using namespace doctest;

    // trigger code path for string with nullptr
    doctest::String       a(0);
    const doctest::String const_str("omgomgomg");
    a = const_str.c_str();
    CHECK(a.size() == const_str.size());
    CHECK(a.length() == const_str.length());
    CHECK(a.compare(const_str, true) == 0);
    CHECK(a.compare("omgomgomg", false) == 0);

    // toString
    a += toString("aaa")                           //
         + toString(true)                          //
         + toString(0.5f)                          //
         + toString(0.5)                           //
         + toString(static_cast<long double>(0.1)) //
         + toString('c')                           //
         + toString(static_cast<signed char>('c')) //
         + toString(static_cast<unsigned char>(1)) //
         + toString(static_cast<short>(1))         //
         + toString(static_cast<long>(1))          //
         + toString(static_cast<unsigned long>(1)) //
         + toString(static_cast<unsigned short>(1));

    // others
    //a += doctest::detail::fileForOutput("c:\\a");
    //a += doctest::detail::fileForOutput("c:/a");
    //a += doctest::detail::fileForOutput("a");

    std::ostringstream oss;
    // trigger code path for String to ostream through operator<<
    oss << a;
    // trigger code path for assert string of a non-existent assert type
    oss << doctest::detail::getAssertString(static_cast<doctest::detail::assertType::Enum>(3));
    a += oss.str().c_str();
    // trigger code path for rawMemoryToString
    CHECK(doctest::detail::rawMemoryToString(a).length() > 0u);
}

TEST_CASE("will end from a std::string exception") {
    throw_if(true, std::string("std::string!"));
}

TEST_CASE("will end from a const char* exception") {
    throw_if(true, "const char*!");
}

#endif // DOCTEST_CONFIG_DISABLE
