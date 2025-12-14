namespace doctest {
namespace detail {

#ifndef DOCTEST_CONFIG_DISABLE

    struct DOCTEST_INTERFACE IExceptionTranslator
    {
        DOCTEST_DECLARE_INTERFACE(IExceptionTranslator)
        virtual bool translate(String&) const = 0;
    };

    template <typename T>
    class ExceptionTranslator : public IExceptionTranslator //!OCLINT destructor of virtual class
    {
    public:
        explicit ExceptionTranslator(String (*translateFunction)(T))
                : m_translateFunction(translateFunction) {}

        bool translate(String& res) const override {
#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
            try {
                throw; // lgtm [cpp/rethrow-no-exception]
                // cppcheck-suppress catchExceptionByValue
            } catch(const T& ex) {
                res = m_translateFunction(ex); //!OCLINT parameter reassignment
                return true;
            } catch(...) {}         //!OCLINT -  empty catch statement
#endif                              // DOCTEST_CONFIG_NO_EXCEPTIONS
            static_cast<void>(res); // to silence -Wunused-parameter
            return false;
        }

    private:
        String (*m_translateFunction)(T);
    };

    DOCTEST_INTERFACE void registerExceptionTranslatorImpl(const IExceptionTranslator* et);

#endif // DOCTEST_CONFIG_DISABLE

} // namespace detail

#ifndef DOCTEST_CONFIG_DISABLE

template <typename T>
int registerExceptionTranslator(String (*translateFunction)(T)) {
    DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wexit-time-destructors")
    static detail::ExceptionTranslator<T> exceptionTranslator(translateFunction);
    DOCTEST_CLANG_SUPPRESS_WARNING_POP
    detail::registerExceptionTranslatorImpl(&exceptionTranslator);
    return 0;
}

#else // DOCTEST_CONFIG_DISABLE

template <typename T>
int registerExceptionTranslator(String (*)(T)) {
    return 0;
}

#endif // DOCTEST_CONFIG_DISABLE

} // namespace doctest
