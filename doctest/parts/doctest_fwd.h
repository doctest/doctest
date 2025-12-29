//
// doctest.h - the lightest feature-rich C++ single-header testing framework for unit tests and TDD
//
// Copyright (c) 2016-2023 Viktor Kirilov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
// The documentation can be found at the library's page:
// https://github.com/doctest/doctest/blob/master/doc/markdown/readme.md
//
// =================================================================================================
// =================================================================================================
// =================================================================================================
//
// The library is heavily influenced by Catch - https://github.com/catchorg/Catch2
// which uses the Boost Software License - Version 1.0
// see here - https://github.com/catchorg/Catch2/blob/master/LICENSE.txt
//
// The concept of subcases (sections in Catch) and expression decomposition are from there.
// Some parts of the code are taken directly:
// - stringification - the detection of "ostream& operator<<(ostream&, const T&)" and StringMaker<>
// - the Approx() helper class for floating point comparison
// - colors in the console
// - breaking into a debugger
// - signal / SEH handling
// - timer
// - XmlWriter class - thanks to Phil Nash for allowing the direct reuse (AKA copy/paste)
//
// The expression decomposing templates are taken from lest - https://github.com/martinmoene/lest
// which uses the Boost Software License - Version 1.0
// see here - https://github.com/martinmoene/lest/blob/master/LICENSE.txt
//
// =================================================================================================
// =================================================================================================
// =================================================================================================

#ifndef DOCTEST_LIBRARY_INCLUDED
#define DOCTEST_LIBRARY_INCLUDED

#include <doctest/parts/public/version.h>
#include <doctest/parts/public/compiler.h>
#include <doctest/parts/public/warnings.h>

DOCTEST_SUPPRESS_COMMON_WARNINGS_PUSH

DOCTEST_CLANG_SUPPRESS_WARNING_PUSH
DOCTEST_CLANG_SUPPRESS_WARNING("-Wnon-virtual-dtor")
DOCTEST_CLANG_SUPPRESS_WARNING("-Wdeprecated")

DOCTEST_GCC_SUPPRESS_WARNING_PUSH
DOCTEST_GCC_SUPPRESS_WARNING("-Wctor-dtor-privacy")
DOCTEST_GCC_SUPPRESS_WARNING("-Wnon-virtual-dtor")
DOCTEST_GCC_SUPPRESS_WARNING("-Wsign-promo")

DOCTEST_MSVC_SUPPRESS_WARNING_PUSH
DOCTEST_MSVC_SUPPRESS_WARNING(4623) // default constructor was implicitly defined as deleted

#include <doctest/parts/public/config.h>
#include <doctest/parts/public/utility.h>
#include <doctest/parts/public/platform.h>
#include <doctest/parts/public/debugger.h>
#include <doctest/parts/public/std/fwd.h>
#include <doctest/parts/public/std/type_traits.h>
#include <doctest/parts/public/std/utility.h>
#include <doctest/parts/public/string.h>
#include <doctest/parts/public/matchers/contains.h>
#include <doctest/parts/public/matchers/approx.h>
#include <doctest/parts/public/matchers/is_nan.h>
#include <doctest/parts/public/context/options.h>
#include <doctest/parts/public/assert/type.h>
#include <doctest/parts/public/assert/data.h>
#include <doctest/parts/public/assert/comparator.h>
#include <doctest/parts/public/assert/result.h>
#include <doctest/parts/public/assert/expression.h>
#include <doctest/parts/public/color.h>
#include <doctest/parts/public/subcase.h>
#include <doctest/parts/public/test_suite.h>
#include <doctest/parts/public/test_case.h>
#include <doctest/parts/public/decorators.h>
#include <doctest/parts/public/exception_translator.h>
#include <doctest/parts/public/context_scope.h>
#include <doctest/parts/public/assert/message.h>
#include <doctest/parts/public/path.h>
#include <doctest/parts/public/exceptions.h>
#include <doctest/parts/public/context.h>
#include <doctest/parts/public/assert/handler.h>
#include <doctest/parts/public/reporter.h>
#include <doctest/parts/public/macros.h>

DOCTEST_CLANG_SUPPRESS_WARNING_POP
DOCTEST_MSVC_SUPPRESS_WARNING_POP
DOCTEST_GCC_SUPPRESS_WARNING_POP

DOCTEST_SUPPRESS_COMMON_WARNINGS_POP

#endif // DOCTEST_LIBRARY_INCLUDED
