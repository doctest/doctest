#!/usr/bin/env python3

"""
Generate the monolithic doctest.h header file.

This script scans for any .h and .cpp files in the parts/ directory,
(topologically) sorts them by #include order, then emits them into
the final doctest.h header.
"""

# /// script
# requires-python = ">=3.6"
# ///


import re
import string
import sys
from itertools import chain
from pathlib import Path
from textwrap import dedent


TEMPLATE = string.Template(
    dedent(
        """\
  // ============================================================= lgtm [cpp/missing-header-guard]
  // == DO NOT MODIFY THIS FILE BY HAND - IT IS AUTO GENERATED! ==
  // =============================================================
  $headers
  #ifndef DOCTEST_SINGLE_HEADER
  #define DOCTEST_SINGLE_HEADER
  #endif // DOCTEST_SINGLE_HEADER

  #if defined(DOCTEST_CONFIG_IMPLEMENT) || !defined(DOCTEST_SINGLE_HEADER)

  #ifndef DOCTEST_SINGLE_HEADER
  #include "doctest_fwd.h"
  #endif // DOCTEST_SINGLE_HEADER

  DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wunused-macros")
  #ifndef DOCTEST_LIBRARY_IMPLEMENTATION
  #define DOCTEST_LIBRARY_IMPLEMENTATION
  DOCTEST_CLANG_SUPPRESS_WARNING_POP

  DOCTEST_SUPPRESS_COMMON_WARNINGS_PUSH

  DOCTEST_CLANG_SUPPRESS_WARNING_PUSH
  DOCTEST_CLANG_SUPPRESS_WARNING("-Wglobal-constructors")
  DOCTEST_CLANG_SUPPRESS_WARNING("-Wexit-time-destructors")
  DOCTEST_CLANG_SUPPRESS_WARNING("-Wsign-conversion")
  DOCTEST_CLANG_SUPPRESS_WARNING("-Wshorten-64-to-32")
  DOCTEST_CLANG_SUPPRESS_WARNING("-Wmissing-variable-declarations")
  DOCTEST_CLANG_SUPPRESS_WARNING("-Wswitch")
  DOCTEST_CLANG_SUPPRESS_WARNING("-Wswitch-enum")
  DOCTEST_CLANG_SUPPRESS_WARNING("-Wcovered-switch-default")
  DOCTEST_CLANG_SUPPRESS_WARNING("-Wmissing-noreturn")
  DOCTEST_CLANG_SUPPRESS_WARNING("-Wdisabled-macro-expansion")
  DOCTEST_CLANG_SUPPRESS_WARNING("-Wmissing-braces")
  DOCTEST_CLANG_SUPPRESS_WARNING("-Wmissing-field-initializers")
  DOCTEST_CLANG_SUPPRESS_WARNING("-Wunused-member-function")
  DOCTEST_CLANG_SUPPRESS_WARNING("-Wnonportable-system-include-path")

  DOCTEST_GCC_SUPPRESS_WARNING_PUSH
  DOCTEST_GCC_SUPPRESS_WARNING("-Wconversion")
  DOCTEST_GCC_SUPPRESS_WARNING("-Wsign-conversion")
  DOCTEST_GCC_SUPPRESS_WARNING("-Wmissing-field-initializers")
  DOCTEST_GCC_SUPPRESS_WARNING("-Wmissing-braces")
  DOCTEST_GCC_SUPPRESS_WARNING("-Wswitch")
  DOCTEST_GCC_SUPPRESS_WARNING("-Wswitch-enum")
  DOCTEST_GCC_SUPPRESS_WARNING("-Wswitch-default")
  DOCTEST_GCC_SUPPRESS_WARNING("-Wunsafe-loop-optimizations")
  DOCTEST_GCC_SUPPRESS_WARNING("-Wold-style-cast")
  DOCTEST_GCC_SUPPRESS_WARNING("-Wunused-function")
  DOCTEST_GCC_SUPPRESS_WARNING("-Wmultiple-inheritance")
  DOCTEST_GCC_SUPPRESS_WARNING("-Wsuggest-attribute")

  DOCTEST_MSVC_SUPPRESS_WARNING_PUSH
  DOCTEST_MSVC_SUPPRESS_WARNING(4267) // 'var' : conversion from 'x' to 'y', possible loss of data
  DOCTEST_MSVC_SUPPRESS_WARNING(4530) // C++ exception handler used, but unwind semantics not enabled
  DOCTEST_MSVC_SUPPRESS_WARNING(4577) // 'noexcept' used with no exception handling mode specified
  DOCTEST_MSVC_SUPPRESS_WARNING(4774) // format string expected in argument is not a string literal
  DOCTEST_MSVC_SUPPRESS_WARNING(4365) // conversion from 'int' to 'unsigned', signed/unsigned mismatch
  DOCTEST_MSVC_SUPPRESS_WARNING(5039) // pointer to potentially throwing function passed to extern C
  DOCTEST_MSVC_SUPPRESS_WARNING(4800) // forcing value to bool 'true' or 'false' (performance warning)
  DOCTEST_MSVC_SUPPRESS_WARNING(5245) // unreferenced function with internal linkage has been removed

  $sources

  DOCTEST_CLANG_SUPPRESS_WARNING_POP
  DOCTEST_MSVC_SUPPRESS_WARNING_POP
  DOCTEST_GCC_SUPPRESS_WARNING_POP

  DOCTEST_SUPPRESS_COMMON_WARNINGS_POP

  #endif // DOCTEST_LIBRARY_IMPLEMENTATION

  #endif // defined(DOCTEST_CONFIG_IMPLEMENT) || !defined(DOCTEST_SINGLE_HEADER)
"""
    )
)


