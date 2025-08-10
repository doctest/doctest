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
  $sources
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

    header_dir = root / "doctest" / "parts"
    source_dir = root / "doctest" / "parts"
    output = root / "doctest" / "doctest.h"

    headers = set(header_dir.rglob("*.h"))
    sources = set(source_dir.rglob("*.cpp"))

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

    def process_file(file, visited):
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
            if (header is not None) and ((header_dir / header) in headers):
                yield from process_file(header_dir / header, visited=visited)
            else:
                yield line

    visited = set()
    result = TEMPLATE.substitute(
        headers="\n".join(
            chain.from_iterable(process_file(file, visited=visited) for file in headers)
        ),
        sources="\n".join(
            chain.from_iterable(process_file(file, visited=visited) for file in sources)
        ),
    )

    output.write_text(result)
    sys.exit(0)


if __name__ == "__main__":
    main(sys.argv)
