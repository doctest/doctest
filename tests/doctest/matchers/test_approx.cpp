#define DOCTEST_CONFIG_INCLUDE_TYPE_TRAITS
#include <doctest/doctest.h>

#include <algorithm>
#include <limits>
#include <cmath>
#include <tuple>
using doctest::Approx;

namespace {
/** Represents a closed-interval of real values */
struct bounds {
    double min, max;

    static bounds determine(const Approx &m) {
        // For the current implementation of Approx::operator==
        //    |x - C| < Ɛ(s + max(|x|, |C|))
        // ...we can derive a closed-form solution for the lower and upper bound by case-analysis
        // This isn't included as it's relatively easily reproducible
        const auto e = m.m_epsilon;
        const auto s = m.m_scale;
        const auto C = m.m_value;

        if (std::isinf(C)) { return { C, C }; }
        const auto min = std::min((C - e*s) / (1 - e), C - e*(s + std::abs(C)));
        const auto max = std::max((C + e*s) / (1 - e), C + e*(s + std::abs(C)));
        return { min, max };
    }
};

doctest::String toString(bounds b) {
    return "{ .min = " + doctest::toString(b.min) + ", .max = " + doctest::toString(b.max) + " }";
}
} // namespace

TEST_CASE_TEMPLATE("Construction with non-double values", T, float, double, long double) {
    const auto m = Approx(T(1)).epsilon(T(2)).scale(T(3));

    CHECK(m.m_value   == Approx(1.0));
    CHECK(m.m_epsilon == Approx(2.0));
    CHECK(m.m_scale   == Approx(3.0));
}

TEST_CASE_TEMPLATE("Verification of relational operators", T, float, double, long double) {
    const auto m = Approx(100.0).epsilon(0.01).scale(3);
    CAPTURE(bounds::determine(m)); // [98.97, 101.04]

    const auto too_small = T( 98.96); // Would be 98.97, but float loses too much precision
    const auto     small = T( 98.98);
    const auto    center = T(100.00);
    const auto     large = T(101.04);
    const auto too_large = T(101.05);

    SUBCASE("operator==") {
        CHECK_FALSE(too_small == m);
        CHECK      (small     == m);
        CHECK      (center    == m);
        CHECK      (large     == m);
        CHECK_FALSE(too_large == m);

        CHECK_FALSE(m == too_small);
        CHECK      (m == small);
        CHECK      (m == center);
        CHECK      (m == large);
        CHECK_FALSE(m == too_large);
    }

    SUBCASE("operator!=") {
        CHECK      (too_small != m);
        CHECK_FALSE(small     != m);
        CHECK_FALSE(center    != m);
        CHECK_FALSE(large     != m);
        CHECK      (too_large != m);

        CHECK      (m != too_small);
        CHECK_FALSE(m != small);
        CHECK_FALSE(m != center);
        CHECK_FALSE(m != large);
        CHECK      (m != too_large);
    }

    SUBCASE("operator<") {
        CHECK      (too_small < m);
        CHECK_FALSE(small     < m);
        CHECK_FALSE(center    < m);
        CHECK_FALSE(large     < m);
        CHECK_FALSE(too_large < m);

        CHECK_FALSE(m < too_small);
        CHECK_FALSE(m < small);
        CHECK_FALSE(m < center);
        CHECK_FALSE(m < large);
        CHECK      (m < too_large);
    }

    SUBCASE("operator<=") {
        CHECK      (too_small <= m);
        CHECK      (    small <= m);
        CHECK      (   center <= m);
        CHECK      (   large  <= m);
        CHECK_FALSE(too_large <= m);

        CHECK_FALSE(m <= too_small);
        CHECK      (m <= small);
        CHECK      (m <= center);
        CHECK      (m <= large);
        CHECK      (m <= too_large);
    }

    SUBCASE("operator>") {
        CHECK_FALSE(too_small > m);
        CHECK_FALSE(small     > m);
        CHECK_FALSE(center    > m);
        CHECK_FALSE(large     > m);
        CHECK      (too_large > m);

        CHECK      (m > too_small);
        CHECK_FALSE(m > small);
        CHECK_FALSE(m > center);
        CHECK_FALSE(m > large);
        CHECK_FALSE(m > too_large);
    }

    SUBCASE("operator>=") {
        CHECK_FALSE(too_small >= m);
        CHECK      (small     >= m);
        CHECK      (center    >= m);
        CHECK      (large     >= m);
        CHECK      (too_large >= m);

        CHECK      (m >= too_small);
        CHECK      (m >= small);
        CHECK      (m >= center);
        CHECK      (m >= large);
        CHECK_FALSE(m >= too_large);
    }
}

