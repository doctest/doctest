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

  #if defined(DOCTEST_CONFIG_IMPLEMENT) && !defined(DOCTEST_LIBRARY_IMPLEMENTATION)

  DOCTEST_CLANG_SUPPRESS_WARNING_WITH_PUSH("-Wunused-macros")
  #define DOCTEST_LIBRARY_IMPLEMENTATION
  DOCTEST_CLANG_SUPPRESS_WARNING_POP

  DOCTEST_SUPPRESS_PRIVATE_WARNINGS_PUSH

  $sources

  DOCTEST_SUPPRESS_PRIVATE_WARNINGS_POP

  #endif // defined(DOCTEST_CONFIG_IMPLEMENT) && !defined(DOCTEST_LIBRARY_IMPLEMENTATION)
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
                    file, visited=visited, headers=public_headers + private_headers + [doctest_fwd]
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
