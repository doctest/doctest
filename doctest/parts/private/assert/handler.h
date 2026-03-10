#ifndef DOCTEST_PARTS_PRIVATE_ASSERT_HANDLER
#define DOCTEST_PARTS_PRIVATE_ASSERT_HANDLER

#include "doctest/parts/private/prelude.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

void addAssert(assertType::Enum at);

void addFailedAssert(assertType::Enum at);

} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP

#endif // DOCTEST_PARTS_PRIVATE_ASSERT_HANDLER
