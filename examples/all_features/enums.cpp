#include <doctest/doctest.h>

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
    return T(val);
}

}

TEST_CASE("enum 1")
{
    std::ostringstream ostr;
    ostr << Zero << One << Two;
    ostr << TypedZero << TypedOne << TypedTwo;
    static_assert(std::is_enum<EnumClassSC>::value, "");
    ostr << printable(EnumClassSC::Zero) << printable(EnumClassSC::One) << printable(EnumClassSC::Two);

    CHECK_EQ(Zero, 0);
    CHECK_EQ(One, 1);
    CHECK_EQ(Two, 2);

    CHECK_EQ(TypedZero, 0);
    CHECK_EQ(TypedOne, 1);
    CHECK_EQ(TypedTwo, 2);

    CHECK_EQ(EnumClassSC::Zero, EnumClassSC::Zero);
    CHECK_EQ(EnumClassSC::One, EnumClassSC::One);
    CHECK_EQ(EnumClassSC::Two, EnumClassSC::Two);
}

TEST_CASE("enum 2" * doctest::should_fail())
{
    CHECK_EQ(Zero, 1);
    CHECK_EQ(One, 2);
    CHECK_EQ(Two, 3);

    CHECK_EQ(TypedZero, 1);
    CHECK_EQ(TypedOne, 2);
    CHECK_EQ(TypedTwo, 3);

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
