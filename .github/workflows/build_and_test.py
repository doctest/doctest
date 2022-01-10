import os
import sys

_os = sys.argv[1]
assert _os in ["Linux", "macOS", "Windows"]

_arch = sys.argv[2]
assert _arch in ["x86", "x64"]

_compiler = sys.argv[3]
assert _compiler in ["cl", "clang-cl", "clang", "gcc", "xcode"]

_version = sys.argv[4] if len(sys.argv) >= 5 else ""

print("Env: " + "; ".join([_os, _arch, _compiler, _version]))

if _compiler == "gcc":
    used_cxx = "g++"
elif _compiler == "clang" or _compiler == "xcode":
    used_cxx = "clang++"
else:
    used_cxx = _compiler

if _os == "Linux":
    used_cxx += "-" + _version


def log_and_call(command):
    print(command)
    return os.system(command)


def run_test(build_type, test_mode, flags, test = True):
    print("Running: " + "; ".join([build_type, test_mode, flags, str(test)]))
    if log_and_call("cmake -E remove_directory build"):
        exit(1)
    if log_and_call(
        f"cmake -S . "
        f"-B build "
        f"-D CMAKE_BUILD_TYPE={build_type} "
        f"-D DOCTEST_TEST_MODE={test_mode} "
        + (flags and f'-D CMAKE_CXX_FLAGS="{flags}" ') +
        f"-D CMAKE_CXX_COMPILER={used_cxx}"
    ):
        exit(2)
    if log_and_call("cmake --build build"):
        exit(3)
    if test and log_and_call("ctest --test-dir build --no-tests=error"):
        exit(4)


def version_tuple(v):
    return tuple(map(int, (v.split("."))))


flags = "-fsanitize=address,undefined -fno-omit-frame-pointer"
if _os == "Windows":
    flags = ""
elif _os == "Linux":
    if _compiler == "clang":
        if version_tuple(_version) <= version_tuple("6.0"):
            flags = ""
    elif _compiler == "gcc":
        if version_tuple(_version) <= version_tuple("5.0"):
            flags = ""

if _os == "Linux" and _compiler == "gcc":
    flags += " -static-libasan"

tsan_flags = "-fsanitize=thread -pie -fPIE"
if _os == "Windows":
    tsan_flags = ""
elif _os == "Linux":
    if _compiler == "clang":
        if version_tuple(_version) <= version_tuple("3.9"):
            tsan_flags = ""
    elif _compiler == "gcc":
        if version_tuple(_version) <= version_tuple("6.0"):
            tsan_flags = ""

if _os == "Linux" and _compiler == "gcc":
    tsan_flags += " -static-libtsan"

x86_flag = " -m32" if _arch == "x86" and _compiler != "cl" else ""

for configuration in ["Debug", "Release"]:
    run_test(configuration, "COMPARE", flags + x86_flag)
    if tsan_flags != "":
        run_test(configuration, "COMPARE", tsan_flags)
    if _os != "Windows":
        run_test(
            configuration,
            "COMPARE",
            "-fno-exceptions -D DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS",
            test = False,
        )
        run_test(configuration, "COMPARE", "-fno-rtti")
    if _os == "Linux":
        run_test(configuration, "VALGRIND", x86_flag)