TEST_CASE("Comparison with finite floating-point values" * doctest::expected_failures(2)) {
    const auto epsilon = std::numeric_limits<double>::epsilon();
    const auto inf     = std::numeric_limits<double>::infinity();

    SUBCASE("Matcher focused around 0") {
        const auto m = Approx(0.0);
        CAPTURE(bounds::determine(m)); // [-1.19211e-5, 1.19211e-5]

        CHECK(-1.19211e-5 != m);
        CHECK(-1.19210e-5 == m);
        CHECK( 0.0        == m);
        CHECK(+1.19210e-5 == m);
        CHECK(+1.19211e-5 != m);
    }

    SUBCASE("Matcher focused around 0 with an error of 0% and no scaling") {
        const auto m = Approx(0.0).epsilon(0.0).scale(0);
        CAPTURE(bounds::determine(m)); // [0, 0]

        CHECK(-epsilon != m);
        CHECK( 0.0     == m); // FAIL
        CHECK(+epsilon != m);
    }

    SUBCASE("Matcher focused around 0 with an error of 100% and no scaling") {
        const auto m = Approx(0.0).epsilon(1.0).scale(0);
        CAPTURE(bounds::determine(m)); // [-NaN, +NaN]

        CHECK(-epsilon != m);
        CHECK( 0.0     == m); // FAIL
        CHECK(+epsilon != m);
    }

    SUBCASE("Matcher focused around smallest positive normalized value") {
        const auto m = Approx(std::numeric_limits<double>::min());
        CAPTURE(bounds::determine(m)); // [-1.19211e-5, 1.19211e-5]

        CHECK(-1.19211e-5 != m);
        CHECK(-1.19210e-5 == m);
        CHECK( 0.0        == m);
        CHECK(+1.19210e-5 == m);
        CHECK(+1.19211e-5 != m);
    }

    SUBCASE("Matcher focused around maximum normalized value") {
        const auto m = Approx(std::numeric_limits<double>::max());
        CAPTURE(bounds::determine(m)); // [1.79767e308, inf]
        CHECK(m != 1.79767e308);
        CHECK(m == 1.79768e308);
        CHECK(m != inf);
    }

    SUBCASE("Matcher focused around minimum normalized value") {
        const auto m = Approx(std::numeric_limits<double>::lowest());
        CAPTURE(bounds::determine(m)); // [-inf, -1.79767e308]

        CHECK(m != -1.79767e308);
        CHECK(m == -1.79768e308);
        CHECK(m != -inf);
    }

    SUBCASE("Matcher focused around 10 with an error of 10% and no scaling") {
        const auto m = Approx(10.0).epsilon(0.1).scale(0);
        CAPTURE(bounds::determine(m)); // [9, 11.1111...]

        // TODO: (9.000 ~ m) is seemingly indeterminate
        CHECK( 8.999 != m);
        CHECK( 9.001 == m);
        CHECK(10.000 == m);
        CHECK(11.111 == m);
        CHECK(11.112 != m);
    }

    SUBCASE("Matcher focused around 25 with an error of 1% and no scaling") {
        const auto m = Approx(25.0).epsilon(0.01).scale(0);
        CAPTURE(bounds::determine(m)); // [24.7500, 25.2525]

        // TODO: (24.7500 ~ m) is seemingly indeterminate
        CHECK(24.7499 != m);
        CHECK(24.7501 == m);
        CHECK(25.0000 == m);
        CHECK(25.2525 == m);
        CHECK(25.2526 != m);
    }

    SUBCASE("Matcher focused around 100 with an error of 100% and no scaling") {
        const auto m = Approx(100.0).epsilon(1.0).scale(0);
        CAPTURE(bounds::determine(m)); // [0, inf]

        CHECK(0.0   != m);
        CHECK(1e-14 == m);
        CHECK(100.0 == m);
        CHECK(1e+18 == m);
    }

    SUBCASE("Matcher focused around 75 with an error of 1% and a scale of 3") {
        const auto m = Approx(75).epsilon(0.01).scale(3);
        CAPTURE(bounds::determine(m)); // [74.2200, 75.7879]

        CHECK(74.2200 != m);
        CHECK(74.2201 == m);
        CHECK(75.0000 == m);
        CHECK(75.7878 == m);
        CHECK(75.7879 != m);
    }
}

