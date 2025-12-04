// =================================================================================================
// == COMPILER WARNINGS HELPERS ====================================================================
// =================================================================================================

#if DOCTEST_CLANG && !DOCTEST_ICC
#define DOCTEST_PRAGMA_TO_STR(x) _Pragma(#x)
#define DOCTEST_CLANG_SUPPRESS_WARNING_PUSH _Pragma("clang diagnostic push")
#define DOCTEST_CLANG_SUPPRESS_WARNING(w) DOCTEST_PRAGMA_TO_STR(clang diagnostic ignored w)
#define DOCTEST_CLANG_SUPPRESS_WARNING_POP _Pragma("clang diagnostic pop")
#define DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH(w)                                                \
    DOCTEST_CLANG_SUPPRESS_WARNING_PUSH DOCTEST_CLANG_SUPPRESS_WARNING(w)
#else // DOCTEST_CLANG
#define DOCTEST_CLANG_SUPPRESS_WARNING_PUSH
#define DOCTEST_CLANG_SUPPRESS_WARNING(w)
#define DOCTEST_CLANG_SUPPRESS_WARNING_POP
#define DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH(w)
#endif // DOCTEST_CLANG

#if DOCTEST_GCC
#define DOCTEST_PRAGMA_TO_STR(x) _Pragma(#x)
#define DOCTEST_GCC_SUPPRESS_WARNING_PUSH _Pragma("GCC diagnostic push")
#define DOCTEST_GCC_SUPPRESS_WARNING(w) DOCTEST_PRAGMA_TO_STR(GCC diagnostic ignored w)
#define DOCTEST_GCC_SUPPRESS_WARNING_POP _Pragma("GCC diagnostic pop")
#define DOCTEST_GCC_SUPPRESS_WARNING_WITH_PUSH(w)                                                  \
    DOCTEST_GCC_SUPPRESS_WARNING_PUSH DOCTEST_GCC_SUPPRESS_WARNING(w)
#else // DOCTEST_GCC
#define DOCTEST_GCC_SUPPRESS_WARNING_PUSH
#define DOCTEST_GCC_SUPPRESS_WARNING(w)
#define DOCTEST_GCC_SUPPRESS_WARNING_POP
#define DOCTEST_GCC_SUPPRESS_WARNING_WITH_PUSH(w)
#endif // DOCTEST_GCC

#if DOCTEST_MSVC
#define DOCTEST_MSVC_SUPPRESS_WARNING_PUSH __pragma(warning(push))
#define DOCTEST_MSVC_SUPPRESS_WARNING(w) __pragma(warning(disable : w))
#define DOCTEST_MSVC_SUPPRESS_WARNING_POP __pragma(warning(pop))
#define DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(w)                                                 \
    DOCTEST_MSVC_SUPPRESS_WARNING_PUSH DOCTEST_MSVC_SUPPRESS_WARNING(w)
#else // DOCTEST_MSVC
#define DOCTEST_MSVC_SUPPRESS_WARNING_PUSH
#define DOCTEST_MSVC_SUPPRESS_WARNING(w)
#define DOCTEST_MSVC_SUPPRESS_WARNING_POP
#define DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(w)
#endif // DOCTEST_MSVC

// =================================================================================================
// == COMPILER WARNINGS ============================================================================
// =================================================================================================

