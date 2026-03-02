#ifndef DOCTEST_PARTS_PRIVATE_TRAVERSAL
#define DOCTEST_PARTS_PRIVATE_TRAVERSAL

#include "doctest/parts/private/prelude.h"
#include "doctest/parts/public/subcase.h"

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

#ifndef DOCTEST_CONFIG_DISABLE

namespace doctest {
namespace detail {

struct DecisionPoint {
    // Encountered sibling subcases in source order for this traversal depth.
    std::vector<SubcaseSignature> subcases;
};

class TraversalState {
public:
    size_t activeSubcaseDepth() const {
        return m_activeSubcaseDepth;
    }

    void resetForTestCase();
    void resetForRun();
    bool advance();
    bool tryEnterSubcase(const SubcaseSignature &signature);
    void leaveSubcase();
    size_t unwindActiveSubcases();

private:
    // decisionPath is the selected traversal prefix; discoveredDecisionPath is rebuilt
    // on each rerun to describe the branches encountered at each depth.
    std::vector<DecisionPoint> m_discoveredDecisionPath;
    std::vector<size_t> m_decisionPath;
    size_t m_decisionDepth = 0;
    std::vector<size_t> m_enteredSubcaseDepths;
    size_t m_activeSubcaseDepth = 0;

    DecisionPoint &ensureDecisionPointAtCurrentDepth();
};

} // namespace detail
} // namespace doctest

#endif // DOCTEST_CONFIG_DISABLE

DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP

#endif // DOCTEST_PARTS_PRIVATE_TRAVERSAL
