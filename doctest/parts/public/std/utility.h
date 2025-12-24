#ifndef DOCTEST_PARTS_PUBLIC_STD_UTILITY
#define DOCTEST_PARTS_PUBLIC_STD_UTILITY

#include "doctest/parts/public/std/type_traits.h"

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_PUSH

namespace doctest {
namespace detail {

  // <utility>
  template <typename T>
  T&& declval();

  template <class T>
  DOCTEST_CONSTEXPR_FUNC T&& forward(typename types::remove_reference<T>::type& t) DOCTEST_NOEXCEPT {
      return static_cast<T&&>(t);
  }

  template <class T>
  DOCTEST_CONSTEXPR_FUNC T&& forward(typename types::remove_reference<T>::type&& t) DOCTEST_NOEXCEPT {
      return static_cast<T&&>(t);
  }

  template <typename T>
  struct deferred_false : types::false_type { };

} // namespace detail
} // namespace doctest

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_POP

#endif // DOCTEST_PARTS_PUBLIC_STD_UTILITY