// both the header and the implementation suppress all of these,
// so it only makes sense to aggregate them like so
#define DOCTEST_SUPPRESS_COMMON_WARNINGS_PUSH                                                      \
    DOCTEST_CLANG_SUPPRESS_WARNING_PUSH                                                            \
    DOCTEST_CLANG_SUPPRESS_WARNING("-Wunknown-pragmas")                                            \
    DOCTEST_CLANG_SUPPRESS_WARNING("-Wunknown-warning-option")                                     \
    DOCTEST_CLANG_SUPPRESS_WARNING("-Wweak-vtables")                                               \
    DOCTEST_CLANG_SUPPRESS_WARNING("-Wpadded")                                                     \
    DOCTEST_CLANG_SUPPRESS_WARNING("-Wmissing-prototypes")                                         \
    DOCTEST_CLANG_SUPPRESS_WARNING("-Wc++98-compat")                                               \
    DOCTEST_CLANG_SUPPRESS_WARNING("-Wc++98-compat-pedantic")                                      \
    DOCTEST_CLANG_SUPPRESS_WARNING("-Wunsafe-buffer-usage")                                        \
                                                                                                   \
    DOCTEST_GCC_SUPPRESS_WARNING_PUSH                                                              \
    DOCTEST_GCC_SUPPRESS_WARNING("-Wunknown-pragmas")                                              \
    DOCTEST_GCC_SUPPRESS_WARNING("-Wpragmas")                                                      \
    DOCTEST_GCC_SUPPRESS_WARNING("-Weffc++")                                                       \
    DOCTEST_GCC_SUPPRESS_WARNING("-Wstrict-overflow")                                              \
    DOCTEST_GCC_SUPPRESS_WARNING("-Wstrict-aliasing")                                              \
    DOCTEST_GCC_SUPPRESS_WARNING("-Wmissing-declarations")                                         \
    DOCTEST_GCC_SUPPRESS_WARNING("-Wuseless-cast")                                                 \
    DOCTEST_GCC_SUPPRESS_WARNING("-Wnoexcept")                                                     \
                                                                                                   \
    DOCTEST_MSVC_SUPPRESS_WARNING_PUSH                                                             \
    /* these 4 also disabled globally via cmake: */                                                \
    DOCTEST_MSVC_SUPPRESS_WARNING(4514) /* unreferenced inline function has been removed */        \
    DOCTEST_MSVC_SUPPRESS_WARNING(4571) /* SEH related */                                          \
    DOCTEST_MSVC_SUPPRESS_WARNING(4710) /* function not inlined */                                 \
    DOCTEST_MSVC_SUPPRESS_WARNING(4711) /* function selected for inline expansion*/                \
    /* common ones */                                                                              \
    DOCTEST_MSVC_SUPPRESS_WARNING(4616) /* invalid compiler warning */                             \
    DOCTEST_MSVC_SUPPRESS_WARNING(4619) /* invalid compiler warning */                             \
    DOCTEST_MSVC_SUPPRESS_WARNING(4996) /* The compiler encountered a deprecated declaration */    \
    DOCTEST_MSVC_SUPPRESS_WARNING(4706) /* assignment within conditional expression */             \
    DOCTEST_MSVC_SUPPRESS_WARNING(4512) /* 'class' : assignment operator could not be generated */ \
    DOCTEST_MSVC_SUPPRESS_WARNING(4127) /* conditional expression is constant */                   \
    DOCTEST_MSVC_SUPPRESS_WARNING(4820) /* padding */                                              \
    DOCTEST_MSVC_SUPPRESS_WARNING(4625) /* copy constructor was implicitly deleted */              \
    DOCTEST_MSVC_SUPPRESS_WARNING(4626) /* assignment operator was implicitly deleted */           \
    DOCTEST_MSVC_SUPPRESS_WARNING(5027) /* move assignment operator implicitly deleted */          \
    DOCTEST_MSVC_SUPPRESS_WARNING(5026) /* move constructor was implicitly deleted */              \
    DOCTEST_MSVC_SUPPRESS_WARNING(4640) /* construction of local static object not thread-safe */  \
    DOCTEST_MSVC_SUPPRESS_WARNING(5045) /* Spectre mitigation for memory load */                   \
    DOCTEST_MSVC_SUPPRESS_WARNING(5264) /* 'variable-name': 'const' variable is not used */        \
    /* static analysis */                                                                          \
    DOCTEST_MSVC_SUPPRESS_WARNING(26439) /* Function may not throw. Declare it 'noexcept' */       \
    DOCTEST_MSVC_SUPPRESS_WARNING(26495) /* Always initialize a member variable */                 \
    DOCTEST_MSVC_SUPPRESS_WARNING(26451) /* Arithmetic overflow ... */                             \
    DOCTEST_MSVC_SUPPRESS_WARNING(26444) /* Avoid unnamed objects with custom ctor and dtor... */  \
    DOCTEST_MSVC_SUPPRESS_WARNING(26812) /* Prefer 'enum class' over 'enum' */

#define DOCTEST_SUPPRESS_COMMON_WARNINGS_POP                                                       \
    DOCTEST_CLANG_SUPPRESS_WARNING_POP                                                             \
    DOCTEST_GCC_SUPPRESS_WARNING_POP                                                               \
    DOCTEST_MSVC_SUPPRESS_WARNING_POP

#define DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN                                 \
    DOCTEST_MSVC_SUPPRESS_WARNING_PUSH                                                             \
    DOCTEST_MSVC_SUPPRESS_WARNING(4548) /* before comma no effect; expected side - effect */       \
    DOCTEST_MSVC_SUPPRESS_WARNING(4265) /* virtual functions, but destructor is not virtual */     \
    DOCTEST_MSVC_SUPPRESS_WARNING(4986) /* exception specification does not match previous */      \
    DOCTEST_MSVC_SUPPRESS_WARNING(4350) /* 'member1' called instead of 'member2' */                \
    DOCTEST_MSVC_SUPPRESS_WARNING(4668) /* not defined as a preprocessor macro */                  \
    DOCTEST_MSVC_SUPPRESS_WARNING(4365) /* signed/unsigned mismatch */                             \
    DOCTEST_MSVC_SUPPRESS_WARNING(4774) /* format string not a string literal */                   \
    DOCTEST_MSVC_SUPPRESS_WARNING(4820) /* padding */                                              \
    DOCTEST_MSVC_SUPPRESS_WARNING(4625) /* copy constructor was implicitly deleted */              \
    DOCTEST_MSVC_SUPPRESS_WARNING(4626) /* assignment operator was implicitly deleted */           \
    DOCTEST_MSVC_SUPPRESS_WARNING(5027) /* move assignment operator implicitly deleted */          \
    DOCTEST_MSVC_SUPPRESS_WARNING(5026) /* move constructor was implicitly deleted */              \
    DOCTEST_MSVC_SUPPRESS_WARNING(4623) /* default constructor was implicitly deleted */           \
    DOCTEST_MSVC_SUPPRESS_WARNING(5039) /* pointer to pot. throwing function passed to extern C */ \
    DOCTEST_MSVC_SUPPRESS_WARNING(5045) /* Spectre mitigation for memory load */                   \
    DOCTEST_MSVC_SUPPRESS_WARNING(5105) /* macro producing 'defined' has undefined behavior */     \
    DOCTEST_MSVC_SUPPRESS_WARNING(4738) /* storing float result in memory, loss of performance */  \
    DOCTEST_MSVC_SUPPRESS_WARNING(5262) /* implicit fall-through */

#define DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END DOCTEST_MSVC_SUPPRESS_WARNING_POP
