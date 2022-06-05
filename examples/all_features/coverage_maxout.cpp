#include <doctest/doctest.h>

#include "header.h"

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <ostream>
#include <sstream>
#include <stdexcept>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

#ifndef DOCTEST_CONFIG_DISABLE

// =================================================================================================
// !!! THESE ARE NOT PROPER EXAMPLES OF LIBRARY USAGE !!! THESE ARE MEANT FOR CODE COVERAGE ONLY !!!
// =================================================================================================

TEST_CASE("exercising tricky code paths of doctest") {
    using namespace doctest;

    // trigger code path for comparing the file in "operator<" of SubcaseSignature
    CHECK(SubcaseSignature{"", "a.cpp", 0} < SubcaseSignature{"", "b.cpp", 0});
    // same for String
    CHECK(String("a.cpp") < String("b.cpp"));

    // trigger code path for string with nullptr
    String       str;
    const String const_str("omgomgomg");
    str = const_str.c_str();
    CHECK(const_str[0] == 'o');
    CHECK(str.capacity() == 24);
    CHECK(str.size() == const_str.size());
    CHECK_MESSAGE(str.compare(const_str, true) != 0, "should fail");
    CHECK_MESSAGE(str.compare("omgomgomg", false) != 0, "should fail");

    String heap_str("012345678901234567890123456789");
    CHECK(heap_str.capacity() == heap_str.size() + 1); // on heap with maxed capacity
    heap_str += "0123456789";
    CHECK(heap_str.capacity() > heap_str.size() + 1);
    heap_str += "0123456789"; // triggers path in +=
    CHECK(heap_str[heap_str.size() - 1] == '9');
    heap_str = "";

    CHECK(String("abc") == "abc");
    CHECK(String("abc") > "aaa");
    CHECK(String("abc") >= "aaa");
    CHECK(String("abc") < "bbb");
    CHECK(String("abc") <= "bbb");
    CHECK(String("abc")[0] == 'a');

    // toString
    str += toString("aaa")                            //
           + toString(nullptr)                        //
           + toString(true)                           //
           + toString(0u)                             //
           + toString('c')                            //
           + toString(static_cast<signed char>('c'))  //
           + toString(static_cast<unsigned char>(1))  //
           + toString(static_cast<short>(1))          //
           + toString(1L)                             //
           + toString(1UL)                            //
           + toString(static_cast<unsigned short>(1)) //
           + toString(1LL)                            //
           + toString(1ULL);

    std::ostringstream oss;

    // trigger code path for String to ostream through operator<<
    oss << str;
    // trigger code path for assert string of a non-existent assert type
#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
    try {
        assertString(static_cast<assertType::Enum>(3));
    } catch (const std::logic_error&) { }
#endif
    str += oss.str().c_str();
    str += failureString(assertType::is_normal);
    CHECK(str == "omgomgomgaaanullptrtrue099991111111"
                 "omgomgomgaaanullptrtrue099991111111");
    // trigger code path for rawMemoryToString
    bool   isThereAnything = str.size() > 0u;
    String unknown         = toString(skip()); // trigger code path for "{?}"
    str                    = unknown;          // trigger code path for deleting memory in operator=
    CHECK_FALSE_MESSAGE(isThereAnything, "should fail");

    Approx a(5);
    a.scale(4);
    Approx b = a(7);

    CHECK(b == 7);
    CHECK(b != 6);
    CHECK(b > 6);
    CHECK(b < 8);
    CHECK(b >= 7);
    CHECK(b <= 7);

    CHECK(5 == a);
    CHECK(6 != a);
    CHECK(6 > a);
    CHECK(4 < a);
    CHECK(5 >= a);
    CHECK(5 <= a);

    // trigger another single line of code... lol
    // NOLINTBEGIN(cppcoreguidelines-pro-type-const-cast)
    auto oldVal = const_cast<ContextOptions*>(getContextOptions())->no_path_in_filenames;
    const_cast<ContextOptions*>(getContextOptions())->no_path_in_filenames = false;
    CHECK(String(skipPathFromFilename("")) == "");
    const_cast<ContextOptions*>(getContextOptions())->no_path_in_filenames = oldVal;
    // NOLINTEND(cppcoreguidelines-pro-type-const-cast)

    // a hack to trigger a bug in doctest: currently a 0 cannot be successfully parsed for an int option!
    Context().setOption("last", 0);
}

TEST_SUITE("will be overridden by a decorator" * doctest::test_suite("exception related")) {
    TEST_CASE("will end from a std::string exception") {
        throw_if(true, std::string("std::string!"));
    }

    TEST_CASE("will end from a const char* exception") { throw_if(true, "const char*!"); }

    TEST_CASE("will end from an unknown exception") {
        throw_if(true, doctest::String("unknown :("));
    }
}

#endif // DOCTEST_CONFIG_DISABLE
