#include "parts/doctest_fwd.h"

template <typename first, typename second>
struct TypePair
{
    typedef first  A;
    typedef second B;
};

using doctest::Types;

typedef Types<TypePair<int, char>, TypePair<char, int> > pairs;
typedef Types<int, char, float, double, int> the_types;

//TYPE_TO_STRING(TypePair<char, int>)

TEST_CASE_TEMPLATE("trololo", T, pairs) {
    typedef typename T::A A;
    typedef typename T::B B;
    INFO("sad :(");
    CHECK(sizeof(A) == static_cast<unsigned>(4));
    CHECK(sizeof(B) == static_cast<unsigned>(4));
    //T a = T();
    //++a;
    //CHECK(doctest::Approx(a) == T());
}

TEST_CASE_TEMPLATE_DEFINE("separate", T, it) {
    MESSAGE("aaa");
    //T a = T();
    //++a;
    //SUBCASE("root") {
    //    ++a;
    //    SUBCASE("1") {
    //        CHECK(doctest::Approx(a) == T());
    //    }
    //    SUBCASE("2") {
    //        CHECK(doctest::Approx(a) == T());
    //    }
    //}
}

TEST_CASE_TEMPLATE_INSTANTIATE(it, the_types)
TEST_CASE_TEMPLATE_INSTANTIATE(it, the_types)
