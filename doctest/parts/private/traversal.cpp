#include "doctest/parts/private/traversal.h"

#include "doctest/parts/private/context_state.h"

#include <algorithm>

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

DOCTEST_NOINLINE DecisionPoint &TraversalState::ensureDecisionPointAtCurrentDepth() {
    const size_t depth = m_decisionDepth;

    if (m_discoveredDecisionPath.size() == depth) {
        m_discoveredDecisionPath.emplace_back();

        if (m_decisionPath.size() == depth)
            m_decisionPath.push_back(0);
    }

    return m_discoveredDecisionPath[depth];
}

void TraversalState::resetForTestCase() {
    m_decisionPath.clear();
    m_discoveredDecisionPath.clear();
    m_enteredSubcaseDepths.clear();
    m_activeSubcaseDepth = 0;
    m_decisionDepth = 0;
}

void TraversalState::resetForRun() {
    m_activeSubcaseDepth = 0;
    m_discoveredDecisionPath.clear();
    m_decisionDepth = 0;
    m_enteredSubcaseDepths.clear();
}

bool TraversalState::advance() {
    const size_t maxDepth = std::min(m_decisionPath.size(), m_discoveredDecisionPath.size());
    for (size_t depth = maxDepth; depth > 0; --depth) {
        const size_t index = depth - 1;
        if (m_decisionPath[index] + 1 < m_discoveredDecisionPath[index].branch_count) {
            ++m_decisionPath[index];
            m_decisionPath.resize(index + 1);
            return true;
        }
    }

    return false;
}

bool TraversalState::tryEnterSubcase(const SubcaseSignature &signature) {
    DecisionPoint &point = ensureDecisionPointAtCurrentDepth();
    std::vector<SubcaseSignature> &subcases = point.subcases;
    size_t siblingIndex = 0;

    for (; siblingIndex < subcases.size(); ++siblingIndex) {
        if (subcases[siblingIndex] == signature)
            break;
    }

    if (siblingIndex == subcases.size())
        subcases.push_back(signature);

    point.branch_count = subcases.size();

    if (siblingIndex != m_decisionPath[m_decisionDepth])
        return false;

    m_enteredSubcaseDepths.push_back(m_decisionDepth);
    m_activeSubcaseDepth++;
    m_decisionDepth++;
    return true;
}

void TraversalState::leaveSubcase() {
    m_decisionDepth = m_enteredSubcaseDepths.back();
    m_enteredSubcaseDepths.pop_back();
    m_activeSubcaseDepth--;
}

size_t TraversalState::unwindActiveSubcases() {
    const size_t activeSubcaseCount = m_activeSubcaseDepth;

    while (m_activeSubcaseDepth > 0)
        leaveSubcase();

    return activeSubcaseCount;
}

size_t TraversalState::acquireGeneratorIndex(size_t count) {
    DecisionPoint &point = ensureDecisionPointAtCurrentDepth();
    point.branch_count = count;

    const size_t index = m_decisionPath[m_decisionDepth];
    m_decisionDepth++;
    return index < count ? index : 0;
}

size_t acquireGeneratorDecisionIndex(size_t count) {
    return g_cs->traversal.acquireGeneratorIndex(count);
}
} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP
