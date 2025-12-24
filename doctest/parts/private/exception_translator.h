#ifndef DOCTEST_PARTS_PRIVATE_EXCEPTION_TRANSLATOR
#define DOCTEST_PARTS_PRIVATE_EXCEPTION_TRANSLATOR

#include "doctest/parts/private/prelude.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

    std::vector<const IExceptionTranslator*>& getExceptionTranslators();
    String translateActiveException();

} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP

#endif // DOCTEST_PARTS_PRIVATE_EXCEPTION_TRANSLATOR
