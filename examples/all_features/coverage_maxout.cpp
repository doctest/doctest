#include "doctest.h"

#include <ostream>

TEST_CASE("doctest internals") {
    // string stuff
    doctest::String       a(0);
    const doctest::String const_str("omgomgomg");
    a = const_str.c_str();
    CHECK(a.size() == const_str.size());
    CHECK(a.length() == const_str.length());
    CHECK(a.compare(const_str, true) == 0);
    CHECK(a.compare("omgomgomg", false) == 0);

    // toString
    a += toString("aaa") + toString(0.5f) + toString('c') + toString(true) +
         toString(static_cast<long double>(0.1))   //
         + toString(static_cast<unsigned char>(1)) //
         + toString(static_cast<short>(1))         //
         + toString(static_cast<long>(1))          //
         + toString(static_cast<unsigned long>(1)) //
         + toString(static_cast<unsigned short>(1));

    // others
    a += doctest::detail::fileForOutput("c:\\a");
    a += doctest::detail::fileForOutput("c:/a");
    a += doctest::detail::fileForOutput("a");
    std::ostringstream oss;
    oss << a;
    oss << doctest::detail::getAssertString(static_cast<doctest::detail::assertType::Enum>(3));
    a += oss.str().c_str();
    CHECK(doctest::detail::rawMemoryToString(a).length() > 0u);
}

TEST_SUITE_BEGIN("ts1");

using doctest::Approx;

static int throws(bool in) {
    if(in)
#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
        throw false;
#else // DOCTEST_CONFIG_NO_EXCEPTIONS
        return 0;
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS
    return 42;
}

struct myType
{
    myType() {}

private:
    myType(const myType&);            // non-copyable
    myType& operator=(const myType&); // non-assignable
};

static std::ostream& operator<<(std::ostream& stream, const myType&) {
    stream << "myType!";
    return stream;
}

static bool operator==(const myType&, const myType&) { return false; }

TEST_CASE("expressions should be evaluated only once") {
    int a = 5;
    REQUIRE(++a == 6);
    REQUIRE_EQ(++a, 7);
}

TEST_CASE("assertions") {
    CHECK(true);
    CHECK(1 == 0);
    CHECK_FALSE(1);
    myType a;
    myType b;
    CHECK(a == b);
    CHECK(Approx(0.1) == 0.2);

    CHECK_THROWS(throws(true));
    CHECK_THROWS(throws(false));
    CHECK_NOTHROW(throws(false));
    CHECK_NOTHROW(throws(true));
    CHECK_THROWS_AS(throws(true), bool);
    REQUIRE_THROWS_AS(throws(false), bool);
}

TEST_CASE("assertions - all of them") {
    WARN(true);
    CHECK(true);
    REQUIRE(true);
    WARN_FALSE(false);
    CHECK_FALSE(false);
    REQUIRE_FALSE(false);
    WARN_THROWS(throws(true));
    CHECK_THROWS(throws(true));
    REQUIRE_THROWS(throws(true));
    WARN_THROWS_AS(throws(true), bool);
    CHECK_THROWS_AS(throws(true), bool);
    REQUIRE_THROWS_AS(throws(true), bool);
    WARN_NOTHROW(throws(false));
    CHECK_NOTHROW(throws(false));
    REQUIRE_NOTHROW(throws(false));
    WARN_EQ(1, 1);
    CHECK_EQ(1, 1);
    REQUIRE_EQ(1, 1);
    WARN_NE(1, 0);
    CHECK_NE(1, 0);
    REQUIRE_NE(1, 0);
    WARN_GT(1, 0);
    CHECK_GT(1, 0);
    REQUIRE_GT(1, 0);
    WARN_LT(0, 1);
    CHECK_LT(0, 1);
    REQUIRE_LT(0, 1);
    WARN_GE(1, 1);
    CHECK_GE(1, 1);
    REQUIRE_GE(1, 1);
    WARN_LE(1, 1);
    CHECK_LE(1, 1);
    REQUIRE_LE(1, 1);
    WARN_UNARY(1);
    CHECK_UNARY(1);
    REQUIRE_UNARY(1);
    WARN_UNARY_FALSE(0);
    CHECK_UNARY_FALSE(0);
    REQUIRE_UNARY_FALSE(0);
    FAST_WARN_EQ(1, 1);
    FAST_CHECK_EQ(1, 1);
    FAST_REQUIRE_EQ(1, 1);
    FAST_WARN_NE(1, 0);
    FAST_CHECK_NE(1, 0);
    FAST_REQUIRE_NE(1, 0);
    FAST_WARN_GT(1, 0);
    FAST_CHECK_GT(1, 0);
    FAST_REQUIRE_GT(1, 0);
    FAST_WARN_LT(0, 1);
    FAST_CHECK_LT(0, 1);
    FAST_REQUIRE_LT(0, 1);
    FAST_WARN_GE(1, 1);
    FAST_CHECK_GE(1, 1);
    FAST_REQUIRE_GE(1, 1);
    FAST_WARN_LE(1, 1);
    FAST_CHECK_LE(1, 1);
    FAST_REQUIRE_LE(1, 1);
    FAST_WARN_UNARY(1);
    FAST_CHECK_UNARY(1);
    FAST_REQUIRE_UNARY(1);
    FAST_WARN_UNARY_FALSE(0);
    FAST_CHECK_UNARY_FALSE(0);
    FAST_REQUIRE_UNARY_FALSE(1);
}

TEST_CASE("throws") { throws(true); }

TEST_SUITE_END();
