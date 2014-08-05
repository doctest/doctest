//#define DOCTEST_CONFIG_DISABLE

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <cstdio>
#include <exception>

#define DECOMPOSE(expr) (expression_decomposer() << expr)

#define EXPECT(expr)                                                                               \
    do {                                                                                           \
        if(result failed = DECOMPOSE(expr))                                                        \
            printf("failed!\n");                                                                   \
    } while(false)

struct result
{
    const bool passed;
    const doctest::String decomposition;

    result(bool passed, const doctest::String& decomposition)
        : passed(passed), decomposition(decomposition) {}
    operator bool() { return !passed; }
};

//inline std::string to_string(std::string    const & text) { return "\"" + text + "\""; }
inline doctest::String to_string(const char * text) { return doctest::String("\"") + text + "\""; }
//inline doctest::String to_string(char text) { return doctest::String("\'") + text + "\'"; }

inline std::ostream & operator<<(std::ostream & os, approx const & appr)
{
    return os << appr.magnitude();
}

template <typename T>
std::string to_string(T const & value, int = 0 /* VC6 */)
{
    std::ostringstream os; os << std::boolalpha << value; return os.str();
}

template<typename T1, typename T2>
std::string to_string(std::pair<T1, T2> const & pair)
{
    std::ostringstream oss;
    oss << "{ " << to_string(pair.first) << ", " << to_string(pair.second) << " }";
    return oss.str();
}

template <typename L, typename R>
std::string to_string(L const & lhs, std::string op, R const & rhs)
{
    std::ostringstream os; os << to_string(lhs) << " " << op << " " << to_string(rhs); return os.str();
}


template <typename L>
struct expression_lhs
{
    const L lhs;

    expression_lhs(L lhs)
            : lhs(lhs) {}

    operator result() { return result(!!lhs, to_string(lhs)); }

    // clang-format off
    template <typename R> result operator==(R const & rhs) { return result(lhs == rhs, to_string(lhs, "==", rhs)); }
    template <typename R> result operator!=(R const & rhs) { return result(lhs != rhs, to_string(lhs, "!=", rhs)); }
    template <typename R> result operator< (R const & rhs) { return result(lhs <  rhs, to_string(lhs, "<", rhs)); }
    template <typename R> result operator<=(R const & rhs) { return result(lhs <= rhs, to_string(lhs, "<=", rhs)); }
    template <typename R> result operator> (R const & rhs) { return result(lhs >  rhs, to_string(lhs, ">", rhs)); }
    template <typename R> result operator>=(R const & rhs) { return result(lhs >= rhs, to_string(lhs, ">=", rhs)); }
    // clang-format on
};

struct expression_decomposer
{
    template <typename L>
    expression_lhs<L const&> operator<<(L const& operand) {
        return expression_lhs<L const &>(operand);
    }
};


testsuite(MAIN);
test(zzz) {
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
