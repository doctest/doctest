#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/context_state.h"

namespace doctest {

    bool SubcaseSignature::operator==(const SubcaseSignature& other) const {
        return m_line == other.m_line
            && std::strcmp(m_file, other.m_file) == 0
            && m_name == other.m_name;
    }

    bool SubcaseSignature::operator<(const SubcaseSignature& other) const {
        if(m_line != other.m_line)
            return m_line < other.m_line;
        if(std::strcmp(m_file, other.m_file) != 0)
            return std::strcmp(m_file, other.m_file) < 0;
        return m_name.compare(other.m_name) < 0;
    }

#ifndef DOCTEST_CONFIG_DISABLE
namespace detail {

    bool Subcase::checkFilters() {
        if (g_cs->subcaseStack.size() < size_t(g_cs->subcase_filter_levels)) {
            if (!matchesAny(m_signature.m_name.c_str(), g_cs->filters[6], true, g_cs->case_sensitive))
                return true;
            if (matchesAny(m_signature.m_name.c_str(), g_cs->filters[7], false, g_cs->case_sensitive))
                return true;
        }
        return false;
    }

    Subcase::Subcase(const String& name, const char* file, int line)
            : m_signature({name, file, line}) {
        if (!g_cs->reachedLeaf) {
            if (g_cs->nextSubcaseStack.size() <= g_cs->subcaseStack.size()
                || g_cs->nextSubcaseStack[g_cs->subcaseStack.size()] == m_signature) {
                // Going down.
                if (checkFilters()) { return; }

                g_cs->subcaseStack.push_back(m_signature);
                g_cs->currentSubcaseDepth++;
                m_entered = true;
                DOCTEST_ITERATE_THROUGH_REPORTERS(subcase_start, m_signature);
            }
        } else {
            if (g_cs->subcaseStack[g_cs->currentSubcaseDepth] == m_signature) {
                // This subcase is reentered via control flow.
                g_cs->currentSubcaseDepth++;
                m_entered = true;
                DOCTEST_ITERATE_THROUGH_REPORTERS(subcase_start, m_signature);
            } else if (g_cs->nextSubcaseStack.size() <= g_cs->currentSubcaseDepth
                    && g_cs->fullyTraversedSubcases.find(hash(hash(g_cs->subcaseStack, g_cs->currentSubcaseDepth), hash(m_signature)))
                    == g_cs->fullyTraversedSubcases.end()) {
                if (checkFilters()) { return; }
                // This subcase is part of the one to be executed next.
                g_cs->nextSubcaseStack.clear();
                g_cs->nextSubcaseStack.insert(g_cs->nextSubcaseStack.end(),
                    g_cs->subcaseStack.begin(), g_cs->subcaseStack.begin() + g_cs->currentSubcaseDepth);
                g_cs->nextSubcaseStack.push_back(m_signature);
            }
        }
    }

    DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4996) // std::uncaught_exception is deprecated in C++17
    DOCTEST_GCC_SUPPRESS_WARNING_WITH_PUSH("-Wdeprecated-declarations")
    DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wdeprecated-declarations")

    Subcase::~Subcase() {
        if (m_entered) {
            g_cs->currentSubcaseDepth--;

            if (!g_cs->reachedLeaf) {
                // Leaf.
                g_cs->fullyTraversedSubcases.insert(hash(g_cs->subcaseStack));
                g_cs->nextSubcaseStack.clear();
                g_cs->reachedLeaf = true;
            } else if (g_cs->nextSubcaseStack.empty()) {
                // All children are finished.
                g_cs->fullyTraversedSubcases.insert(hash(g_cs->subcaseStack));
            }

    #if defined(__cpp_lib_uncaught_exceptions) && __cpp_lib_uncaught_exceptions >= 201411L && (!defined(__MAC_OS_X_VERSION_MIN_REQUIRED) || __MAC_OS_X_VERSION_MIN_REQUIRED >= 101200)
            if(std::uncaught_exceptions() > 0
    #else
            if(std::uncaught_exception()
    #endif
                && g_cs->shouldLogCurrentException) {
                DOCTEST_ITERATE_THROUGH_REPORTERS(
                        test_case_exception, {"exception thrown in subcase - will translate later "
                                                "when the whole test case has been exited (cannot "
                                                "translate while there is an active exception)",
                                                false});
                g_cs->shouldLogCurrentException = false;
            }

            DOCTEST_ITERATE_THROUGH_REPORTERS(subcase_end, DOCTEST_EMPTY);
        }
    }

    DOCTEST_CLANG_SUPPRESS_WARNING_POP
    DOCTEST_GCC_SUPPRESS_WARNING_POP
    DOCTEST_MSVC_SUPPRESS_WARNING_POP

    Subcase::operator bool() const { return m_entered; }

} // namespace detail
#endif // DOCTEST_CONFIG_DISABLE

} // namespace doctest
