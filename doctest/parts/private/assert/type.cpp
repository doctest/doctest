#include "doctest/parts/private/prelude.h"
#include "doctest/parts/private/exceptions.h"

namespace doctest {

// clang-format off
const char* assertString(assertType::Enum at) {
    DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4061) // enum 'x' in switch of enum 'y' is not explicitly handled
    #define DOCTEST_GENERATE_ASSERT_TYPE_CASE(assert_type) case assertType::DT_ ## assert_type: return #assert_type
    #define DOCTEST_GENERATE_ASSERT_TYPE_CASES(assert_type) \
        DOCTEST_GENERATE_ASSERT_TYPE_CASE(WARN_ ## assert_type); \
        DOCTEST_GENERATE_ASSERT_TYPE_CASE(CHECK_ ## assert_type); \
        DOCTEST_GENERATE_ASSERT_TYPE_CASE(REQUIRE_ ## assert_type)
    DOCTEST_CLANG_SUPPRESS_WARNING_PUSH
    DOCTEST_CLANG_SUPPRESS_WARNING("-Wswitch-enum")
    DOCTEST_GCC_SUPPRESS_WARNING_PUSH
    DOCTEST_GCC_SUPPRESS_WARNING("-Wswitch-enum")
    switch(at) {
        DOCTEST_GENERATE_ASSERT_TYPE_CASE(WARN);
        DOCTEST_GENERATE_ASSERT_TYPE_CASE(CHECK);
        DOCTEST_GENERATE_ASSERT_TYPE_CASE(REQUIRE);

        DOCTEST_GENERATE_ASSERT_TYPE_CASES(FALSE);

        DOCTEST_GENERATE_ASSERT_TYPE_CASES(THROWS);

        DOCTEST_GENERATE_ASSERT_TYPE_CASES(THROWS_AS);

        DOCTEST_GENERATE_ASSERT_TYPE_CASES(THROWS_WITH);

        DOCTEST_GENERATE_ASSERT_TYPE_CASES(THROWS_WITH_AS);

        DOCTEST_GENERATE_ASSERT_TYPE_CASES(NOTHROW);

        DOCTEST_GENERATE_ASSERT_TYPE_CASES(EQ);
        DOCTEST_GENERATE_ASSERT_TYPE_CASES(NE);
        DOCTEST_GENERATE_ASSERT_TYPE_CASES(GT);
        DOCTEST_GENERATE_ASSERT_TYPE_CASES(LT);
        DOCTEST_GENERATE_ASSERT_TYPE_CASES(GE);
        DOCTEST_GENERATE_ASSERT_TYPE_CASES(LE);

        DOCTEST_GENERATE_ASSERT_TYPE_CASES(UNARY);
        DOCTEST_GENERATE_ASSERT_TYPE_CASES(UNARY_FALSE);

        default: DOCTEST_INTERNAL_ERROR("Tried stringifying invalid assert type!");
    }
    DOCTEST_CLANG_SUPPRESS_WARNING_POP
    DOCTEST_GCC_SUPPRESS_WARNING_POP
    DOCTEST_MSVC_SUPPRESS_WARNING_POP
}
// clang-format on

const char* failureString(assertType::Enum at) {
    if(at & assertType::is_warn) //!OCLINT bitwise operator in conditional
        return "WARNING";
    if(at & assertType::is_check) //!OCLINT bitwise operator in conditional
        return "ERROR";
    if(at & assertType::is_require) //!OCLINT bitwise operator in conditional
        return "FATAL ERROR";
    return "";
}

} // namespace doctest
