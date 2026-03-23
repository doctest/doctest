#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/context_state.h"
#include "doctest/parts/private/filters.h"
#include "doctest/parts/private/reporter.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

namespace doctest {

bool SubcaseSignature::operator==(const SubcaseSignature &other) const {
    return m_line == other.m_line && std::strcmp(m_file, other.m_file) == 0 && m_name == other.m_name;
}

bool SubcaseSignature::operator<(const SubcaseSignature &other) const {
    if (m_line != other.m_line)
        return m_line < other.m_line;
    if (std::strcmp(m_file, other.m_file) != 0)
        return std::strcmp(m_file, other.m_file) < 0;
    return m_name.compare(other.m_name) < 0;
}

#ifndef DOCTEST_CONFIG_DISABLE
namespace detail {

bool Subcase::checkFilters() {
    if (g_cs->traversal.activeSubcaseDepth() < static_cast<size_t>(g_cs->subcase_filter_levels)) {
        if (!matchesAny(m_signature.m_name.c_str(), g_cs->filters[6], true, g_cs->case_sensitive))
            return true;
        if (matchesAny(m_signature.m_name.c_str(), g_cs->filters[7], false, g_cs->case_sensitive))
            return true;
    }
    return false;
}

Subcase::Subcase(const String &name, const char *file, int line)
    : m_signature({name, file, line}) {
    if (checkFilters())
        return;

    if (!g_cs->traversal.tryEnterSubcase(m_signature))
        return;

    m_entered = true;
    DOCTEST_ITERATE_THROUGH_REPORTERS(subcase_start, m_signature);
}

DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4996) // std::uncaught_exception is deprecated in C++17
DOCTEST_GCC_SUPPRESS_WARNING_WITH_PUSH("-Wdeprecated-declarations")
DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wdeprecated-declarations")

Subcase::~Subcase() {
    if (m_entered) {
        g_cs->traversal.leaveSubcase();

#if defined(__cpp_lib_uncaught_exceptions) && __cpp_lib_uncaught_exceptions >= 201411L &&                              \
    (!defined(__MAC_OS_X_VERSION_MIN_REQUIRED) || __MAC_OS_X_VERSION_MIN_REQUIRED >= 101200)
        if (std::uncaught_exceptions() > 0
#else
        if (std::uncaught_exception()
#endif
            && g_cs->shouldLogCurrentException) {
            DOCTEST_ITERATE_THROUGH_REPORTERS(
                test_case_exception,
                {"exception thrown in subcase - will translate later "
                 "when the whole test case has been exited (cannot "
                 "translate while there is an active exception)",
                 false}
            );
            g_cs->shouldLogCurrentException = false;
        }

        DOCTEST_ITERATE_THROUGH_REPORTERS(subcase_end, DOCTEST_EMPTY);
    }
}

DOCTEST_CLANG_SUPPRESS_WARNING_POP
DOCTEST_GCC_SUPPRESS_WARNING_POP
DOCTEST_MSVC_SUPPRESS_WARNING_POP

Subcase::operator bool() const {
    return m_entered;
}

} // namespace detail
#endif // DOCTEST_CONFIG_DISABLE

} // namespace doctest

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP
