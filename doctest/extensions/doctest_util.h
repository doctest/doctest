//
// doctest_util.h - an accompanying extensions header to the main doctest.h header
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

#ifndef DOCTEST_UTIL_H
#define DOCTEST_UTIL_H

#ifndef DOCTEST_LIBRARY_INCLUDED
#include "../doctest.h"
#endif

#include <memory>
#include <vector>
#include <string>

namespace doctest {

    inline void applyCommandLine(doctest::Context& ctx, const std::vector<std::string>& args) {
        auto doctest_args = std::make_unique<const char*[]>(args.size());
        for (size_t i = 0; i < args.size(); ++i) {
            doctest_args[i] = args[i].c_str();
        }
        ctx.applyCommandLine(args.size(), doctest_args.get());
    }

} // namespace doctest

#endif // DOCTEST_UTIL_H
