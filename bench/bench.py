#!/usr/bin/python2.7

# Notes:
#   - cmake, gcc, make and git should be added to the PATH
#   - to see what exactly this script does just check the git modifications to the assimp sources after running it
#   - framework headers are included directly in the source files and not in headers and this may be lighter
#   that a real-world scenario because the ifdef-ed headers are included only once in each translation unit

framework   = "doctest" # catch/doctest
compiler    = "GCC"     # GCC/MSVC/BOTH     - GCC is MinGW under Windows and is the only option for linux
config      = "Debug"   # Debug/Release     - the configuration that will be built
include     = True      # True/False        - whether to include the framework headers at all
in_sources  = True      # True/False        - whether to insert tests in the source files
in_headers  = False     # True/False        - whether to insert tests in header files (doctest only)
disable     = False     # True/False        - whether to disable the test registration (doctest only)
test_mult   = 10         # 1/2/3..100..     - a multiplier for the amount of tests to be added

MSVC_dir                = "C:\\Program Files (x86)\\Microsoft Visual Studio 12.0"
MSVC_cmake_generator    = "Visual Studio 12 Win64"

#================================================================================================================================

import os
import sys
import random
import string
import multiprocessing
from datetime import datetime

# setup the right values for OS-specific vars
windows = False
makefile = "Unix Makefiles"
make = "make"
if os.name == "nt":
    windows = True
    makefile = "MinGW Makefiles"
    make = "mingw32-make"

# the path to the appropriate header
framework_header = '"../../catch/single_include/catch.hpp"'
if framework == "doctest":
    framework_header = '"../../../doctest.h"'

# files that (probably) should not be modified
skipList = ["AssimpPCH.cpp", "AssimpPCH.h"]

# clones the repositories and calls cmake for the first time
def setup():
    os.system("git clone https://github.com/philsquared/Catch catch")
    os.system("git clone https://github.com/assimp/assimp assimp")
    
    os.chdir("assimp")
    if windows:
        os.makedirs("MSVC")
        os.chdir("MSVC")
        os.system('cmake -DASSIMP_BUILD_ASSIMP_TOOLS=OFF -DASSIMP_BUILD_TESTS=OFF -G "' + MSVC_cmake_generator + '" ../')
        os.chdir("../");
    os.makedirs("GCC_Debug")
    os.chdir("GCC_Debug")
    os.system('cmake -DASSIMP_BUILD_ASSIMP_TOOLS=OFF -DASSIMP_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Debug -G "' + makefile + '" ../')
    os.chdir("../");
    os.makedirs("GCC_Release")
    os.chdir("GCC_Release")
    os.system('cmake -DASSIMP_BUILD_ASSIMP_TOOLS=OFF -DASSIMP_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release -G "' + makefile + '" ../')
    os.chdir("../../");

def randomword():
    return ''.join(random.choice(string.digits + string.letters + "_ <>(){}[]-=+&*&^%$#@!~") for i in range(random.randint(5, 15)))

def createTest(framework):
    if framework == "catch":
        return '\nTEST_CASE("' + randomword() + '", "") {\n    printf("hello from %s!\\n", __func__);\n}\n'
    else:
        return '\ndoctest_test("' + randomword() + '") {\n    printf("hello from %s!\\n", __func__);\n}\n'

# setup everything if not already done
if not os.path.exists("assimp"):
    setup()

# clear the log file (will be appended to it later)
os.system('echo ==================================== >log.txt');

# reset git stuff
os.chdir("assimp")
os.system("git checkout .")

# headers/defines at the top of translation units
if include:
    # insert headers and defines for all translation units (assimp/code/*.cpp)
    for filename in os.listdir('code'):
        if filename[-3:] == "cpp" and filename not in skipList:
            f = open('./code/' + filename, "r+")
            old = f.read()
            f.seek(0)
            
            # ensure the __func__ macro is present everywhere
            f.write('#if defined(_MSC_VER)\n')
            f.write('#define __func__ __FUNCTION__\n')
            f.write('#endif // _MSC_VER\n\n')
            
            if framework == "doctest" and disable:
                f.write('#define DOCTEST_GLOBAL_DISABLE\n')
            
            # stuff that should be in only one translation unit
            if filename == "Assimp.cpp":
                if framework == "catch":
                    f.write("#define CATCH_CONFIG_RUNNER\n")
                else:
                    f.write('#include ' + framework_header + '\n')
            
            if framework == "doctest":
                f.write("#define DOCTEST_DONT_INCLUDE_IMPLEMENTATION\n")
            f.write('#include ' + framework_header + '\n')
            f.write('#include <stdio.h>\n')
            f.write(old)
            f.close()
    # if inserting tests and not only including the headers
    if in_sources:
        numTests = 0
        for filename in os.listdir('code'):
            if filename[-4:] == ".cpp" and filename not in skipList:
                f = open('./code/' + filename, "r+")
                lines = f.readlines()
                f.seek(0)
                prev = ""
                for line in lines:
                    if prev == "}\n" and line == "\n":
                        for i in range(0, test_mult):
                            numTests = numTests + 1
                            f.write(createTest(framework))
                    f.write(line)
                    prev = line;
                f.close()
        print("Inserted " + str(numTests) + " tests in source files")
    # if inserting tests in headers too
    if in_headers and framework == "doctest":
        numTests = 0
        for filename in os.listdir('code'):
            if filename[-2:] == ".h" and filename not in skipList:
                f = open('./code/' + filename, "r+")
                f.seek(-1, 2) # no idea what this does... seems like going to the end?
                f.write("\n")
                f.write("#define DOCTEST_DONT_INCLUDE_IMPLEMENTATION\n")
                f.write('#include ' + framework_header + '\n')
                f.write('#include <stdio.h>\n')
                for k in range(0, test_mult):
                    for i in range(0, 10):
                        numTests = numTests + 1
                        f.write(createTest(framework))
                f.close()
        print("Inserted " + str(numTests) + " tests in headers")

# MSVC
if windows and (compiler == "MSVC" or compiler == "BOTH"):
    os.chdir("MSVC")
    os.system('call "' + MSVC_dir + '\\VC\\vcvarsall.bat" amd64 >>../../log.txt 2>&1 & ' + 
              'msbuild Assimp.sln /t:Clean >>../../log.txt 2>&1')
    print("About to compile with MSVC")
    start = datetime.now()
    os.system('call "' + MSVC_dir + '\\VC\\vcvarsall.bat" amd64 >>../../log.txt 2>&1 & ' + 
              'msbuild Assimp.sln /p:Configuration=' + config + ';Platform=X64 >>../../log.txt 2>&1')
    end = datetime.now()
    print("Time for compiling (+ linking): " + str(end - start))
    os.chdir("../");

# GCC
if compiler == "GCC" or compiler == "BOTH":
    os.chdir("GCC_" + config)
    os.system(make + " clean")
    print("About to compile with GCC")
    start = datetime.now()
    os.system(make + " -j" + str(multiprocessing.cpu_count()) + " >>../../log.txt 2>&1")
    end = datetime.now()
    print("Time for compiling (+ linking): " + str(end - start))
    os.chdir("../")

os.chdir("../");















