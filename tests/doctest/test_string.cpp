#define DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
#include <doctest/doctest.h>
#include <limits>
#include <sstream>
using doctest::String;

namespace {

/** Determines the address of the actual string content */
inline const void *data_address(const String &s) {
  return reinterpret_cast<const void *>(s.c_str());
}
/**
 * Determines if the String object is on the stack,
 * i.e. the private String::isOnStack method would return true
 * This is mostly to check we're hitting small-string optimizations
 */
inline bool is_on_stack(const String &s) {
    return reinterpret_cast<const void *>(&s) == data_address(s);
}

/** Convenience alias for !is_on_stack */
inline bool is_on_heap(const String &s) {
    return !is_on_stack(s);
}

// For type-level stringification tests
struct X;
struct Y;
struct Z;
namespace nested { struct Type; }

} // namespace

TEST_SUITE("String construction") {
    TEST_CASE("Default construction") {
        auto string = String();

        CHECK(string.c_str() == "");
        CHECK(string.size() == 0u);
        CHECK(string.capacity() == 24u);
        CHECK(is_on_stack(string));
    }

    TEST_CASE("Construction from string-literal") {
        SUBCASE("Small-string-optimizable string literal") {
            auto string = String("doctest");

            CHECK(string.c_str() == "doctest");
            CHECK(string.size() == 7u);
            CHECK(string.capacity() == 24u);
            CHECK(is_on_stack(string));
        }

        SUBCASE("Large string literal") {
            auto string = String("a very big string literal");

            CHECK(string.c_str() == "a very big string literal");
            CHECK(string.size() == 25u);
            CHECK(string.capacity() == 26u);
            CHECK(is_on_heap(string));
        }
    }

    TEST_CASE("Construction from sized string-literal") {
        SUBCASE("Small-string-optimizable string literal") {
            auto string = String("a particularly long string", 6);

            CHECK(string.c_str() == "a part");
            CHECK(string.size() == 6u);
            CHECK(string.capacity() == 24u);
            CHECK(is_on_stack(string));
        }

        SUBCASE("Large string literal, size set to below threshold") {
            auto string = String("a very big string literal", 23);

            CHECK(string.c_str() == "a very big string liter");
            CHECK(string.size() == 23u);
            CHECK(string.capacity() == 24u);
            CHECK(is_on_stack(string));
        }

        SUBCASE("Large string literal, size set to threshold") {
            auto string = String("a very big string literal", 24);

            CHECK(string.c_str() == "a very big string litera");
            CHECK(string.size() == 24u);
            CHECK(string.capacity() == 25u);
            CHECK(is_on_heap(string));
        }
    }

    TEST_CASE("Construction from an input-stream") {
        std::stringstream ss { };
        ss << "a very big string literal";

        SUBCASE("Size set to below threshold") {
            auto string = String(ss, 17);

            CHECK(string.c_str() == "a very big string");
            CHECK(string.size() == 17u);
            CHECK(string.capacity() == 24u);
            CHECK(is_on_stack(string));
        }

        SUBCASE("Size set to threshold") {
            auto string = String(ss, 24);

            CHECK(string.c_str() == "a very big string litera");
            CHECK(string.size() == 24u);
            CHECK(string.capacity() == 25u);
            CHECK(is_on_heap(string));
        }
    }

    TEST_CASE("Construction from a substring") {
        auto base = String("an extraordinarily large string literal");
        REQUIRE(base.size() == 39u);
        REQUIRE(base.capacity() == 40u);
        REQUIRE(is_on_heap(base));

        SUBCASE("Substring over [0, 0] with const source") {
            auto string = base.substr(0, 0);

            CHECK(string.c_str() == "");
            CHECK(string.size() == 0u);
            CHECK(string.capacity() == 24u);
            CHECK(is_on_stack(string));
        }

        SUBCASE("Substring over [0, 0] with rvalue source") {
            auto string = std::move(base).substr(0, 0);

            CHECK(string.c_str() == "");
            CHECK(string.size() == 0u);
            CHECK(string.capacity() == 40u);
            CHECK(is_on_heap(string));
        }

        SUBCASE("Substring over [3, 24] with const source") {
            auto string = base.substr(3, 21); // NOLINT(bugprone-use-after-move) NOLINT(hicpp-invalid-access-moved)

            CHECK(string.c_str() == "extraordinarily large");
            CHECK(string.size() == 21u);
            CHECK(string.capacity() == 24u);
            CHECK(is_on_stack(string));
        }

        SUBCASE("Substring over [19, 31] with rvalue source") {
            auto string = std::move(base).substr(19, 12); // NOLINT(bugprone-use-after-move) NOLINT(hicpp-invalid-access-moved)

            CHECK(string.c_str() == "large string");
            CHECK(string.size() == 12u);
            CHECK(string.capacity() == 40u);
            CHECK(is_on_heap(string));
        }

        SUBCASE("Substring over [0, SIZE_MAX] with const source") {
            auto string = base.substr(0, std::numeric_limits<String::size_type>::max()); // NOLINT(bugprone-use-after-move) NOLINT(hicpp-invalid-access-moved)

            CHECK(string.c_str() == "an extraordinarily large string literal");
            CHECK(string.size() == 39u);
            CHECK(string.capacity() == 40u);
            CHECK(is_on_heap(string));
        }

        SUBCASE("Substring over [0, SIZE_MAX] with rvalue source") {
            auto string = base.substr(0, std::numeric_limits<String::size_type>::max()); // NOLINT(bugprone-use-after-move) NOLINT(hicpp-invalid-access-moved)

            CHECK(string.c_str() == "an extraordinarily large string literal");
            CHECK(string.size() == 39u);
            CHECK(string.capacity() == 40u);
            CHECK(is_on_heap(string));
        }
    }

    TEST_CASE("Copy-construction") {
        const auto stack_string = String("stack-string");
        const auto heap_string  = String("a very long string that will be on heap");
        REQUIRE(is_on_stack(stack_string));
        REQUIRE(is_on_heap(heap_string));

        SUBCASE("Copying stack-string") {
            auto string = String(stack_string);
            CHECK(string == stack_string);
        }

        SUBCASE("Copying heap-string") {
            auto string = String(heap_string);
            CHECK(string == heap_string);
        }
    }

    TEST_CASE("Copy-assign construction") {
        const auto stack_string = String("stack-string");
        const auto heap_string  = String("a very long string that will be on heap");
        REQUIRE(is_on_stack(stack_string));
        REQUIRE(is_on_heap(heap_string));

        SUBCASE("Copying stack-string to existing stack-string") {
            auto string = String("another stack-string");
            REQUIRE(is_on_stack(string));

            string = stack_string;
            CHECK(string == stack_string);
            CHECK(is_on_stack(string));
        }

        SUBCASE("Copying stack-string to existing heap-string") {
            auto string = String("another very long string that will be on heap");
            REQUIRE(is_on_heap(string));

            string = stack_string;
            CHECK(string == stack_string);
            CHECK(is_on_stack(string));
        }

        SUBCASE("Copying heap-string to existing stack-string") {
            auto string = String("another stack-string");
            REQUIRE(is_on_stack(string));

            string = heap_string;
            CHECK(string == heap_string);
            CHECK(is_on_heap(string));
        }

        SUBCASE("Copying heap-string to existing heap-string") {
            auto string = String("another very long string that will be on heap");
            REQUIRE(is_on_heap(string));

            string = heap_string;
            CHECK(string == heap_string);
            CHECK(is_on_heap(string));
        }
    }

    TEST_CASE("Move-construction") {
        const auto stack_string = String("stack-string");
        const auto heap_string  = String("a very long string that will be on heap");
        REQUIRE(is_on_stack(stack_string));
        REQUIRE(is_on_heap(heap_string));

        SUBCASE("Moving stack-string") {
            auto string = std::move(String(stack_string));
            CHECK(string == stack_string);
            CHECK(is_on_stack(string));
        }

        SUBCASE("Moving heap-string") {
            auto string = std::move(String(heap_string));
            CHECK(string == heap_string);
            CHECK(is_on_heap(string));
        }
    }

    TEST_CASE("Move-assign construction") {
        const auto stack_string = String("stack-string");
        const auto heap_string  = String("a very long string that will be on heap");
        REQUIRE(is_on_stack(stack_string));
        REQUIRE(is_on_heap(heap_string));

        SUBCASE("Moving stack-string to existing stack-string") {
            auto string = String("another stack-string");
            REQUIRE(is_on_stack(string));

            string = std::move(String(stack_string));
            CHECK(string == stack_string);
            CHECK(is_on_stack(string));
        }

        SUBCASE("Moving stack-string to existing heap-string") {
            auto string = String("another very long string that will be on heap");
            REQUIRE(is_on_heap(string));

            string = std::move(String(stack_string));
            CHECK(string == stack_string);
            CHECK(is_on_stack(string));
        }

        SUBCASE("Moving heap-string to existing stack-string") {
            auto string = String("another stack-string");
            REQUIRE(is_on_stack(string));

            string = std::move(String(heap_string));
            CHECK(string == heap_string);
            CHECK(is_on_heap(string));
        }

        SUBCASE("Moving heap-string to existing heap-string") {
            auto string = String("another very long string that will be on heap");
            REQUIRE(is_on_heap(string));

            string = std::move(String(heap_string));
            CHECK(string == heap_string);
            CHECK(is_on_heap(string));
        }
    }
}

