#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

  struct DOCTEST_INTERFACE TestFailureException
  {
  };

  DOCTEST_INTERFACE bool checkIfShouldThrow(assertType::Enum at);

#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
  DOCTEST_NORETURN
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS
  DOCTEST_INTERFACE void throwException();

} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE
