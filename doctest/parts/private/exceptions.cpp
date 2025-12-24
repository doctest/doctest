#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/context_state.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

    bool checkIfShouldThrow(assertType::Enum at) {
        if(at & assertType::is_require) //!OCLINT bitwise operator in conditional
            return true;

        if((at & assertType::is_check) //!OCLINT bitwise operator in conditional
           && getContextOptions()->abort_after > 0 &&
           (g_cs->numAssertsFailed + g_cs->numAssertsFailedCurrentTest_atomic) >=
                   getContextOptions()->abort_after)
            return true;

        return false;
    }

#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
    DOCTEST_NORETURN void throwException() {
        g_cs->shouldLogCurrentException = false;
        throw TestFailureException(); // NOLINT(hicpp-exception-baseclass)
    }
#else // DOCTEST_CONFIG_NO_EXCEPTIONS
    void throwException() {}
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS

} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP
