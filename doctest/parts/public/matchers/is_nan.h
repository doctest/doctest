#ifndef DOCTEST_PARTS_PUBLIC_MATCHERS_IS_NAN
#define DOCTEST_PARTS_PUBLIC_MATCHERS_IS_NAN

#include "doctest/parts/public/string.h"

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_PUSH

namespace doctest {

template <typename F>
struct DOCTEST_INTERFACE_DECL IsNaN
{
    F value; bool flipped;
    IsNaN(F f, bool flip = false) : value(f), flipped(flip) { }
    IsNaN<F> operator!() const { return { value, !flipped }; }
    operator bool() const;
};

#ifndef __MINGW32__
extern template struct DOCTEST_INTERFACE_DECL IsNaN<float>;
extern template struct DOCTEST_INTERFACE_DECL IsNaN<double>;
extern template struct DOCTEST_INTERFACE_DECL IsNaN<long double>;
#endif

DOCTEST_INTERFACE String toString(IsNaN<float> in);
DOCTEST_INTERFACE String toString(IsNaN<double> in);
DOCTEST_INTERFACE String toString(IsNaN<double long> in);

} // namespace doctest

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_POP

#endif // DOCTEST_PARTS_PUBLIC_MATCHERS_IS_NAN
