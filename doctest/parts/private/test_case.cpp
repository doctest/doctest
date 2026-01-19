#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/test_case.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

std::set<TestCase>& getRegisteredTests() {
    static std::set<TestCase> data;
    return data;
}

TestCase::TestCase(funcType test, const char* file, unsigned line, const TestSuite& test_suite,
                    const String& type, int template_id) {
    m_file              = file;
    m_line              = line;
    m_name              = nullptr; // will be later overridden in operator*
    m_test_suite        = test_suite.m_test_suite;
    m_description       = test_suite.m_description;
    m_skip              = test_suite.m_skip;
    m_no_breaks         = test_suite.m_no_breaks;
    m_no_output         = test_suite.m_no_output;
    m_may_fail          = test_suite.m_may_fail;
    m_should_fail       = test_suite.m_should_fail;
    m_expected_failures = test_suite.m_expected_failures;
    m_timeout           = test_suite.m_timeout;

    m_test        = test;
    m_type        = type;
    m_template_id = template_id;
}

TestCase::TestCase(const TestCase& other)
        : TestCaseData() {
    *this = other;
}

DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(26434) // hides a non-virtual function
TestCase& TestCase::operator=(const TestCase& other) {
    TestCaseData::operator=(other);
    m_test        = other.m_test;
    m_type        = other.m_type;
    m_template_id = other.m_template_id;
    m_full_name   = other.m_full_name;

    if(m_template_id != -1)
        m_name = m_full_name.c_str();
    return *this;
}
DOCTEST_MSVC_SUPPRESS_WARNING_POP

TestCase& TestCase::operator*(const char* in) {
    m_name = in;
    // make a new name with an appended type for templated test case
    if(m_template_id != -1) {
        m_full_name = String(m_name) + "<" + m_type + ">";
        // redirect the name to point to the newly constructed full name
        m_name = m_full_name.c_str();
    }
    return *this;
}

bool TestCase::operator<(const TestCase& other) const {
    // this will be used only to differentiate between test cases - not relevant for sorting
    if(m_line != other.m_line)
        return m_line < other.m_line;
    const int name_cmp = strcmp(m_name, other.m_name);
    if(name_cmp != 0)
        return name_cmp < 0;
    const int file_cmp = m_file.compare(other.m_file);
    if(file_cmp != 0)
        return file_cmp < 0;
    return m_template_id < other.m_template_id;
}

// used by the macros for registering tests
int regTest(const TestCase& tc) {
    getRegisteredTests().insert(tc);
    return 0;
}

} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP
