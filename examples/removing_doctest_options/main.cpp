#define DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 6)
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif // > gcc 4.6

#include <string>
#include <vector>

int program(int argc, const char** argv);

class dt_removed
{
public:
    dt_removed(const char** argv_in)
            : vec(0) {
        for(; *argv_in; ++argv_in)
            if(!starts_with(*argv_in, "--dt-"))
                vec.push_back(*argv_in);
        vec.push_back(NULL);
    }

// to fix gcc 4.7 "-Winline" warnings
#if defined(__GNUC__) && !defined(__clang__)
    __attribute__((noinline))
#endif
    ~dt_removed() {}
    
    int          argc() { return static_cast<int>(vec.size()) - 1; }
    const char** argv() { return &vec[0]; } // Note: non-const char **:

private:
    static bool starts_with(std::string text, std::string with) {
        return with == text.substr(0, with.length());
    }

    std::vector<const char*> vec;
};

int main(int argc, const char** argv) {
    doctest::Context context(argc, argv);
    int              test_result = context.run(); // run queries, or run tests unless --no-run

    if(context.shouldExit()) // honor query flags and --exit
        return test_result;

    dt_removed args(argv);
    int        app_result = program(args.argc(), args.argv());

    return test_result + app_result;
}

TEST_CASE("Fail") { REQUIRE(0); }
TEST_CASE("Pass") { REQUIRE(1); }

int program(int argc, const char** argv) {
    printf("Program: %d arguments received:\n", argc - 1);
    while(*++argv)
        printf("'%s'\n", *argv);
    return EXIT_SUCCESS;
}