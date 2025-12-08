namespace doctest {

    struct DOCTEST_INTERFACE TestCaseData;

    struct DOCTEST_INTERFACE AssertData
    {
        // common - for all asserts
        const TestCaseData* m_test_case;
        assertType::Enum    m_at;
        const char*         m_file;
        int                 m_line;
        const char*         m_expr;
        bool                m_failed;

        // exception-related - for all asserts
        bool   m_threw;
        String m_exception;

        // for normal asserts
        String m_decomp;

        // for specific exception-related asserts
        bool           m_threw_as;
        const char*    m_exception_type;

        class DOCTEST_INTERFACE StringContains {
            private:
                Contains content;
                bool isContains;

            public:
                StringContains(const String& str) : content(str), isContains(false) { }
                StringContains(Contains cntn) : content(static_cast<Contains&&>(cntn)), isContains(true) { }

                bool check(const String& str) { return isContains ? (content == str) : (content.string == str); }

                operator const String&() const { return content.string; }

                const char* c_str() const { return content.string.c_str(); }
        } m_exception_string;

        AssertData(assertType::Enum at, const char* file, int line, const char* expr,
            const char* exception_type, const StringContains& exception_string);
    };

} // namespace doctest
