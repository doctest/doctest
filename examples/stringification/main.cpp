#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
// Pass
#if __cplusplus >= 202002L
#   define CXX_STANDARD 20
#elif __cplusplus >= 201703L
#   define CXX_STANDARD 17
#elif __cplusplus >= 201402L
#   define CXX_STANDARD 14
#elif __cplusplus >= 201103L
#   define CXX_STANDARD 11
#else
#   define CXX_STANDARD 3
#endif

#include <ostream>

#include <string>
#include <utility>
#include <tuple>
#include <array>
#include <valarray>
#include <initializer_list>
#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <stack>
#include <queue>
#include <set>
#include <map>

#if CXX_STANDARD >= 14
#   include <string_view>
#endif

#if CXX_STANDARD >= 17
#   include <optional>
#   include <variant>
#endif

#if CXX_STANDARD >= 20
#   include <span>
#endif
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

namespace
{

enum Enum
{
    E_VALUE = 1
};

enum EnumWithOss
{
    E_VALUE_WITH_OSS = 2
};

std::ostream& operator<<( std::ostream& os, EnumWithOss value )
{
    switch (value) {
    case EnumWithOss::E_VALUE_WITH_OSS:
        os << "E_VALUE_WITH_OSS";
        break;
    }
    return os;
}

enum class EnumClass : int
{
    VALUE = 3
};

enum class EnumClassWithOss : int
{
    A = 4
};

std::ostream& operator<<( std::ostream& os, EnumClassWithOss value )
{
    switch (value) {
    case EnumClassWithOss::A:
        os << "A";
        break;
    }
    return os;
}

struct Struct
{
};

struct StructOss
{
    int value;
};

inline std::ostream& operator<<(std::ostream& os, StructOss s)
{
    return os << s.value;
}

}

#define ENABLE_BROKEN_TESTS

using namespace doctest;

TEST_CASE("nullptr_t")
{
    CHECK_EQ(toString(nullptr), "NULL");
}

TEST_CASE("Integer_types")
{
    CHECK_EQ(toString(static_cast<short int>(-15679)), "-15679");
    CHECK_EQ(toString(static_cast<short int>(0)), "0");
    CHECK_EQ(toString(static_cast<short int>(28154)), "28154");
    
    CHECK_EQ(toString(static_cast<unsigned short int>(0u)), "0");
    CHECK_EQ(toString(static_cast<unsigned short int>(42407u)), "42407");
    
    CHECK_EQ(toString(static_cast<int>(-26985)), "-26985");
    CHECK_EQ(toString(static_cast<int>(0)), "0");
    CHECK_EQ(toString(static_cast<int>(51425)), "51425");
    
    CHECK_EQ(toString(static_cast<unsigned int>(0u)), "0");
    CHECK_EQ(toString(static_cast<unsigned int>(57974u)), "57974");
    
    CHECK_EQ(toString(static_cast<long int>(-8088342l)), "-8088342");
    CHECK_EQ(toString(static_cast<long int>(0l)), "0");
    CHECK_EQ(toString(static_cast<long int>(7784301l)), "7784301");
    
    CHECK_EQ(toString(static_cast<unsigned long int>(0ul)), "0");
    CHECK_EQ(toString(static_cast<unsigned long int>(396060450ul)), "396060450");
    
    CHECK_EQ(toString(static_cast<long long int>(-38997570817ll)), "-38997570817");
    CHECK_EQ(toString(static_cast<long long int>(0ll)), "0");
    CHECK_EQ(toString(static_cast<long long int>(21204560479ll)), "21204560479");
    
    CHECK_EQ(toString(static_cast<unsigned long long int>(0ull)), "0");
    CHECK_EQ(toString(static_cast<unsigned long long int>(13324909658562760708ull)), "13324909658562760708");
}

TEST_CASE("Boolean_type")
{
    CHECK_EQ(toString(true), "true");
    CHECK_EQ(toString(false), "false");
}

TEST_CASE("Standard_character_types")
{
    CHECK_EQ(toString(static_cast<signed char>('A')), "65");
    CHECK_EQ(toString(static_cast<unsigned char>('B')), "66");
    CHECK_EQ(toString(static_cast<char>('C')), "67");
}

// String representation is lost in c++20 for whatever reason
TEST_CASE("Wide_character_types")
{
#ifdef ENABLE_BROKEN_TESTS
#if CXX_STANDARD >= 20
    CHECK_EQ(toString(static_cast<wchar_t>(L'A')), "{?}");
    CHECK_EQ(toString(static_cast<char16_t>(L'B')), "{?}");
    CHECK_EQ(toString(static_cast<char32_t>(L'C')), "{?}");
#else
    CHECK_EQ(toString(static_cast<wchar_t>(L'A')), "65");
    CHECK_EQ(toString(static_cast<char16_t>(L'B')), "66");
    CHECK_EQ(toString(static_cast<char32_t>(L'C')), "67");
#endif
#endif
}

TEST_CASE("Floating_point_types_zero")
{
    CHECK_EQ(toString(static_cast<float>(0.0f)), "0.0f");
    CHECK_EQ(toString(static_cast<double>(0.0)), "0.0");
    CHECK_EQ(toString(static_cast<long double>(0.0L)), "0.0");
}

// Output format is quite fragile due to absence of output precision
// For example 904.1f may be printed as 904.09998f
// I have no idea how to test it reliably due to this fact
TEST_CASE("Floating_point_types__non_zero" * may_fail())
{
    CHECK_EQ(toString(static_cast<float>(-10.0f)), "-10.0f");
    CHECK_EQ(toString(static_cast<float>(-1.1f)), "-1.1f");
    CHECK_EQ(toString(static_cast<float>(105.1f)), "105.1f");
    CHECK_EQ(toString(static_cast<float>(51425.0f)), "51425.0f");
    
    CHECK_EQ(toString(static_cast<double>(-10.0)), "-10.0");
    CHECK_EQ(toString(static_cast<double>(-1.1)), "-1.1");
    CHECK_EQ(toString(static_cast<double>(105.1)), "105.1");
    CHECK_EQ(toString(static_cast<double>(51425.0)), "51425.0");
    
    CHECK_EQ(toString(static_cast<long double>(-10.0L)), "-10.0");
    CHECK_EQ(toString(static_cast<long double>(-1.1L)), "-1.1");
    CHECK_EQ(toString(static_cast<long double>(15.1L)), "15.1");
    CHECK_EQ(toString(static_cast<long double>(51425.0L)), "51425.0");
}

