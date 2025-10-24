import os
import sys

_os = sys.argv[1]
assert _os in ["Linux", "macOS", "Windows"]

_arch = sys.argv[2].lower()
assert _arch in ["x86", "x64", "arm64"]

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

if _os == "Linux" or (_os == "macOS" and _compiler == "gcc"):
    used_cxx += "-" + _version


def log_and_call(command):
    print(command)
    return os.system(command)


def run_test(build_type, test_mode, flags, test=True, cxx_standard=""):
    standard_desc = f" (C++{cxx_standard})" if cxx_standard else ""
    print("Running: " + "; ".join([build_type, test_mode, flags, str(test)]) + standard_desc)
    if log_and_call("cmake -E remove_directory build"):
        exit(1)
    cmake_cmd = (
        f"cmake -S . "
        f"-B build "
        f"-D CMAKE_BUILD_TYPE={build_type} "
        f"-D DOCTEST_TEST_MODE={test_mode} "
        + (flags and f'-D CMAKE_CXX_FLAGS="{flags}" ')
        + (cxx_standard and f'-D CMAKE_CXX_STANDARD={cxx_standard} ')
        + f"-D CMAKE_CXX_COMPILER={used_cxx}"
    )
    if log_and_call(cmake_cmd):
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
        if version_tuple(_version) <= version_tuple("6.0") or (
            version_tuple("11") <= version_tuple(_version) < version_tuple("13")
        ):
            flags = ""
    elif _compiler == "gcc":
        if version_tuple(_version) <= version_tuple("5.0"):
            flags = ""
elif _os == "macOS" and _compiler == "gcc":
    flags = ""

if _os == "Linux" and _compiler == "gcc":
    flags += " -static-libasan"

tsan_flags = "-fsanitize=thread -pie -fPIE"
if _os == "Windows":
    tsan_flags = ""
elif _os == "Linux":
    if _compiler == "clang":
        if (version_tuple(_version) <= version_tuple("3.9") or
            version_tuple(_version) == version_tuple("11")):
            tsan_flags = ""
    elif _compiler == "gcc":
        if version_tuple(_version) <= version_tuple("6.0"):
            tsan_flags = ""
elif _os == "macOS" and _compiler == "gcc":
    tsan_flags = ""

if _os == "Linux" and _compiler == "gcc":
    tsan_flags += " -static-libtsan"

x86_flag = " -m32" if _arch == "x86" and _compiler != "cl" else ""

# Determine supported C++ standards based on compiler and version
def get_supported_cpp_standards():
    standards = ["11"]  # Default standard (C++11)
    
    if _version and _compiler in ["gcc", "clang"]:
        version_nums = version_tuple(_version)
        
        # Add C++20 support for GCC >= 12 and Clang >= 12
        if ((_compiler == "gcc" and version_nums >= version_tuple("12.0")) or
            (_compiler == "clang" and version_nums >= version_tuple("12.0"))):
            standards.append("20")
            
            # Add C++23 support for GCC >= 12 and Clang >= 12
            # Note: C++23 support may be experimental in earlier versions
            standards.append("23")
    
    return standards

supported_standards = get_supported_cpp_standards()

for configuration in ["Debug", "Release"]:
    for std in supported_standards:
        std_desc = f" with C++{std}" if std else ""
        print(f"\n=== Running {configuration} configuration{std_desc} ===")
        
        run_test(configuration, "COMPARE", flags + x86_flag, cxx_standard=std)
        if tsan_flags != "":
            run_test(configuration, "COMPARE", tsan_flags, cxx_standard=std)
        if _os != "Windows":
            run_test(
                configuration,
                "COMPARE",
                "-fno-exceptions -D DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS",
                test=False,
                cxx_standard=std,
            )
            run_test(configuration, "COMPARE", "-fno-rtti", cxx_standard=std)
        if _os == "Linux":
            run_test(configuration, "VALGRIND", x86_flag, cxx_standard=std)
