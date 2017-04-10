#include "parts/doctest_fwd.h"

#include <iostream>
using namespace std;

using doctest::Approx;

class Volatility
{
    double underlying_;

public:
    explicit Volatility(double u)
            : underlying_(u) {}
    //explicit
    operator double() const { return underlying_; }
};

static int throws(bool in) { if(in) throw 42; return 42; }

TEST_CASE("") {
    Volatility asd(1.0);
    CHECK(static_cast<double>(asd) == Approx(1));
    CHECK(asd == Approx(1));
    CHECK(Approx(1) == asd);
    CHECK(Approx(asd) == 1.0);

    INFO("aaaa" << "sad :(");
    throws(true);

    CHECK(Approx(1) == 2);
    CHECK(Approx(1) == 2.0);
    CHECK(Approx(1) == 2.f);
    CHECK(Approx(1.0) == 2);
    CHECK(Approx(1.0) == 2.0);
    CHECK(Approx(1.0) == 2.f);
    CHECK(Approx(1.f) == 2);
    CHECK(Approx(1.f) == 2.0);
    CHECK(Approx(1.f) == 2.f);
}