TEST_CASE("Pointer_types")
{
#if SIZEOF_VOID_P == 4
#   define POINTER_PREFIX "0x"
#elif SIZEOF_VOID_P == 8
#   define POINTER_PREFIX "0x00000000"
#endif
    
    // doctest overloads toString() for pointer types. So string doesn't match ostreamed result
    // NULL is handled separatly somewhat
    // other value representation has fixed size string representation depending on sizeof(void*)
    CHECK_EQ(toString(static_cast<int*>(nullptr)), "NULL");
    CHECK_EQ(toString(reinterpret_cast<int*>(0xABADF00D)), POINTER_PREFIX "abadf00d");
    
    CHECK_EQ(toString(static_cast<signed char*>(nullptr)), "NULL");
    CHECK_EQ(toString(reinterpret_cast<signed char*>(0xDEADBABE)), POINTER_PREFIX "deadbabe");
    
    CHECK_EQ(toString(static_cast<unsigned char*>(nullptr)), "NULL");
    CHECK_EQ(toString(reinterpret_cast<unsigned char*>(0xB16B00B5)), POINTER_PREFIX "b16b00b5");
    
    CHECK_EQ(toString(static_cast<char*>(nullptr)), "NULL");
    CHECK_EQ(toString(reinterpret_cast<char*>(0xFEEDFACE)), POINTER_PREFIX "feedface");
    
    CHECK_EQ(toString(static_cast<wchar_t*>(nullptr)), "NULL");
    CHECK_EQ(toString(reinterpret_cast<wchar_t*>(0xDEFEC8ED)), POINTER_PREFIX "defec8ed");
    
    CHECK_EQ(toString(static_cast<char16_t*>(nullptr)), "NULL");
    CHECK_EQ(toString(reinterpret_cast<char16_t*>(0xBEEFCACE)), POINTER_PREFIX "beefcace");
    
    CHECK_EQ(toString(static_cast<char32_t*>(nullptr)), "NULL");
    CHECK_EQ(toString(reinterpret_cast<char32_t*>(0xCAFEBABE)), POINTER_PREFIX "cafebabe");

    // constant versions
    CHECK_EQ(toString(static_cast<const int*>(nullptr)), "NULL");
    CHECK_EQ(toString(reinterpret_cast<const int*>(0xABADF00D)), POINTER_PREFIX "abadf00d");
    
    CHECK_EQ(toString(static_cast<const signed char*>(nullptr)), "NULL");
    CHECK_EQ(toString(reinterpret_cast<const signed char*>(0xDEADBABE)), POINTER_PREFIX "deadbabe");
    
    CHECK_EQ(toString(static_cast<const unsigned char*>(nullptr)), "NULL");
    CHECK_EQ(toString(reinterpret_cast<const unsigned char*>(0xB16B00B5)), POINTER_PREFIX "b16b00b5");
    
    CHECK_EQ(toString(static_cast<const char*>(nullptr)), "NULL");
    CHECK_EQ(toString(reinterpret_cast<const char*>(0xFEEDFACE)), POINTER_PREFIX "feedface");
    
    CHECK_EQ(toString(static_cast<const wchar_t*>(nullptr)), "NULL");
    CHECK_EQ(toString(reinterpret_cast<const wchar_t*>(0xDEFEC8ED)), POINTER_PREFIX "defec8ed");
    
    CHECK_EQ(toString(static_cast<const char16_t*>(nullptr)), "NULL");
    CHECK_EQ(toString(reinterpret_cast<const char16_t*>(0xBEEFCACE)), POINTER_PREFIX "beefcace");
    
    CHECK_EQ(toString(static_cast<const char32_t*>(nullptr)), "NULL");
    CHECK_EQ(toString(reinterpret_cast<const char32_t*>(0xCAFEBABE)), POINTER_PREFIX "cafebabe");
}

TEST_CASE("C_arrays")
{
#ifdef ENABLE_BROKEN_TESTS
    // TODO: cover more types. Especially character types
    {
        const char ztConstCharArray[] = {'f', 'o', 'o', '\0'};  CHECK_EQ(toString(ztConstCharArray), "foo");
        char ztCharArray[] = {'b', 'a', 'r', '\0'};             CHECK_EQ(toString(ztCharArray), "bar");
    }

    const char constCharArray[] = {'f', 'o', 'o'};          CHECK_EQ(toString(constCharArray), "foo");
    char charArray[] = {'b', 'a', 'r'};                     CHECK_EQ(toString(charArray), "bar");
    const int constIntArray[] = {1, 2, 3};                  CHECK_EQ(toString(constIntArray), "[1, 2, 3]");
    const int intArray[] = {4, 5, 6};                       CHECK_EQ(toString(intArray), "[4, 5, 6]");
#endif
}

TEST_CASE("Enumerations")
{
    CHECK_EQ(toString(Enum::E_VALUE), "1");
    CHECK_EQ(toString(EnumClass::VALUE), "3");
#ifdef ENABLE_BROKEN_TESTS
    CHECK_EQ(toString(EnumWithOss::E_VALUE_WITH_OSS), "E_VALUE_WITH_OSS");
    CHECK_EQ(toString(EnumClassWithOss::A), "A");
#endif
}

TEST_CASE("Structs")
{
    CHECK_EQ(toString(Struct{}), "{?}"); // No overloaded operator, no text representation
    CHECK_EQ(toString(StructOss{7}), "7"); // Use overloaded ostream operator
}

