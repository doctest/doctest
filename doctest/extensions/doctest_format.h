//
// doctest_format.h - an accompanying extensions header to the main doctest.h header
//
// Copyright (c) 2026-2030 Elie Gédéon
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
// The documentation can be found at the library's page:
// https://github.com/doctest/doctest/blob/master/doc/markdown/readme.md
//

#ifndef DOCTEST_PARTS_PUBLIC_FORMAT
#define DOCTEST_PARTS_PUBLIC_FORMAT

#ifndef DOCTEST_LIBRARY_INCLUDED
#include "../doctest.h"
#endif

#if DOCTEST_CPLUSPLUS >= 201703L && __has_include(<version>)
#include <version>
#endif

#if defined(DOCTEST_CONFIG_USE_FMTLIB) && defined(DOCTEST_CONFIG_USE_STDFORMAT)
#error "Choose either DOCTEST_CONFIG_USE_FMTLIB or DOCTEST_CONFIG_USE_STDFORMAT"
#endif

#if defined(__cpp_concepts) && __cpp_concepts >= 201907L
#define DOCTEST_USE_CONCEPTS
#endif
#if DOCTEST_CPLUSPLUS >= 202002L && defined(__cpp_lib_format) && __cpp_lib_format >= 201907L && __has_include(<format>)
#if !defined(DOCTEST_CONFIG_USE_STDFORMAT) && !defined(DOCTEST_CONFIG_USE_FMTLIB)
#define DOCTEST_CONFIG_USE_STDFORMAT
#endif
#elif defined(DOCTEST_CONFIG_USE_STDFORMAT)
#error "No available <format>, please use DOCTEST_CONFIG_USE_FMTLIB instead"
#endif

#if !defined(DOCTEST_CONFIG_USE_STDFORMAT) && !defined(DOCTEST_CONFIG_USE_FMTLIB) && defined(__has_include) &&         \
    __has_include(<fmt/format.h>)
#define DOCTEST_CONFIG_USE_FMTLIB
#endif

#ifdef DOCTEST_CONFIG_USE_STDFORMAT
#ifndef DOCTEST_USE_CONCEPTS
#error "Cannot use DOCTEST_CONFIG_USE_STDFORMAT without support for concepts"
#endif
#include <utility>
#include <format>
#elif defined(DOCTEST_CONFIG_USE_FMTLIB)
DOCTEST_SUPPRESS_PUBLIC_WARNINGS_PUSH
#include <fmt/format.h>
DOCTEST_SUPPRESS_PUBLIC_WARNINGS_POP
#else
#error "Use DOCTEST_CONFIG_USE_FMTLIB or DOCTEST_CONFIG_USE_STDFORMAT"
#endif

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_PUSH
namespace doctest {
namespace detail {
namespace meta {
#ifdef DOCTEST_CONFIG_USE_FMTLIB
template <typename T>
struct is_formattable {
    static constexpr bool value = fmt::is_formattable<T>::value && std::is_constructible<fmt::formatter<T>>::value;
};
#ifdef DOCTEST_USE_CONCEPTS
template <typename T>
concept formattable = is_formattable<T>::value;
#endif
#elif defined(__cpp_lib_format) && __cpp_lib_format >= 202207L && DOCTEST_CPLUSPLUS >= 202302L
template <typename T>
concept formattable = std::formattable<T, char>;

template <typename T>
struct is_formattable {
    static constexpr bool value = formattable<T>;
};
#else
struct fake_format_context_iterator {
    fake_format_context_iterator operator++(int);
    fake_format_context_iterator &operator++();
    char &operator*();
    int operator-(fake_format_context_iterator them) const;
};

using fake_format_context = std::basic_format_context<fake_format_context_iterator, char>;

template <typename T>
concept formattable = requires(
    T t,
    fake_format_context::formatter_type<T> &f,
    const fake_format_context::formatter_type<T> &cf,
    std::format_parse_context &pc,
    fake_format_context &fc
) {
    {f.parse(pc)}->std::same_as<std::format_parse_context::iterator>;
    {cf.format(t, fc)}->std::same_as<fake_format_context::iterator>;
};

template <typename T>
struct is_formattable {
    static constexpr bool value = formattable<T>;
};
#endif
#ifdef DOCTEST_CONFIG_USE_FMTLIB
#define DOCTEST_FORMAT_PARSE_CONTEXT fmt::format_parse_context
#define DOCTEST_FORMAT_FORMATTER fmt::formatter
#define DOCTEST_FORMAT_TO fmt::format_to
#define DOCTEST_FORMAT fmt::format
#define DOCTEST_FORMAT_EMPTY_STRING FMT_STRING("")
#else
#define DOCTEST_FORMAT_PARSE_CONTEXT std::format_parse_context
#define DOCTEST_FORMAT_FORMATTER std::formatter
#define DOCTEST_FORMAT_TO std::format_to
#define DOCTEST_FORMAT std::format
#define DOCTEST_FORMAT_EMPTY_STRING ""
#endif

#ifdef DOCTEST_USE_CONCEPTS
template <formattable T>
constexpr bool check_format_string() {
    DOCTEST_FORMAT_PARSE_CONTEXT pc{"}"};
    DOCTEST_FORMAT_FORMATTER<typename types::remove_const<typename types::remove_reference<T>::type>::type> formatter;
    if (formatter.parse(pc) != pc.begin())
        std::abort();
    return true;
}

template <bool v>
struct check_valid {};

template <typename T>
concept default_formattable = requires {
    typename check_valid<check_format_string<T>()>;
}
&&!use_default_string_maker<T>::value;

template <typename T>
struct is_default_formattable {
    static constexpr bool value = default_formattable<T>;
};
#else

template <typename T>
struct is_default_formattable {
    static constexpr bool value = is_formattable<T>::value && !use_default_string_maker<T>::value;
};
#endif
} // namespace meta
} // namespace detail

#ifdef DOCTEST_USE_CONCEPTS
template <detail::meta::default_formattable T>
struct StringMaker<T> {
    static doctest::String convert(const T &t) {
        return DOCTEST_FORMAT("{}", t);
    }
    using UsingFmt = int;
};
#else
template <typename T>
struct StringMaker<T, typename detail::types::enable_if<detail::meta::is_default_formattable<T>::value>::type> {
    static doctest::String convert(const T &t) {
        return DOCTEST_FORMAT("{}", t);
    }
    using UsingFmt = int;
};
#endif
} // namespace doctest
DOCTEST_SUPPRESS_PUBLIC_WARNINGS_POP
#endif
