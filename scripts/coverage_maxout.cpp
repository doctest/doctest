#include <doctest/doctest.h>

// helper for throwing exceptions
template <typename T>
int throw_if(bool in, const T& ex) {
    if(in)
#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
        throw ex;
#else  // DOCTEST_CONFIG_NO_EXCEPTIONS
        ((void)ex);
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS
    return 42;
}

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <ostream>
#include <sstream>
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
           + toString(static_cast<unsigned int>(0))   //
           + toString(0.5f)                           //
           + toString(0.5)                            //
           + toString(static_cast<long double>(0.1))  //
           + toString('c')                            //
           + toString(static_cast<signed char>('c'))  //
           + toString(static_cast<unsigned char>(1))  //
           + toString(static_cast<short>(1))          //
           + toString(static_cast<long>(1))           //
           + toString(static_cast<unsigned long>(1))  //
           + toString(static_cast<unsigned short>(1)) //
           + toString(static_cast<long long>(1))      //
           + toString(static_cast<unsigned long long>(1));

    std::ostringstream oss;

    // toStream
    detail::toStream(&oss, true);
    detail::toStream(&oss, 0.5f);
    detail::toStream(&oss, 0.5);
    detail::toStream(&oss, static_cast<long double>(0.1));
    detail::toStream(&oss, 'c');
    detail::toStream(&oss, static_cast<signed char>('c'));
    detail::toStream(&oss, static_cast<unsigned char>(1));
    detail::toStream(&oss, static_cast<short>(1));
    detail::toStream(&oss, static_cast<long>(1));
    detail::toStream(&oss, static_cast<unsigned long>(1));
    detail::toStream(&oss, static_cast<unsigned short>(1));
    detail::toStream(&oss, static_cast<long long>(1));
    detail::toStream(&oss, static_cast<unsigned long long>(1));

    // trigger code path for String to ostream through operator<<
    oss << str;
    // trigger code path for assert string of a non-existent assert type
    oss << assertString(static_cast<assertType::Enum>(3));
    str += oss.str().c_str();
    str += failureString(assertType::is_normal);
    CHECK(str == "omgomgomgaaaNULLtrue00.5f0.50.199991111111true0.50.50.1cc"
                 "111111omgomgomgaaaNULLtrue00.5f0.50.199991111111");
    // trigger code path for rawMemoryToString
    bool   isThereAnything = str.size() > 0u;
    bool   len_is_zero     = detail::rawMemoryToString(isThereAnything).size() == 0u;
    String unknown         = toString(skip()); // trigger code path for "{?}"
    str                    = unknown;          // trigger code path for deleting memory in operator=
    CHECK_MESSAGE(len_is_zero, "should fail");

    Approx a(5);
    a.scale(4);
    Approx b = a(7);

    CHECK(b == 5);
    CHECK(b != 5);
    CHECK(b > 5);
    CHECK(b < 5);
    CHECK(b >= 5);
    CHECK(b <= 5);

    CHECK(6 == a);
    CHECK(6 != a);
    CHECK(6 > a);
    CHECK(6 < a);
    CHECK(6 >= a);
    CHECK(6 <= a);

    // trigger another single line of code... lol
    auto oldVal = const_cast<ContextOptions*>(getContextOptions())->no_path_in_filenames;
    const_cast<ContextOptions*>(getContextOptions())->no_path_in_filenames = false;
    CHECK(String(skipPathFromFilename("")) == "");
    const_cast<ContextOptions*>(getContextOptions())->no_path_in_filenames = oldVal;

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