TEST_CASE("std_classes")
{
#if CXX_STANDARD >= 11
    
    {
        const std::string value = "Hello world!";
        CHECK_EQ(toString(value), "Hello world!");
    }

    {
        std::pair<int, float> value { 1, 2.3f };
        CHECK_EQ(toString(value), "{?}");
    }

    {
        std::tuple<bool, int, double> value { false, 7, 6.6 };
        CHECK_EQ(toString(value), "{?}");
    }
    
    {
        std::array<int, 3> value {{ 1, 5, 8 }};
        CHECK_EQ(toString(value), "{?}");
    }

    {
        std::valarray<int> value { 7, 3, 2 };
        CHECK_EQ(toString(value), "{?}");
    }

    {
        std::initializer_list<int> value { 1, 7, 0 };
        CHECK_EQ(toString(value), "{?}");
    }

    {
        std::vector<int> value { 4, 2, 7 };
        CHECK_EQ(toString(value), "{?}");
    }

    {
        std::deque<int> value { 1, 1, 6, 2 };
        CHECK_EQ(toString(value), "{?}");
    }

    {
        std::list<int> value { 7, 5, 2, 1 };
        CHECK_EQ(toString(value), "{?}");
    }

    {
        std::forward_list<int> value { 7, 0, 3, 7, 4 };
        CHECK_EQ(toString(value), "{?}");
    }

    {
        std::stack<int> value {{ 1, 2, 3, 4, 5 }};
        CHECK_EQ(toString(value), "{?}");
    }

    {
        std::queue<int> value {{ 1, 2, 3, 4, 5 }};
        CHECK_EQ(toString(value), "{?}");
    }

    {
        const auto data = { 1, 8, 5, 6, 3 };

        std::priority_queue<int> value { data.begin(), data.end() };
        CHECK_EQ(toString(value), "{?}");
    }
    
    {
        std::set<int> value {{ 1, 2, 2, 6, 1, 4 }};
        CHECK_EQ(toString(value), "{?}");
    }
    
    {
        std::multiset<int> value {{ 1, 2, 2, 6, 1, 4 }};
        CHECK_EQ(toString(value), "{?}");
    }
    
    {
        std::map<int, std::string> value {{ { 1, "foo" }, { 2, "bar" }, { 1, "baz" } }};
        CHECK_EQ(toString(value), "{?}");
    }
    
    {
        std::multimap<int, std::string> value {{ { 1, "foo" }, { 2, "bar" }, { 1, "baz" } }};
        CHECK_EQ(toString(value), "{?}");
    }
#endif  // CXX_STANDARD >= 11

#if CXX_STANDARD >= 14
    {
        std::integer_sequence<int, 9, 2, 5, 1, 9, 1, 6> value {};
        CHECK_EQ(toString(value), "{?}");
    }
#endif  // CXX_STANDARD >= 14

#if CXX_STANDARD >= 17
    {
        const std::string s = "Hello world!";
        std::string_view value{ s };
        value = value.substr(0, 5);
        CHECK_EQ(toString(value), "Hello"); // standard ostream operator is used
    }
    
    {
        CHECK_EQ(toString(std::nullopt), "{?}");
        std::optional<int> value {};
        CHECK_EQ(toString(value), "{?}");
        value = 9;
        CHECK_EQ(toString(value), "{?}");
    }
    
#if !DOCTEST_MSVC || (DOCTEST_MSVC >= DOCTEST_COMPILER(19, 29, 0)) // MSVC v141 has defect is default constructable std::variant
    {
        std::variant<std::monostate, int, bool> value {};
        CHECK_EQ(toString(value), "{?}"); // std::monostate
        value = 5;
        CHECK_EQ(toString(value), "{?}");
        value = true;
        CHECK_EQ(toString(value), "{?}");
    }
#endif
    
#endif  // CXX_STANDARD >= 17

#if CXX_STANDARD >= 20
    {
        std::array<int, 3> ar {{ 9, 1, 5 }};
        std::span<int> value { ar };
        CHECK_EQ(toString(value), "{?}");
    }
#endif  // CXX_STANDARD >= 20
}

#ifdef ENABLE_BROKEN_TESTS
TEST_CASE("Ostream_poisoning_regression")
{
    // Hex result of toString shouldn't leave internal stream mode into hex mode
    CHECK_EQ(toString(StructOss{16}), "16");
    toString(reinterpret_cast<int*>(0xBEEFCACE));
    CHECK_EQ(toString(StructOss{16}), "16");
}
#endif
