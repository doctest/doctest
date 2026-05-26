#ifndef DOCTEST_PARTS_PUBLIC_UTILITY
#define DOCTEST_PARTS_PUBLIC_UTILITY

#include "doctest/parts/public/config.h"

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_PUSH

#define DOCTEST_DECLARE_INTERFACE(name)                                                                                \
    virtual ~name();                                                                                                   \
    name() = default;                                                                                                  \
    name(const name &) = delete;                                                                                       \
    name(name &&) = delete;                                                                                            \
    name &operator=(const name &) = delete;                                                                            \
    name &operator=(name &&) = delete;

#define DOCTEST_DEFINE_INTERFACE(name) name::~name() = default;

#if !defined(DOCTEST_COUNTER)
#if DOCTEST_CLANG >= DOCTEST_COMPILER(22, 0, 0)
#define DOCTEST_COUNTER __LINE__
#elif defined(__COUNTER__)
#define DOCTEST_COUNTER __COUNTER__
#else
#define DOCTEST_COUNTER __LINE__
#endif
#endif // defined(DOCTEST_COUNTER)

// internal macros for string concatenation and anonymous variable name generation
#define DOCTEST_CAT_IMPL(s1, s2) s1##s2
#define DOCTEST_CAT(s1, s2) DOCTEST_CAT_IMPL(s1, s2)
#define DOCTEST_ANONYMOUS(x) DOCTEST_CAT(x, DOCTEST_COUNTER)

#ifndef DOCTEST_CONFIG_ASSERTION_PARAMETERS_BY_VALUE
#define DOCTEST_REF_WRAP(x) x &
#else // DOCTEST_CONFIG_ASSERTION_PARAMETERS_BY_VALUE
#define DOCTEST_REF_WRAP(x) x
#endif // DOCTEST_CONFIG_ASSERTION_PARAMETERS_BY_VALUE

namespace doctest {
namespace detail {
DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wunused-function")
static DOCTEST_CONSTEXPR int consume(const int *, int) noexcept {
    return 0;
}
DOCTEST_CLANG_SUPPRESS_WARNING_POP
} // namespace detail
} // namespace doctest

#define DOCTEST_GLOBAL_NO_WARNINGS(var, ...)                                                                           \
    DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wglobal-constructors")                                                  \
    static const int var = doctest::detail::consume(&var, __VA_ARGS__);                                                \
    DOCTEST_CLANG_SUPPRESS_WARNING_POP

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_POP

#endif // DOCTEST_PARTS_PUBLIC_UTILITY
