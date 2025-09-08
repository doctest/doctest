#define DOCTEST_DECLARE_INTERFACE(name)                                                            \
    virtual ~name();                                                                               \
    name() = default;                                                                              \
    name(const name&) = delete;                                                                    \
    name(name&&) = delete;                                                                         \
    name& operator=(const name&) = delete;                                                         \
    name& operator=(name&&) = delete;

#define DOCTEST_DEFINE_INTERFACE(name)                                                             \
    name::~name() = default;

// internal macros for string concatenation and anonymous variable name generation
#define DOCTEST_CAT_IMPL(s1, s2) s1##s2
#define DOCTEST_CAT(s1, s2) DOCTEST_CAT_IMPL(s1, s2)
#ifdef __COUNTER__ // not standard and may be missing for some compilers
#define DOCTEST_ANONYMOUS(x) DOCTEST_CAT(x, __COUNTER__)
#else // __COUNTER__
#define DOCTEST_ANONYMOUS(x) DOCTEST_CAT(x, __LINE__)
#endif // __COUNTER__

#ifndef DOCTEST_CONFIG_ASSERTION_PARAMETERS_BY_VALUE
#define DOCTEST_REF_WRAP(x) x&
#else // DOCTEST_CONFIG_ASSERTION_PARAMETERS_BY_VALUE
#define DOCTEST_REF_WRAP(x) x
#endif // DOCTEST_CONFIG_ASSERTION_PARAMETERS_BY_VALUE

namespace doctest { namespace detail {
    static DOCTEST_CONSTEXPR int consume(const int*, int) noexcept { return 0; }
}}

#define DOCTEST_GLOBAL_NO_WARNINGS(var, ...)                                                         \
    DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wglobal-constructors")                                \
    static const int var = doctest::detail::consume(&var, __VA_ARGS__);                              \
    DOCTEST_CLANG_SUPPRESS_WARNING_POP