TEST_SUITE("String searching") {
    // For some reason, String::npos produces an undefined-reference link error
    // So for these tests, we have to invent our own...
    const auto npos = String::size_type(-1);

    TEST_CASE("Forward-searching") {
        SUBCASE("Empty source string") {
            auto string = String("");
            CHECK(string.find('x') == npos);
            CHECK(string.find('y') == npos);
            CHECK(string.find('z') == npos);
        }

        SUBCASE("Non-empty source string") {
            auto string = String("doctest");

            CHECK(string.find('x') == npos);

            CHECK(string.find('d')    == 0u);
            CHECK(string.find('d', 1) == npos);

            CHECK(string.find('c')    == 2u);
            CHECK(string.find('c', 1) == 2u);
            CHECK(string.find('c', 2) == 2u);
            CHECK(string.find('c', 3) == npos);

            CHECK(string.find('t')    == 3u);
            CHECK(string.find('t', 2) == 3u);
            CHECK(string.find('t', 3) == 3u);
            CHECK(string.find('t', 4) == 6u);
        }
    }

    TEST_CASE("Reverse-searching") {
        SUBCASE("Empty source string") {
            auto string = String("");
            CHECK(string.rfind('x') == npos);
            CHECK(string.rfind('y') == npos);
            CHECK(string.rfind('z') == npos);
        }

        SUBCASE("Non-empty source string") {
            auto string = String("doctest");

            CHECK(string.rfind('x') == npos);

            CHECK(string.rfind('d')    == 0u);
            CHECK(string.rfind('d', 1) == 0u);

            CHECK(string.rfind('c')    == 2u);
            CHECK(string.rfind('c', 1) == npos);
            CHECK(string.rfind('c', 2) == 2u);
            CHECK(string.rfind('c', 3) == 2u);

            CHECK(string.rfind('t')    == 6u);
            CHECK(string.rfind('t', 2) == npos);
            CHECK(string.rfind('t', 3) == 3u);
            CHECK(string.rfind('t', 4) == 3u);
        }
    }
}

