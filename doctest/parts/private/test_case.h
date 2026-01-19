#ifndef DOCTEST_PARTS_PRIVATE_TEST_CASE
#define DOCTEST_PARTS_PRIVATE_TEST_CASE

#include "doctest/parts/private/prelude.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

    // all the registered tests
    std::set<TestCase>& getRegisteredTests();

} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP

#endif // DOCTEST_PARTS_PRIVATE_TEST_CASE
