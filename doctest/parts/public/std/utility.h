#ifndef DOCTEST_PARTS_PUBLIC_STD_UTILITY
#define DOCTEST_PARTS_PUBLIC_STD_UTILITY

#include "doctest/parts/public/std/type_traits.h"

#ifdef DOCTEST_CONFIG_USE_STD_HEADERS
#include <utility>
#endif

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_PUSH

namespace doctest {
namespace detail {

// <utility>
#ifdef DOCTEST_CONFIG_USE_STD_HEADERS
using std::declval;
using std::forward;
using std::move;
#else
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

template <class T>
DOCTEST_CONSTEXPR_FUNC typename types::remove_reference<T>::type &&move(T &&t) DOCTEST_NOEXCEPT {
    return static_cast<typename types::remove_reference<T>::type &&>(t);
}
#endif

template <typename T>
struct deferred_false : types::false_type {};

} // namespace detail
} // namespace doctest

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_POP

#endif // DOCTEST_PARTS_PUBLIC_STD_UTILITY
