#include "doctest/parts/private/prelude.h"

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

    // all the registered tests
    std::set<TestCase>& getRegisteredTests() {
        static std::set<TestCase> data;
        return data;
    }

} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE
