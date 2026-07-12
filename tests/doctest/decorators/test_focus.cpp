#include <doctest/doctest.h>

namespace {

/** A phony test case detached from the current test context */
const doctest::detail::TestCase phony_test_case{
    []() { /* empty */ }, __FILE__, __LINE__, doctest_detail_test_suite_ns::getCurrentTestSuite()
};

} // namespace

TEST_CASE("Test cases are not marked as foucsed by default") {
    auto tc = phony_test_case;
    CHECK(tc.m_focus == false);
}

TEST_CASE("Test cases can become focused by applying doctest::focus()") {
    auto tc = phony_test_case;
    static_cast<void>(tc * doctest::focus());
    CHECK(tc.m_focus == true);
}
