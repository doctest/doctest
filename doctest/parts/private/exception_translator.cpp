#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/exception_translator.h"

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

    DOCTEST_DEFINE_INTERFACE(IExceptionTranslator)

    void registerExceptionTranslatorImpl(const IExceptionTranslator* et) {
        if(std::find(getExceptionTranslators().begin(), getExceptionTranslators().end(), et) ==
           getExceptionTranslators().end())
            getExceptionTranslators().push_back(et);
    }

    std::vector<const IExceptionTranslator*>& getExceptionTranslators() {
        static std::vector<const IExceptionTranslator*> data;
        return data;
    }

    String translateActiveException() {
#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
        String res;
        auto&  translators = getExceptionTranslators();
        for(auto& curr : translators)
            if(curr->translate(res))
                return res;
        // clang-format off
        DOCTEST_GCC_SUPPRESS_WARNING_WITH_PUSH("-Wcatch-value")
        try {
            throw;
        } catch(std::exception& ex) {
            return ex.what();
        } catch(std::string& msg) {
            return msg.c_str();
        } catch(const char* msg) {
            return msg;
        } catch(...) {
            return "unknown exception";
        }
        DOCTEST_GCC_SUPPRESS_WARNING_POP
// clang-format on
#else  // DOCTEST_CONFIG_NO_EXCEPTIONS
        return "";
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS
    }

} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE
