#!/usr/bin/python2.7

# Notes: 
#   - framework headers are included directly in the source files and not in headers and this may be lighter
#   that a real-world scenario because the ifdef-ed headers are included only once in each translation unit
#   - cmake, gcc, make and git should be added to the PATH

framework   = "doctest" # catch/doctest
compiler    = "GCC"     # GCC/MSVC/BOTH     - GCC is MinGW under Windows and is the only option for linux
config      = "Debug"   # Debug/Release     - the configuration that will be built
include     = True      # True/False        - whether to include the framework headers at all
insert      = True      # True/False        - whether to insert tests in the source files
in_headers  = False     # True/False        - whether to insert tests in header files too (doctest only)
disable     = False     # True/False        - whether to disable the test registration (doctest only)

MSVC_dir                = "C:\\Program Files (x86)\\Microsoft Visual Studio 12.0"
MSVC_cmake_generator    = "Visual Studio 12 Win64"

#================================================================================================================================

import os
import sys
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
    # the path to the appropriate header
    framework_header = '"../../catch/single_include/catch.hpp"'
    if framework == "doctest":
        framework_header = '"../../../doctest.h"'
    # for each translation unit (*.cpp)
    for filename in os.listdir('code'):
        if filename[-3:] == "cpp":
            f = open('./code/' + filename, "r+")
            old = f.read()
            f.seek(0)
            # prepend the disabling define
            if disable:
                f.write('#define DOCTEST_GLOBAL_DISABLE\n')
            # prepend the inclusion of the header
            f.write('#include ' + framework_header + '\n')
            # append the contents of the file
            f.write(old)
            f.close()

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















