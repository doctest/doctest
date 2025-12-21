#ifdef DOCTEST_CONFIG_INCLUDE_TYPE_TRAITS
#include <type_traits>
#endif // DOCTEST_CONFIG_INCLUDE_TYPE_TRAITS

namespace doctest {
namespace detail {
namespace types {

#ifdef DOCTEST_CONFIG_INCLUDE_TYPE_TRAITS
  using namespace std;
#else
  template <bool COND, typename T = void>
  struct enable_if { };

  template <typename T>
  struct enable_if<true, T> { using type = T; };

  struct true_type { static DOCTEST_CONSTEXPR bool value = true; };
  struct false_type { static DOCTEST_CONSTEXPR bool value = false; };

  template <typename T> struct remove_reference { using type = T; };
  template <typename T> struct remove_reference<T&> { using type = T; };
  template <typename T> struct remove_reference<T&&> { using type = T; };

  template <typename T> struct is_rvalue_reference : false_type { };
  template <typename T> struct is_rvalue_reference<T&&> : true_type { };

  template<typename T> struct remove_const { using type = T; };
  template <typename T> struct remove_const<const T> { using type = T; };

  // Compiler intrinsics
  template <typename T> struct is_enum { static DOCTEST_CONSTEXPR bool value = __is_enum(T); };
  template <typename T> struct underlying_type { using type = __underlying_type(T); };

  template <typename T> struct is_pointer : false_type { };
  template <typename T> struct is_pointer<T*> : true_type { };

  template <typename T> struct is_array : false_type { };
  // NOLINTNEXTLINE(*-avoid-c-arrays)
  template <typename T, size_t SIZE> struct is_array<T[SIZE]> : true_type { };
#endif

} // namespace types
} // namespace detail
} // namespace doctest
