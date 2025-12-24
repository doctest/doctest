#ifndef DOCTEST_PARTS_PUBLIC_STD_FWD
#define DOCTEST_PARTS_PUBLIC_STD_FWD

#include "doctest/parts/public/config.h"
#include "doctest/parts/public/warnings.h"

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_PUSH

#ifdef DOCTEST_CONFIG_USE_STD_HEADERS
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <cstddef>
#include <ostream>
#include <istream>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END
#else // DOCTEST_CONFIG_USE_STD_HEADERS

// Forward declaring 'X' in namespace std is not permitted by the C++ Standard.
DOCTEST_MSVC_SUPPRESS_WARNING_WITH_PUSH(4643)

namespace std { // NOLINT(cert-dcl58-cpp)
typedef decltype(nullptr) nullptr_t; // NOLINT(modernize-use-using)
typedef decltype(sizeof(void*)) size_t; // NOLINT(modernize-use-using)
template <class charT>
struct char_traits;
template <>
struct char_traits<char>;
template <class charT, class traits>
class basic_ostream; // NOLINT(fuchsia-virtual-inheritance)
typedef basic_ostream<char, char_traits<char>> ostream; // NOLINT(modernize-use-using)
template<class traits>
// NOLINTNEXTLINE
basic_ostream<char, traits>& operator<<(basic_ostream<char, traits>&, const char*);
template <class charT, class traits>
class basic_istream;
typedef basic_istream<char, char_traits<char>> istream; // NOLINT(modernize-use-using)
template <class... Types>
class tuple;
#if DOCTEST_MSVC >= DOCTEST_COMPILER(19, 20, 0)
// see this issue on why this is needed: https://github.com/doctest/doctest/issues/183
template <class Ty>
class allocator;
template <class Elem, class Traits, class Alloc>
class basic_string;
using string = basic_string<char, char_traits<char>, allocator<char>>;
#endif // VS 2019
} // namespace std

DOCTEST_MSVC_SUPPRESS_WARNING_POP

#endif // DOCTEST_CONFIG_USE_STD_HEADERS

namespace doctest {
  using std::size_t;
}

DOCTEST_SUPPRESS_PUBLIC_WARNINGS_POP

#endif // DOCTEST_PARTS_PUBLIC_STD_FWD
