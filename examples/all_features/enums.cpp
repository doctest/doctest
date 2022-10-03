#include <doctest/doctest.h>

// GCC < 5 breaks when trying to compare enums to integers when using std headers.
#if !defined(DOCTEST_CONFIG_USE_STD_HEADERS) || DOCTEST_GCC == 0 || DOCTEST_GCC >= DOCTEST_COMPILER(5, 0, 0)
#define RET_TYPE(x) x
#else
#define RET_TYPE(x) typename doctest::detail::types::underlying_type<x>::type
#endif

template <typename S>
static RET_TYPE(S) castToUnderlying(S in) {
    return in;
}

#include "header.h"

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <cstdint>
#include <sstream>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

namespace
{

enum StandardEnum
{
    Zero,
    One,
    Two,
};

enum TypedEnum : int64_t
{
    TypedZero,
    TypedOne,
    TypedTwo,
};

enum class EnumClassC : char
{
    Zero = '0',
    One = '1',
    Two = '2',
};

enum class EnumClassSC : signed char
{
    Zero = '0',
    One = '1',
    Two = '2',
};

enum class EnumClassUC : unsigned char
{
    Zero = '0',
    One = '1',
    Two = '2',
};

enum class EnumClassU8 : uint8_t
{
    Zero,
    One,
    Two,
};

template<class E, class T = typename std::underlying_type<E>::type>
T printable(E val)
{
    return static_cast<T>(val);
}

}

TEST_CASE("enum 1")
{
    std::ostringstream ostr;
    ostr << Zero << One << Two;
    ostr << TypedZero << TypedOne << TypedTwo;
    static_assert(std::is_enum<EnumClassSC>::value, "");
    ostr << printable(EnumClassSC::Zero) << printable(EnumClassSC::One) << printable(EnumClassSC::Two);

    CHECK_EQ(castToUnderlying(Zero), 0);
    CHECK_EQ(castToUnderlying(One), 1);
    CHECK_EQ(castToUnderlying(Two), 2);

    CHECK_EQ(castToUnderlying(TypedZero), 0);
    CHECK_EQ(castToUnderlying(TypedOne), 1);
    CHECK_EQ(castToUnderlying(TypedTwo), 2);

    CHECK_EQ(EnumClassSC::Zero, EnumClassSC::Zero);
    CHECK_EQ(EnumClassSC::One, EnumClassSC::One);
    CHECK_EQ(EnumClassSC::Two, EnumClassSC::Two);
}

TEST_CASE("enum 2" * doctest::should_fail())
{
    CHECK_EQ(castToUnderlying(Zero), 1);
    CHECK_EQ(castToUnderlying(One), 2);
    CHECK_EQ(castToUnderlying(Two), 3);

    CHECK_EQ(castToUnderlying(TypedZero), 1);
    CHECK_EQ(castToUnderlying(TypedOne), 2);
    CHECK_EQ(castToUnderlying(TypedTwo), 3);

    CHECK_EQ(EnumClassC::Zero, EnumClassC::One);
    CHECK_EQ(EnumClassC::One, EnumClassC::Two);
    CHECK_EQ(EnumClassC::Two, EnumClassC::Zero);

    CHECK_EQ(EnumClassSC::Zero, EnumClassSC::One);
    CHECK_EQ(EnumClassSC::One, EnumClassSC::Two);
    CHECK_EQ(EnumClassSC::Two, EnumClassSC::Zero);

    CHECK_EQ(EnumClassUC::Zero, EnumClassUC::One);
    CHECK_EQ(EnumClassUC::One, EnumClassUC::Two);
    CHECK_EQ(EnumClassUC::Two, EnumClassUC::Zero);

    CHECK_EQ(EnumClassU8::Zero, EnumClassU8::One);
    CHECK_EQ(EnumClassU8::One, EnumClassU8::Two);
    CHECK_EQ(EnumClassU8::Two, EnumClassU8::Zero);
}
