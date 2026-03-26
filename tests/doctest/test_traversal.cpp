#include <doctest/doctest.h>

#include "doctest/parts/private/traversal.h"

using doctest::SubcaseSignature;
using doctest::detail::TraversalState;

namespace {

TEST_CASE("TraversalState advances through discovered subcase paths") {
    const SubcaseSignature outer_a = SubcaseSignature{"outer_a", "test_traversal.cpp", 10};
    const SubcaseSignature inner_a = SubcaseSignature{"inner_a", "test_traversal.cpp", 11};
    const SubcaseSignature inner_b = SubcaseSignature{"inner_b", "test_traversal.cpp", 12};
    const SubcaseSignature outer_b = SubcaseSignature{"outer_b", "test_traversal.cpp", 13};

    std::vector<std::vector<std::string>> expected_paths = {
        {"outer_a", "inner_a"},
        {"outer_a", "inner_b"},
        {"outer_b"},
    };

    TraversalState traversal;
    traversal.resetForTestCase();

    std::vector<std::vector<std::string>> visited_paths;

    do { /* NOLINT(cppcoreguidelines-avoid-do-while) */
        traversal.resetForRun();
        std::vector<std::string> visited_path;

        if (traversal.tryEnterSubcase(outer_a)) {
            visited_path.emplace_back(outer_a.m_name.c_str());

            if (traversal.tryEnterSubcase(inner_a)) {
                visited_path.emplace_back(inner_a.m_name.c_str());
                traversal.leaveSubcase();
            }

            if (traversal.tryEnterSubcase(inner_b)) {
                visited_path.emplace_back(inner_b.m_name.c_str());
                traversal.leaveSubcase();
            }

            traversal.leaveSubcase();
        }

        if (traversal.tryEnterSubcase(outer_b)) {
            visited_path.emplace_back(outer_b.m_name.c_str());
            traversal.leaveSubcase();
        }

        visited_paths.push_back(visited_path);
    } while (traversal.advance());

    CHECK(visited_paths == expected_paths);
}

TEST_CASE("TraversalState can unwind active subcases after an abrupt exit") {
    const SubcaseSignature outer = SubcaseSignature{"outer", "test_traversal.cpp", 20};
    const SubcaseSignature inner = SubcaseSignature{"inner", "test_traversal.cpp", 21};

    TraversalState traversal;
    traversal.resetForTestCase();
    traversal.resetForRun();

    REQUIRE(traversal.tryEnterSubcase(outer));
    REQUIRE(traversal.tryEnterSubcase(inner));
    CHECK(traversal.activeSubcaseDepth() == 2);

    CHECK(traversal.unwindActiveSubcases() == 2);
    CHECK(traversal.activeSubcaseDepth() == 0);
}

TEST_CASE("TraversalState advance handles runs with no discovered decision points") {
    const SubcaseSignature first = SubcaseSignature{"first", "test_traversal.cpp", 30};
    const SubcaseSignature second = SubcaseSignature{"second", "test_traversal.cpp", 31};

    TraversalState traversal;
    traversal.resetForTestCase();

    traversal.resetForRun();
    REQUIRE(traversal.tryEnterSubcase(first));
    traversal.leaveSubcase();
    CHECK_FALSE(traversal.tryEnterSubcase(second));

    REQUIRE(traversal.advance());

    // Simulate a rerun that exits before reaching any SUBCASE.
    traversal.resetForRun();
    CHECK_FALSE(traversal.advance());
}

TEST_CASE("TraversalState advance ignores deeper stale decisions when rerun exits early") {
    const SubcaseSignature outer = SubcaseSignature{"outer", "test_traversal.cpp", 40};

    TraversalState traversal;
    traversal.resetForTestCase();

    // First run discovers a deeper generator choice point under the outer subcase.
    traversal.resetForRun();
    REQUIRE(traversal.tryEnterSubcase(outer));
    CHECK(traversal.acquireGeneratorIndex(2) == 0);
    traversal.leaveSubcase();

    // Second run follows the same outer prefix but exits before reaching that generator.
    traversal.resetForRun();
    REQUIRE(traversal.tryEnterSubcase(outer));
    traversal.leaveSubcase();

    // There is no decision point at depth 1 on this replay, so advance() must not
    // advance using stale metadata from the earlier run.
    CHECK_FALSE(traversal.advance());
}

} // namespace
