#!/usr/bin/env python

"""
Generates a relevant CMake setup command.

This script accepts information about the target build, and:

1. Sets various environment variables
2. Sets up a build directory
3. Compiles the build directory
4. Executes registered tests
"""

# /// script
# requires-python = ">=3.6"
# ///


import multiprocessing
import optparse
import os
import subprocess
import sys


class Version:
  def __init__(self, major=0, minor=0, patch=0, *, string = None):
    self.major  = major
    self.minor  = minor
    self.patch  = patch
    self.string = string

  @staticmethod
  def from_string(string):
    if string is None:
      return Version(0)
    else:
      parts = map(int, string.split("."))
      return Version(*parts, string=string)

  @property
  def _tuple(self):
    return (self.major, self.minor, self.patch)

  def __eq__(self, other): return self._tuple == other._tuple
  def __ne__(self, other): return self._tuple != other._tuple
  def __lt__(self, other): return self._tuple <  other._tuple
  def __le__(self, other): return self._tuple <= other._tuple
  def __gt__(self, other): return self._tuple >  other._tuple
  def __ge__(self, other): return self._tuple >= other._tuple


class Compiler:
  def __init__(self, exe, os, arch, version):
    self.exe     = exe
    self.os      = os
    self.arch    = arch
    self.version = version or Version(0)

    assert self.exe  in { "cl" ,"clang-cl", "clang", "clang++", "gcc", "g++", "xcode" }
    assert self.os   in { "Linux", "macOS", "Windows" }
    assert self.arch in { "x86", "x64", "arm64" }

  @property
  def cxx(self):
    exe = (
      "g++"     if self.exe == "gcc" else
      "clang++" if self.exe == "clang" else
      "clang++" if self.exe == "xcode" else
      self.exe
    )

    if self.os == "Linux" or (self.os == "macOS" and self.exe == "gcc"):
      return f"{exe}-{self.version.string}"
    else:
      return exe

  @property
  def sanitizer_flags(self):
    if self.os == "Windows":
      return None

    if self.os == "macOS" and self.exe == "gcc":
      return None

    if self.os == "Linux" and self.exe == "clang" and (
      self.version <= Version(6) or
      Version(11) <= self.version < Version(13)
    ):
      return None

    if self.os == "Linux" and self.exe == "gcc" and (
      self.version <= Version(5)
    ):
      return None

    flags = ["-fsanitize=address,undefined", "-fno-omit-frame-pointer"]
    if self.os == "Linux" and self.exe == "gcc":
      flags.append("-static-libasan")
    return flags

  @property
  def thread_sanitizer_flags(self):
    if self.os == "Windows":
      return None

    if self.os == "Linux" and self.exe == "clang" and (
      self.version <= Version(3, 9) or
      self.version == Version(11)
    ):
      return None

    if self.os == "Linux" and self.exe == "gcc" and (
      self.version <= Version(6)
    ):
      return None

    if self.os == "macOS" and self.exe == "gcc":
      return None

    flags = ["-fsanitize=thread", "-pie", "-fPIE"]
    if self.os == "Linux" and self.exe == "gcc":
      flags.append("-static-libtsan")
    return flags

  @property
  def fnoexceptions_flags(self):
    return ["-fno-exceptions", "-DDOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS"]

  @property
  def fnortti_flags(self):
    return ["-fno-rtti"]

  @property
  def warning_flags(self):
    if self.os == "macOS" and self.exe == "xcode":
      return ["-Wno-poison-system-directories"]
    return []

  @property
  def arch_flags(self):
    if (self.arch == "x64") or (self.arch == "arm64"):
      return []

    if self.arch == "x86" and self.exe in { "gcc", "clang" }:
      return ["-m32"]

    return None


def export(key, value):
  if key in os.environ:
    return
  print(f"export {key}={value}", flush=True)
  os.environ[key] = str(value)


def sh(*command):
  command = [part for part in command if part is not None]
  print(*command, flush=True)
  try:
    subprocess.run(command, check=True)
  except subprocess.CalledProcessError as e:
    sys.exit(e.returncode)


def main(args):
  parser = optparse.OptionParser()
  parser.add_option("--os")
  parser.add_option("--arch")
  parser.add_option("--compiler")
  parser.add_option("--version")
  parser.add_option("--target")
  parser.add_option("--build-type")
  opts, args = parser.parse_args(args)

  if len(args) != 0:
    reason = f"unparsed arguments: {args}"
    raise RuntimeError(reason)

  compiler = Compiler(
    os=opts.os,
    arch=opts.arch,
    exe=opts.compiler,
    version=Version.from_string(opts.version),
  )

  target = opts.target or "test"
  assert target in {"test", "asan/ubsan", "tsan", "fno-exceptions", "fno-rtti", "valgrind"}

  build_type = opts.build_type or "Release"
  assert build_type in {"Debug", "Release"}

  # Build directory generation
  export("CMAKE_GENERATOR", "Ninja")

  # Test execution
  export("CTEST_OUTPUT_ON_FAILURE", "ON")
  export("CTEST_PARALLEL_LEVEL",    multiprocessing.cpu_count())

  # Sanitizers
  export("UBSAN_OPTIONS", "halt_on_error=1:abort_on_error=1:print_summary=1:print_stacktrace=1")
  export("MSAN_OPTIONS",  "halt_on_error=1:abort_on_error=1:print_summary=1:print_stacktrace=1")
  export("TSAN_OPTIONS",  "force_seq_cst_atomics=1")
  export("ASAN_OPTIONS",  "halt_on_error=1:abort_on_error=1:print_summary=1:print_stacktrace=1:" +
                          "strict_string_checks=true:" +
                          "detect_odr_violation=2:" +
                          "detect_stack_use_after_return=true:" +
                          "check_initialization_order=true:" +
                          "strict_init_order=true")
  export("VALGRIND_OPTS", "--error-exitcode=1")

  # Build directory setup
  flags = compiler.warning_flags + compiler.arch_flags + (
    compiler.sanitizer_flags        if target == "asan/ubsan"     else
    compiler.thread_sanitizer_flags if target == "tsan"           else
    compiler.fnoexceptions_flags    if target == "fno-exceptions" else
    compiler.fnortti_flags          if target == "fno-rtti"       else
    []
  )

  sh("cmake", "-E", "remove_directory", "build")

  sh(
    "cmake", "-B", "build",
    (f"-DCMAKE_CXX_FLAGS={' '.join(flags)}" if len(flags) > 0 else None),
     f"-DCMAKE_CXX_COMPILER={compiler.cxx}",
     f"-DCMAKE_BUILD_TYPE={build_type}",
     f"-DDOCTEST_TEST_MODE={'VALGRIND' if target == 'valgrind' else 'COMPARE'}",
      "-DDOCTEST_INTERNAL_WERROR=ON",
  )

  # Compilation
  sh("cmake", "--build", "build")

  # Test
  if target != "fno-exceptions":
    sh("ctest", "--test-dir", "build", "--no-tests=error")


if __name__ == "__main__":
  main(sys.argv[1:])
