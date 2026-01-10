#include <doctest/doctest.h>
#include <limits>
#include <random>
using doctest::IsNaN;

TEST_CASE_TEMPLATE("Stringification", F, float, double, long double) {
    constexpr auto inf  = std::numeric_limits<F>::infinity();
    constexpr auto qnan = std::numeric_limits<F>::quiet_NaN();
    constexpr auto snan = std::numeric_limits<F>::signaling_NaN();

    const auto flip = true;
    CHECK(doctest::toString(IsNaN<F>(0)) == "IsNaN( 0 )");
    CHECK(doctest::toString(IsNaN<F>(0, flip)) == "! IsNaN( 0 )");
    CHECK(doctest::toString(IsNaN<F>(1.5)) == "IsNaN( 1.5 )");
    CHECK(doctest::toString(IsNaN<F>(1.5, flip)) == "! IsNaN( 1.5 )");
    CHECK(doctest::toString(IsNaN<F>(-1.5)) == "IsNaN( -1.5 )");
    CHECK(doctest::toString(IsNaN<F>(-1.5, flip)) == "! IsNaN( -1.5 )");

    CHECK(doctest::toString(IsNaN<F>(inf)) == "IsNaN( inf )");
    CHECK(doctest::toString(IsNaN<F>(qnan)) == "IsNaN( nan )");
    CHECK(doctest::toString(IsNaN<F>(snan)) == "IsNaN( nan )");
    CHECK(doctest::toString(IsNaN<F>(-inf)) == "IsNaN( -inf )");
    CHECK(doctest::toString(IsNaN<F>(-qnan)) == "IsNaN( -nan )");
    CHECK(doctest::toString(IsNaN<F>(-snan)) == "IsNaN( -nan )");

}

TEST_CASE_TEMPLATE("Matching against floating values", F, float, double, long double) {
    const auto flip = true;

    SUBCASE("Finite values") {
        std::random_device trng; // Cannot use auto as non-movable type

        auto csprng = std::mt19937(trng());
        auto min    = std::numeric_limits<F>::lowest() / 2;
        auto max    = std::numeric_limits<F>::max() / 2;
        auto dist   = std::uniform_real_distribution<F>(min, max);
        for(auto count = 0; count < 100; count++) {
            auto value = dist(csprng);
            CHECK_FALSE(IsNaN<F>(value));
            CHECK      (IsNaN<F>(value, flip));
        }
    }

    SUBCASE("Infinite values") {
        constexpr auto inf = std::numeric_limits<F>::infinity();

        CHECK_FALSE(IsNaN<F>(inf));
        CHECK_FALSE(IsNaN<F>(-inf));

        CHECK(IsNaN<F>( inf, flip));
        CHECK(IsNaN<F>(-inf, flip));
    }

    SUBCASE("Quiet NaN value") {
        constexpr auto qnan = std::numeric_limits<F>::quiet_NaN();

        CHECK(IsNaN<F>(qnan));
        CHECK(IsNaN<F>(-qnan));

        CHECK_FALSE(IsNaN<F>( qnan, flip));
        CHECK_FALSE(IsNaN<F>(-qnan, flip));

    }

    SUBCASE("Signalling NaN value") {
        constexpr auto snan = std::numeric_limits<F>::signaling_NaN();

        CHECK(IsNaN<F>( snan));
        CHECK(IsNaN<F>(-snan));

        CHECK_FALSE(IsNaN<F>( snan, flip));
        CHECK_FALSE(IsNaN<F>(-snan, flip));
    }
}
