#include "doctest/parts/private/prelude.h"

#ifndef DOCTEST_CONFIG_OPTIONS_FILE_PREFIX_SEPARATOR
#define DOCTEST_CONFIG_OPTIONS_FILE_PREFIX_SEPARATOR ':'
#endif

#include "doctest/parts/private/exceptions.h"
#include "doctest/parts/private/timer.h"
#include "doctest/parts/private/atomic.h"
#include "doctest/parts/private/context_state.h"
#include "doctest/parts/private/test_case.h"
#include "doctest/parts/private/exception_translator.h"
#include "doctest/parts/private/context_scope.h"
#include "doctest/parts/private/reporter.h"

namespace doctest {

DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wnull-dereference")
DOCTEST_GCC_SUPPRESS_WARNING_WITH_PUSH("-Wnull-dereference")
// depending on the current options this will remove the path of filenames
const char* skipPathFromFilename(const char* file) {
#ifndef DOCTEST_CONFIG_DISABLE
    if(getContextOptions()->no_path_in_filenames) {
        auto back    = std::strrchr(file, '\\');
        auto forward = std::strrchr(file, '/');
        if(back || forward) {
            if(back > forward)
                forward = back;
            return forward + 1;
        }
    } else {
        const auto prefixes = getContextOptions()->strip_file_prefixes;
        const char separator = DOCTEST_CONFIG_OPTIONS_FILE_PREFIX_SEPARATOR;
        String::size_type longest_match = 0U;
        for(String::size_type pos = 0U; pos < prefixes.size(); ++pos)
        {
            const auto prefix_start = pos;
            pos = std::min(prefixes.find(separator, prefix_start), prefixes.size());

            const auto prefix_size = pos - prefix_start;
            if(prefix_size > longest_match)
            {
                // TODO under DOCTEST_MSVC: does the comparison need strnicmp() to work with drive letter capitalization?
                if(0 == std::strncmp(prefixes.c_str() + prefix_start, file, prefix_size))
                {
                    longest_match = prefix_size;
                }
            }
        }
        return &file[longest_match];
    }
#endif // DOCTEST_CONFIG_DISABLE
    return file;
}
DOCTEST_CLANG_SUPPRESS_WARNING_POP
DOCTEST_GCC_SUPPRESS_WARNING_POP

} // namespace doctest

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace {
    using namespace detail;

    DOCTEST_NO_SANITIZE_INTEGER
    unsigned long long hash(unsigned long long a, unsigned long long b) {
        return (a << 5) + b;
    }

    // C string hash function (djb2) - taken from http://www.cse.yorku.ca/~oz/hash.html
    DOCTEST_NO_SANITIZE_INTEGER
    unsigned long long hash(const char* str) {
        unsigned long long hash = 5381;
        char c;
        while ((c = *str++))
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        return hash;
    }

    unsigned long long hash(const SubcaseSignature& sig) {
        return hash(hash(hash(sig.m_file), hash(sig.m_name.c_str())), sig.m_line);
    }

    unsigned long long hash(const std::vector<SubcaseSignature>& sigs, size_t count) {
        unsigned long long running = 0;
        auto end = sigs.begin() + count;
        for (auto it = sigs.begin(); it != end; it++) {
            running = hash(running, hash(*it));
        }
        return running;
    }

    unsigned long long hash(const std::vector<SubcaseSignature>& sigs) {
        unsigned long long running = 0;
        for (const SubcaseSignature& sig : sigs) {
            running = hash(running, hash(sig));
        }
        return running;
    }
} // namespace


namespace detail {

    void ResultBuilder::translateException() {
        m_threw     = true;
        m_exception = translateActiveException();
    }

    bool ResultBuilder::log() {
        if(m_at & assertType::is_throws) { //!OCLINT bitwise operator in conditional
            m_failed = !m_threw;
        } else if((m_at & assertType::is_throws_as) && (m_at & assertType::is_throws_with)) { //!OCLINT
            m_failed = !m_threw_as || !m_exception_string.check(m_exception);
        } else if(m_at & assertType::is_throws_as) { //!OCLINT bitwise operator in conditional
            m_failed = !m_threw_as;
        } else if(m_at & assertType::is_throws_with) { //!OCLINT bitwise operator in conditional
            m_failed = !m_exception_string.check(m_exception);
        } else if(m_at & assertType::is_nothrow) { //!OCLINT bitwise operator in conditional
            m_failed = m_threw;
        }

        if(m_exception.size())
            m_exception = "\"" + m_exception + "\"";

        if(is_running_in_test) {
            addAssert(m_at);
            DOCTEST_ITERATE_THROUGH_REPORTERS(log_assert, *this);

            if(m_failed)
                addFailedAssert(m_at);
        } else if(m_failed) {
            failed_out_of_a_testing_context(*this);
        }

        return m_failed && isDebuggerActive() && !getContextOptions()->no_breaks &&
            (g_cs->currentTest == nullptr || !g_cs->currentTest->m_no_breaks); // break into debugger
    }

} // namespace detail
} // namespace doctest

#include "doctest/parts/private/xml.h"
#include "doctest/parts/private/reporters/xml.h"
#include "doctest/parts/private/reporters/junit.h"
#include "doctest/parts/private/reporters/console.h"
#include "doctest/parts/private/reporters/debug_output_window.h"

#endif // DOCTEST_CONFIG_DISABLE

#ifdef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4007) // 'function' : must be 'attribute' - see issue #182
int main(int argc, char** argv) { return doctest::Context(argc, argv).run(); }
DOCTEST_MSVC_SUPPRESS_WARNING_POP
#endif // DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
