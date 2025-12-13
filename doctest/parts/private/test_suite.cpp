#include "doctest/parts/private/prelude.h"

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

TestSuite& TestSuite::operator*(const char* in) {
    m_test_suite = in;
    return *this;
}

// sets the current test suite
int setTestSuite(const TestSuite& ts) {
    doctest_detail_test_suite_ns::getCurrentTestSuite() = ts;
    return 0;
}

} // namespace detail
} // namespace doctest

namespace doctest_detail_test_suite_ns {
// holds the current test suite
doctest::detail::TestSuite& getCurrentTestSuite() {
    static doctest::detail::TestSuite data{};
    return data;
}
} // namespace doctest_detail_test_suite_ns

#endif // DOCTEST_CONFIG_DISABLE
