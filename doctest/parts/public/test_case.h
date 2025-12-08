namespace doctest {

    struct DOCTEST_INTERFACE TestCaseData
    {
        String      m_file;       // the file in which the test was registered (using String - see #350)
        unsigned    m_line;       // the line where the test was registered
        const char* m_name;       // name of the test case
        const char* m_test_suite; // the test suite in which the test was added
        const char* m_description;
        bool        m_skip;
        bool        m_no_breaks;
        bool        m_no_output;
        bool        m_may_fail;
        bool        m_should_fail;
        int         m_expected_failures;
        double      m_timeout;
    };

#ifndef DOCTEST_CONFIG_DISABLE
namespace detail {

    using funcType = void (*)();

    struct DOCTEST_INTERFACE TestCase : public TestCaseData
    {
        funcType m_test; // a function pointer to the test case

        String m_type; // for templated test cases - gets appended to the real name
        int m_template_id; // an ID used to distinguish between the different versions of a templated test case
        String m_full_name; // contains the name (only for templated test cases!) + the template type

        TestCase(funcType test, const char* file, unsigned line, const TestSuite& test_suite,
                  const String& type = String(), int template_id = -1);

        TestCase(const TestCase& other);
        TestCase(TestCase&&) = delete;

        DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(26434) // hides a non-virtual function
        TestCase& operator=(const TestCase& other);
        DOCTEST_MSVC_SUPPRESS_WARNING_POP

        TestCase& operator=(TestCase&&) = delete;

        TestCase& operator*(const char* in);

        template <typename T>
        TestCase& operator*(const T& in) {
            in.fill(*this);
            return *this;
        }

        bool operator<(const TestCase& other) const;

        ~TestCase() = default;
    };

    // forward declarations of functions used by the macros
    DOCTEST_INTERFACE int regTest(const TestCase& tc);

} // namespace detail
#endif // DOCTEST_CONFIG_DISABLE

} // namespace doctest