TEST_CASE("String comparison") {
    SUBCASE("Case-sensitive C-style comparison") {
        CHECK(String(""       ).compare(""       ) == 0);
        CHECK(String(""       ).compare("doctest") <  0);
        CHECK(String("doctest").compare(""       ) >  0);
        CHECK(String("doctest").compare("Doctest") >  0);
    }

    SUBCASE("Case-insensitive C-style comparison") {
        const auto caseless = true;
        CHECK(String(""       ).compare("",        caseless) == 0);
        CHECK(String(""       ).compare("doctest", caseless) <  0);
        CHECK(String("doctest").compare("",        caseless) >  0);
        CHECK(String("doctest").compare("Doctest", caseless) == 0);
    }

    SUBCASE("operator<") {
        CHECK_FALSE(String("")        < String(""));
        CHECK      (String("")        < String("doctest"));
        CHECK_FALSE(String("doctest") < String(""));
        CHECK_FALSE(String("doctest") < String("doctest"));

        CHECK      (String("x") < String("y"));
        CHECK_FALSE(String("y") < String("x"));
    }

    SUBCASE("operator>=") {
        CHECK      (String("")        >= String(""));
        CHECK_FALSE(String("")        >= String("doctest"));
        CHECK      (String("doctest") >= String(""));
        CHECK      (String("doctest") >= String("doctest"));

        CHECK_FALSE(String("x") >= String("y"));
        CHECK      (String("y") >= String("x"));
    }

    SUBCASE("operator>") {
        CHECK_FALSE(String("")        > String(""));
        CHECK_FALSE(String("")        > String("doctest"));
        CHECK      (String("doctest") > String(""));
        CHECK_FALSE(String("doctest") > String("doctest"));

        CHECK_FALSE(String("x") > String("y"));
        CHECK      (String("y") > String("x"));
    }

    SUBCASE("operator<=") {
        CHECK      (String("")        <= String(""));
        CHECK      (String("")        <= String("doctest"));
        CHECK_FALSE(String("doctest") <= String(""));
        CHECK      (String("doctest") <= String("doctest"));

        CHECK      (String("x") <= String("y"));
        CHECK_FALSE(String("y") <= String("x"));
    }
}

TEST_SUITE("String manipulation") {
    TEST_CASE("Appending to a new string") {
        CHECK(String("")    + String("")     == String(""));
        CHECK(String("doc") + String("")     == String("doc"));
        CHECK(String("")    + String("test") == String("test"));
        CHECK(String("doc") + String("test") == String("doctest"));
    }

    TEST_CASE("Appending in-place") {
        auto result = String("");

        result += String("doc");
        CHECK(result == String("doc"));

        result += String("");
        CHECK(result == String("doc"));

        result += String("test");
        CHECK(result == String("doctest"));

        result += String("");
        CHECK(result == String("doctest"));
    }
}

TEST_SUITE("Type stringification") {
    using namespace doctest;

    TEST_CASE("Fundamental types") {
        CHECK(toString<void          >() == "void");
        CHECK(toString<bool          >() == "bool");
        CHECK(toString<std::nullptr_t>() == "std::nullptr_t");

        CHECK(toString<char              >() == "char");
        CHECK(toString<char16_t          >() == "char16_t");
        CHECK(toString<char32_t          >() == "char32_t");

        CHECK(toString<signed char       >() == "signed char");
        CHECK(toString<unsigned char     >() == "unsigned char");
        CHECK(toString<short             >() == "short int");
        CHECK(toString<unsigned short    >() == "short unsigned int");
        CHECK(toString<int               >() == "int");
        CHECK(toString<unsigned int      >() == "unsigned int");
        CHECK(toString<long              >() == "long int");
        CHECK(toString<unsigned long     >() == "long unsigned int");
        CHECK(toString<long long         >() == "long long int");
        CHECK(toString<unsigned long long>() == "long long unsigned int");

        CHECK(toString<float      >() == "float");
        CHECK(toString<double     >() == "double");
        CHECK(toString<long double>() == "long double");

        CHECK(toString<volatile const void *const>() == "const volatile void* const");
    }

    TEST_CASE("Doctest internal types") {
        CHECK(toString<doctest::String      >() == "String");
        CHECK(toString<doctest::Approx      >() == "Approx");
        CHECK(toString<doctest::Contains    >() == "Contains");
        CHECK(toString<doctest::IsNaN<float>>() == "IsNaN<float>");
    }

    TEST_CASE("Custom types") {
        CHECK(toString<X           >() == "{anonymous}::X");
        CHECK(toString<Y           >() == "{anonymous}::Y");
        CHECK(toString<Z           >() == "{anonymous}::Z");
        CHECK(toString<nested::Type>() == "{anonymous}::nested::Type");
    }
}

TEST_SUITE("Value stringification") {
    using namespace doctest;

    // Some types can't be braced-initialized, so...
    template <typename T>
    T lit(T value) { return value; }

    TEST_CASE("Fundamental types") {
        // Cannot instantiate 'void' so excluded
        SUBCASE("bool") {
            CHECK(toString(false) == "false");
            CHECK(toString(true)  == "true");
        }

        SUBCASE("nullptr") {
            CHECK(toString(nullptr) == "nullptr");
        }

        SUBCASE("Character types") {
            CHECK(toString(lit<char         >('X' )) == "88");
            CHECK(toString(lit<char16_t     >(u'X')) == "{?}");
            CHECK(toString(lit<char32_t     >(U'X')) == "{?}");
        }

        SUBCASE("Integral types") {
            CHECK(toString(lit<unsigned      char >(12)) == "12");
            CHECK(toString(lit<  signed      char >(23)) == "23");
            CHECK(toString(lit<              short>(34)) == "34");
            CHECK(toString(lit<unsigned      short>(45)) == "45");
            CHECK(toString(lit<              int  >(56)) == "56");
            CHECK(toString(lit<unsigned      int  >(67)) == "67");
            CHECK(toString(lit<              long >(78)) == "78");
            CHECK(toString(lit<unsigned      long >(89)) == "89");
            CHECK(toString(lit<         long long >(90)) == "90");
            CHECK(toString(lit<unsigned long long >(99)) == "99");
        }

        SUBCASE("Floating types") {
            CHECK(toString(lit<      float>(1.5)) == "1.5");
            CHECK(toString(lit<     double>(2.5)) == "2.5");
            CHECK(toString(lit<long double>(3.5)) == "3.5");
        }
    }
}
