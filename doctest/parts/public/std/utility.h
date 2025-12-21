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
