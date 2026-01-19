#ifndef DOCTEST_PARTS_PRIVATE_EXCEPTIONS
#define DOCTEST_PARTS_PRIVATE_EXCEPTIONS

#include "doctest/parts/private/prelude.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

namespace doctest {
namespace detail {

    template <typename Ex>
    DOCTEST_NORETURN void throw_exception(Ex const& e) {
#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
        throw e;
#else  // DOCTEST_CONFIG_NO_EXCEPTIONS
#ifdef DOCTEST_CONFIG_HANDLE_EXCEPTION
        DOCTEST_CONFIG_HANDLE_EXCEPTION(e);
#else // DOCTEST_CONFIG_HANDLE_EXCEPTION
#ifndef DOCTEST_CONFIG_NO_INCLUDE_IOSTREAM
        std::cerr << "doctest will terminate because it needed to throw an exception.\n"
                  << "The message was: " << e.what() << '\n';
#endif // DOCTEST_CONFIG_NO_INCLUDE_IOSTREAM
#endif // DOCTEST_CONFIG_HANDLE_EXCEPTION
        std::terminate();
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS
    }

#ifndef DOCTEST_INTERNAL_ERROR
#define DOCTEST_INTERNAL_ERROR(msg)                                                                \
    detail::throw_exception(std::logic_error(                                                              \
            __FILE__ ":" DOCTEST_TOSTR(__LINE__) ": Internal doctest error: " msg))
#endif // DOCTEST_INTERNAL_ERROR
} // namespace detail

} // namespace doctest

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP

#endif // DOCTEST_PARTS_PRIVATE_EXCEPTIONS
