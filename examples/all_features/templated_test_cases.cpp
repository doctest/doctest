#include "doctest.h"

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <vector>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

// typedefs are required if variadic macro support is not available (otherwise the commas are a problem)
typedef doctest::Types<char, short, int> int_types;
typedef doctest::Types<double, double> float_types; // note that types won't be filtered for uniqueness

// =================================================================================================
// NORMAL TEMPLATED TEST CASES
// =================================================================================================

TEST_CASE_TEMPLATE("signed integers stuff", T, int_types) {
    T var = T();
    --var;
    CHECK(var == -1);
}

// teach the library how to stringify this type - otherwise <> will be used
TYPE_TO_STRING(std::vector<int>);

TEST_CASE_TEMPLATE("vector stuff", T, doctest::Types<std::vector<int> >) {
    T vec(10);
    CHECK(vec.size() == 20); // fill fail
}

// =================================================================================================
// NAMED TEMPLATED TEST CASES WITH DEFERRED INSTANTIATION
// =================================================================================================

TEST_CASE_TEMPLATE_DEFINE("default construction", T, test_id) {
    T var = T();
    CHECK(doctest::Approx(var) == T());
}

TEST_CASE_TEMPLATE_INSTANTIATE(test_id, int_types);
TEST_CASE_TEMPLATE_INSTANTIATE(test_id, float_types);

// =================================================================================================
// MULTIPLE TYPES AS PARAMETERS
// =================================================================================================

template <typename first, typename second>
struct TypePair
{
    typedef first  A;
    typedef second B;
};

typedef doctest::Types<
    TypePair<int, char>,
    TypePair<char, int>,
    TypePair<bool, int>
> pairs;

TEST_CASE_TEMPLATE("multiple types", T, pairs) {
    typedef typename T::A T1;
    typedef typename T::B T2;
    T1 t1 = T1();
    T2 t2 = T2();
    // use T1 and T2 types
    CHECK(t1 == T1());
    CHECK(t2 != T2());
}

// if variadic macros are supported then "TypePair<int, int>" can be passed directly to the macro (otherwise the commas are a problem)
// currently the string result will be "int_pair" instead of "TypePair<int, int>" because of the way the type stringification works
typedef TypePair<int, int> int_pair;
TYPE_TO_STRING(int_pair);

TEST_CASE_TEMPLATE("bad stringification of type pair", T, doctest::Types<int_pair>) {
    typedef typename T::A T1;
    typedef typename T::B T2;
    T1 t1 = T1();
    T2 t2 = T2();
    // use T1 and T2 types
    CHECK(t1 == T1());
    CHECK(t2 != T2());
}
