// =================================================================================================
// == FEATURE DETECTION ============================================================================
// =================================================================================================

// general compiler feature support table: https://en.cppreference.com/w/cpp/compiler_support
// MSVC C++11 feature support table: https://msdn.microsoft.com/en-us/library/hh567368.aspx
// GCC C++11 feature support table: https://gcc.gnu.org/projects/cxx-status.html
// MSVC version table:
// https://en.wikipedia.org/wiki/Microsoft_Visual_C%2B%2B#Internal_version_numbering
// MSVC++ 14.3 (17) _MSC_VER == 1930 (Visual Studio 2022)
// MSVC++ 14.2 (16) _MSC_VER == 1920 (Visual Studio 2019)
// MSVC++ 14.1 (15) _MSC_VER == 1910 (Visual Studio 2017)
// MSVC++ 14.0      _MSC_VER == 1900 (Visual Studio 2015)
// MSVC++ 12.0      _MSC_VER == 1800 (Visual Studio 2013)
// MSVC++ 11.0      _MSC_VER == 1700 (Visual Studio 2012)
// MSVC++ 10.0      _MSC_VER == 1600 (Visual Studio 2010)
// MSVC++ 9.0       _MSC_VER == 1500 (Visual Studio 2008)
// MSVC++ 8.0       _MSC_VER == 1400 (Visual Studio 2005)

// Universal Windows Platform support
#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
#define DOCTEST_CONFIG_NO_WINDOWS_SEH
#endif // WINAPI_FAMILY
#if DOCTEST_MSVC && !defined(DOCTEST_CONFIG_WINDOWS_SEH)
#define DOCTEST_CONFIG_WINDOWS_SEH
#endif // MSVC
#if defined(DOCTEST_CONFIG_NO_WINDOWS_SEH) && defined(DOCTEST_CONFIG_WINDOWS_SEH)
#undef DOCTEST_CONFIG_WINDOWS_SEH
#endif // DOCTEST_CONFIG_NO_WINDOWS_SEH

#if !defined(_WIN32) && !defined(__QNX__) && !defined(DOCTEST_CONFIG_POSIX_SIGNALS) &&             \
        !defined(__EMSCRIPTEN__) && !defined(__wasi__)
#define DOCTEST_CONFIG_POSIX_SIGNALS
#endif // _WIN32
#if defined(DOCTEST_CONFIG_NO_POSIX_SIGNALS) && defined(DOCTEST_CONFIG_POSIX_SIGNALS)
#undef DOCTEST_CONFIG_POSIX_SIGNALS
#endif // DOCTEST_CONFIG_NO_POSIX_SIGNALS

#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
#if !defined(__cpp_exceptions) && !defined(__EXCEPTIONS) && !defined(_CPPUNWIND)                   \
        || defined(__wasi__)
#define DOCTEST_CONFIG_NO_EXCEPTIONS
#endif // no exceptions
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS

#ifdef DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#ifndef DOCTEST_CONFIG_NO_EXCEPTIONS
#define DOCTEST_CONFIG_NO_EXCEPTIONS
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS

#if defined(DOCTEST_CONFIG_NO_EXCEPTIONS) && !defined(DOCTEST_CONFIG_NO_TRY_CATCH_IN_ASSERTS)
#define DOCTEST_CONFIG_NO_TRY_CATCH_IN_ASSERTS
#endif // DOCTEST_CONFIG_NO_EXCEPTIONS && !DOCTEST_CONFIG_NO_TRY_CATCH_IN_ASSERTS

#ifdef __wasi__
#define DOCTEST_CONFIG_NO_MULTITHREADING
#endif

#if defined(DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN) && !defined(DOCTEST_CONFIG_IMPLEMENT)
#define DOCTEST_CONFIG_IMPLEMENT
#endif // DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#if defined(_WIN32) || defined(__CYGWIN__)
#if DOCTEST_MSVC
#define DOCTEST_SYMBOL_EXPORT __declspec(dllexport)
#define DOCTEST_SYMBOL_IMPORT __declspec(dllimport)
#else // MSVC
#define DOCTEST_SYMBOL_EXPORT __attribute__((dllexport))
#define DOCTEST_SYMBOL_IMPORT __attribute__((dllimport))
#endif // MSVC
#else  // _WIN32
#define DOCTEST_SYMBOL_EXPORT __attribute__((visibility("default")))
#define DOCTEST_SYMBOL_IMPORT
#endif // _WIN32

#ifdef DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#ifdef DOCTEST_CONFIG_IMPLEMENT
#define DOCTEST_INTERFACE DOCTEST_SYMBOL_EXPORT
#else // DOCTEST_CONFIG_IMPLEMENT
#define DOCTEST_INTERFACE DOCTEST_SYMBOL_IMPORT
#endif // DOCTEST_CONFIG_IMPLEMENT
#else  // DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#define DOCTEST_INTERFACE
#endif // DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL

// needed for extern template instantiations
// see https://github.com/fmtlib/fmt/issues/2228
#if DOCTEST_MSVC
#define DOCTEST_INTERFACE_DECL
#define DOCTEST_INTERFACE_DEF DOCTEST_INTERFACE
#else // DOCTEST_MSVC
#define DOCTEST_INTERFACE_DECL DOCTEST_INTERFACE
#define DOCTEST_INTERFACE_DEF
#endif // DOCTEST_MSVC

#define DOCTEST_EMPTY

#if DOCTEST_MSVC
#define DOCTEST_NOINLINE __declspec(noinline)
#define DOCTEST_UNUSED
#define DOCTEST_ALIGNMENT(x)
#elif DOCTEST_CLANG && DOCTEST_CLANG < DOCTEST_COMPILER(3, 5, 0)
#define DOCTEST_NOINLINE
#define DOCTEST_UNUSED
#define DOCTEST_ALIGNMENT(x)
#else
#define DOCTEST_NOINLINE __attribute__((noinline))
#define DOCTEST_UNUSED __attribute__((unused))
#define DOCTEST_ALIGNMENT(x) __attribute__((aligned(x)))
#endif

#ifdef DOCTEST_CONFIG_NO_CONTRADICTING_INLINE
#define DOCTEST_INLINE_NOINLINE inline
#else
#define DOCTEST_INLINE_NOINLINE inline DOCTEST_NOINLINE
#endif

#ifndef DOCTEST_NORETURN
#if DOCTEST_MSVC && (DOCTEST_MSVC < DOCTEST_COMPILER(19, 0, 0))
#define DOCTEST_NORETURN
#else // DOCTEST_MSVC
#define DOCTEST_NORETURN [[noreturn]]
#endif // DOCTEST_MSVC
#endif // DOCTEST_NORETURN

#ifndef DOCTEST_NOEXCEPT
#if DOCTEST_MSVC && (DOCTEST_MSVC < DOCTEST_COMPILER(19, 0, 0))
#define DOCTEST_NOEXCEPT
#else // DOCTEST_MSVC
#define DOCTEST_NOEXCEPT noexcept
#endif // DOCTEST_MSVC
#endif // DOCTEST_NOEXCEPT

#ifndef DOCTEST_CONSTEXPR
#if DOCTEST_MSVC && (DOCTEST_MSVC < DOCTEST_COMPILER(19, 0, 0))
#define DOCTEST_CONSTEXPR const
#define DOCTEST_CONSTEXPR_FUNC inline
#else // DOCTEST_MSVC
#define DOCTEST_CONSTEXPR constexpr
#define DOCTEST_CONSTEXPR_FUNC constexpr
#endif // DOCTEST_MSVC
#endif // DOCTEST_CONSTEXPR

#ifndef DOCTEST_NO_SANITIZE_INTEGER
#if DOCTEST_CLANG >= DOCTEST_COMPILER(3, 7, 0)
#define DOCTEST_NO_SANITIZE_INTEGER __attribute__((no_sanitize("integer")))
#else
#define DOCTEST_NO_SANITIZE_INTEGER
#endif
#endif // DOCTEST_NO_SANITIZE_INTEGER

// this is kept here for backwards compatibility since the config option was changed
#ifdef DOCTEST_CONFIG_USE_IOSFWD
#ifndef DOCTEST_CONFIG_USE_STD_HEADERS
#define DOCTEST_CONFIG_USE_STD_HEADERS
#endif
#endif // DOCTEST_CONFIG_USE_IOSFWD

// for clang - always include <version> or <ciso646> (which drags some std stuff)
// because we want to check if we are using libc++ with the _LIBCPP_VERSION macro in
// which case we don't want to forward declare stuff from std - for reference:
// https://github.com/doctest/doctest/issues/126
// https://github.com/doctest/doctest/issues/356
#if DOCTEST_CLANG
#if DOCTEST_CPLUSPLUS >= 201703L && __has_include(<version>)
#include <version>
#else
#include <ciso646>
#endif
#endif // clang

#ifdef _LIBCPP_VERSION
#ifndef DOCTEST_CONFIG_USE_STD_HEADERS
#define DOCTEST_CONFIG_USE_STD_HEADERS
#endif
#endif // _LIBCPP_VERSION

#ifdef DOCTEST_CONFIG_USE_STD_HEADERS
#ifndef DOCTEST_CONFIG_INCLUDE_TYPE_TRAITS
#define DOCTEST_CONFIG_INCLUDE_TYPE_TRAITS
#endif // DOCTEST_CONFIG_INCLUDE_TYPE_TRAITS
#endif // DOCTEST_CONFIG_USE_STD_HEADERS

// =================================================================================================
// == FEATURE DETECTION END ========================================================================
// =================================================================================================