def main(args):
    """Script entry-point."""

    if len(args) != 1:
        print("Usage: scripts/assemble.py", file=sys.stderr)
        sys.exit(1)

    script = Path(__file__).resolve()
    root = script.parent.parent

    public_dir  = root / "doctest" / "parts" / "public"
    private_dir = root / "doctest" / "parts" / "private"
    output      = root / "doctest" / "doctest.h"

    public_headers  = sorted(set(public_dir.rglob("*.h")))
    private_headers = sorted(set(private_dir.rglob("*.h")))
    private_sources = sorted(set(private_dir.rglob("*.cpp")))

    def extract_header(line):
        """
        Extract a header file name from a line of C code.

        Assuming the input looks something like:

          ```c
          #include "foo.h"
          #include <bar.h>
          ```

        This function will return "foo.h" and "bar.h" respectively
        """

        matches = re.findall(r'#include\s*["<]([^">]+)[">]', line)
        if len(matches) == 0:
            return None
        if len(matches) == 1:
            return matches[0]

        reason = f"'{line}' has multiple includes"
        raise RuntimeError(reason)

    def process_file(file, visited, headers):
        """
        Process a file, yielding lines of code with #include's scrubbed.

        Assuming the file represents a C source file, iterates over each line,
        yielding the contents. If the line is an #include which has NOT
        yet been seen (as indicated by the `visitor` set), then the
        contents of THAT file is recursively run through this same method.

        If the file HAS been visited, then it is ignored and the line is not yielded.

        The effect of this is a fusion of:
          1. A topological sort by-header, and
          2. Inlining header content, and
          3. Scrubbing #include's to other doctest files
        """

        if file in visited:
            return

        visited.add(file)
        content = file.read_text(encoding="utf-8")

        for line in content.splitlines(keepends=False):
            header = extract_header(line)
            if (header is not None) and ((root / header) in headers):
                yield from process_file(root / header, visited=visited, headers=headers)
            else:
                yield line

    visited     = set()
    doctest_fwd = root / "doctest" / "parts" / "doctest_fwd.h"
    result = TEMPLATE.substitute(
        headers="\n".join(
            process_file(doctest_fwd, visited=visited, headers=public_headers)
        ),
        sources="\n".join(
            chain.from_iterable(
                process_file(
                    file, visited=visited, headers=public_headers + private_headers
                )
                for file in private_sources
            )
        ),
    )

    with open(output, "w", encoding="utf-8", newline="\n") as out:
        out.write(result)
    sys.exit(0)


if __name__ == "__main__":
    main(sys.argv)
