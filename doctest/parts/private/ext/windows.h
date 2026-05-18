#ifndef DOCTEST_PARTS_PRIVATE_EXT_WINDOWS
#define DOCTEST_PARTS_PRIVATE_EXT_WINDOWS

#include "doctest/parts/public/config.h"

// TODO: This wrapper should really be applied at the call site, but there's a bug in our Codecov
//  setup causing conditional guards around includes to conflict with `--remap`.
#ifdef DOCTEST_PLATFORM_WINDOWS

// Internal windows headers require explicit supressions to handle `-weverything`.
DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wnonportable-system-include-path")
DOCTEST_MSVC_SUPPRESS_WARNING_PUSH
DOCTEST_MSVC_SUPPRESS_WARNING(4265) // virtual functions, but destructor is not virtual
DOCTEST_MSVC_SUPPRESS_WARNING(4350) // 'member1' called instead of 'member2'
DOCTEST_MSVC_SUPPRESS_WARNING(4548) // before comma no effect; expected side - effect
DOCTEST_MSVC_SUPPRESS_WARNING(4623) // default constructor was implicitly deleted
DOCTEST_MSVC_SUPPRESS_WARNING(4625) // copy constructor was implicitly deleted
DOCTEST_MSVC_SUPPRESS_WARNING(4626) // assignment operator was implicitly deleted
DOCTEST_MSVC_SUPPRESS_WARNING(4668) // not defined as a preprocessor macro
DOCTEST_MSVC_SUPPRESS_WARNING(4774) // format string not a string literal
DOCTEST_MSVC_SUPPRESS_WARNING(4820) // padding
DOCTEST_MSVC_SUPPRESS_WARNING(4865) // the underlying type will change if '/Zc:enumTypes' is passed
DOCTEST_MSVC_SUPPRESS_WARNING(4986) // exception specification does not match previous
DOCTEST_MSVC_SUPPRESS_WARNING(5026) // move constructor was implicitly deleted
DOCTEST_MSVC_SUPPRESS_WARNING(5027) // move assignment operator implicitly deleted
DOCTEST_MSVC_SUPPRESS_WARNING(5039) // pointer to pot. throwing function passed to extern C
DOCTEST_MSVC_SUPPRESS_WARNING(5045) // Spectre mitigation for memory load
DOCTEST_MSVC_SUPPRESS_WARNING(5105) // macro producing 'defined' has undefined behavior
DOCTEST_MSVC_SUPPRESS_WARNING(5262) // implicit fall-through

// defines for a leaner windows.h
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define DOCTEST_UNDEF_WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#define DOCTEST_UNDEF_NOMINMAX
#endif

// not sure what AfxWin.h is for - here I do what Catch does
#ifdef __AFXDLL
#include <AfxWin.h> // IWYU pragma: export
#else
#include <windows.h> // IWYU pragma: export
#endif

#ifdef DOCTEST_UNDEF_WIN32_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#undef DOCTEST_UNDEF_WIN32_LEAN_AND_MEAN
#endif
#ifdef DOCTEST_UNDEF_NOMINMAX
#undef NOMINMAX
#undef DOCTEST_UNDEF_NOMINMAX
#endif

DOCTEST_CLANG_SUPPRESS_WARNING_POP
DOCTEST_MSVC_SUPPRESS_WARNING_POP

#endif // DOCTEST_PLATFORM_WINDOWS

#endif // DOCTEST_PARTS_PRIVATE_EXT_WINDOWS
