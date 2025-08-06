#!/usr/bin/env python3

"""
Generate the monolithic doctest.h header file.

Our original script, scripts/cmake/assemble_single_header.cmake,
simply read doctest_fwd.h and doctest.cpp and pasted them in-order,
with some defines sandwiched inbetween.

This script attempts to be a bit more future-proof, by automatically
scanning for any .h and .cpp files that could be included,
(topologically) sorting them by #include order, then creating
the final header file.
"""

# /// script
# requires-python = ">=3.6"
# ///


from __future__ import annotations

import re
import sys
from pathlib import Path
from typing import Generator, NoReturn


def main(args: list[str]) -> NoReturn:
  """Script entry-point."""

  if len(args) != 1:
    print("Usage: scripts/assemble.py", file=sys.stderr)
    sys.exit(1)

  script = Path(__file__).resolve()
  root   = script.parent.parent

  header_dir = root / "doctest" / "parts"
  source_dir = root / "doctest" / "parts"
  output     = root / "doctest" / "doctest.h"

  headers = set(header_dir.rglob("*.h"))
  sources = set(source_dir.rglob("*.cpp"))

  def extract_header(line: str) -> str | None:
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


  def process_file(file: Path, /, visited: set[Path]) -> Generator[str, None, None]:
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
    with file.open("r") as fd:
      content = fd.read()

    for line in content.splitlines(keepends=False):
      header = extract_header(line)
      if header is not None:
        if (header_dir / header) in headers:
          yield from process_file(header_dir / header, visited=visited)
        else:
          yield line
      else:
        yield line


  # Would be a lot nicer with t-strings (Python 3.14)
  # or jinja2, but it's best to avoid unnecessary dependencies,
  # even for dev tooling
  visited: set[Path] = set()
  content = [
    '// ============================================================= lgtm [cpp/missing-header-guard]',
    '// == DO NOT MODIFY THIS FILE BY HAND - IT IS AUTO GENERATED! ==',
    '// =============================================================',
    *(process_file(file, visited=visited) for file in headers),
    '#ifndef DOCTEST_SINGLE_HEADER',
    '#define DOCTEST_SINGLE_HEADER',
    '#endif // DOCTEST_SINGLE_HEADER',
    *(process_file(file, visited=visited) for file in sources),
  ]


  def flatten(iterable):
    for item in iterable:
      if isinstance(item, str):
        yield item
      else:
        yield from flatten(item)


  with output.open("w") as sink:
    for line in flatten(content):
      print(line, file=sink)

  sys.exit(0)

if __name__ == "__main__":
  main(sys.argv)
