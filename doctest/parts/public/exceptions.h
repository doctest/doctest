#ifndef DOCTEST_PARTS_PUBLIC_EXCEPTIONS
#define DOCTEST_PARTS_PUBLIC_EXCEPTIONS

#include "doctest/parts/public/config.h"
#include "doctest/parts/public/warnings.h"
#include "doctest/parts/public/assert/type.h"

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_PUSH

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

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_POP

#endif // DOCTEST_PARTS_PUBLIC_EXCEPTIONS
