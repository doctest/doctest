#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <vector>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

// =================================================================================================
// NORMAL TEMPLATED TEST CASES
// =================================================================================================

TYPE_TO_STRING_AS("SHORT!!!", short);

TEST_CASE_TEMPLATE("signed integers stuff", T, signed char, short, int) {
    T var = T();
    --var;
    CHECK(var == -1);
}

// teach the library how to stringify this type - otherwise <> will be used
TYPE_TO_STRING(std::vector<int>);

TEST_CASE_TEMPLATE("vector stuff", T, std::vector<int>) {
    T vec(10);
    CHECK(vec.size() == 20); // will fail
}

// =================================================================================================
// NAMED TEMPLATED TEST CASES WITH DEFERRED INSTANTIATION
// =================================================================================================

TEST_CASE_TEMPLATE_DEFINE("default construction", T, test_id) {
    T var = T();
    CHECK(doctest::Approx(var) == T());
}

TEST_CASE_TEMPLATE_INVOKE(test_id, signed char, short, int);
TEST_CASE_TEMPLATE_INVOKE(test_id, double, double); // note that types won't be filtered for uniqueness

TEST_CASE_TEMPLATE_APPLY(test_id, std::tuple<unsigned char, char>);

// =================================================================================================
// MULTIPLE TYPES AS PARAMETERS
// =================================================================================================

template <typename first, typename second>
struct TypePair
{
    using A = first;
    using B = second;
};

TYPE_TO_STRING_AS("Custom name test", TypePair<int, char>);
TYPE_TO_STRING_AS("Other custom name", TypePair<char, int>);
TYPE_TO_STRING(TypePair<bool, int>);

TEST_CASE_TEMPLATE("multiple types", T, TypePair<int, char>, TypePair<char, int>, TypePair<bool, int>) {
    using T1 = typename T::A;
    using T2 = typename T::B;
    T1 t1 = T1();
    T2 t2 = T2();
    // use T1 and T2 types
    CHECK(t1 == T1());
    CHECK(t2 != T2());
}

// currently the string result will be "int_pair" instead of "TypePair<int, int>" because of the way the type stringification works
using int_pair = TypePair<int, int>;
TYPE_TO_STRING(int_pair);

TEST_CASE_TEMPLATE("bad stringification of type pair", T, int_pair) {
    using T1 = typename T::A;
    using T2 = typename T::B;
    T1 t1 = T1();
    T2 t2 = T2();
    // use T1 and T2 types
    CHECK(t1 == T1());
    CHECK(t2 != T2());
}