TEST_CASE("Comparison with non-finite floating-point values") {
    SUBCASE("Matcher focused around infinity") {
        const auto inf = std::numeric_limits<float>::infinity();
        const auto m   = Approx(inf);
        CAPTURE(bounds::determine(m)); // [inf, inf]

        CHECK_FALSE(inf == m);
        CHECK      (inf != m);
        CHECK_FALSE(inf <  m);
        CHECK_FALSE(inf <= m);
        CHECK_FALSE(inf >  m);
        CHECK_FALSE(inf >= m);
    }

    SUBCASE("Matcher focused around negative-infinity") {
        const auto inf = std::numeric_limits<float>::infinity();
        const auto m   = Approx(-inf);
        CAPTURE(bounds::determine(m)); // [-inf, -inf]

        CHECK_FALSE(-inf == m);
        CHECK      (-inf != m);
        CHECK_FALSE(-inf <  m);
        CHECK_FALSE(-inf <= m);
        CHECK_FALSE(-inf >  m);
        CHECK_FALSE(-inf >= m);
     }

    SUBCASE("Matcher focused around a quiet NaN") {
        const auto qnan = std::numeric_limits<float>::quiet_NaN();
        const auto m    = Approx(qnan);
        CAPTURE(bounds::determine(m)); // [nan, nan]

        CHECK_FALSE(qnan == m);
        CHECK      (qnan != m);
        CHECK_FALSE(qnan <  m);
        CHECK_FALSE(qnan <= m);
        CHECK_FALSE(qnan >  m);
        CHECK_FALSE(qnan >= m);
     }

    SUBCASE("Matcher focused around a signalling NaN") {
        const auto snan = std::numeric_limits<float>::signaling_NaN();
        const auto m    = Approx(snan);
        CAPTURE(bounds::determine(m)); // [nan, nan]

        CHECK_FALSE(snan == m);
        CHECK      (snan != m);
        CHECK_FALSE(snan <  m);
        CHECK_FALSE(snan <= m);
        CHECK_FALSE(snan >  m);
        CHECK_FALSE(snan >= m);
     }
}

TEST_CASE("Stringification") {
    SUBCASE("Matcher without epsilon or scale set") {
        constexpr auto inf = std::numeric_limits<double>::infinity();
        constexpr auto nan = std::numeric_limits<double>::signaling_NaN();

        CHECK(doctest::toString(Approx( 0  )) == "Approx( 0 )");
        CHECK(doctest::toString(Approx( 1  )) == "Approx( 1 )");
        CHECK(doctest::toString(Approx( 1.5)) == "Approx( 1.5 )");
        CHECK(doctest::toString(Approx(-1.5)) == "Approx( -1.5 )");
        CHECK(doctest::toString(Approx( inf)) == "Approx( inf )");
        CHECK(doctest::toString(Approx(-inf)) == "Approx( -inf )");
        CHECK(doctest::toString(Approx( nan)) == "Approx( nan )");
        CHECK(doctest::toString(Approx(-nan)) == "Approx( -nan )");
    }

    SUBCASE("Matcher with both epsilon and scale set") {
        CHECK(doctest::toString(Approx(0).epsilon(1).scale(2)) == "Approx( 0 )");
    }
}
