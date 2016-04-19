#define DOCTEST_DISABLE

#define DOCTEST_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cstdio>
#include <exception>

testcase("name1") {
    printf("Anyone there?\n");
}

testsuite(the testsuite!);

void throws(); // to silence GCC "-Wmissing-declarations"
void throws() { throw std::exception(); }
void nothrows(); // to silence GCC "-Wmissing-declarations"
void nothrows() {}

testcase(ops) {
    printf("Anyone there?\n");

    check(1 == 1);
    require(1 == 1);

    check_false(0);
    require_false(0);

    check_throws(throws());
    require_throws(throws());

    check_throws_as(throws(), std::exception);
    require_throws_as(throws(), std::exception);

    check_nothrow(nothrows());
    require_nothrow(nothrows());
    
    subcase("") {}
}

testsuite_end;

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Weffc++"
#endif

struct Empty
{};

testcase_fixture(Empty, "name") {
    printf("Help?\n");
}

testcase_fixture(Empty, ops) {
    printf("Help?\n");
}
