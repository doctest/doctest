//#define DOCTEST_CONFIG_DISABLE

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <cstdio>
#include <exception>

#include <string>
namespace doctest
{
namespace detail
{
    template <>
    String stringify(const std::string& in) {
        return in.c_str();
    }
} // namespace detail
} // namespace doctest

testsuite(MAIN);
test(zzz) {
    check(true == false);
    check(std::string("OMG2") == std::string("OMG"));

    printf("main\n");
    subtest("") {
        printf("1\n");
        subtest("") { printf("1-1\n"); }
        subtest("") { printf("1-2\n"); }
    }
    subtest("") { printf("2\n"); }
}
testsuite_end;

struct Empty
{
    virtual ~Empty() {}
};

doctest_fixture(Empty, trololo) { printf("Help?\n"); }

// test(thrower)
//{
//    if(rand() > 4) {
//        throw std::exception();
//    } else {
//        cout << "trololo" << endl;
//    }
//}

static int testWrapper(void (*f)(void)) {
    try {
        f();
    } catch(std::exception& e) {
        printf("caught the bugger! %s\n", e.what());
        return 1;
    }
    return 0;
}

int main(int argc, char** argv) {
    // initialize
    doctest::Context context(argc, argv);
    context.setTestExecutionWrapper(testWrapper);

    // overrides
    context.setOption("dt-case-sensitive", true);
    context.addFilter("dt-name", "zzz");

    // run
    int res = context.runTests();

#if defined(WITH_PAUSE)
    system("pause");
#endif // _MSC_VER

    return res;
}

// test("") { printf("TEST %d\n", __LINE__); }
// test("") { printf("TEST %d\n", __LINE__); }
// test("") { printf("TEST %d\n", __LINE__); }
// test("") { printf("TEST %d\n", __LINE__); }
// test("") { printf("TEST %d\n", __LINE__); }
// test("") { printf("TEST %d\n", __LINE__); }
// test("") { printf("TEST %d\n", __LINE__); }
// test("") { printf("TEST %d\n", __LINE__); }
// test("") { printf("TEST %d\n", __LINE__); }
// test("") { printf("TEST %d\n", __LINE__); }
// test("") { printf("TEST %d\n", __LINE__); }
