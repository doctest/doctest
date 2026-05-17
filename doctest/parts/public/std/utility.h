#ifndef DOCTEST_PARTS_PUBLIC_STD_UTILITY
#define DOCTEST_PARTS_PUBLIC_STD_UTILITY

#include "doctest/parts/public/std/type_traits.h"

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_PUSH

#ifdef DOCTEST_CONFIG_USE_STD_HEADERS
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <utility>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

namespace doctest {
namespace detail {

template <typename T>
T &&declval();

using std::forward;
using std::move;

template <typename T>
struct deferred_false : types::false_type {};

} // namespace detail
} // namespace doctest
#else // DOCTEST_CONFIG_USE_STD_HEADERS

namespace doctest {
namespace detail {

// <utility>
template <typename T>
T &&declval();

template <class T>
DOCTEST_CONSTEXPR_FUNC T &&forward(typename types::remove_reference<T>::type &t) DOCTEST_NOEXCEPT {
    return static_cast<T &&>(t);
}

template <class T>
// NOLINTNEXTLINE(cppcoreguidelines-rvalue-reference-param-not-moved)
DOCTEST_CONSTEXPR_FUNC T &&forward(typename types::remove_reference<T>::type &&t) DOCTEST_NOEXCEPT {
    return static_cast<T &&>(t);
}

template <typename T>
struct deferred_false : types::false_type {};

} // namespace detail
} // namespace doctest
#endif // DOCTEST_CONFIG_USE_STD_HEADERS

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_POP

#endif // DOCTEST_PARTS_PUBLIC_STD_UTILITY
