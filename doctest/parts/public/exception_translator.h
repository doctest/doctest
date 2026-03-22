#ifndef DOCTEST_PARTS_PUBLIC_EXCEPTION_TRANSLATOR
#define DOCTEST_PARTS_PUBLIC_EXCEPTION_TRANSLATOR

#include "doctest/parts/public/string.h"

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_PUSH

namespace doctest {
namespace detail {

#ifndef DOCTEST_CONFIG_DISABLE

struct DOCTEST_INTERFACE IExceptionTranslator {
    DOCTEST_DECLARE_INTERFACE(IExceptionTranslator)
    virtual bool translate(String &) const = 0;
};

template <typename T>
class ExceptionTranslator : public IExceptionTranslator {
public:
    explicit ExceptionTranslator(String (*translateFunction)(T)) noexcept
        : m_translateFunction(translateFunction) {}

    bool translate(String &res) const override {
#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
        try {
            throw;
        } catch (const T &ex) {
            res = m_translateFunction(ex);
            return true;
        } catch (...) {}        // NOLINT(bugprone-empty-catch)
#endif                          // DOCTEST_CONFIG_NO_EXCEPTIONS
        static_cast<void>(res); // to silence -Wunused-parameter
        return false;
    }

private:
    String (*m_translateFunction)(T);
};

DOCTEST_INTERFACE void registerExceptionTranslatorImpl(const IExceptionTranslator *et) noexcept;

#endif // DOCTEST_CONFIG_DISABLE

} // namespace detail

#ifndef DOCTEST_CONFIG_DISABLE

template <typename T>
int registerExceptionTranslator(String (*translateFunction)(T)) noexcept {
    DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wexit-time-destructors")
    static detail::ExceptionTranslator<T> exceptionTranslator(translateFunction);
    DOCTEST_CLANG_SUPPRESS_WARNING_POP
    detail::registerExceptionTranslatorImpl(&exceptionTranslator);
    return 0;
}

#else // DOCTEST_CONFIG_DISABLE

template <typename T>
int registerExceptionTranslator(String (*)(T)) noexcept {
    return 0;
}

#endif // DOCTEST_CONFIG_DISABLE

} // namespace doctest

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_POP

#endif // DOCTEST_PARTS_PUBLIC_EXCEPTION_TRANSLATOR
