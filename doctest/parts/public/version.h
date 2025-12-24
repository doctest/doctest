// =================================================================================================
// == VERSION ======================================================================================
// =================================================================================================

#ifndef DOCTEST_PARTS_PUBLIC_VERSION
#define DOCTEST_PARTS_PUBLIC_VERSION

#define DOCTEST_VERSION_MAJOR 2
#define DOCTEST_VERSION_MINOR 4
#define DOCTEST_VERSION_PATCH 12

// util we need here
#define DOCTEST_TOSTR_IMPL(x) #x
#define DOCTEST_TOSTR(x) DOCTEST_TOSTR_IMPL(x)

#define DOCTEST_VERSION_STR                                                                        \
    DOCTEST_TOSTR(DOCTEST_VERSION_MAJOR) "."                                                       \
    DOCTEST_TOSTR(DOCTEST_VERSION_MINOR) "."                                                       \
    DOCTEST_TOSTR(DOCTEST_VERSION_PATCH)

#define DOCTEST_VERSION                                                                            \
    (DOCTEST_VERSION_MAJOR * 10000 + DOCTEST_VERSION_MINOR * 100 + DOCTEST_VERSION_PATCH)

#endif // DOCTEST_PARTS_PUBLIC_VERSION
