#ifndef DOCTEST_PARTS_PUBLIC_TEST_SUITE
#define DOCTEST_PARTS_PUBLIC_TEST_SUITE

#include "doctest/parts/public/config.h"
#include "doctest/parts/public/warnings.h"

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_PUSH

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

struct DOCTEST_INTERFACE TestSuite
{
    const char* m_test_suite = nullptr;
    const char* m_description = nullptr;
    bool        m_skip = false;
    bool        m_no_breaks = false;
    bool        m_no_output = false;
    bool        m_may_fail = false;
    bool        m_should_fail = false;
    int         m_expected_failures = 0;
    double      m_timeout = 0;

    TestSuite& operator*(const char* in);

    template <typename T>
    TestSuite& operator*(const T& in) {
        in.fill(*this);
        return *this;
    }
};

// forward declarations of functions used by the macros
DOCTEST_INTERFACE int setTestSuite(const TestSuite& ts);

} // namespace detail

} // namespace doctest

// in a separate namespace outside of doctest because the DOCTEST_TEST_SUITE macro
// introduces an anonymous namespace in which getCurrentTestSuite gets overridden
namespace doctest_detail_test_suite_ns {
DOCTEST_INTERFACE doctest::detail::TestSuite& getCurrentTestSuite();

// this is here to clear the 'current test suite' for the current translation unit - at the top
DOCTEST_GLOBAL_NO_WARNINGS( /* NOLINT(cert-err58-cpp) */
    DOCTEST_ANONYMOUS(DOCTEST_ANON_VAR_),
    doctest::detail::setTestSuite(doctest::detail::TestSuite() * "")
)

} // namespace doctest_detail_test_suite_ns

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_POP

#endif // DOCTEST_PARTS_PUBLIC_TEST_SUITE
