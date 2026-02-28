#ifndef DOCTEST_PARTS_PUBLIC_GENERATOR
#define DOCTEST_PARTS_PUBLIC_GENERATOR

#include "doctest/parts/public/std/fwd.h"
#include "doctest/parts/public/utility.h"

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_PUSH

#ifndef DOCTEST_CONFIG_DISABLE
namespace doctest {
namespace detail {

DOCTEST_INTERFACE size_t acquireGeneratorDecisionIndex(size_t count);

template <typename T, typename... Rest>
T acquireGeneratorValue(T first, Rest... rest) {
    const T values[] = {first, static_cast<T>(rest)...};
    const size_t idx = acquireGeneratorDecisionIndex(1 + sizeof...(Rest));
    return values[idx];
}

} // namespace detail
} // namespace doctest
#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_POP

#endif // DOCTEST_PARTS_PUBLIC_GENERATOR
