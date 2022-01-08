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

usedCc = _compiler
usedCxx = ""
if usedCc == "gcc":
    usedCxx = "g++"
elif usedCc == "clang":
    usedCxx = "clang++"
else:
    usedCxx = usedCc

if (_os == "Linux"):
    usedCc += "-" + _version
    usedCxx += "-" + _version

def logAndCall(command):
    print(command)
    return os.system(command)

def runTest(buildType, testMode, flags, test = True):
    print("Running: " + "; ".join([buildType, testMode, flags, str(test)]))
    if logAndCall("cmake -E remove_directory build"):
        exit(1)
    if logAndCall("cmake -S . -B build -DCMAKE_BUILD_TYPE=" + buildType + " -DDOCTEST_TEST_MODE=" + testMode +
        " -DCMAKE_CXX_FLAGS=\"" + flags + "\" -DCMAKE_C_COMPILER=" + usedCc + " -DCMAKE_CXX_COMPILER=" + usedCxx):
        exit(2)
    if logAndCall("cmake --build build"):
        exit(3)
    if test and logAndCall("ctest --test-dir build"):
        exit(4)

def versiontuple(v):
    return tuple(map(int, (v.split("."))))

flags = "-fsanitize=address,undefined -fno-omit-frame-pointer"
if _os == "Windows":
    flags = ""
elif _os == "Linux":
    if _compiler == "clang":
        if versiontuple(_version) <= versiontuple("6.0"):
            flags = ""
    elif _compiler == "gcc":
        if versiontuple(_version) <= versiontuple("5.0"):
            flags = ""

if _os == "Linux" and _compiler == "gcc":
    flags += " -static-libasan"


tsanFlags = "-fsanitize=thread -pie -fPIE"
if _os == "Windows":
    tsanFlags = ""
elif _os == "Linux":
    if _compiler == "clang":
        if versiontuple(_version) <= versiontuple("3.9"):
            tsanFlags = ""
    elif _compiler == "gcc":
        if versiontuple(_version) <= versiontuple("6.0"):
            tsanFlags = ""

if _os == "Linux" and _compiler == "gcc":
    tsanFlags += " -static-libtsan"

possibleX86Flag = " -m32" if (_arch == "x86" and _compiler != "cl") else ""

for configuration in ["Debug", "Release"]:
    runTest(configuration, "COMPARE", flags + possibleX86Flag)
    if tsanFlags != "":
        runTest(configuration, "COMPARE", tsanFlags)
    if _os != "Windows":
        runTest(configuration, "COMPARE", "-fno-exceptions -DDOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS", test = False)
        runTest(configuration, "COMPARE", "-fno-rtti")
    if _os == "Linux":
        runTest(configuration, "VALGRIND", possibleX86Flag)
