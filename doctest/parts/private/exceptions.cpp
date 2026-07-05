#include "doctest/parts/private/context_state.h"
#include "doctest/parts/public/exceptions.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

bool checkIfShouldThrow(assertType::Enum at) {
    if (at & assertType::is_require)
        return true;

    if ((at & assertType::is_check) && getContextOptions()->abort_after > 0 &&
        (g_cs->numAssertsFailed + g_cs->numAssertsFailedCurrentTest_atomic) >= getContextOptions()->abort_after)
        return true;

    return false;
}

bool has_uncaught_exceptions() {
#if defined(__cpp_lib_uncaught_exceptions) && __cpp_lib_uncaught_exceptions >= 201411L &&                              \
    (!defined(__MAC_OS_X_VERSION_MIN_REQUIRED) || __MAC_OS_X_VERSION_MIN_REQUIRED >= 101200)
    return (std::uncaught_exceptions() > 0);
#else
    return std::uncaught_exception();
#endif
}

#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
DOCTEST_NORETURN void throwException() {
    g_cs->shouldLogCurrentException = false;
    throw TestFailureException(); // NOLINT(hicpp-exception-baseclass)
}
#else  // DOCTEST_CONFIG_NO_EXCEPTIONS
void throwException() {}
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS

} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP
